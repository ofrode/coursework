#include "../../hdr/utils/FileManager.h"
#include "../../hdr/core/Test.h"
#include "../../hdr/core/TestResult.h"
#include "../../hdr/core/Question.h"
#include "../../hdr/core/Answer.h"
#include "../../hdr/core/Statistics.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QApplication>
#include <QDateTime>
#include <optional>
#include <stdexcept>

namespace {
bool parseMetadataLine(const QString& line, int& testId, QString& userName, int& score, QDateTime& dateTime) {
    if (line.startsWith("ID теста:")) {
        testId = line.mid(9).trimmed().toInt();
        return true;
    }
    if (line.startsWith("Пользователь:")) {
        userName = line.mid(14).trimmed();
        return true;
    }
    if (line.startsWith("Баллы:")) {
        score = line.mid(7).trimmed().toInt();
        return true;
    }
    if (line.startsWith("Дата и время:")) {
        const QString dateTimeStr = line.mid(14).trimmed();
        // Используем 24-часовой формат (HH для часов 0-23)
        dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd HH:mm:ss");
        if (!dateTime.isValid()) {
            // Пробуем альтернативный формат (hh для часов 0-11)
            dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm:ss");
        }
        return true;
    }
    return false;
}

std::optional<AnswerType> parseAnswerLine(const QString& line) {
    const int dotIndex = line.indexOf(".");
    if (dotIndex <= 0) {
        return std::nullopt;
    }

    const QString answerStr = line.mid(dotIndex + 1).trimmed();
    using enum AnswerType;
    if (answerStr == "Да") {
        return YES;
    }
    if (answerStr == "Нет") {
        return NO;
    }
    if (answerStr == "Не могу ответить") {
        return UNSURE;
    }
    return std::nullopt;
}

std::optional<TestResult> tryLoadResultFile(const QFileInfo& fileInfo) {
    try {
        return FileManager::loadResultFromFile(fileInfo.absoluteFilePath());
    } catch (const FileException& e) {
        qDebug() << "Ошибка загрузки результата из файла:" << fileInfo.fileName() << e.what();
    }
    return std::nullopt;
}

std::optional<Test> tryLoadTestFile(const QFileInfo& fileInfo) {
    try {
        return FileManager::loadTestFromFile(fileInfo.absoluteFilePath());
    } catch (const FileException& e) {
        qDebug() << "Ошибка загрузки файла:" << fileInfo.fileName() << e.what();
    }
    return std::nullopt;
}
} // namespace

QString FileManager::getProjectRootDir() {
    // Получаем директорию исполняемого файла
    QString appDir = QApplication::applicationDirPath();
    QDir dir(appDir);
    
    // Если исполняемый файл находится в build/, поднимаемся на уровень вверх
    if (dir.dirName() == "build") {
        dir.cdUp();
    }
    
    // Проверяем наличие CMakeLists.txt для подтверждения, что это корень проекта
    if (const auto cmakeListsPath = dir.absoluteFilePath("CMakeLists.txt"); QFile::exists(cmakeListsPath)) {
        return dir.absolutePath();
    }
    
    // Если CMakeLists.txt не найден, пытаемся найти его, поднимаясь вверх по дереву
    QDir currentDir = QDir::current();
    while (!currentDir.isRoot()) {
        if (const auto cmakePath = currentDir.absoluteFilePath("CMakeLists.txt"); QFile::exists(cmakePath)) {
            return currentDir.absolutePath();
        }
        if (!currentDir.cdUp()) {
            break;
        }
    }
    
    // Если не удалось найти корень проекта, возвращаем текущую директорию
    return QDir::current().absolutePath();
}

QString FileManager::getTestQuestionDir() {
    QString rootDir = getProjectRootDir();
    QDir dir(rootDir);
    QString testDir = dir.absoluteFilePath("TestQuestionTxt");
    
    // Создаем директорию, если её нет
    if (QDir testQDir(testDir); !testQDir.exists()) {
        testQDir.mkpath(".");
    }
    
    return testDir;
}

QString FileManager::getTestAnswerDir() {
    QString rootDir = getProjectRootDir();
    QDir dir(rootDir);
    QString answerDir = dir.absoluteFilePath("TestAnswerTxt");
    
    // Создаем директорию, если её нет
    if (QDir testADir(answerDir); !testADir.exists()) {
        testADir.mkpath(".");
    }
    
    return answerDir;
}

QString FileManager::getStatisticsDir() {
    QString rootDir = getProjectRootDir();
    QDir dir(rootDir);
    QString statDir = dir.absoluteFilePath("Stat");
    
    // Создаем директорию, если её нет
    if (QDir statDirectory(statDir); !statDirectory.exists()) {
        statDirectory.mkpath(".");
    }
    
    return statDir;
}

void FileManager::validateFile(const QString& filename) {
    QFile file(filename);
    if (!file.exists()) {
        throw FileException("Файл не существует: " + filename);
    }
    if (!file.open(QIODevice::ReadOnly)) {
        throw FileException("Не удалось открыть файл: " + filename);
    }
}

void FileManager::saveTestToFile(const Test& test, const QString& filename) {
    try {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw FileException("Не удалось создать файл: " + filename);
        }
        
        QTextStream out(&file);
        out.setCodec("UTF-8");
        
        out << "TEST_ID:" << test.getId() << "\n";
        out << "TEST_NAME:" << test.getName() << "\n";
        out << "QUESTIONS:\n";
        
        for (const auto& question : test.getQuestions()) {
            out << "Q:" << question.getId() << ":" << question.getText() << "\n";
        }
        
        file.close();
    } catch (const std::exception& e) {
        throw FileException("Ошибка при сохранении теста: " + QString(e.what()));
    }
}

Test FileManager::loadTestFromFile(const QString& filename) {
    try {
        validateFile(filename);
        
        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        in.setCodec("UTF-8");
        
        Test test;
        QString line;
        bool inQuestions = false;
        int questionId = 0;
        
        while (in.readLineInto(&line)) {
            if (line.startsWith("TEST_ID:")) {
                int id = line.mid(8).toInt();
                test = Test(id, "");
            } else if (line.startsWith("TEST_NAME:")) {
                QString name = line.mid(10);
                test.setName(name);
            } else if (line == "QUESTIONS:") {
                inQuestions = true;
            } else if (inQuestions && line.startsWith("Q:")) {
                QStringList parts = line.mid(2).split(":");
                if (parts.size() >= 2) {
                    questionId = parts[0].toInt();
                    QString questionText = parts.mid(1).join(":");
                    Question q(questionId, questionText);
                    test.addQuestion(q);
                }
            }
        }
        
        file.close();
        return test;
    } catch (const std::exception& e) {
        throw FileException("Ошибка при загрузке теста: " + QString(e.what()));
    }
}

void FileManager::saveResultToFile(const TestResult& result, const QString& filename) {
    try {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw FileException("Не удалось открыть файл для записи: " + filename);
        }
        
        QTextStream out(&file);
        out.setCodec("UTF-8");
        
        out << "=== Результат теста ===\n";
        out << "ID теста: " << result.getTestId() << "\n";
        out << "Пользователь: " << result.getUserName() << "\n";
        out << "Баллы: " << result.getScore() << "\n";
        out << "Результат: " << result.getResultDescription() << "\n";
        out << "Дата и время: " << result.getDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
        out << "Ответы:\n";
        
        int i = 1;
        for (auto answer : result.getAnswers()) {
            QString answerStr;
            using enum AnswerType;
            switch(answer) {
                case YES: answerStr = "Да"; break;
                case NO: answerStr = "Нет"; break;
                case UNSURE: answerStr = "Не могу ответить"; break;
            }
            out << "  " << i << ". " << answerStr << "\n";
            i++;
        }
        
        out << "\n";
        file.close();
    } catch (const std::exception& e) {
        throw FileException("Ошибка при сохранении результата: " + QString(e.what()));
    }
}

TestResult FileManager::loadResultFromFile(const QString& filename) {
    try {
        validateFile(filename);
        
        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        in.setCodec("UTF-8");
        
        TestResult result;
        QString line;
        bool inAnswers = false;
        int testId = 0;
        QString userName;
        int score = 0;
        QDateTime dateTime;
        std::vector<AnswerType> answers;
        
        while (in.readLineInto(&line)) {
            QString trimmedLine = line.trimmed();
            
            // Пропускаем пустые строки и заголовок
            if (trimmedLine.isEmpty() || trimmedLine.startsWith("===")) {
                continue;
            }
            
            if (trimmedLine.startsWith("Результат:")) {
                // Пропускаем строку с описанием результата, она вычисляется автоматически
                continue;
            }

            if (trimmedLine == "Ответы:") {
                inAnswers = true;
                continue;
            }

            if (!inAnswers) {
                if (parseMetadataLine(trimmedLine, testId, userName, score, dateTime)) {
                    continue;
                }
                continue;
            }

            if (auto answer = parseAnswerLine(trimmedLine)) {
                answers.push_back(*answer);
            }
        }
        
        file.close();
        
        // Создаем объект результата
        result = TestResult(testId, userName, score);
        
        // Устанавливаем дату и время, если они были загружены из файла
        if (dateTime.isValid()) {
            result.setDateTime(dateTime);
        }
        
        // Добавляем ответы
        for (auto answer : answers) {
            result.addAnswer(answer);
        }
        
        return result;
    } catch (const std::exception& e) {
        throw FileException("Ошибка при загрузке результата: " + QString(e.what()));
    }
}

void FileManager::saveResultAutomatically(const TestResult& result) {
    try {
        QString answerDir = getTestAnswerDir();
        
        // Формируем имя файла: result_test{testId}_{userName}_{dateTime}.txt
        QString userName = result.getUserName();
        userName.replace(" ", "_");
        userName.replace("/", "_");
        userName.replace("\\", "_");
        userName.replace(":", "_");
        userName.replace("*", "_");
        userName.replace("?", "_");
        userName.replace("\"", "_");
        userName.replace("<", "_");
        userName.replace(">", "_");
        userName.replace("|", "_");
        
        QString dateTimeStr = result.getDateTime().toString("yyyy-MM-dd_hh-mm-ss");
        QDir dir(answerDir);
        QString filename = dir.absoluteFilePath(QString("result_test%1_%2_%3.txt")
            .arg(result.getTestId())
            .arg(userName)
            .arg(dateTimeStr));
        
        saveResultToFile(result, filename);
    } catch (const FileException& e) {
        qDebug() << "Ошибка автоматического сохранения результата:" << e.what();
    }
}

std::vector<TestResult> FileManager::loadAllResultsForTest(int testId) {
    std::vector<TestResult> results;
    
    try {
        QString answerDir = getTestAnswerDir();
        QDir dir(answerDir);
        
        if (!dir.exists()) {
            return results;
        }
        
        // Ищем все файлы результатов для конкретного теста
        // Формат имени: result_test{testId}_*.txt
        QString pattern = QString("result_test%1_*.txt").arg(testId);
        QStringList filters;
        filters << pattern;
        
        QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);
        
        for (const QFileInfo& fileInfo : fileList) {
            if (auto result = tryLoadResultFile(fileInfo)) {
                // Проверяем, что ID теста совпадает (на случай, если в имени файла что-то не так)
                if (result->getTestId() == testId) {
                    results.push_back(*result);
                }
            }
        }
    } catch (const FileException& e) {
        qDebug() << "Ошибка при загрузке результатов для теста:" << e.what();
    }
    
    return results;
}

void FileManager::saveStatisticsAutomatically(const Test& test) {
    try {
        QString statDir = getStatisticsDir();
        
        // Формируем имя файла: statistics_test{testId}.txt (один файл на тест)
        QDir dir(statDir);
        QString filename = dir.absoluteFilePath(QString("statistics_test%1.txt")
            .arg(test.getId()));
        
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw FileException("Не удалось открыть файл для записи: " + filename);
        }
        
        QTextStream out(&file);
        out.setCodec("UTF-8");
        
        // Собираем статистику по всем результатам теста
        Statistics statistics;
        statistics.collectStatistics(test);
        
        out << "=== Статистика по тесту ===\n";
        out << "ID теста: " << test.getId() << "\n";
        out << "Название теста: " << test.getName() << "\n";
        out << "Дата обновления: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
        out << "Всего прохождений: " << test.getResults().size() << "\n\n";
        
        const auto& allStats = statistics.getAllStats();
        
        if (allStats.empty()) {
            out << "Статистика отсутствует (нет результатов прохождения теста).\n";
            file.close();
            return;
        }
        
        out << "Статистика по вопросам:\n";
        out << "----------------------------------------------------------------\n";
        
        int totalYes = 0;
        int totalNo = 0;
        int totalUnsure = 0;
        int totalAnswers = 0;
        
        for (const auto& [questionIndex, stats] : allStats) {
            out << "Вопрос " << (questionIndex + 1) << ":\n";
            out << "  Всего ответов: " << stats.totalAnswers << "\n";
            out << "  Да: " << stats.yesCount << " (" << QString::number(stats.yesPercent, 'f', 1) << "%)\n";
            out << "  Нет: " << stats.noCount << " (" << QString::number(stats.noPercent, 'f', 1) << "%)\n";
            out << "  Не могу ответить: " << stats.unsureCount << " (" << QString::number(stats.unsurePercent, 'f', 1) << "%)\n";
            out << "----------------------------------------------------------------\n";
            
            totalYes += stats.yesCount;
            totalNo += stats.noCount;
            totalUnsure += stats.unsureCount;
            totalAnswers += stats.totalAnswers;
        }
        
        out << "\nОбщая статистика:\n";
        out << "Всего ответов: " << totalAnswers << "\n";
        if (totalAnswers > 0) {
            double yesPercent = (totalYes * 100.0) / totalAnswers;
            double noPercent = (totalNo * 100.0) / totalAnswers;
            double unsurePercent = (totalUnsure * 100.0) / totalAnswers;
            out << "Да: " << totalYes << " (" << QString::number(yesPercent, 'f', 1) << "%)\n";
            out << "Нет: " << totalNo << " (" << QString::number(noPercent, 'f', 1) << "%)\n";
            out << "Не могу ответить: " << totalUnsure << " (" << QString::number(unsurePercent, 'f', 1) << "%)\n";
        }
        
        file.close();
    } catch (const FileException& e) {
        qDebug() << "Ошибка автоматического сохранения статистики:" << e.what();
    }
}

std::vector<Test> FileManager::loadAllTests(const QString& directory) {
    std::vector<Test> tests;
    
    try {
        QDir dir(directory);
        if (!dir.exists()) {
            return tests;
        }
        
        QStringList filters;
        filters << "*.txt" << "*.test";
        
        QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);
        
        for (const QFileInfo& fileInfo : fileList) {
            if (auto test = tryLoadTestFile(fileInfo)) {
                tests.push_back(*test);
            }
        }
    } catch (const FileException&) {
        throw;
    } catch (const std::runtime_error& e) {
        throw FileException("Ошибка при загрузке тестов: " + QString(e.what()));
    }
    
    return tests;
}


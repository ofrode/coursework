#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../core/Test.h"
#include "../core/TestResult.h"
#include <QString>
#include <vector>
#include <fstream>
#include <stdexcept>

// Класс для работы с файлами (обработка исключений)
class FileManager {
public:
    // Получение корневой директории проекта (где находится CMakeLists.txt)
    static QString getProjectRootDir();
    
    // Получение пути к папке TestQuestionTxt в корне проекта
    static QString getTestQuestionDir();
    
    // Получение пути к папке TestAnswerTxt в корне проекта
    static QString getTestAnswerDir();
    
    // Получение пути к папке Stat в корне проекта (для статистики)
    static QString getStatisticsDir();
    
    // Сохранение теста в файл
    static void saveTestToFile(const Test& test, const QString& filename);
    
    // Загрузка теста из файла
    static Test loadTestFromFile(const QString& filename);
    
    // Сохранение результата теста
    static void saveResultToFile(const TestResult& result, const QString& filename);
    
    // Загрузка результата теста из файла
    static TestResult loadResultFromFile(const QString& filename);
    
    // Автоматическое сохранение результата теста (создает уникальное имя файла)
    static void saveResultAutomatically(const TestResult& result);
    
    // Загрузка всех результатов для конкретного теста из TestAnswerTxt
    static std::vector<TestResult> loadAllResultsForTest(int testId);
    
    // Автоматическое сохранение статистики теста (один файл на тест)
    static void saveStatisticsAutomatically(const Test& test);
    
    // Загрузка всех тестов из директории
    static std::vector<Test> loadAllTests(const QString& directory);
    
private:
    static void validateFile(const QString& filename);
};

// Исключение для работы с файлами
class FileException : public std::runtime_error {
public:
    explicit FileException(const QString& message) 
        : std::runtime_error(message.toStdString()) {}
};

#endif // FILEMANAGER_H


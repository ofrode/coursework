#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../core/Test.h"
#include "../core/TestResult.h"
#include <QString>
#include <vector>
#include <fstream>
#include <stdexcept>

class FileManager {
public:
    static QString getProjectRootDir();
    
    static QString getTestQuestionDir();
    
    static QString getTestAnswerDir();
    
    static QString getStatisticsDir();
    
    static void saveTestToFile(const Test& test, const QString& filename);
    
    static Test loadTestFromFile(const QString& filename);
    
    static void saveResultToFile(const TestResult& result, const QString& filename);
    
    static TestResult loadResultFromFile(const QString& filename);
    
    static void saveResultAutomatically(const TestResult& result);
    
    static std::vector<TestResult> loadAllResultsForTest(int testId);
    
    static void saveStatisticsAutomatically(const Test& test);
    
    static std::vector<Test> loadAllTests(const QString& directory);
    
private:
    static void validateFile(const QString& filename);
};

class FileException : public std::runtime_error {
public:
    explicit FileException(const QString& message) 
        : std::runtime_error(message.toStdString()) {}
};

#endif


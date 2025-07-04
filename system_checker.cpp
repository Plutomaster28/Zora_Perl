#include "system_checker.h"
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

SystemChecker::SystemChecker(QObject *parent) : QObject(parent) {
    m_zoraPerlPath = getZoraPerlPath();
    m_configPath = m_zoraPerlPath + "/etc/config.json";
}

bool SystemChecker::isSystemConfigured() {
    qDebug() << "Checking if system is configured...";
    qDebug() << "ZoraPerl path:" << m_zoraPerlPath;
    qDebug() << "Config path:" << m_configPath;
    
    return checkZoraPerlDirectory() && checkConfigFile();
}

bool SystemChecker::checkZoraPerlDirectory() {
    QDir zoraPerlDir(m_zoraPerlPath);
    if (!zoraPerlDir.exists()) {
        qDebug() << "ZoraPerl directory does not exist:" << m_zoraPerlPath;
        return false;
    }
    
    // Check required subdirectories
    QStringList requiredDirs = {"bin", "compat", "etc", "system", "users"};
    for (const QString &dir : requiredDirs) {
        QDir subDir(zoraPerlDir.absoluteFilePath(dir));
        if (!subDir.exists()) {
            qDebug() << "Required subdirectory missing:" << dir;
            return false;
        }
    }
    
    qDebug() << "ZoraPerl directory structure is valid";
    return true;
}

bool SystemChecker::checkConfigFile() {
    QFile configFile(m_configPath);
    if (!configFile.exists()) {
        qDebug() << "Config file does not exist:" << m_configPath;
        return false;
    }
    
    // Try to parse the config file
    if (!configFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot read config file:" << m_configPath;
        return false;
    }
    
    QByteArray data = configFile.readAll();
    configFile.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Invalid JSON in config file:" << error.errorString();
        return false;
    }
    
    QJsonObject config = doc.object();
    
    // Check if config has required fields
    if (!config.contains("username") || !config.contains("language") || !config.contains("setupVersion")) {
        qDebug() << "Config file missing required fields";
        return false;
    }
    
    qDebug() << "Config file is valid";
    return true;
}

QString SystemChecker::getZoraPerlPath() {
    // Get the application directory and navigate to find ZoraPerl
    QDir appDir(QCoreApplication::applicationDirPath());
    qDebug() << "Application directory:" << appDir.absolutePath();
    
    // Try different approaches to find ZoraPerl directory
    
    // Method 1: Check if we're already in ZoraPerl directory
    if (appDir.dirName() == "ZoraPerl") {
        return appDir.absolutePath();
    }
    
    // Method 2: Check if ZoraPerl is a sibling directory
    QDir parentDir = appDir;
    if (parentDir.cdUp()) {
        QDir zoraPerlDir(parentDir.absoluteFilePath("ZoraPerl"));
        if (zoraPerlDir.exists()) {
            return zoraPerlDir.absolutePath();
        }
    }
    
    // Method 3: Search up the directory tree
    QDir searchDir = appDir;
    for (int i = 0; i < 5; ++i) {
        QDir zoraPerlDir(searchDir.absoluteFilePath("ZoraPerl"));
        if (zoraPerlDir.exists()) {
            return zoraPerlDir.absolutePath();
        }
        if (!searchDir.cdUp()) {
            break;
        }
    }
    
    // Method 4: Use current working directory
    QDir currentDir = QDir::current();
    QDir zoraPerlDir(currentDir.absoluteFilePath("ZoraPerl"));
    if (zoraPerlDir.exists()) {
        return zoraPerlDir.absolutePath();
    }
    
    // Fallback: relative to application directory
    return appDir.absoluteFilePath("ZoraPerl");
}

bool SystemChecker::runOnboarding() {
    QString onboardingPath = findOnboardingExecutable();
    if (onboardingPath.isEmpty()) {
        qDebug() << "Onboarding executable not found";
        return false;
    }
    
    qDebug() << "Starting onboarding process:" << onboardingPath;
    
    // Start the onboarding process
    QProcess process;
    process.setProgram(onboardingPath);
    process.setWorkingDirectory(QFileInfo(onboardingPath).absolutePath());
    
    // Start the process and wait for it to finish
    if (!process.startDetached()) {
        qDebug() << "Failed to start onboarding process";
        return false;
    }
    
    return true;
}

QString SystemChecker::findOnboardingExecutable() {
    // Search for ZoraPerl_Onboarding.exe in various locations
    QDir appDir(QCoreApplication::applicationDirPath());
    
    // List of possible locations
    QStringList searchPaths = {
        appDir.absoluteFilePath("ZoraPerl_Onboarding/ZoraPerl_Onboarding.exe"),
        appDir.absoluteFilePath("ZoraPerl_Onboarding/build/ZoraPerl_Onboarding.exe"),
        appDir.absoluteFilePath("../ZoraPerl_Onboarding/ZoraPerl_Onboarding.exe"),
        appDir.absoluteFilePath("../ZoraPerl_Onboarding/build/ZoraPerl_Onboarding.exe")
    };
    
    // Also search in parent directories
    QDir searchDir = appDir;
    for (int i = 0; i < 3; ++i) {
        searchPaths.append(searchDir.absoluteFilePath("ZoraPerl_Onboarding/ZoraPerl_Onboarding.exe"));
        searchPaths.append(searchDir.absoluteFilePath("ZoraPerl_Onboarding/build/ZoraPerl_Onboarding.exe"));
        if (!searchDir.cdUp()) {
            break;
        }
    }
    
    // Check each path
    for (const QString &path : searchPaths) {
        QFile file(path);
        if (file.exists()) {
            qDebug() << "Found onboarding executable at:" << path;
            return path;
        }
    }
    
    return QString();
}
#include "softlanding.h"
#include <QVBoxLayout>
#include <QFont>
#include <QPalette>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

SoftLanding::SoftLanding(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #101010;");  // Dark clean bg

    greeting = new QLabel("Welcome to Zora Perl", this);
    greeting->setAlignment(Qt::AlignCenter);

    QFont font("Segoe UI", 32, QFont::Medium);
    greeting->setFont(font);
    greeting->setStyleSheet("color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(greeting);
    layout->addStretch();
    setLayout(layout);

    // Fade in animation (like macOS Setup)
    auto *opacity = new QGraphicsOpacityEffect(greeting);
    greeting->setGraphicsEffect(opacity);

    auto *fade = new QPropertyAnimation(opacity, "opacity");
    fade->setDuration(1500);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->start(QPropertyAnimation::DeleteWhenStopped);

    // Start directory setup after fade completes
    QTimer::singleShot(2000, this, SLOT(setupDirectories()));
}

void SoftLanding::setupDirectories() {
    greeting->setText("Setting up directories...");
    
    // Find the main Zora_Perl directory by backtracking from executable location
    QString executablePath = QCoreApplication::applicationDirPath();
    QDir currentDir(executablePath);
    
    qDebug() << "Starting from executable path:" << executablePath;
    
    // Navigate up until we find the main Zora_Perl directory
    QString zoraMainPath;
    while (currentDir.cdUp()) {
        qDebug() << "Checking directory:" << currentDir.absolutePath();
        if (currentDir.dirName() == "Zora_Perl") {
            zoraMainPath = currentDir.absolutePath();
            qDebug() << "Found Zora_Perl main directory:" << zoraMainPath;
            break;
        }
    }
    
    if (zoraMainPath.isEmpty()) {
        qDebug() << "Could not find Zora_Perl main directory, using current path";
        zoraMainPath = executablePath + "/../../";  // Fallback
    }
    
    // Create ZoraPerl directory and subdirectories
    QDir mainDir(zoraMainPath);
    QString zoraPerlPath = mainDir.absoluteFilePath("ZoraPerl");
    
    QDir zoraPerlDir(zoraPerlPath);
    if (!zoraPerlDir.exists()) {
        qDebug() << "Creating ZoraPerl directory structure...";
        
        // Create main ZoraPerl directory
        if (!mainDir.mkdir("ZoraPerl")) {
            qDebug() << "Failed to create ZoraPerl directory";
        } else {
            qDebug() << "Created ZoraPerl directory at:" << zoraPerlPath;
        }
        
        // Create subdirectories
        QStringList subdirs = {"bin", "compat", "etc", "system", "users"};
        for (const QString &subdir : subdirs) {
            if (!zoraPerlDir.mkdir(subdir)) {
                qDebug() << "Failed to create" << subdir << "directory";
            } else {
                qDebug() << "Created" << subdir << "directory";
            }
        }
    } else {
        qDebug() << "ZoraPerl directory structure already exists";
        
        // Check if all subdirectories exist, create missing ones
        QStringList subdirs = {"bin", "compat", "etc", "system", "users"};
        for (const QString &subdir : subdirs) {
            QString subdirPath = zoraPerlDir.absoluteFilePath(subdir);
            if (!QDir(subdirPath).exists()) {
                if (!zoraPerlDir.mkdir(subdir)) {
                    qDebug() << "Failed to create missing" << subdir << "directory";
                } else {
                    qDebug() << "Created missing" << subdir << "directory";
                }
            }
        }
    }
    
    // Show completion message
    greeting->setText("Ready to begin setup...");
    
    // Wait a bit longer before transitioning to setup wizard
    QTimer::singleShot(2500, this, SLOT(goToSetup()));
}

void SoftLanding::goToSetup() {
    greeting->setText("Starting Setup...");
    QFont font("Segoe UI", 24, QFont::Normal);
    greeting->setFont(font);
    
    // Small delay before emitting finished signal for smooth transition
    QTimer::singleShot(500, this, SIGNAL(finished()));
}

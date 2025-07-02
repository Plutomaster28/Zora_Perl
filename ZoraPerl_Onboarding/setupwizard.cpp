#include "setupwizard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include <QComboBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

QWidget* SetupWizard::createLanguageRegionStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Select Language and Region:");
    
    languageCombo = new QComboBox;
    languageCombo->addItem("English (US)");
    languageCombo->addItem("Español (Latinoamérica)");
    languageCombo->addItem("Français (Canada)");
    
    regionCombo = new QComboBox;
    regionCombo->addItem("United States");
    regionCombo->addItem("Mexico");
    regionCombo->addItem("Canada");

    layout->addWidget(label);
    layout->addWidget(languageCombo);
    layout->addWidget(regionCombo);
    return step;
}

QWidget* SetupWizard::createKeyboardStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Choose Keyboard Layout:");
    
    keyboardCombo = new QComboBox;
    keyboardCombo->addItem("US QWERTY");
    keyboardCombo->addItem("Latin American QWERTY");
    keyboardCombo->addItem("Canadian Multilingual");

    layout->addWidget(label);
    layout->addWidget(keyboardCombo);
    return step;
}

QWidget* SetupWizard::createAppearanceStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Choose Appearance:");
    
    lightModeRadio = new QRadioButton("Light Mode");
    darkModeRadio = new QRadioButton("Dark Mode");
    lightModeRadio->setChecked(true);

    layout->addWidget(label);
    layout->addWidget(lightModeRadio);
    layout->addWidget(darkModeRadio);
    return step;
}

QWidget* SetupWizard::createUserAccountStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Create User Account:");
    
    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Enter username");
    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Password (optional)");
    passwordEdit->setEchoMode(QLineEdit::Password);

    layout->addWidget(label);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    return step;
}

QWidget* SetupWizard::createNetworkStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Connect to a Network:");
    
    networksList = new QListWidget;
    networksList->addItem("WiFi-Home");
    networksList->addItem("ZoraNet");
    networksList->addItem("Skip for now");
    
    layout->addWidget(label);
    layout->addWidget(networksList);
    return step;
}

QWidget* SetupWizard::createDevToggleStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Do You Code?");
    
    devToolsCheckbox = new QCheckBox("Enable developer tools and terminal");
    
    layout->addWidget(label);
    layout->addWidget(devToolsCheckbox);
    return step;
}

QWidget* SetupWizard::createAppSuggestionsStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Choose Recommended Apps:");
    
    webBrowserCheckbox = new QCheckBox("Web Browser");
    musicPlayerCheckbox = new QCheckBox("Music Player");
    devToolsAppCheckbox = new QCheckBox("Dev Tools");
    officeSuiteCheckbox = new QCheckBox("Office Suite");

    layout->addWidget(label);
    layout->addWidget(webBrowserCheckbox);
    layout->addWidget(musicPlayerCheckbox);
    layout->addWidget(devToolsAppCheckbox);
    layout->addWidget(officeSuiteCheckbox);
    return step;
}

QWidget* SetupWizard::createSummaryStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("You’re Ready to Go!");
    QLabel *sub = new QLabel("Click 'Next' to begin using Zora Perl.");
    layout->addWidget(label);
    layout->addWidget(sub);
    return step;
}

SetupWizard::SetupWizard(QWidget *parent) : QWidget(parent), currentIndex(0) {
    stack = new QStackedWidget(this);
    stack->addWidget(createLanguageRegionStep());
    stack->addWidget(createKeyboardStep());
    stack->addWidget(createAppearanceStep());
    stack->addWidget(createUserAccountStep());
    stack->addWidget(createNetworkStep());
    stack->addWidget(createDevToggleStep());
    stack->addWidget(createAppSuggestionsStep());
    stack->addWidget(createSummaryStep());

    nextButton = new QPushButton("Next");
    backButton = new QPushButton("Back");
    backButton->setEnabled(false);

    connect(nextButton, &QPushButton::clicked, this, &SetupWizard::nextStep);
    connect(backButton, &QPushButton::clicked, this, &SetupWizard::previousStep);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(nextButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stack);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void SetupWizard::nextStep() {
    if (currentIndex < stack->count() - 1) {
        currentIndex++;
        stack->setCurrentIndex(currentIndex);
        backButton->setEnabled(true);
        if (currentIndex == stack->count() - 1) {
            nextButton->setText("Finish");
        }
    } else {
        finishSetup();
    }
}

void SetupWizard::previousStep() {
    if (currentIndex > 0) {
        currentIndex--;
        stack->setCurrentIndex(currentIndex);
        nextButton->setText("Next");
        if (currentIndex == 0) {
            backButton->setEnabled(false);
        }
    }
}

void SetupWizard::finishSetup() {
    collectUserData();
    saveConfigToFile();
    close();
}

void SetupWizard::collectUserData() {
    // Language and Region
    configData["language"] = languageCombo->currentText();
    configData["region"] = regionCombo->currentText();
    
    // Keyboard Layout
    configData["keyboardLayout"] = keyboardCombo->currentText();
    
    // Appearance
    configData["theme"] = lightModeRadio->isChecked() ? "light" : "dark";
    
    // User Account
    configData["username"] = usernameEdit->text();
    configData["hasPassword"] = !passwordEdit->text().isEmpty();
    // Note: We don't save the actual password for security reasons
    
    // Network
    QListWidgetItem *selectedNetwork = networksList->currentItem();
    if (selectedNetwork) {
        configData["selectedNetwork"] = selectedNetwork->text();
    } else {
        configData["selectedNetwork"] = "None selected";
    }
    
    // Developer Tools
    configData["developerMode"] = devToolsCheckbox->isChecked();
    
    // App Suggestions
    QJsonObject apps;
    apps["webBrowser"] = webBrowserCheckbox->isChecked();
    apps["musicPlayer"] = musicPlayerCheckbox->isChecked();
    apps["devTools"] = devToolsAppCheckbox->isChecked();
    apps["officeSuite"] = officeSuiteCheckbox->isChecked();
    configData["recommendedApps"] = apps;
    
    // Add metadata
    configData["setupVersion"] = "1.0";
    configData["setupDate"] = QDateTime::currentDateTime().toString(Qt::ISODate);
}

void SetupWizard::saveConfigToFile() {
    // Get the application's directory and navigate to find ZoraPerl/etc
    QDir appDir(QCoreApplication::applicationDirPath());
    qDebug() << "Application directory:" << appDir.absolutePath();
    
    // Navigate up to find the ZoraPerl directory
    QString configPath;
    QDir searchDir = appDir;
    
    // Try different levels up to find ZoraPerl/etc
    for (int i = 0; i < 5; ++i) {
        qDebug() << "Searching in:" << searchDir.absolutePath();
        
        QDir zoraDir(searchDir.absoluteFilePath("ZoraPerl/etc"));
        qDebug() << "Checking path:" << zoraDir.absolutePath();
        
        if (zoraDir.exists()) {
            configPath = zoraDir.absolutePath();
            qDebug() << "Found ZoraPerl/etc at:" << configPath;
            break;
        }
        
        // Also check if we're already in ZoraPerl and etc exists
        QDir etcDir(searchDir.absoluteFilePath("etc"));
        if (etcDir.exists() && searchDir.dirName() == "ZoraPerl") {
            configPath = etcDir.absolutePath();
            qDebug() << "Found etc directory in ZoraPerl at:" << configPath;
            break;
        }
        
        if (!searchDir.cdUp()) {
            qDebug() << "Cannot navigate up further";
            break; // Can't go up anymore
        }
    }
    
    // If not found, try relative to current working directory
    if (configPath.isEmpty()) {
        qDebug() << "Trying current working directory approach";
        QDir currentDir = QDir::current();
        qDebug() << "Current working directory:" << currentDir.absolutePath();
        
        for (int i = 0; i < 5; ++i) {
            QDir zoraDir(currentDir.absoluteFilePath("ZoraPerl/etc"));
            qDebug() << "Checking path:" << zoraDir.absolutePath();
            
            if (zoraDir.exists()) {
                configPath = zoraDir.absolutePath();
                qDebug() << "Found ZoraPerl/etc at:" << configPath;
                break;
            }
            if (!currentDir.cdUp()) {
                break;
            }
        }
    }
    
    // Create the directory if it doesn't exist
    QDir configDir(configPath);
    if (configPath.isEmpty() || !configDir.exists()) {
        if (configPath.isEmpty()) {
            // Fallback: create relative to application directory
            configPath = appDir.absoluteFilePath("../ZoraPerl/etc");
            configDir = QDir(configPath);
        }
        
        if (!configDir.mkpath(".")) {
            QMessageBox::warning(this, "Error", "Failed to create configuration directory: " + configPath);
            return;
        }
    }
    
    // Create the full file path - use config.json as requested
    QString configFilePath = configDir.absoluteFilePath("config.json");
    
    // Create JSON document
    QJsonDocument doc(configData);
    
    // Save to file
    QFile file(configFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Configuration saved to:" << configFilePath;
        QMessageBox::information(this, "Success", "Configuration saved successfully to:\n" + configFilePath);
    } else {
        QMessageBox::warning(this, "Error", "Failed to save configuration file: " + configFilePath);
    }
}
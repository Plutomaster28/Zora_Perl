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
#include <QButtonGroup>
#include <QFrame>

QWidget* SetupWizard::createLanguageRegionStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    // Title
    QLabel *title = new QLabel("Select Language and Region");
    title->setStyleSheet("font-size: 28px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    // Language selection with radio buttons
    QLabel *langLabel = new QLabel("Language:");
    langLabel->setStyleSheet("font-size: 16px; color: #424245; font-weight: 500; margin-top: 20px;");
    
    QVBoxLayout *langLayout = new QVBoxLayout;
    langLayout->setSpacing(15);
    
    // Language radio buttons
    QButtonGroup *langGroup = new QButtonGroup(this);
    
    QRadioButton *englishRadio = new QRadioButton("English (US)");
    QRadioButton *spanishRadio = new QRadioButton("Español (Latinoamérica)");
    QRadioButton *frenchRadio = new QRadioButton("Français (Canada)");
    
    englishRadio->setChecked(true);
    
    QString radioStyle = "QRadioButton { font-size: 16px; color: #1d1d1f; spacing: 10px; padding: 8px; }"
                         "QRadioButton::indicator { width: 20px; height: 20px; }"
                         "QRadioButton::indicator::unchecked { border: 2px solid #d2d2d7; border-radius: 10px; background-color: white; }"
                         "QRadioButton::indicator::checked { border: 2px solid #007AFF; border-radius: 10px; background-color: #007AFF; }"
                         "QRadioButton::indicator::checked::hover { background-color: #0056d3; }";
    
    englishRadio->setStyleSheet(radioStyle);
    spanishRadio->setStyleSheet(radioStyle);
    frenchRadio->setStyleSheet(radioStyle);
    
    langGroup->addButton(englishRadio, 0);
    langGroup->addButton(spanishRadio, 1);
    langGroup->addButton(frenchRadio, 2);
    
    // Store reference for data collection
    languageButtonGroup = langGroup;
    
    langLayout->addWidget(englishRadio);
    langLayout->addWidget(spanishRadio);
    langLayout->addWidget(frenchRadio);
    
    // Region selection
    QLabel *regionLabel = new QLabel("Region:");
    regionLabel->setStyleSheet("font-size: 16px; color: #424245; font-weight: 500; margin-top: 30px;");
    
    QVBoxLayout *regionLayout = new QVBoxLayout;
    regionLayout->setSpacing(15);
    
    QButtonGroup *regionGroup = new QButtonGroup(this);
    
    QRadioButton *usRadio = new QRadioButton("United States");
    QRadioButton *mxRadio = new QRadioButton("Mexico");
    QRadioButton *caRadio = new QRadioButton("Canada");
    
    usRadio->setChecked(true);
    
    usRadio->setStyleSheet(radioStyle);
    mxRadio->setStyleSheet(radioStyle);
    caRadio->setStyleSheet(radioStyle);
    
    regionGroup->addButton(usRadio, 0);
    regionGroup->addButton(mxRadio, 1);
    regionGroup->addButton(caRadio, 2);
    
    // Store reference for data collection
    regionButtonGroup = regionGroup;
    
    regionLayout->addWidget(usRadio);
    regionLayout->addWidget(mxRadio);
    regionLayout->addWidget(caRadio);
    
    layout->addWidget(title);
    layout->addWidget(langLabel);
    layout->addLayout(langLayout);
    layout->addWidget(regionLabel);
    layout->addLayout(regionLayout);
    layout->addStretch();
    
    return step;
}

QWidget* SetupWizard::createKeyboardStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel *title = new QLabel("Choose Keyboard Layout");
    title->setStyleSheet("font-size: 28px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    QVBoxLayout *keyboardLayout = new QVBoxLayout;
    keyboardLayout->setSpacing(15);
    
    QButtonGroup *keyboardGroup = new QButtonGroup(this);
    
    QRadioButton *usRadio = new QRadioButton("US QWERTY");
    QRadioButton *latinRadio = new QRadioButton("Latin American QWERTY");
    QRadioButton *canadianRadio = new QRadioButton("Canadian Multilingual");
    
    usRadio->setChecked(true);
    
    QString radioStyle = "QRadioButton { font-size: 16px; color: #1d1d1f; spacing: 10px; padding: 8px; }"
                         "QRadioButton::indicator { width: 20px; height: 20px; }"
                         "QRadioButton::indicator::unchecked { border: 2px solid #d2d2d7; border-radius: 10px; background-color: white; }"
                         "QRadioButton::indicator::checked { border: 2px solid #007AFF; border-radius: 10px; background-color: #007AFF; }";
    
    usRadio->setStyleSheet(radioStyle);
    latinRadio->setStyleSheet(radioStyle);
    canadianRadio->setStyleSheet(radioStyle);
    
    keyboardGroup->addButton(usRadio, 0);
    keyboardGroup->addButton(latinRadio, 1);
    keyboardGroup->addButton(canadianRadio, 2);
    
    keyboardButtonGroup = keyboardGroup;
    
    keyboardLayout->addWidget(usRadio);
    keyboardLayout->addWidget(latinRadio);
    keyboardLayout->addWidget(canadianRadio);
    
    layout->addWidget(title);
    layout->addLayout(keyboardLayout);
    layout->addStretch();
    
    return step;
}

QWidget* SetupWizard::createAppearanceStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel *title = new QLabel("Choose Appearance");
    title->setStyleSheet("font-size: 28px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    QVBoxLayout *appearanceLayout = new QVBoxLayout;
    appearanceLayout->setSpacing(15);
    
    QButtonGroup *appearanceGroup = new QButtonGroup(this);
    
    lightModeRadio = new QRadioButton("Light Mode");
    darkModeRadio = new QRadioButton("Dark Mode");
    
    lightModeRadio->setChecked(true);
    
    QString radioStyle = "QRadioButton { font-size: 16px; color: #1d1d1f; spacing: 10px; padding: 8px; }"
                         "QRadioButton::indicator { width: 20px; height: 20px; }"
                         "QRadioButton::indicator::unchecked { border: 2px solid #d2d2d7; border-radius: 10px; background-color: white; }"
                         "QRadioButton::indicator::checked { border: 2px solid #007AFF; border-radius: 10px; background-color: #007AFF; }";
    
    lightModeRadio->setStyleSheet(radioStyle);
    darkModeRadio->setStyleSheet(radioStyle);
    
    appearanceGroup->addButton(lightModeRadio, 0);
    appearanceGroup->addButton(darkModeRadio, 1);
    
    appearanceLayout->addWidget(lightModeRadio);
    appearanceLayout->addWidget(darkModeRadio);
    
    layout->addWidget(title);
    layout->addLayout(appearanceLayout);
    layout->addStretch();
    
    return step;
}

QWidget* SetupWizard::createUserAccountStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel *title = new QLabel("Create User Account");
    title->setStyleSheet("font-size: 28px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    QVBoxLayout *formLayout = new QVBoxLayout;
    formLayout->setSpacing(20);
    
    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Enter username");
    usernameEdit->setStyleSheet("QLineEdit { font-size: 16px; padding: 12px; border: 2px solid #d2d2d7; border-radius: 8px; background-color: white; }"
                               "QLineEdit:focus { border-color: #007AFF; }");
    
    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Password (optional)");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("QLineEdit { font-size: 16px; padding: 12px; border: 2px solid #d2d2d7; border-radius: 8px; background-color: white; }"
                               "QLineEdit:focus { border-color: #007AFF; }");
    
    formLayout->addWidget(usernameEdit);
    formLayout->addWidget(passwordEdit);
    
    layout->addWidget(title);
    layout->addLayout(formLayout);
    layout->addStretch();
    
    return step;
}

QWidget* SetupWizard::createNetworkStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel *title = new QLabel("Connect to Network");
    title->setStyleSheet("font-size: 28px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    QVBoxLayout *networkLayout = new QVBoxLayout;
    networkLayout->setSpacing(15);
    
    QButtonGroup *networkGroup = new QButtonGroup(this);
    
    QRadioButton *wifiHomeRadio = new QRadioButton("WiFi-Home");
    QRadioButton *zoraNetRadio = new QRadioButton("ZoraNet");
    QRadioButton *skipRadio = new QRadioButton("Skip for now");
    
    skipRadio->setChecked(true);
    
    QString radioStyle = "QRadioButton { font-size: 16px; color: #1d1d1f; spacing: 10px; padding: 8px; }"
                         "QRadioButton::indicator { width: 20px; height: 20px; }"
                         "QRadioButton::indicator::unchecked { border: 2px solid #d2d2d7; border-radius: 10px; background-color: white; }"
                         "QRadioButton::indicator::checked { border: 2px solid #007AFF; border-radius: 10px; background-color: #007AFF; }";
    
    wifiHomeRadio->setStyleSheet(radioStyle);
    zoraNetRadio->setStyleSheet(radioStyle);
    skipRadio->setStyleSheet(radioStyle);
    
    networkGroup->addButton(wifiHomeRadio, 0);
    networkGroup->addButton(zoraNetRadio, 1);
    networkGroup->addButton(skipRadio, 2);
    
    networkButtonGroup = networkGroup;
    
    networkLayout->addWidget(wifiHomeRadio);
    networkLayout->addWidget(zoraNetRadio);
    networkLayout->addWidget(skipRadio);
    
    layout->addWidget(title);
    layout->addLayout(networkLayout);
    layout->addStretch();
    
    return step;
}

QWidget* SetupWizard::createDevToggleStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel *title = new QLabel("Developer Tools");
    title->setStyleSheet("font-size: 28px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    QLabel *subtitle = new QLabel("Do you need developer tools and terminal access?");
    subtitle->setStyleSheet("font-size: 16px; color: #424245; margin-bottom: 30px;");
    subtitle->setAlignment(Qt::AlignCenter);
    
    devToolsCheckbox = new QCheckBox("Enable developer tools and terminal");
    devToolsCheckbox->setStyleSheet("QCheckBox { font-size: 16px; color: #1d1d1f; spacing: 10px; padding: 8px; }"
                                   "QCheckBox::indicator { width: 20px; height: 20px; }"
                                   "QCheckBox::indicator::unchecked { border: 2px solid #d2d2d7; border-radius: 4px; background-color: white; }"
                                   "QCheckBox::indicator::checked { border: 2px solid #007AFF; border-radius: 4px; background-color: #007AFF; }"
                                   "QCheckBox::indicator::checked::hover { background-color: #0056d3; }");
    
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addWidget(devToolsCheckbox);
    layout->addStretch();
    
    return step;
}

QWidget* SetupWizard::createAppSuggestionsStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel *title = new QLabel("Choose Apps");
    title->setStyleSheet("font-size: 28px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    QLabel *subtitle = new QLabel("Select the apps you'd like to install:");
    subtitle->setStyleSheet("font-size: 16px; color: #424245; margin-bottom: 30px;");
    subtitle->setAlignment(Qt::AlignCenter);
    
    QVBoxLayout *appsLayout = new QVBoxLayout;
    appsLayout->setSpacing(15);
    
    webBrowserCheckbox = new QCheckBox("Web Browser");
    musicPlayerCheckbox = new QCheckBox("Music Player");
    devToolsAppCheckbox = new QCheckBox("Development Tools");
    officeSuiteCheckbox = new QCheckBox("Office Suite");
    
    QString checkboxStyle = "QCheckBox { font-size: 16px; color: #1d1d1f; spacing: 10px; padding: 8px; }"
                           "QCheckBox::indicator { width: 20px; height: 20px; }"
                           "QCheckBox::indicator::unchecked { border: 2px solid #d2d2d7; border-radius: 4px; background-color: white; }"
                           "QCheckBox::indicator::checked { border: 2px solid #007AFF; border-radius: 4px; background-color: #007AFF; }"
                           "QCheckBox::indicator::checked::hover { background-color: #0056d3; }";
    
    webBrowserCheckbox->setStyleSheet(checkboxStyle);
    musicPlayerCheckbox->setStyleSheet(checkboxStyle);
    devToolsAppCheckbox->setStyleSheet(checkboxStyle);
    officeSuiteCheckbox->setStyleSheet(checkboxStyle);
    
    // Check some by default
    webBrowserCheckbox->setChecked(true);
    musicPlayerCheckbox->setChecked(true);
    
    appsLayout->addWidget(webBrowserCheckbox);
    appsLayout->addWidget(musicPlayerCheckbox);
    appsLayout->addWidget(devToolsAppCheckbox);
    appsLayout->addWidget(officeSuiteCheckbox);
    
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addLayout(appsLayout);
    layout->addStretch();
    
    return step;
}

QWidget* SetupWizard::createSummaryStep() {
    QWidget *step = new QWidget;
    step->setStyleSheet("background-color: #f8f9fa; padding: 60px;");
    
    QVBoxLayout *layout = new QVBoxLayout(step);
    layout->setSpacing(30);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel *title = new QLabel("You're All Set!");
    title->setStyleSheet("font-size: 32px; font-weight: 300; color: #1d1d1f; margin-bottom: 20px;");
    title->setAlignment(Qt::AlignCenter);
    
    QLabel *subtitle = new QLabel("ZoraPerl is ready to use.\nClick Continue to start exploring.");
    subtitle->setStyleSheet("font-size: 18px; color: #424245; line-height: 1.5;");
    subtitle->setAlignment(Qt::AlignCenter);
    
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addStretch();
    
    return step;
}

SetupWizard::SetupWizard(QWidget *parent) : QWidget(parent), currentIndex(0) {
    setStyleSheet("background-color: #ffffff;");
    
    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create sidebar
    QWidget *sidebar = new QWidget;
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet("background-color: #f5f5f7; border-right: 1px solid #d2d2d7;");
    
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(0);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    
    // Add logo/title to sidebar
    QLabel *logoLabel = new QLabel("ZoraPerl");
    logoLabel->setStyleSheet("font-size: 18px; font-weight: 600; color: #1d1d1f; padding: 30px 20px 20px 20px;");
    logoLabel->setAlignment(Qt::AlignLeft);
    sidebarLayout->addWidget(logoLabel);
    
    // Add progress steps
    QStringList steps = {"Language", "Keyboard", "Appearance", "Account", "Network", "Developer", "Apps", "Finish"};
    for (int i = 0; i < steps.size(); ++i) {
        QLabel *stepLabel = new QLabel(QString("%1. %2").arg(i + 1).arg(steps[i]));
        stepLabel->setStyleSheet("font-size: 14px; color: #86868b; padding: 10px 20px; margin: 0;");
        stepLabel->setProperty("stepIndex", i);
        sidebarLayout->addWidget(stepLabel);
        stepLabels.append(stepLabel);
    }
    
    sidebarLayout->addStretch();
    
    // Create main content area
    QWidget *contentArea = new QWidget;
    QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create stacked widget for steps
    stack = new QStackedWidget;
    stack->addWidget(createLanguageRegionStep());
    stack->addWidget(createKeyboardStep());
    stack->addWidget(createAppearanceStep());
    stack->addWidget(createUserAccountStep());
    stack->addWidget(createNetworkStep());
    stack->addWidget(createDevToggleStep());
    stack->addWidget(createAppSuggestionsStep());
    stack->addWidget(createSummaryStep());
    
    // Create button area
    QWidget *buttonArea = new QWidget;
    buttonArea->setStyleSheet("background-color: #fafafa; border-top: 1px solid #d2d2d7;");
    buttonArea->setFixedHeight(80);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonArea);
    buttonLayout->setContentsMargins(30, 20, 30, 20);
    
    backButton = new QPushButton("Back");
    backButton->setStyleSheet("QPushButton { font-size: 16px; color: #007AFF; background-color: transparent; border: none; padding: 10px 20px; }"
                             "QPushButton:hover { background-color: #f0f0f0; border-radius: 8px; }"
                             "QPushButton:disabled { color: #c7c7cc; }");
    backButton->setEnabled(false);
    
    nextButton = new QPushButton("Continue");
    nextButton->setStyleSheet("QPushButton { font-size: 16px; color: white; background-color: #007AFF; border: none; padding: 10px 30px; border-radius: 8px; font-weight: 500; }"
                             "QPushButton:hover { background-color: #0056d3; }"
                             "QPushButton:pressed { background-color: #004bb5; }");
    
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(nextButton);
    
    contentLayout->addWidget(stack);
    contentLayout->addWidget(buttonArea);
    
    // Add to main layout
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentArea);
    
    // Connect signals
    connect(nextButton, &QPushButton::clicked, this, &SetupWizard::nextStep);
    connect(backButton, &QPushButton::clicked, this, &SetupWizard::previousStep);
    
    // Update initial step
    updateStepIndicator();
}

void SetupWizard::updateStepIndicator() {
    for (int i = 0; i < stepLabels.size(); ++i) {
        if (i == currentIndex) {
            stepLabels[i]->setStyleSheet("font-size: 14px; color: #007AFF; font-weight: 500; padding: 10px 20px; margin: 0;");
        } else if (i < currentIndex) {
            stepLabels[i]->setStyleSheet("font-size: 14px; color: #1d1d1f; padding: 10px 20px; margin: 0;");
        } else {
            stepLabels[i]->setStyleSheet("font-size: 14px; color: #86868b; padding: 10px 20px; margin: 0;");
        }
    }
}

void SetupWizard::nextStep() {
    if (currentIndex < stack->count() - 1) {
        currentIndex++;
        stack->setCurrentIndex(currentIndex);
        backButton->setEnabled(true);
        updateStepIndicator();
        
        if (currentIndex == stack->count() - 1) {
            nextButton->setText("Finish");
            nextButton->setStyleSheet("QPushButton { font-size: 16px; color: white; background-color: #34c759; border: none; padding: 10px 30px; border-radius: 8px; font-weight: 500; }"
                                     "QPushButton:hover { background-color: #2ea043; }"
                                     "QPushButton:pressed { background-color: #268038; }");
        }
    } else {
        finishSetup();
    }
}

void SetupWizard::previousStep() {
    if (currentIndex > 0) {
        currentIndex--;
        stack->setCurrentIndex(currentIndex);
        nextButton->setText("Continue");
        nextButton->setStyleSheet("QPushButton { font-size: 16px; color: white; background-color: #007AFF; border: none; padding: 10px 30px; border-radius: 8px; font-weight: 500; }"
                                 "QPushButton:hover { background-color: #0056d3; }"
                                 "QPushButton:pressed { background-color: #004bb5; }");
        updateStepIndicator();
        
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
    QStringList languages = {"English (US)", "Español (Latinoamérica)", "Français (Canada)"};
    QStringList regions = {"United States", "Mexico", "Canada"};
    
    configData["language"] = languages[languageButtonGroup->checkedId()];
    configData["region"] = regions[regionButtonGroup->checkedId()];
    
    // Keyboard Layout
    QStringList keyboards = {"US QWERTY", "Latin American QWERTY", "Canadian Multilingual"};
    configData["keyboardLayout"] = keyboards[keyboardButtonGroup->checkedId()];
    
    // Appearance
    configData["theme"] = lightModeRadio->isChecked() ? "light" : "dark";
    
    // User Account
    configData["username"] = usernameEdit->text();
    configData["hasPassword"] = !passwordEdit->text().isEmpty();
    
    // Network
    QStringList networks = {"WiFi-Home", "ZoraNet", "Skip for now"};
    configData["selectedNetwork"] = networks[networkButtonGroup->checkedId()];
    
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
        QMessageBox::information(this, "Success", "Configuration saved successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save configuration file: " + configFilePath);
    }
}
// :3
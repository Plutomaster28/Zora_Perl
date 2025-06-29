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

QWidget* SetupWizard::createLanguageRegionStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Select Language and Region:");
    QComboBox *language = new QComboBox;
    language->addItem("English (US)");
    language->addItem("Español (Latinoamérica)");
    language->addItem("Français (Canada)");
    QComboBox *region = new QComboBox;
    region->addItem("United States");
    region->addItem("Mexico");
    region->addItem("Canada");

    layout->addWidget(label);
    layout->addWidget(language);
    layout->addWidget(region);
    return step;
}

QWidget* SetupWizard::createKeyboardStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Choose Keyboard Layout:");
    QComboBox *keyboardLayout = new QComboBox;
    keyboardLayout->addItem("US QWERTY");
    keyboardLayout->addItem("Latin American QWERTY");
    keyboardLayout->addItem("Canadian Multilingual");

    layout->addWidget(label);
    layout->addWidget(keyboardLayout);
    return step;
}

QWidget* SetupWizard::createAppearanceStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Choose Appearance:");
    QRadioButton *light = new QRadioButton("Light Mode");
    QRadioButton *dark = new QRadioButton("Dark Mode");
    light->setChecked(true);

    layout->addWidget(label);
    layout->addWidget(light);
    layout->addWidget(dark);
    return step;
}

QWidget* SetupWizard::createUserAccountStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Create User Account:");
    QLineEdit *username = new QLineEdit;
    username->setPlaceholderText("Enter username");
    QLineEdit *password = new QLineEdit;
    password->setPlaceholderText("Password (optional)");
    password->setEchoMode(QLineEdit::Password);

    layout->addWidget(label);
    layout->addWidget(username);
    layout->addWidget(password);
    return step;
}

QWidget* SetupWizard::createNetworkStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Connect to a Network:");
    QListWidget *networks = new QListWidget;
    networks->addItem("WiFi-Home");
    networks->addItem("ZoraNet");
    networks->addItem("Skip for now");
    layout->addWidget(label);
    layout->addWidget(networks);
    return step;
}

QWidget* SetupWizard::createDevToggleStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Do You Code?");
    QCheckBox *toggle = new QCheckBox("Enable developer tools and terminal");
    layout->addWidget(label);
    layout->addWidget(toggle);
    return step;
}

QWidget* SetupWizard::createAppSuggestionsStep() {
    QWidget *step = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(step);
    QLabel *label = new QLabel("Choose Recommended Apps:");
    QCheckBox *web = new QCheckBox("Web Browser");
    QCheckBox *music = new QCheckBox("Music Player");
    QCheckBox *dev = new QCheckBox("Dev Tools");
    QCheckBox *office = new QCheckBox("Office Suite");

    layout->addWidget(label);
    layout->addWidget(web);
    layout->addWidget(music);
    layout->addWidget(dev);
    layout->addWidget(office);
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
    // TODO: Finalize setup and close wizard
    close();
}
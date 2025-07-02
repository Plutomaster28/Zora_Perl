// setupwizard.h
#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class SetupWizard : public QWidget {
    Q_OBJECT

public:
    SetupWizard(QWidget *parent = nullptr);

private slots:
    void nextStep();
    void previousStep();
    void finishSetup();

private:
    QStackedWidget *stack;
    QPushButton *nextButton;
    QPushButton *backButton;
    int currentIndex;

    // UI components to store references for data collection
    QComboBox *languageCombo;
    QComboBox *regionCombo;
    QComboBox *keyboardCombo;
    QRadioButton *lightModeRadio;
    QRadioButton *darkModeRadio;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QListWidget *networksList;
    QCheckBox *devToolsCheckbox;
    QCheckBox *webBrowserCheckbox;
    QCheckBox *musicPlayerCheckbox;
    QCheckBox *devToolsAppCheckbox;
    QCheckBox *officeSuiteCheckbox;

    // Configuration data
    QJsonObject configData;

    QWidget *createLanguageRegionStep();
    QWidget *createKeyboardStep();
    QWidget *createAppearanceStep();
    QWidget *createUserAccountStep();
    QWidget *createNetworkStep();
    QWidget *createDevToggleStep();
    QWidget *createAppSuggestionsStep();
    QWidget *createSummaryStep();
    
    void collectUserData();
    void saveConfigToFile();
};

#endif // SETUPWIZARD_H
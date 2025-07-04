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
#include <QButtonGroup>
#include <QLabel>

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
    QList<QLabel*> stepLabels;

    // UI components to store references for data collection
    QButtonGroup *languageButtonGroup;
    QButtonGroup *regionButtonGroup;
    QButtonGroup *keyboardButtonGroup;
    QRadioButton *lightModeRadio;
    QRadioButton *darkModeRadio;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QButtonGroup *networkButtonGroup;
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
    void updateStepIndicator();
};

#endif // SETUPWIZARD_H
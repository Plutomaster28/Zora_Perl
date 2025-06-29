// setupwizard.h
#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>

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

    QWidget *createLanguageRegionStep();
    QWidget *createKeyboardStep();
    QWidget *createAppearanceStep();
    QWidget *createUserAccountStep();
    QWidget *createNetworkStep();
    QWidget *createDevToggleStep();
    QWidget *createAppSuggestionsStep();
    QWidget *createSummaryStep();
};

#endif // SETUPWIZARD_H
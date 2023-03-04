#ifndef PASSWORDGENERATORMODULEWIDGET_HPP
#define PASSWORDGENERATORMODULEWIDGET_HPP

#include <QWidget>

namespace Ui {
    class PasswordGeneratorModuleWidget;
}

class PasswordGeneratorModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordGeneratorModuleWidget(QWidget *parent = nullptr);
    ~PasswordGeneratorModuleWidget();

    void triggerAllConfigSignals();

public slots:
    void passwordsGenerated(const QList<QByteArray>& passwords);

signals:
    void generate();
    void passwordLengthChanged(int length);
    void passwordCountChanged(int count);
    void useLowercaseChanged(bool useLowerCase);
    void useUppercaseChanged(bool useUpperCase);
    void useNumbersChanged(bool useNumbers);
    void useSpecialCharacterGroup1Changed(bool useSpecialCharacterGroup1);
    void useSpecialCharacterGroup2Changed(bool useSpecialCharacterGroup2);
    void useSpecialCharacterGroup3Changed(bool useSpecialCharacterGroup3);
    void useSpecialCharacterGroup4Changed(bool useSpecialCharacterGroup4);
    void useSpecialCharacterGroup5Changed(bool useSpecialCharacterGroup5);
    void useAdditionalCharactersChanged(bool useAdditionalCharacters);
    void additionalCharacterGroupChanged(
        const QByteArray& additionalCharacters);
    void selectFromEveryGroupChanged(bool selectFromEveryGroup);
    void useExcludeCharactersChanged(bool useExcludeCharacters);
    void excludeCharactersChanged(const QByteArray& excludeCharacters);
    void excludeSimilarCharactersChanged(bool excludeSimilarCharacters);
    void useCustomAlphabetChanged(bool useCustomAlphabet);
    void customAlphabetChanged(const QByteArray& alphabet);
    void randomizeRoundsChanged(int rounds);

private slots:

    void copyOutput();
    void advancedConfigurationClicked(bool checked);

    void passwordLengthSliderValueChanged(int value);
    void passwordLengthSpinBoxValueChanged(int value);
    void passwordCountSpinBoxChanged(int count);
    void useLowercaseCheckboxChanged(int useLowerCase);
    void useUppercaseCheckboxChanged(int useUpperCase);
    void useNumbersCheckboxChanged(int useNumbers);
    void useSpecialCharacterGroup1CheckboxChanged(
        int useSpecialCharacterGroup1);
    void useSpecialCharacterGroup2CheckboxChanged(
        int useSpecialCharacterGroup2);
    void useSpecialCharacterGroup3CheckboxChanged(
        int useSpecialCharacterGroup3);
    void useSpecialCharacterGroup4CheckboxChanged(
        int useSpecialCharacterGroup4);
    void useSpecialCharacterGroup5CheckboxChanged(
        int useSpecialCharacterGroup5);
    void useAdditionalCharacterCheckBoxChanged(int use);
    void additionalCharacterGroupEditChanged(
        const QString& additionalCharacters);
    void selectFromEveryGroupCheckboxChanged(int selectFromEveryGroup);
    void excludeCharactersCheckBoxChanged(int useExcludeCharacters);
    void excludeCharactersEditChanged(const QString& excludeCharacters);
    void excludeSimilarCharacterCheckboxsChanged(int excludeSimilarCharacters);
    void useCustomAlphabetCheckboxChanged(int useCustomAlphabet);
    void customAlphabetEditChanged(const QString& alphabet);
    void randomizeRoundsSpinBoxChanged(int rounds);

private:
    Ui::PasswordGeneratorModuleWidget *ui;
};

#endif // PASSWORDGENERATORMODULEWIDGET_HPP

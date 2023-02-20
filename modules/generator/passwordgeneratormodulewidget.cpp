#include "passwordgeneratormodulewidget.hpp"
#include "ui_passwordgeneratormodulewidget.h"

PasswordGeneratorModuleWidget::PasswordGeneratorModuleWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::PasswordGeneratorModuleWidget)
{
    ui->setupUi(this);

    connect(ui->passwordLengthSlider, &QSlider::sliderMoved, this,
            &PasswordGeneratorModuleWidget::passwordLengthSliderValueChanged);
    connect(ui->passwordLengthSpinBox, &QSpinBox::valueChanged, this,
            &PasswordGeneratorModuleWidget::passwordLengthSpinBoxValueChanged);
    connect(ui->passwordCountSpinBox, &QSpinBox::valueChanged, this,
            &PasswordGeneratorModuleWidget::passwordCountSpinBoxChanged);
    connect(ui->useLowercaseCheckBox, &QCheckBox::stateChanged, this,
            &PasswordGeneratorModuleWidget::useLowercaseCheckboxChanged);
    connect(ui->useUppercaseCheckBox, &QCheckBox::stateChanged, this,
            &PasswordGeneratorModuleWidget::useUppercaseCheckboxChanged);
    connect(ui->useNumberCheckBox, &QCheckBox::stateChanged, this,
            &PasswordGeneratorModuleWidget::useNumbersCheckboxChanged);
    connect(ui->useSpecialCharactersGroup1CheckBox, &QCheckBox::stateChanged,
            this,
            &PasswordGeneratorModuleWidget::
                useSpecialCharacterGroup1CheckboxChanged);
    connect(ui->useSpecialCharactersGroup2CheckBox, &QCheckBox::stateChanged,
            this,
            &PasswordGeneratorModuleWidget::
                useSpecialCharacterGroup2CheckboxChanged);
    connect(ui->useSpecialCharactersGroup3CheckBox, &QCheckBox::stateChanged,
            this,
            &PasswordGeneratorModuleWidget::
                useSpecialCharacterGroup3CheckboxChanged);
    connect(ui->useSpecialCharactersGroup4CheckBox, &QCheckBox::stateChanged,
            this,
            &PasswordGeneratorModuleWidget::
                useSpecialCharacterGroup4CheckboxChanged);
    connect(ui->useSpecialCharactersGroup5CheckBox, &QCheckBox::stateChanged,
            this,
            &PasswordGeneratorModuleWidget::
                useSpecialCharacterGroup5CheckboxChanged);
    connect(
        ui->useAdditionalCharactersCheckBox, &QCheckBox::stateChanged, this,
        &PasswordGeneratorModuleWidget::useAdditionalCharacterCheckBoxChanged);
    connect(
        ui->additionalCharactersEdit, &QLineEdit::textEdited, this,
        &PasswordGeneratorModuleWidget::additionalCharacterGroupEditChanged);
    connect(
        ui->useAllGroupsCheckBox, &QCheckBox::stateChanged, this,
        &PasswordGeneratorModuleWidget::selectFromEveryGroupCheckboxChanged);
    connect(ui->excludeCharactersCheckBox, &QCheckBox::stateChanged, this,
            &PasswordGeneratorModuleWidget::excludeCharactersCheckBoxChanged);
    connect(ui->excludeCharactersEdit, &QLineEdit::textEdited, this,
            &PasswordGeneratorModuleWidget::excludeCharactersEditChanged);
    connect(ui->excludeSimilarCharactersCheckBox, &QCheckBox::stateChanged,
            this,
            &PasswordGeneratorModuleWidget::
                excludeSimilarCharacterCheckboxsChanged);
    connect(ui->useCustomAlphabetCheckBox, &QCheckBox::stateChanged, this,
            &PasswordGeneratorModuleWidget::useCustomAlphabetCheckboxChanged);
    connect(ui->customAlphabetEdit, &QLineEdit::textEdited, this,
            &PasswordGeneratorModuleWidget::customAlphabetEditChanged);
    connect(ui->randomizeRoundsSpinBox, &QSpinBox::valueChanged, this,
            &PasswordGeneratorModuleWidget::randomizeRoundsSpinBoxChanged);

    connect(ui->advancedConfigurationButton, &QPushButton::clicked, this,
            &PasswordGeneratorModuleWidget::advancedConfigurationClicked);
    connect(ui->generateButton, &QPushButton::clicked, this,
            &PasswordGeneratorModuleWidget::generate);

    passwordCountSpinBoxChanged(1);

    advancedConfigurationClicked(false);
    useCustomAlphabetCheckboxChanged(false);
}

PasswordGeneratorModuleWidget::~PasswordGeneratorModuleWidget()
{
    delete ui;
}

void PasswordGeneratorModuleWidget::triggerAllConfigSignals()
{
    emit passwordLengthChanged(ui->passwordLengthSpinBox->value());
    emit passwordCountChanged(ui->passwordCountSpinBox->value());
    emit useLowercaseChanged(ui->useLowercaseCheckBox->isChecked());
    emit useUppercaseChanged(ui->useUppercaseCheckBox->isChecked());
    emit useNumbersChanged(ui->useNumberCheckBox->isChecked());
    emit useSpecialCharacterGroup1Changed(
        ui->useSpecialCharactersGroup1CheckBox->isChecked());
    emit useSpecialCharacterGroup2Changed(
        ui->useSpecialCharactersGroup2CheckBox->isChecked());
    emit useSpecialCharacterGroup3Changed(
        ui->useSpecialCharactersGroup3CheckBox->isChecked());
    emit useSpecialCharacterGroup4Changed(
        ui->useSpecialCharactersGroup4CheckBox->isChecked());
    emit useSpecialCharacterGroup5Changed(
        ui->useSpecialCharactersGroup5CheckBox->isChecked());
    emit useAdditionalCharactersChanged(
        ui->useAdditionalCharactersCheckBox->isChecked());
    emit additionalCharacterGroupChanged(
        ui->additionalCharactersEdit->text().toUtf8());
    emit selectFromEveryGroupChanged(ui->useAllGroupsCheckBox->isChecked());
    emit useExcludeCharactersChanged(
        ui->excludeCharactersCheckBox->isChecked());
    emit excludeCharactersChanged(ui->excludeCharactersEdit->text().toUtf8());
    emit excludeSimilarCharactersChanged(
        ui->excludeSimilarCharactersCheckBox->isChecked());
    emit useCustomAlphabetChanged(ui->useCustomAlphabetCheckBox->isChecked());
    emit customAlphabetChanged(ui->customAlphabetEdit->text().toUtf8());
    emit randomizeRoundsChanged(ui->randomizeRoundsSpinBox->value());
}
void PasswordGeneratorModuleWidget::passwordsGenerated(
    const QList<QByteArray>& passwords)
{
    if (passwords.isEmpty())
    {
        ui->singlePasswordEdit->clear();
        ui->multiplePasswordEdit->clear();
        return;
    }
    ui->singlePasswordEdit->setText(passwords.first());
    ui->multiplePasswordEdit->setPlainText(
        passwords.join(QChar(QChar::LineFeed).unicode()));
}

void PasswordGeneratorModuleWidget::advancedConfigurationClicked(bool checked)
{
    ui->advancedConfigurationWidget->setVisible(checked);
    ui->advancedConfigurationButton->setText(
        checked ? tr("Advanced Configuration <<<")
                : tr("Advanced Configuration >>>"));
}

void PasswordGeneratorModuleWidget::passwordLengthSliderValueChanged(int value)
{
    if (value != ui->passwordLengthSpinBox->value())
    {
        ui->passwordLengthSpinBox->setValue(value);
    }
    emit passwordLengthChanged(value);
    emit generate();
}

void PasswordGeneratorModuleWidget::passwordLengthSpinBoxValueChanged(int value)
{
    if (value != ui->passwordLengthSlider->value())
    {
        ui->passwordLengthSlider->setValue(value);
    }
    emit passwordLengthChanged(value);
    emit generate();
}

void PasswordGeneratorModuleWidget::passwordCountSpinBoxChanged(int count)
{
    if (count == 1)
    {
        ui->outputWidget->setCurrentIndex(0);
    }
    else
    {
        ui->outputWidget->setCurrentIndex(1);
    }
    emit passwordCountChanged(count);
    emit generate();
}

void PasswordGeneratorModuleWidget::useLowercaseCheckboxChanged(
    int useLowerCase)
{
    emit useLowercaseChanged(useLowerCase == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useUppercaseCheckboxChanged(
    int useUpperCase)
{
    emit useUppercaseChanged(useUpperCase == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useNumbersCheckboxChanged(int useNumbers)
{
    emit useNumbersChanged(useNumbers == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useSpecialCharacterGroup1CheckboxChanged(
    int useSpecialCharacterGroup1)
{
    emit useSpecialCharacterGroup1Changed(useSpecialCharacterGroup1
                                          == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useSpecialCharacterGroup2CheckboxChanged(
    int useSpecialCharacterGroup2)
{
    emit useSpecialCharacterGroup2Changed(useSpecialCharacterGroup2
                                          == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useSpecialCharacterGroup3CheckboxChanged(
    int useSpecialCharacterGroup3)
{
    emit useSpecialCharacterGroup3Changed(useSpecialCharacterGroup3
                                          == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useSpecialCharacterGroup4CheckboxChanged(
    int useSpecialCharacterGroup4)
{
    emit useSpecialCharacterGroup4Changed(useSpecialCharacterGroup4
                                          == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useSpecialCharacterGroup5CheckboxChanged(
    int useSpecialCharacterGroup5)
{
    emit useSpecialCharacterGroup5Changed(useSpecialCharacterGroup5
                                          == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useAdditionalCharacterCheckBoxChanged(
    int use)
{
    emit useAdditionalCharactersChanged(use == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::additionalCharacterGroupEditChanged(
    const QString& additionalCharacters)
{
    emit additionalCharacterGroupChanged(additionalCharacters.toUtf8());
    emit generate();
}

void PasswordGeneratorModuleWidget::selectFromEveryGroupCheckboxChanged(
    int selectFromEveryGroup)
{
    emit selectFromEveryGroupChanged(selectFromEveryGroup == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::excludeCharactersCheckBoxChanged(
    int useExcludeCharacters)
{
    emit useExcludeCharactersChanged(useExcludeCharacters == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::excludeCharactersEditChanged(
    const QString& excludeCharacters)
{
    emit excludeCharactersChanged(excludeCharacters.toUtf8());
    emit generate();
}

void PasswordGeneratorModuleWidget::excludeSimilarCharacterCheckboxsChanged(
    int excludeSimilarCharacters)
{
    emit excludeSimilarCharactersChanged(excludeSimilarCharacters
                                         == Qt::Checked);
    emit generate();
}

void PasswordGeneratorModuleWidget::useCustomAlphabetCheckboxChanged(
    int useCustomAlphabet)
{
    emit useCustomAlphabetChanged(useCustomAlphabet == Qt::Checked);
    ui->alphabetStackedWidget->setCurrentIndex(
        useCustomAlphabet == Qt::Checked ? 1 : 0);
    emit generate();
}

void PasswordGeneratorModuleWidget::customAlphabetEditChanged(
    const QString& alphabet)
{
    emit customAlphabetChanged(alphabet.toUtf8());
    emit generate();
}

void PasswordGeneratorModuleWidget::randomizeRoundsSpinBoxChanged(int rounds)
{
    emit randomizeRoundsChanged(rounds);
    emit generate();
}

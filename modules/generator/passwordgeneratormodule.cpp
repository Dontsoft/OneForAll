#include "passwordgeneratormodule.hpp"
#include "passwordgeneratormodulewidget.hpp"
#include "random/random.hpp"

using namespace Qt::Literals::StringLiterals;

static const QByteArray LOWERCASE = "abcdefghijklmnopqrstuvwxyz"_ba;
static const QByteArray UPPERCASE = LOWERCASE.toUpper();
static const QByteArray NUMBERS = "0123456789"_ba;

static const QByteArray LOWERCASE_NO_SIMILAR = "abcdefghijkmnopqrstuvwxyz"_ba;
static const QByteArray UPPERCASE_NO_SIMILAR = "ACDEFHJKLMNPQRSTUVWXYZ"_ba;
static const QByteArray NUMBERS_NO_SIMILAR = "234579"_ba;

static const QByteArray SPECIAL_GROUP_1 = "<>!$%&=?#*+"_ba;
static const QByteArray SPECIAL_GROUP_2 = ",;.:"_ba;
static const QByteArray SPECIAL_GROUP_3 = "^@~\"'`"_ba;
static const QByteArray SPECIAL_GROUP_4 = "/|\\-_"_ba;
static const QByteArray SPECIAL_GROUP_4_NO_SIMILAR = "/\\-_"_ba;
static const QByteArray SPECIAL_GROUP_5 = "{[()]}"_ba;

PasswordGeneratorModule::PasswordGeneratorModule(
    const PasswordGeneratorModule::Dependency &dependency, QObject *parent)
    : Module(tr("Password Generator"),
             "com.dontsoft.devtools.passwordgenerator",
             QObject::tr("Generators"), QIcon(), parent),
      Dependant<QSharedPointer<Random>>(dependency),
      Loggable("com.dontsoft.devtools.passwordgenerator"),
      _widget(new PasswordGeneratorModuleWidget())
{

    auto widgetCasted = static_cast<PasswordGeneratorModuleWidget *>(_widget);

    connect(widgetCasted, &PasswordGeneratorModuleWidget::generate, this,
            &PasswordGeneratorModule::generate);
    connect(widgetCasted, &PasswordGeneratorModuleWidget::passwordLengthChanged,
            this, &PasswordGeneratorModule::passwordLengthChanged);
    connect(widgetCasted, &PasswordGeneratorModuleWidget::passwordCountChanged,
            this, &PasswordGeneratorModule::passwordCountChanged);
    connect(widgetCasted, &PasswordGeneratorModuleWidget::useLowercaseChanged,
            this, &PasswordGeneratorModule::useLowercaseChanged);
    connect(widgetCasted, &PasswordGeneratorModuleWidget::useUppercaseChanged,
            this, &PasswordGeneratorModule::useUppercaseChanged);
    connect(widgetCasted, &PasswordGeneratorModuleWidget::useNumbersChanged,
            this, &PasswordGeneratorModule::useNumbersChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useSpecialCharacterGroup1Changed,
            this, &PasswordGeneratorModule::useSpecialCharacterGroup1Changed);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useSpecialCharacterGroup2Changed,
            this, &PasswordGeneratorModule::useSpecialCharacterGroup2Changed);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useSpecialCharacterGroup3Changed,
            this, &PasswordGeneratorModule::useSpecialCharacterGroup3Changed);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useSpecialCharacterGroup4Changed,
            this, &PasswordGeneratorModule::useSpecialCharacterGroup4Changed);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useSpecialCharacterGroup5Changed,
            this, &PasswordGeneratorModule::useSpecialCharacterGroup5Changed);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useAdditionalCharactersChanged,
            this, &PasswordGeneratorModule::useAdditionalCharactersChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::additionalCharacterGroupChanged,
            this, &PasswordGeneratorModule::additionalCharacterGroupChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::selectFromEveryGroupChanged, this,
            &PasswordGeneratorModule::selectFromEveryGroupChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useExcludeCharactersChanged, this,
            &PasswordGeneratorModule::useExcludeCharactersChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::excludeCharactersChanged, this,
            &PasswordGeneratorModule::excludeCharactersChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::excludeSimilarCharactersChanged,
            this, &PasswordGeneratorModule::excludeSimilarCharactersChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::useCustomAlphabetChanged, this,
            &PasswordGeneratorModule::useCustomAlphabetChanged);
    connect(widgetCasted, &PasswordGeneratorModuleWidget::customAlphabetChanged,
            this, &PasswordGeneratorModule::customAlphabetChanged);
    connect(widgetCasted,
            &PasswordGeneratorModuleWidget::randomizeRoundsChanged, this,
            &PasswordGeneratorModule::randomizeRoundsChanged);

    connect(this, &PasswordGeneratorModule::passwordsGenerated, widgetCasted,
            &PasswordGeneratorModuleWidget::passwordsGenerated);

    widgetCasted->triggerAllConfigSignals();
}

QWidget *PasswordGeneratorModule::getWidget() const
{
    return _widget;
}

void PasswordGeneratorModule::generate()
{
    QList<QByteArray> passwordList;
    QList<QByteArray> characterGroupList;
    if (_useCustomAlphabet)
    {
        if (!_customAlphabet.isEmpty())
        {
            characterGroupList.append(_customAlphabet);
        }
    }
    else
    {
        if (_useLowerCase)
        {
            filterAndAppend(characterGroupList, _excludeSimilarCharacters
                                                    ? LOWERCASE_NO_SIMILAR
                                                    : LOWERCASE);
        }
        if (_useUpperCase)
        {
            filterAndAppend(characterGroupList, _excludeSimilarCharacters
                                                    ? UPPERCASE_NO_SIMILAR
                                                    : UPPERCASE);
        }
        if (_useNumbers)
        {
            filterAndAppend(characterGroupList, _excludeSimilarCharacters
                                                    ? NUMBERS_NO_SIMILAR
                                                    : NUMBERS);
        }
        if (_useSpecialCharacterGroup1)
        {
            filterAndAppend(characterGroupList, SPECIAL_GROUP_1);
        }
        if (_useSpecialCharacterGroup2)
        {
            filterAndAppend(characterGroupList, SPECIAL_GROUP_2);
        }
        if (_useSpecialCharacterGroup3)
        {
            filterAndAppend(characterGroupList, SPECIAL_GROUP_3);
        }
        if (_useSpecialCharacterGroup4)
        {
            filterAndAppend(characterGroupList, _excludeSimilarCharacters
                                                    ? SPECIAL_GROUP_4_NO_SIMILAR
                                                    : SPECIAL_GROUP_4);
        }
        if (_useSpecialCharacterGroup5)
        {
            filterAndAppend(characterGroupList, SPECIAL_GROUP_5);
        }
        if (_useAdditionalCharacters)
        {
            filterAndAppend(characterGroupList, _additionalCharacters);
        }
    }
    for (int i = 0; i < _passwordCount; ++i)
    {
        passwordList.append(generatePassword(characterGroupList));
    }
    emit passwordsGenerated(passwordList);
}

void PasswordGeneratorModule::filterAndAppend(QList<QByteArray> &groupList,
                                              QByteArray group) const
{
    if (_useExcludeCharacters)
    {
        for (const auto &c : _excludeCharacters)
        {
            group.removeIf([=](const auto &inC) { return inC == c; });
        }
    }
    if (!group.isEmpty())
    {
        groupList.append(group);
    }
}

QByteArray PasswordGeneratorModule::generatePassword(
    const QByteArray &characterGroup) const
{
    const auto random = getDependency<0>();
    QByteArray password;
    for (int i = 0; i < _passwordLength; ++i)
    {
        password.append(random->selectRandom(characterGroup));
    }
    random->randomize(password, qMax(1, _randomizeRounds));
    return password;
}

QByteArray PasswordGeneratorModule::generatePassword(
    QList<QByteArray> characterGroupList) const
{
    if (characterGroupList.isEmpty())
    {
        return QByteArray();
    }
    if (characterGroupList.length() == 1)
    {
        return generatePassword(characterGroupList[0]);
    }
    const bool needsEveryGroup
        = _selectFromEveryGroup
          && (_passwordLength >= characterGroupList.length());

    static const int MAX_REGENERATION = 10;
    int regenerationIndex = 0;

    QList<bool> groupUsed(characterGroupList.size(), false);

    const auto random = getDependency<0>();
    QByteArray password;
    while (groupUsed.contains(false) && regenerationIndex < MAX_REGENERATION)
    {
        password.clear();
        while (password.length() < _passwordLength)
        {
            int characterGroupIndex = 0;
            const auto characterGroup = random->selectRandom(
                characterGroupList, &characterGroupIndex);
            groupUsed[characterGroupIndex] = true;
            password.append(random->selectRandom(characterGroup));
        }
        if (!needsEveryGroup)
        {
            break;
        }
        regenerationIndex += 1;
    }

    random->randomize(password, qMax(1, _randomizeRounds));
    return password;
}

void PasswordGeneratorModule::passwordLengthChanged(int length)
{
    _passwordLength = length;
}

void PasswordGeneratorModule::passwordCountChanged(int count)
{
    _passwordCount = count;
}

void PasswordGeneratorModule::useLowercaseChanged(bool useLowerCase)
{
    _useLowerCase = useLowerCase;
}

void PasswordGeneratorModule::useUppercaseChanged(bool useUpperCase)
{
    _useUpperCase = useUpperCase;
}

void PasswordGeneratorModule::useNumbersChanged(bool useNumbers)
{
    _useNumbers = useNumbers;
}

void PasswordGeneratorModule::useSpecialCharacterGroup1Changed(
    bool useSpecialCharacterGroup1)
{
    _useSpecialCharacterGroup1 = useSpecialCharacterGroup1;
}

void PasswordGeneratorModule::useSpecialCharacterGroup2Changed(
    bool useSpecialCharacterGroup2)
{
    _useSpecialCharacterGroup2 = useSpecialCharacterGroup2;
}

void PasswordGeneratorModule::useSpecialCharacterGroup3Changed(
    bool useSpecialCharacterGroup3)
{
    _useSpecialCharacterGroup3 = useSpecialCharacterGroup3;
}

void PasswordGeneratorModule::useSpecialCharacterGroup4Changed(
    bool useSpecialCharacterGroup4)
{
    _useSpecialCharacterGroup4 = useSpecialCharacterGroup4;
}

void PasswordGeneratorModule::useSpecialCharacterGroup5Changed(
    bool useSpecialCharacterGroup5)
{
    _useSpecialCharacterGroup5 = useSpecialCharacterGroup5;
}
void PasswordGeneratorModule::useAdditionalCharactersChanged(
    bool useAdditionalCharacters)
{
    _useAdditionalCharacters = useAdditionalCharacters;
}

void PasswordGeneratorModule::additionalCharacterGroupChanged(
    const QByteArray &additionalCharacters)
{
    _additionalCharacters = additionalCharacters;
}

void PasswordGeneratorModule::selectFromEveryGroupChanged(
    bool selectFromEveryGroup)
{
    _selectFromEveryGroup = selectFromEveryGroup;
}

void PasswordGeneratorModule::useExcludeCharactersChanged(
    bool useExcludeCharacters)
{
    _useExcludeCharacters = useExcludeCharacters;
}

void PasswordGeneratorModule::excludeCharactersChanged(
    const QByteArray &excludeCharacters)
{
    _excludeCharacters = excludeCharacters;
}

void PasswordGeneratorModule::excludeSimilarCharactersChanged(
    bool excludeSimilarCharacters)
{
    _excludeSimilarCharacters = excludeSimilarCharacters;
}

void PasswordGeneratorModule::useCustomAlphabetChanged(bool useCustomAlphabet)
{
    _useCustomAlphabet = useCustomAlphabet;
}

void PasswordGeneratorModule::customAlphabetChanged(const QByteArray &alphabet)
{
    _customAlphabet = alphabet;
}

void PasswordGeneratorModule::randomizeRoundsChanged(int rounds)
{
    _randomizeRounds = rounds;
}

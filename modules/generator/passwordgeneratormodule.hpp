#ifndef PASSWORDGENERATORMODULE_HPP
#define PASSWORDGENERATORMODULE_HPP

#include <QObject>
#include <QSharedPointer>

#include "core/dependant.hpp"
#include "log/loggable.hpp"
#include "modules/module.hpp"

class Random;

class PasswordGeneratorModule : public Module,
                                public Dependant<QSharedPointer<Random>>,
                                public Loggable
{
    Q_OBJECT
public:
    explicit PasswordGeneratorModule(
        const PasswordGeneratorModule::Dependency& dependency,
        QObject* parent = nullptr);

public slots:
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

signals:
    void passwordsGenerated(const QList<QByteArray>& passwords);

private:
    void filterAndAppend(QList<QByteArray>& groupList, QByteArray group) const;

    QByteArray generatePassword(const QByteArray& characterGroup) const;
    QByteArray generatePassword(QList<QByteArray> characterGroupList) const;

    int _passwordLength;
    int _passwordCount;
    bool _useLowerCase;
    bool _useUpperCase;
    bool _useNumbers;
    bool _useSpecialCharacterGroup1;
    bool _useSpecialCharacterGroup2;
    bool _useSpecialCharacterGroup3;
    bool _useSpecialCharacterGroup4;
    bool _useSpecialCharacterGroup5;
    bool _useAdditionalCharacters;
    QByteArray _additionalCharacters;
    bool _selectFromEveryGroup;
    bool _useExcludeCharacters;
    QByteArray _excludeCharacters;
    bool _excludeSimilarCharacters;
    bool _useCustomAlphabet;
    QByteArray _customAlphabet;
    int _randomizeRounds;
};

#endif // PASSWORDGENERATORMODULE_HPP

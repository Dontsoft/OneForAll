#ifndef APP_INFORMATION_HPP
#define APP_INFORMATION_HPP

#include <QList>
#include <QMap>

class AppInformation {
   public:
    enum class Type {
        Name,
        Author,
        SupportMail,
        SupportWebpage,
        SupportGithub,
        Type,
        Version,
        Version_Long,
        LogoPath,
        License
    };

    struct ThirdPartyInformation
    {
        enum class Type
        {
            Library
        };

        Type type;
        QMap<AppInformation::Type, QString> values;
    };

    void set(Type type, const QString& value);
    QString get(Type type) const;
    bool has(Type type) const;

    void addThirdPartyInformation(const ThirdPartyInformation& information);

private:
    QMap<Type, QString> _values;
    QList<ThirdPartyInformation> _thirdPartyInformation;
};

#endif

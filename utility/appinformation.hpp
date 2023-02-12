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
        LogoPath
    };

    struct ThirdPartyInformation {};

    void set(Type type, const QString& value);
    QString get(Type type) const;
    bool has(Type type) const;

   private:
    QMap<Type, QString> _values;
};

#endif

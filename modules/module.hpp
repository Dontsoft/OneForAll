#ifndef MODULE_HPP
#define MODULE_HPP

#include <QIcon>
#include <QObject>

class Module : public QObject
{
    Q_OBJECT
public:
    using Identifier = QString;

    virtual QWidget* getWidget() const = 0;
    QString getName() const
    {
        return _name;
    }
    Identifier getIdentifier() const
    {
        return _identifier;
    }
    QString getCategory() const
    {
        return _category;
    }
    QIcon getIcon() const
    {
        return _icon;
    }

protected:
    explicit Module(const QString& name, const QString& identifier,
                    const QString& category, const QIcon& _icon = QIcon(),
                    QObject* parent = nullptr);

signals:

private:
    const QString _name;
    const Identifier _identifier;
    const QString _category;
    const QIcon _icon;
};

#endif // MODULE_HPP

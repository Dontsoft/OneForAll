#ifndef MODULE_HPP
#define MODULE_HPP

#include <QIcon>
#include <QObject>

class Module : public QObject
{
    Q_OBJECT
public:
    using Identifier = QString;

    virtual QWidget* getWidget() const
    {
        return _widget;
    }
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
    Module(const QString& name, const QString& identifier,
           const QString& category, const QIcon& _icon = QIcon(),
           QWidget* widget = nullptr, QObject* parent = nullptr);

    void setWidget(QWidget* widget)
    {
        _widget = widget;
    }

signals:

private:
    const QString _name;
    const Identifier _identifier;
    const QString _category;
    const QIcon _icon;

    QWidget* _widget;
};

#endif // MODULE_HPP

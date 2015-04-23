#ifndef KAMOSOSETTINGS_H
#define KAMOSOSETTINGS_H

#include <QObject>
#include <QUrl>

/* FIXME: KDE5. nuke
 * This class only exists because KConfigCompiler can't generate classes
 * with properties. 
 */
class KamosoSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl saveUrl READ saveUrl WRITE setSaveUrl NOTIFY saveUrlChanged)
    public:
        QUrl saveUrl() const;
        void setSaveUrl(const QUrl& url);

    Q_SIGNALS:
        void saveUrlChanged();
};

#endif // KAMOSOSETTINGS_H

#ifndef MODENTRY_H
#define MODENTRY_H

#include <QFrame>

class ModEntry : public QFrame
{
    Q_OBJECT
public:
    ModEntry(QString name, QString desc, QString dlLink, QString dlSha, QWidget *parent = nullptr);
    ModEntry(QString name, QString desc, QList<QString> dlLinks, QList<QString> dlShas, QWidget *parent = nullptr);
    ~ModEntry();
    QString name();
    QString description();
    QString downloadLink();
    QString downloadSha();
    QList<QString> downloadLinks();
    QList<QString> downloadShas();
    bool installed();
    bool enabled();

private:
    int osToIndex();
    void setup();

public slots:
    void installButtonClicked();
    void uninstallButtonClicked();
    void enableButtonClicked();
    void disableButtonClicked();
    void setModFiles(QList<QString> value);
    void setInstalled(bool value);
    void setEnabled(bool value);

private:
    QString m_name;
    QString m_description;
    QString m_downloadLink;
    QString m_downloadSha;
    QList<QString> m_downloadLinks;
    QList<QString> m_downloadShas;
    QList<QString> m_modFiles;
    bool m_installed;
    bool m_enabled;

    QList<QWidget*> m_subWidgets;
};

#endif // MODENTRY_H

#include "modentry.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <iostream>

ModEntry::ModEntry(QString name, QString desc, QString dlLink, QString dlSha, QWidget *parent)
    : QFrame(parent)
    , m_name(name)
    , m_description(desc)
    , m_downloadLink(dlLink)
    , m_downloadSha(dlSha)
    , m_downloadLinks(QList<QString>())
    , m_downloadShas(QList<QString>())
    , m_modFiles(QList<QString>())
    , m_installed(false)
    , m_enabled(false)
    , m_subWidgets(QList<QWidget*>())
{
    this->setup();
}
ModEntry::ModEntry(QString name, QString desc, QList<QString> dlLinks, QList<QString> dlShas, QWidget *parent)
    : QFrame(parent)
    , m_name(name)
    , m_description(desc)
    , m_downloadLink(QString())
    , m_downloadSha(QString())
    , m_downloadLinks(dlLinks)
    , m_downloadShas(dlShas)
    , m_modFiles(QList<QString>())
    , m_installed()
    , m_enabled()
    , m_subWidgets(QList<QWidget*>())
{
    this->setup();
}
ModEntry::~ModEntry()
{
    for (int i = m_subWidgets.count() - 1; i >= 0; i--)
    {
        m_subWidgets.at(i)->deleteLater();
        delete m_subWidgets.takeAt(i);
    }
    this->layout()->deleteLater();
    delete this->layout();
    this->setLayout(nullptr);
}
QString ModEntry::name()
{
    return m_name;
}
QString ModEntry::description()
{
    return m_description;
}
QString ModEntry::downloadLink()
{
    return m_downloadLink;
}
QString ModEntry::downloadSha()
{
    return m_downloadSha;
}
QList<QString> ModEntry::downloadLinks()
{
    return m_downloadLinks;
}
QList<QString> ModEntry::downloadShas()
{
    return m_downloadShas;
}
bool ModEntry::installed()
{
    return m_installed;
}
bool ModEntry::enabled()
{
    return m_enabled;
}

int ModEntry::osToIndex()
{
    return 0;
}
void ModEntry::setup()
{
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QHBoxLayout* rowLayout = new QHBoxLayout(this);
    this->setLayout(rowLayout);

    QLabel* nameLabel = new QLabel(m_name, this);
    rowLayout->addWidget(nameLabel);
    m_subWidgets.append(nameLabel);

    QLabel* descLabel = new QLabel(m_description, this);
    rowLayout->addWidget(descLabel);
    m_subWidgets.append(descLabel);

    QPushButton* installButton = new QPushButton("Install", this);
    rowLayout->addWidget(installButton);
    m_subWidgets.append(installButton);
    connect(installButton, &QPushButton::clicked, this, &ModEntry::installButtonClicked);

    QPushButton* uninstallButton = new QPushButton("Uninstall", this);
    rowLayout->addWidget(uninstallButton);
    m_subWidgets.append(uninstallButton);
    connect(uninstallButton, &QPushButton::clicked, this, &ModEntry::uninstallButtonClicked);

    QPushButton* enableButton = new QPushButton("Enable", this);
    rowLayout->addWidget(enableButton);
    m_subWidgets.append(enableButton);
    connect(enableButton, &QPushButton::clicked, this, &ModEntry::enableButtonClicked);

    QPushButton* disableButton = new QPushButton("Disable", this);
    rowLayout->addWidget(disableButton);
    m_subWidgets.append(disableButton);
    connect(disableButton, &QPushButton::clicked, this, &ModEntry::disableButtonClicked);
}

void ModEntry::installButtonClicked()
{
    std::cout << "Installing '" << m_name.toStdString() << "'!" << std::endl;
}
void ModEntry::uninstallButtonClicked()
{
    std::cout << "Uninstalling '" << m_name.toStdString() << "'!" << std::endl;
}
void ModEntry::enableButtonClicked()
{
    std::cout << "Enabling '" << m_name.toStdString() << "'!" << std::endl;
}
void ModEntry::disableButtonClicked()
{
    std::cout << "Disabling '" << m_name.toStdString() << "'!" << std::endl;
}
void ModEntry::setModFiles(QList<QString> value)
{
    m_modFiles = value;
}
void ModEntry::setInstalled(bool value)
{
    m_installed = value;
    m_subWidgets.at(2)->setVisible(!value);
    m_subWidgets.at(3)->setVisible(value);
    m_subWidgets.at(4)->setEnabled(value);
    m_subWidgets.at(5)->setEnabled(value);
}
void ModEntry::setEnabled(bool value)
{
    m_enabled = value;
    m_subWidgets.at(4)->setVisible(!value);
    m_subWidgets.at(5)->setVisible(value);
}

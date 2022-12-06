#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDomDocument>
#include <QFile>
#include <QUrl>
#include <QHBoxLayout>
#include <QFileDialog>
#include <iostream>

#include "modentry.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , labelGamePath(nullptr)
    , scrollArea(nullptr)
    , scrollAreaWidgetContents(nullptr)
    , m_mlDownload(nullptr)
    , m_alDownload(nullptr)
{
    ui->setupUi(this);

    labelGamePath = findChild<QLabel*>("labelGamePath");
    scrollArea = findChild<QScrollArea*>("scrollArea");
    scrollAreaWidgetContents = findChild<QWidget*>("scrollAreaWidgetContents");

    QUrl modLinksUrl("https://raw.githubusercontent.com/hk-modding/modlinks/main/ModLinks.xml");
    QUrl apiLinksUrl("https://raw.githubusercontent.com/hk-modding/modlinks/main/ApiLinks.xml");

    m_mlDownload = new FileDownloader(modLinksUrl, this);
    m_alDownload = new FileDownloader(apiLinksUrl, this);

    connect(m_mlDownload, SIGNAL (downloaded()), this, SLOT (loadModLinks()));
    connect(m_alDownload, SIGNAL (downloaded()), this, SLOT (loadApiLinks()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadModLinks()
{
    QDomDocument doc("modlinks");
    if (!doc.setContent(m_mlDownload->downloadedData())) {
        // error
        return;
    }

    // clear the list
    for (int i = scrollAreaWidgetContents->layout()->count() - 1; i >= 0; i--) {
        auto widget = scrollAreaWidgetContents->layout()->takeAt(i)->widget();
        widget->deleteLater();
        delete widget;
    }

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            // the node really is an element.
            // in this case, this definetly is a Manifest object
            QDomNodeList children = e.childNodes();
            QString modName = children.at(0).toElement().text();
            QString modDescription = children.at(1).toElement().text();
            QString modVersion = children.at(2).toElement().text();
            QString modLink;
            QString modLinkSha;
            QList<QString> modLinks;
            QList<QString> modLinksSha;
            if (children.at(3).toElement().tagName() == "Links") {
                modLinks.append(children.at(3).childNodes().at(0).toElement().text());
                modLinks.append(children.at(3).childNodes().at(1).toElement().text());
                modLinks.append(children.at(3).childNodes().at(2).toElement().text());
                modLinksSha.append(children.at(3).childNodes().at(0).toElement().attribute("SHA256"));
                modLinksSha.append(children.at(3).childNodes().at(1).toElement().attribute("SHA256"));
                modLinksSha.append(children.at(3).childNodes().at(2).toElement().attribute("SHA256"));
            }
            else {
                modLink = children.at(3).toElement().text();
                modLinkSha = children.at(3).toElement().attribute("SHA256");
            }

            std::cout << "Adding '" << modName.toStdString() << "'" << std::endl;

            ModEntry *modRow = nullptr;
            if (modLink != QString())
            {
                modRow = new ModEntry(modName, modDescription, modLink, modLinkSha, scrollAreaWidgetContents);
            }
            else
            {
                modRow = new ModEntry(modName, modDescription, modLinks, modLinksSha, scrollAreaWidgetContents);
            }
            modRow->setInstalled(false);
            modRow->setEnabled(false);
            scrollAreaWidgetContents->layout()->addWidget(modRow);
        }
        n = n.nextSibling();
    }
}

void MainWindow::loadApiLinks()
{
    QDomDocument doc("apilinks");
    if (!doc.setContent(m_alDownload->downloadedData())) {
        // error
        return;
    }

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //std::cout << qPrintable(e.tagName()) << std::endl; // the node really is an element.
        }
        n = n.nextSibling();
    }
}

void MainWindow::on_pbChangePath_clicked() {
    std::cout << "Clicked" << std::endl;

    QString directory = QFileDialog::getExistingDirectory(this, "Select Hollow Knight Installation Path", ".");

    std::cout << "Selected directory: '" << directory.toStdString() << "'" << std::endl;
}

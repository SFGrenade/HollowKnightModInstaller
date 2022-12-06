#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_mlDownload(nullptr),
      m_alDownload(nullptr) {
  ui->setupUi(this);
  ui->tModList->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  ui->tModList->horizontalHeader()->setSectionResizeMode(1,
                                                         QHeaderView::Stretch);

  QUrl modLinksUrl("https://raw.githubusercontent.com/hk-modding/modlinks/main/"
                   "ModLinks.xml");
  QUrl apiLinksUrl("https://raw.githubusercontent.com/hk-modding/modlinks/main/"
                   "ApiLinks.xml");

  m_mlDownload = new FileDownloader(modLinksUrl, this);
  m_alDownload = new FileDownloader(apiLinksUrl, this);

  connect(m_mlDownload, &FileDownloader::downloaded, this,
          &MainWindow::loadModLinks);
  connect(m_alDownload, &FileDownloader::downloaded, this,
          &MainWindow::loadApiLinks);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadModLinks() {
  QDomDocument doc("modlinks");
  if (!doc.setContent(m_mlDownload->downloadedData())) {
    // error
    return;
  }

  // clear the list
  ui->tModList->clearContents();

  // print out the element names of all elements that are direct children
  // of the outermost element.
  QDomElement docElem = doc.documentElement();

  int i = 0;
  QDomNode n = docElem.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement(); // try to convert the node to an element.
    if (!e.isNull()) {
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
        modLinksSha.append(
            children.at(3).childNodes().at(0).toElement().attribute("SHA256"));
        modLinksSha.append(
            children.at(3).childNodes().at(1).toElement().attribute("SHA256"));
        modLinksSha.append(
            children.at(3).childNodes().at(2).toElement().attribute("SHA256"));
      } else {
        modLink = children.at(3).toElement().text();
        modLinkSha = children.at(3).toElement().attribute("SHA256");
      }

      qDebug() << "Adding" << modName;

      // ModEntry *modRow = nullptr;
      // if (modLink != QString()) {
      //  modRow = new ModEntry(modName, modDescription, modLink, modLinkSha,
      //                        scrollAreaWidgetContents);
      //} else {
      //  modRow = new ModEntry(modName, modDescription, modLinks, modLinksSha,
      //                        scrollAreaWidgetContents);
      //}
      // modRow->setInstalled(false);
      // modRow->setEnabled(false);
      // scrollAreaWidgetContents->layout()->addWidget(modRow);

      ui->tModList->setRowCount(i + 1);
      ui->tModList->setItem(i, 0, new QTableWidgetItem(modName));
      ui->tModList->setItem(i, 1, new QTableWidgetItem(modDescription));
      ui->tModList->setItem(i, 2, new QTableWidgetItem("Enable"));
      ui->tModList->setItem(i, 3, new QTableWidgetItem("Disable"));
      ui->tModList->setItem(i, 4, new QTableWidgetItem("Install"));
      ui->tModList->setItem(i, 5, new QTableWidgetItem("Uninstall"));

      i++;
    }
    n = n.nextSibling();
  }
}

void MainWindow::loadApiLinks() {
  QDomDocument doc("apilinks");
  if (!doc.setContent(m_alDownload->downloadedData())) {
    // error
    return;
  }

  // print out the element names of all elements that are direct children
  // of the outermost element.
  QDomElement docElem = doc.documentElement();

  QDomNode n = docElem.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement(); // try to convert the node to an element.
    if (!e.isNull()) {
      // qDebug() << qPrintable(e.tagName()); // the node really
      // is an element.
    }
    n = n.nextSibling();
  }
}

void MainWindow::on_pbChangePath_clicked() {
  qDebug() << "Clicked";

  QString directory = QFileDialog::getExistingDirectory(
      this, "Select Hollow Knight Installation Path", ".");

  qDebug() << "Selected directory:" << directory;
}

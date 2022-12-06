#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>

#include "filedownloader.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  // QLabel *labelGamePath;
  // QScrollArea *scrollArea;
  // QWidget *scrollAreaWidgetContents;

  FileDownloader *m_mlDownload;
  FileDownloader *m_alDownload;

private slots:
  void loadModLinks();
  void loadApiLinks();

  void on_pbChangePath_clicked();
};

#endif // MAINWINDOW_H

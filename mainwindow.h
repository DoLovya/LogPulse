#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QAction>
#include <QMenu>
#include <QTranslator>
#include <QEvent>
#include "codeeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void openFile();
    void showGoToLineDialog();
    void closeTab(int index);
    void closeAllTabs();
    void switchLanguage(QAction *action);
    void showTabContextMenu(const QPoint &pos);

private:
    void createActions();
    void createMenus();
    void retranslateUi();

    QTabWidget *tabWidget;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *languageMenu;
    QAction *openAction;
    QAction *goToLineAction;
    QActionGroup *languageActionGroup;
    QTranslator *translator;
};
#endif // MAINWINDOW_H

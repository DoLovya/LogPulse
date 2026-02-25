#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QAction>
#include <QMenu>
#include <QTranslator>
#include <QEvent>

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
    void switchLanguage(QAction *action);

private:
    void createActions();
    void createMenus();
    void retranslateUi();

    QPlainTextEdit *logViewer;
    QMenu *fileMenu;
    QMenu *languageMenu;
    QAction *openAction;
    QActionGroup *languageActionGroup;
    QTranslator *translator;
};
#endif // MAINWINDOW_H

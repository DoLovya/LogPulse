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
	void showSettingsDialog();
	void switchLanguage(QAction *action);
	void showTabContextMenu(const QPoint &pos);
	void openRecentFile();

private:
	void createActions();
	void createMenus();
	void retranslateUi();
	void applySettings(CodeEditor *editor);
	void updateRecentFileActions();
	void setCurrentFile(const QString &fileName);
	bool loadFile(const QString &fileName);

	enum
	{
		MaxRecentFiles = 5
	};

	QTabWidget *tabWidget;
	QMenu *fileMenu;
	QMenu *recentFilesMenu;
	QMenu *editMenu;
	QMenu *languageMenu;
	QAction *openAction;
	QAction *recentFileActs[MaxRecentFiles];
	QAction *goToLineAction;
	QAction *settingsAction;
	QActionGroup *languageActionGroup;
	QTranslator *translator;
	QTranslator *qtTranslator;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QActionGroup>
#include <QTranslator>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QTabBar>
#include <QClipboard>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QInputDialog>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), translator(new QTranslator)
{
    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    
    // Customize tab close button
    tabWidget->setStyleSheet(
        "QTabBar::close-button {"
        "    image: url(:/icons/close.svg);"
        "    subcontrol-position: right;"
        "    width: 16px;"
        "    height: 16px;"
        "    margin: 2px;"
        "}"
        "QTabBar::close-button:hover {"
        "    image: url(:/icons/close_hover.svg);"
        "}"
    );

    tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabWidget->tabBar(), &QTabBar::customContextMenuRequested, this, &MainWindow::showTabContextMenu);

    setCentralWidget(tabWidget);

    createActions();
    createMenus();
    
    retranslateUi();
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete translator;
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::openFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Log File"), "",
                                                    tr("Log Files (*.log *.txt);;All Files (*)"));

    if (fileNames.isEmpty())
        return;

    for (const QString &fileName : fileNames) {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("LogPulse"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            continue;
        }

        QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        QString content = in.readAll();
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        
        CodeEditor *logViewer = new CodeEditor;
        logViewer->setReadOnly(true);
        logViewer->setPlainText(content);
        logViewer->setProperty("filePath", fileName);
        
        QFileInfo fileInfo(fileName);
        tabWidget->addTab(logViewer, fileInfo.fileName());
        tabWidget->setCurrentWidget(logViewer);
    }
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::showGoToLineDialog()
{
    QWidget *currentWidget = tabWidget->currentWidget();
    CodeEditor *editor = qobject_cast<CodeEditor*>(currentWidget);
    if (!editor) return;

    bool ok;
    int maxLine = editor->blockCount();
    int currentLine = editor->textCursor().blockNumber() + 1;
    
    QInputDialog dialog(this);
    dialog.setWindowTitle(tr("Go to Line"));
    dialog.setLabelText(tr("Line number:"));
    dialog.setIntRange(1, maxLine);
    dialog.setIntValue(currentLine);
    dialog.setIntStep(1);
    
    // Set dialog flags to be a popup/tool window that closes when clicking outside might require 
    // simply using the standard exec() which is modal.
    // However, user requested "popup like" and "close when clicking outside".
    // Standard QInputDialog::getInt is modal and blocking.
    // Let's customize a QInputDialog or QDialog to achieve this behavior if needed.
    // But for simplicity and standard behavior, let's try standard modal first, 
    // or set Qt::Popup flag.
    
    dialog.setWindowFlags(Qt::Popup); 
    // Qt::Popup makes it close when clicking outside.
    
    // Position the dialog center to the parent
    dialog.move(geometry().center() - dialog.rect().center());

    if (dialog.exec() == QDialog::Accepted) {
        int lineNumber = dialog.intValue();
        QTextBlock block = editor->document()->findBlockByNumber(lineNumber - 1);
        QTextCursor cursor(block);
        editor->setTextCursor(cursor);
        editor->centerCursor();
    }
}

void MainWindow::closeTab(int index)
{
    QWidget *widget = tabWidget->widget(index);
    tabWidget->removeTab(index);
    delete widget;
}

void MainWindow::closeAllTabs()
{
    while (tabWidget->count() > 0) {
        closeTab(0);
    }
}

void MainWindow::showTabContextMenu(const QPoint &pos)
{
    int index = tabWidget->tabBar()->tabAt(pos);
    if (index == -1) return;

    QMenu contextMenu(this);
    
    QAction *closeAction = new QAction(tr("Close"), this);
    connect(closeAction, &QAction::triggered, [this, index]() {
        closeTab(index);
    });
    contextMenu.addAction(closeAction);

    QAction *closeAllAction = new QAction(tr("Close All"), this);
    connect(closeAllAction, &QAction::triggered, this, &MainWindow::closeAllTabs);
    contextMenu.addAction(closeAllAction);

    contextMenu.addSeparator();

    QAction *copyPathAction = new QAction(tr("Copy Full Path"), this);
    connect(copyPathAction, &QAction::triggered, [this, index]() {
        QWidget *widget = tabWidget->widget(index);
        QString filePath = widget->property("filePath").toString();
        if (!filePath.isEmpty()) {
            QApplication::clipboard()->setText(QDir::toNativeSeparators(filePath));
        }
    });
    contextMenu.addAction(copyPathAction);

    QAction *openInExplorerAction = new QAction(tr("Open in Explorer"), this);
    connect(openInExplorerAction, &QAction::triggered, [this, index]() {
        QWidget *widget = tabWidget->widget(index);
        QString filePath = widget->property("filePath").toString();
        if (!filePath.isEmpty()) {
            QFileInfo fileInfo(filePath);
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
        }
    });
    contextMenu.addAction(openInExplorerAction);

    contextMenu.exec(tabWidget->tabBar()->mapToGlobal(pos));
}

void MainWindow::switchLanguage(QAction *action)
{
    if (!action) return;
    
    QString langCode = action->data().toString();
    QString qmFile = QString(":/translations/LogPulse_%1.qm").arg(langCode);

    qApp->removeTranslator(translator);
    if (translator->load(qmFile)) {
        qApp->installTranslator(translator);
    } else {
        qDebug() << "Failed to load translation:" << qmFile;
    }
}

void MainWindow::createActions()
{
    openAction = new QAction(this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    goToLineAction = new QAction(this);
    goToLineAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(goToLineAction, &QAction::triggered, this, &MainWindow::showGoToLineDialog);

    languageActionGroup = new QActionGroup(this);
    
    QAction *enAction = new QAction(tr("English"), this);
    enAction->setData("en_US");
    enAction->setCheckable(true);
    languageActionGroup->addAction(enAction);

    QAction *zhAction = new QAction(tr("Chinese"), this);
    zhAction->setData("zh_CN");
    zhAction->setCheckable(true);
    languageActionGroup->addAction(zhAction);

    // Set default checked to Chinese
    zhAction->setChecked(true); 
    // Trigger the switch to apply the default language
    switchLanguage(zhAction);

    connect(languageActionGroup, &QActionGroup::triggered, this, &MainWindow::switchLanguage);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(""); // Will be set in retranslateUi
    fileMenu->addAction(openAction);

    editMenu = menuBar()->addMenu("");
    editMenu->addAction(goToLineAction);
    
    // Create language menu separately, not inside file menu
    languageMenu = menuBar()->addMenu(""); // Will be set in retranslateUi
    languageMenu->addActions(languageActionGroup->actions());
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("LogPulse"));
    openAction->setText(tr("&Open..."));
    openAction->setStatusTip(tr("Open an existing file"));

    goToLineAction->setText(tr("&Go to Line..."));
    goToLineAction->setStatusTip(tr("Go to a specific line number"));
    
    if (fileMenu) fileMenu->setTitle(tr("&File"));
    if (editMenu) editMenu->setTitle(tr("&Edit"));
    if (languageMenu) languageMenu->setTitle(tr("&Language"));
    
    // Actions in the group need to have their text retranslated if they use tr()
    // But "English" and "Chinese" usually stay in their native language or fixed.
    // However, if we want them translated:
    QList<QAction*> acts = languageActionGroup->actions();
    for(auto act : acts) {
        if (act->data().toString() == "en_US") act->setText(tr("English"));
        if (act->data().toString() == "zh_CN") act->setText(tr("Chinese"));
    }
}

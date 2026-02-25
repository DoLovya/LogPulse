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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), translator(new QTranslator)
{
    logViewer = new QPlainTextEdit;
    logViewer->setReadOnly(true);
    setCentralWidget(logViewer);

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Log File"), "",
                                                    tr("Log Files (*.log *.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("LogPulse"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    logViewer->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    statusBar()->showMessage(tr("File loaded"), 2000);
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
    
    // Create language menu separately, not inside file menu
    languageMenu = menuBar()->addMenu(""); // Will be set in retranslateUi
    languageMenu->addActions(languageActionGroup->actions());
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("LogPulse"));
    openAction->setText(tr("&Open..."));
    openAction->setStatusTip(tr("Open an existing file"));
    
    if (fileMenu) fileMenu->setTitle(tr("&File"));
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

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    mView        = new TraceView;
    mYSlider     = new QSlider(Qt::Horizontal);
    mXSlider     = new QSlider(Qt::Horizontal);
    mSearchbar   = new QLineEdit;
    mSeqView     = new SequenceView;


    QToolBar * bar = addToolBar("actions");

    setCentralWidget(mView);

    mSearchbar->setMaximumWidth(200);
    mSearchbar->setPlaceholderText("Sequence ...");
    mSearchbar->setVisible(false);

    QAction * openAction = bar->addAction("Open");
    QWidget * spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    bar->addWidget(spacer);
    bar->addWidget(mSearchbar);

    mYSlider->setRange(6,1000);
    mXSlider->setRange(10,1000);
    mXSlider->setToolTip("Scale");

    mXSlider->setMaximumWidth(100);
    mYSlider->setMaximumWidth(100);
    mYSlider->setValue(0.2);
    mYSlider->setToolTip("Amplitude");

    QStatusBar * statusBar = new QStatusBar;

    statusBar->addPermanentWidget(new QLabel("Scale"));
    statusBar->addPermanentWidget(mXSlider);
    statusBar->addPermanentWidget(new QLabel("Amplitude"));
    statusBar->addPermanentWidget(mYSlider);
    setStatusBar(statusBar);



    addDock(mSeqView);
    addDock(new InfoView);




    connect(mYSlider, &QSlider::valueChanged, [=](){mView->setAmplitudeFactor(mYSlider->value() / 1000.0 );});
    connect(mXSlider, &QSlider::valueChanged, [=](){mView->setScaleFactor(mXSlider->value() / 100.0);});
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(mSeqView, &SequenceView::selectionChanged, this, &MainWindow::updateSelection);

    resize(1000, 800);
}

MainWindow::~MainWindow()
{

}



void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::currentPath());
    setFilename(file);

}

void MainWindow::setFilename(const QString &filename)
{
    mFile = filename;

    if (filename.isEmpty()) return;

    if (QFile::exists(filename)){
        mView->setFilename(filename);
        mSeqView->setSequence(mView->sequenceTrace()->sequence());
    }
    else
        QMessageBox::warning(this,"error","cannot find file " + filename);


}


void MainWindow::closeEvent(QCloseEvent *)
{
}

void MainWindow::writeSettings()
{

}

void MainWindow::restoreSettings()
{

}

void MainWindow::updateSelection()
{

    QTextCursor cursor = mSeqView->textCursor();
    if (cursor.hasSelection())
    {
        int start  = cursor.selectionStart();
        int length = cursor.selectionEnd() + start;

        mView->setSelection(start, length);
    }


}

void MainWindow::addDock(QWidget *widget)
{
    QDockWidget * dock = new QDockWidget;
    dock->setWidget(widget);
    dock->setWindowTitle(widget->windowTitle());
    addDockWidget(Qt::BottomDockWidgetArea, dock);


}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    QSvgGenerator generator;
    generator.setFileName("/tmp/capture.svg");
    generator.setTitle("test");
    generator.setDescription("description");
    render(&generator);


    return QMainWindow::keyPressEvent(event);

}

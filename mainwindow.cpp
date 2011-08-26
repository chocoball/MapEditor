#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csetting.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	ui->action_Open->setShortcut(QKeySequence::Open);
	connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(slot_fileOpen())) ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_fileOpen()
{
	QString fileName = QFileDialog::getOpenFileName(
											this,
											tr("Open File"),
											g_Setting->getFileOpenDir(),
											tr("All Files (*);;Image Files (*.png);;")) ;
	if ( fileName.isEmpty() ) {
		return ;
	}
}

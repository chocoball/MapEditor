#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"
#include "cimagelabel.h"

#define kExecName		"MapEditor"
#define kVersion		0x00000000
#define kFileExt_JSON	".json"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setWindowTitle(kExecName);

	QLabel *pLabelImage = new CImageLabel(this) ;
	ui->scrollArea_Image->setWidget(pLabelImage);
	g_EditData->setImageLabel(pLabelImage) ;

	m_pSplitterImage = new QSplitter(ui->centralWidget) ;
	m_pSplitterImage->addWidget(ui->frame_image) ;
	m_pSplitterImage->addWidget(ui->groupBox_setting);
	m_pSplitterImage->setOrientation(Qt::Vertical);
	QPoint pos = ui->frame_image->mapToParent(QPoint(0, 0)) ;
	m_pSplitterImage->setGeometry(pos.x(),
								  pos.y(),
								  ui->frame_image->width(),
								  ui->groupBox_setting->y()+ui->groupBox_setting->height()-pos.y());

	m_pSplitterMap = new QSplitter(ui->centralWidget) ;
	m_pSplitterMap->addWidget(m_pSplitterImage) ;
	m_pSplitterMap->addWidget(ui->frame_map);
	pos = m_pSplitterImage->mapToParent(QPoint(0, 0)) ;
	m_pSplitterMap->setGeometry(pos.x(),
								pos.y(),
								m_pSplitterImage->width()+ui->frame_map->width(),
								ui->frame_map->height());

	restoreSettings() ;

	ui->action_Open->setShortcut(QKeySequence::Open);
	ui->action_Save->setShortcut(QKeySequence::Save);
	ui->action_SaveAs->setShortcut(QKeySequence::SaveAs);
	connect(ui->action_Open,	SIGNAL(triggered()), this, SLOT(slot_fileOpen())) ;
	connect(ui->action_Save,	SIGNAL(triggered()), this, SLOT(slot_fileSave())) ;
	connect(ui->action_SaveAs,	SIGNAL(triggered()), this, SLOT(slot_fileSaveAs())) ;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
	g_Setting->setMainWindowGeometry(saveGeometry()) ;
	g_Setting->setMainWindowState(saveState(kVersion)) ;
	g_Setting->writeSetting();
}

// 開く
void MainWindow::slot_fileOpen()
{
	QString fileName = QFileDialog::getOpenFileName(
											this,
											tr("Open File"),
											g_Setting->getFileOpenDir(),
											tr("Image Files (*.png);;Text Files("kFileExt_JSON");;")) ;
	if ( fileName.isEmpty() ) {
		return ;
	}

	fileOpen(fileName) ;
}

// 保存
void MainWindow::slot_fileSave()
{
	if ( m_strSaveFileName.isEmpty() ) {
		slot_fileSaveAs();
	}
	else {
		fileWrite(m_strSaveFileName) ;
	}
}

// 名前を付けて保存
void MainWindow::slot_fileSaveAs()
{
	QString str = QFileDialog::getSaveFileName(this,
											   trUtf8("名前を付けて保存"),
											   g_Setting->getFileSaveDir(),
											   tr("Text (*"kFileExt_JSON");;")) ;
	if ( str.isEmpty() ) { return ; }

	fileWrite(str) ;
}

// ファイルを開く
void MainWindow::fileOpen(QString &fileName)
{
	g_Setting->setFileOpenDir(fileName) ;

	if ( fileName.toLower().indexOf(".png") <= 0
	  && fileName.toLower().indexOf(kFileExt_JSON) <= 0 ) {
		QMessageBox::warning(this, tr("warning"), trUtf8("対応していないファイルです") ) ;
		return ;
	}

	if ( fileName.toLower().indexOf(".png") > 0 ) {	// PNGファイル
		QImage image ;
		if ( !image.load(fileName) ) {
			QMessageBox::warning(this, trUtf8("エラー"), trUtf8("読み込みに失敗しました:%1").arg(fileName)) ;
			return ;
		}
		qDebug() << "orig size:" << image.size() ;
		g_EditData->setImage(image) ;
		g_EditData->updateMap() ;
		m_strSaveFileName = QString() ;
	}
	else if ( fileName.toLower().indexOf(kFileExt_JSON) > 0 ) {	// JSONファイル
		m_strSaveFileName = fileName ;
	}

	setWindowTitle(tr(kExecName"[%1]").arg(m_strSaveFileName));
}

// ファイル保存
void MainWindow::fileWrite(QString &fileName)
{
	g_Setting->setFileSaveDir(fileName) ;
}

void MainWindow::restoreSettings()
{
	ui->spinBox_grid_w->setValue(g_Setting->getGridW()) ;
	ui->spinBox_grid_h->setValue(g_Setting->getGridH()) ;

	restoreState(g_Setting->getMainWindowState()) ;
	restoreGeometry(g_Setting->getMainWindowGeometry()) ;
}


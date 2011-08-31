#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"
#include "cimagelabel.h"
#include "cmaplabel.h"

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
	pLabelImage->show();
	ui->scrollArea_Image->setWidget(pLabelImage);
	g_EditData->setImageLabel(pLabelImage) ;

	CMapLabel *pLabelMap = new CMapLabel(this) ;
	pLabelMap->show();
	ui->scrollArea_Map->setWidget(pLabelMap) ;
	g_EditData->setMapLabel(pLabelMap) ;
	g_EditData->updateMap();

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
	m_pSplitterMap->addWidget(ui->frame_map);
	m_pSplitterMap->addWidget(m_pSplitterImage) ;
	pos = m_pSplitterImage->mapToParent(QPoint(0, 0)) ;
	m_pSplitterMap->setGeometry(pos.x(),
								pos.y(),
								m_pSplitterImage->width()+ui->frame_map->width(),
								ui->frame_map->height());

	setSpaceSize() ;
	restoreSettings() ;

	ui->action_Open->setShortcut(QKeySequence::Open);
	ui->action_Save->setShortcut(QKeySequence::Save);
	ui->action_SaveAs->setShortcut(QKeySequence::SaveAs);
	connect(ui->action_Open,	SIGNAL(triggered()), this, SLOT(slot_fileOpen())) ;
	connect(ui->action_Save,	SIGNAL(triggered()), this, SLOT(slot_fileSave())) ;
	connect(ui->action_SaveAs,	SIGNAL(triggered()), this, SLOT(slot_fileSaveAs())) ;

	connect(m_pSplitterImage, SIGNAL(splitterMoved(int,int)), this, SLOT(slot_splitterMoveImage(int, int))) ;
	connect(m_pSplitterMap, SIGNAL(splitterMoved(int,int)), this, SLOT(slot_splitterMoveMap(int, int))) ;
	connect(pLabelImage, SIGNAL(sig_changeSelectGridRect()), pLabelMap, SLOT(slot_changeSelectGridRect())) ;
	connect(ui->spinBox_grid_w_img, SIGNAL(valueChanged(int)), this, SLOT(slot_changeImageGridW(int))) ;
	connect(ui->spinBox_grid_h_img, SIGNAL(valueChanged(int)), this, SLOT(slot_changeImageGridH(int))) ;
	connect(ui->spinBox_grid_w_map, SIGNAL(valueChanged(int)), this, SLOT(slot_changeMapGridW(int))) ;
	connect(ui->spinBox_grid_h_map, SIGNAL(valueChanged(int)), this, SLOT(slot_changeMapGridH(int))) ;
	connect(this, SIGNAL(sig_keyPress(QKeyEvent*)), pLabelMap, SLOT(slot_keyPress(QKeyEvent*))) ;
	connect(this, SIGNAL(sig_keyRelease(QKeyEvent*)), pLabelMap, SLOT(slot_keyRelease(QKeyEvent*))) ;
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

void MainWindow::resizeEvent(QResizeEvent *)
{
	QSize size = this->size() - m_windowSpace ;
	m_pSplitterMap->resize(size) ;

	slot_splitterMoveMap(0, 0);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	emit sig_keyPress(event) ;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	emit sig_keyRelease(event) ;
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

void MainWindow::slot_splitterMoveImage(int, int)
{
	QSize size ;
	size = ui->frame_image->size() - QSize(ui->scrollArea_Image->x(), ui->scrollArea_Image->y()) - m_frameImageSpace ;
	ui->scrollArea_Image->resize(size) ;
}

void MainWindow::slot_splitterMoveMap(int, int)
{
	QSize size ;
	size = ui->frame_map->size() - QSize(ui->scrollArea_Map->x(), ui->scrollArea_Map->y()) - m_frameMapSpace ;
	ui->scrollArea_Map->resize(size) ;

	slot_splitterMoveImage(0, 0) ;
}

void MainWindow::slot_changeImageGridW(int val)
{
	QSize size = g_Setting->getImageGridSize() ;
	if ( size.width() == val ) { return ; }

	size.setWidth(val) ;
	g_Setting->setImageGridSize(size) ;
	g_EditData->updateImage();
}

void MainWindow::slot_changeImageGridH(int val)
{
	QSize size = g_Setting->getImageGridSize() ;
	if ( size.height() == val ) { return ; }

	size.setHeight(val) ;
	g_Setting->setImageGridSize(size) ;
	g_EditData->updateImage();
}

void MainWindow::slot_changeMapGridW(int val)
{
	QSize size = g_Setting->getMapGridSize() ;
	if ( size.width() == val ) { return ; }

	size.setWidth(val) ;
	g_Setting->setMapGridSize(size) ;
	g_EditData->updateMap();
}

void MainWindow::slot_changeMapGridH(int val)
{
	QSize size = g_Setting->getMapGridSize() ;
	if ( size.height() == val ) { return ; }

	size.setHeight(val) ;
	g_Setting->setMapGridSize(size) ;
	g_EditData->updateMap();
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
		g_EditData->update() ;
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
	ui->spinBox_grid_w_img->setValue(g_Setting->getImageGridSize().width()) ;
	ui->spinBox_grid_h_img->setValue(g_Setting->getImageGridSize().height()) ;
	ui->spinBox_grid_w_map->setValue(g_Setting->getMapGridSize().width()) ;
	ui->spinBox_grid_h_map->setValue(g_Setting->getMapGridSize().height()) ;

	restoreState(g_Setting->getMainWindowState()) ;
	restoreGeometry(g_Setting->getMainWindowGeometry()) ;

//	slot_splitterMoveMap(0, 0);
}

void MainWindow::setSpaceSize()
{
	m_frameImageSpace = QSize(291, 321) - QSize(10, 50) - QSize(271, 261) ;
	m_frameMapSpace = QSize(401, 451) - QSize(10, 20) - QSize(381, 421) ;
	m_windowSpace = QSize(700, 473) - QSize(692, 451) ;

	qDebug() << "image space" << m_frameImageSpace ;
	qDebug() << "map space" << m_frameMapSpace ;
	qDebug() << "window space" << m_windowSpace ;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"
#include "cimagelabel.h"
#include "cmaplabel.h"
#include "csavefile.h"

#define kExecName		"MapEditor"
#define kVersion		0x00000000
#define kFileExt_JSON	".json"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->treeView->setModel(&g_EditData->getModel()) ;
	ui->treeView->setHeaderHidden(true) ;

	setWindowTitle(kExecName);

	QLabel *pLabelImage = new CImageLabel(this) ;
	pLabelImage->show();
	ui->scrollArea_Image->setWidget(pLabelImage);
	g_EditData->setImageLabel(pLabelImage) ;

	CMapLabel *pLabelMap = new CMapLabel(this) ;
	pLabelMap->show();
	ui->scrollArea_Map->setWidget(pLabelMap) ;
	g_EditData->setMapLabel(pLabelMap) ;

	QTabWidget *tabWidget = new QTabWidget(this) ;
	tabWidget->addTab(ui->frame_image, trUtf8("マップ")) ;

	m_pSplitterMap = new QSplitter(ui->centralWidget) ;
	m_pSplitterMap->addWidget(ui->frame_tree);
	m_pSplitterMap->addWidget(ui->frame_map);
	m_pSplitterMap->addWidget(tabWidget) ;
	m_pSplitterMap->setGeometry(0,
								0,
								tabWidget->width()+ui->frame_map->width()+ui->frame_tree->width(),
								ui->frame_map->height());

	setSpaceSize() ;
	restoreSettings() ;

	m_pActAddMap		= new QAction("Map", this) ;
	m_pActAddTreasure	= new QAction(trUtf8("お宝"), this) ;

	ui->action_Open->setShortcut(QKeySequence::Open);
	ui->action_Save->setShortcut(QKeySequence::Save);
	ui->action_SaveAs->setShortcut(QKeySequence::SaveAs);

	connect(ui->action_Open,		SIGNAL(triggered()),				this,		SLOT(slot_fileOpen())) ;
	connect(ui->action_Save,		SIGNAL(triggered()),				this,		SLOT(slot_fileSave())) ;
	connect(ui->action_SaveAs,		SIGNAL(triggered()),				this,		SLOT(slot_fileSaveAs())) ;
	connect(ui->spinBox_grid_w_img, SIGNAL(valueChanged(int)),			this,		SLOT(slot_changeImageGridW(int))) ;
	connect(ui->spinBox_grid_h_img, SIGNAL(valueChanged(int)),			this,		SLOT(slot_changeImageGridH(int))) ;
	connect(ui->spinBox_grid_w_map, SIGNAL(valueChanged(int)),			this,		SLOT(slot_changeMapGridW(int))) ;
	connect(ui->spinBox_grid_h_map, SIGNAL(valueChanged(int)),			this,		SLOT(slot_changeMapGridH(int))) ;
	connect(ui->checkBox_unit,		SIGNAL(clicked(bool)),				this,		SLOT(slot_clickCheckBoxUnit(bool))) ;
	connect(ui->checkBox_through,	SIGNAL(clicked(bool)),				this,		SLOT(slot_clickCheckBoxThrough(bool))) ;
	connect(m_pSplitterMap,			SIGNAL(splitterMoved(int,int)),		this,		SLOT(slot_splitterMoveMap(int, int))) ;
	connect(pLabelImage,			SIGNAL(sig_changeSelectGridRect()),	pLabelMap,	SLOT(slot_changeSelectGridRect())) ;
	connect(pLabelImage,			SIGNAL(sig_changeSelectGridRect()),	this,		SLOT(slot_changeSelectGridRect())) ;
	connect(this,					SIGNAL(sig_keyPress(QKeyEvent*)),	pLabelMap,	SLOT(slot_keyPress(QKeyEvent*))) ;
	connect(this,					SIGNAL(sig_keyRelease(QKeyEvent*)),	pLabelMap,	SLOT(slot_keyRelease(QKeyEvent*))) ;
	connect(ui->pushButton_add,		SIGNAL(clicked()),					this,		SLOT(slot_clickPushAdd())) ;
	connect(ui->pushButton_del,		SIGNAL(clicked()),					this,		SLOT(slot_clickPushDel())) ;
	connect(ui->treeView,			SIGNAL(clicked(QModelIndex)),		this,		SLOT(slot_treeViewClicked(QModelIndex))) ;
	connect(m_pActAddMap,			SIGNAL(triggered()),				this,		SLOT(slot_addMap())) ;
	connect(m_pActAddTreasure,		SIGNAL(triggered()),				this,		SLOT(slot_addTreasure())) ;
	connect(&g_EditData->getModel(),SIGNAL(itemChanged(QStandardItem*)),this,		SLOT(slot_changeTreeItem(QStandardItem*))) ;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
	QSize size ;
	size.setWidth(ui->spinBox_grid_w_img->value()) ;
	size.setHeight(ui->spinBox_grid_h_img->value()) ;
	g_Setting->setImageGridSize(size) ;

	size.setWidth(ui->spinBox_grid_w_map->value()) ;
	size.setHeight(ui->spinBox_grid_h_map->value()) ;
	g_Setting->setMapGridSize(size) ;

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

// Image frame 幅変更
void MainWindow::slot_splitterMoveImage(int, int)
{
	QSize size ;
	size = ui->frame_image->size() - QSize(ui->scrollArea_Image->x(), ui->scrollArea_Image->y()) - m_frameImageSpace ;
	ui->scrollArea_Image->resize(size) ;

	int space = 340 - (70+261) ;
	ui->groupBox_setting->move(ui->groupBox_setting->x(), ui->scrollArea_Image->y()+size.height()+space);
}

// Map frame 幅変更
void MainWindow::slot_splitterMoveMap(int, int)
{
	QSize size ;
	size = ui->frame_map->size() - QSize(ui->scrollArea_Map->x(), ui->scrollArea_Map->y()) - m_frameMapSpace ;
	ui->scrollArea_Map->resize(size) ;
	size = ui->frame_tree->size() - QSize(ui->treeView->x(), ui->treeView->y()) - m_frameTreeSpace ;
	ui->treeView->resize(size) ;

	int y = 410 - 391 + ui->treeView->y() + ui->treeView->height() ;
	ui->pushButton_add->move(ui->pushButton_add->x(), y) ;
	ui->pushButton_del->move(ui->pushButton_del->x(), y) ;

	slot_splitterMoveImage(0, 0) ;
}

// image grid w 変更
void MainWindow::slot_changeImageGridW(int val)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->imgGridSize.width() == val ) { return ; }

	p->imgGridSize.setWidth(val) ;
	g_EditData->updateImage();
}

// image grid h 変更
void MainWindow::slot_changeImageGridH(int val)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->imgGridSize.height() == val ) { return ; }

	p->imgGridSize.setHeight(val) ;
	g_EditData->updateImage();
}

// map grid w 変更
void MainWindow::slot_changeMapGridW(int val)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->mapGridSize.width() == val ) { return ; }

	p->mapGridSize.setWidth(val) ;
	g_EditData->updateMap();
}

// map grid h 変更
void MainWindow::slot_changeMapGridH(int val)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->mapGridSize.height() == val ) { return ; }

	p->mapGridSize.setHeight(val) ;
	g_EditData->updateMap();
}

// ユニット配置 チェックボックス変更
void MainWindow::slot_clickCheckBoxUnit(bool val)
{
	qDebug() << "slot_clickCheckBoxUnit" << val ;

	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	ui->checkBox_unit->setCheckState(val ? Qt::Checked : Qt::Unchecked) ;

	QPoint st = g_EditData->getSelStartGrid() ;
	QPoint end = g_EditData->getSelEndGrid() ;
	for ( int y = st.y() ; y <= end.y() ; y ++ ) {
		for ( int x = st.x() ; x <= end.x() ; x ++ ) {
			QPoint grid = QPoint(x, y) ;
			int index = p->getImageDataIndex(grid) ;
			if ( index >= 0 ) {
				CEditData::ImageData &data = p->getImageData(index) ;
				data.bUnitable = val ;
			}
			else {
				p->addImageData(grid, val, false) ;
			}
		}
	}
}

// 敵通過 チェックボックス変更
void MainWindow::slot_clickCheckBoxThrough(bool val)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	ui->checkBox_through->setCheckState(val ? Qt::Checked : Qt::Unchecked) ;

	QPoint st = g_EditData->getSelStartGrid() ;
	QPoint end = g_EditData->getSelEndGrid() ;
	for ( int y = st.y() ; y <= end.y() ; y ++ ) {
		for ( int x = st.x() ; x <= end.x() ; x ++ ) {
			QPoint grid = QPoint(x, y) ;
			int index = p->getImageDataIndex(grid);
			if ( index >= 0 ) {
				CEditData::ImageData &data = p->getImageData(index) ;
				data.bThrough = val ;
			}
			else {
				p->addImageData(grid, false, val) ;
			}
		}
	}
}

// image label 選択グリッド変更
void MainWindow::slot_changeSelectGridRect()
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	bool bUnit = false, bThrough = false ;
	QPoint st = g_EditData->getSelStartGrid() ;
	QPoint end = g_EditData->getSelEndGrid() ;
	for ( int y = st.y() ; y <= end.y() ; y ++ ) {
		for ( int x = st.x() ; x <= end.x() ; x ++ ) {
			QPoint grid = QPoint(x, y) ;
			int index = p->getImageDataIndex(grid) ;
			if ( index < 0 ) { continue ; }
			CEditData::ImageData &data = p->getImageData(index) ;
			if ( data.bUnitable ) { bUnit = data.bUnitable ; }
			if ( data.bThrough ) { bThrough = data.bThrough ; }
		}
	}

	if ( st.x() != end.x() || st.y() != end.y() ) {
		if ( bUnit ) {
			ui->checkBox_unit->setCheckState(Qt::PartiallyChecked);
		}
		else {
			ui->checkBox_unit->setChecked(false) ;
		}
		if ( bThrough ) {
			ui->checkBox_through->setCheckState(Qt::PartiallyChecked);
		}
		else {
			ui->checkBox_through->setChecked(false);
		}
	}
	else {
		ui->checkBox_unit->setChecked(bUnit) ;
		ui->checkBox_through->setChecked(bThrough);
	}
}

// 追加 押下
void MainWindow::slot_clickPushAdd()
{
	QMenu menu(this) ;
	QList<QAction *> list ;
	list << m_pActAddMap << m_pActAddTreasure ;
	menu.exec(list, this->cursor().pos()) ;
}

// 削除 押下
void MainWindow::slot_clickPushDel()
{
	if ( !g_EditData->isSelectMap() ) { return ; }

	QModelIndex index = g_EditData->getSelModelIndex() ;
	QStandardItem *pItem = g_EditData->getModel().itemFromIndex(index) ;
	if ( !pItem ) { return ; }

	if ( index.internalPointer() == g_EditData->getModel().invisibleRootItem() ) {
		g_EditData->delMap(pItem) ;
	}
	else {
		CEditData::MapData *p = g_EditData->getSelectMapData() ;
		if ( !p ) { return ; }
		p->removeTreasureData(pItem) ;
		QStandardItem *pMapItem = g_EditData->getSelectMapItem() ;
		if ( pMapItem ) {
			pMapItem->removeRow(index.row());
		}
	}
}

// treeView クリック
void MainWindow::slot_treeViewClicked(QModelIndex index)
{
	qDebug() << "slot_treeViewClicked" ;

	QModelIndex old = g_EditData->getSelModelIndex() ;
	g_EditData->setSelModelIndex(index) ;

	if ( old != index ) {
		CEditData::MapData *p = g_EditData->getSelectMapData() ;
		if ( p ) {
			ui->spinBox_grid_w_img->setValue(p->imgGridSize.width());
			ui->spinBox_grid_h_img->setValue(p->imgGridSize.height());
			ui->spinBox_grid_w_map->setValue(p->mapGridSize.width());
			ui->spinBox_grid_h_map->setValue(p->mapGridSize.height());

			g_EditData->update() ;
		}
	}
}

// マップ追加
void MainWindow::slot_addMap()
{
	bool bOk = false ;
	QString str = QInputDialog::getText(this,
										trUtf8("New Map"),
										trUtf8("マップ名を入力してください"),
										QLineEdit::Normal,
										trUtf8("0x00000000"),
										&bOk) ;
	if ( !bOk )				{ return ; }
	if ( str.isEmpty() )	{ return ; }

	g_EditData->addMap(str) ;
}

// お宝追加
void MainWindow::slot_addTreasure()
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) {
		QMessageBox::warning(this, trUtf8("エラー"), trUtf8("マップが選択されていません") ) ;
		return ;
	}

	bool bOk = false ;
	int num = QInputDialog::getInt(this, trUtf8("個数を入力してください"), trUtf8("個数"), 1, 1, 1024, 1, &bOk) ;
	if ( !bOk ) { return ; }

	QModelIndex modelIndex = g_EditData->getSelModelIndex() ;
	while ( modelIndex.internalPointer() != g_EditData->getModel().invisibleRootItem() ) { modelIndex = modelIndex.parent() ; }
	QStandardItem *pRoot = g_EditData->getModel().itemFromIndex(modelIndex) ;
	if ( !pRoot ) { return ; }

	QStandardItem *pNew = new QStandardItem(trUtf8("%0").arg(num)) ;
	pRoot->appendRow(pNew) ;
	p->addTreasureData(QPoint(), num, pNew) ;

	ui->treeView->setCurrentIndex(pNew->index());
}

// ツリービューアイテム 変更
void MainWindow::slot_changeTreeItem(QStandardItem *item)
{
	qDebug() << "slot_changeTreeItem" ;
	if ( item->index().internalPointer() == g_EditData->getModel().invisibleRootItem() ) { return ; }

	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	int index = p->getTreasureDataIndex(item) ;
	if ( index < 0 ) { return ; }
	CEditData::TreasureData &data = p->getTreasureData(index) ;
	data.num = item->text().toInt() ;
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

		CEditData::MapData *p = g_EditData->getSelectMapData() ;
		if ( !p ) {
			QStandardItem *id = g_EditData->addMap("0x00000000") ;
			int index = g_EditData->getMapDataIndex(id) ;
			if ( index < 0 ) {
				QMessageBox::warning(this, trUtf8("エラー"), trUtf8("未知の不具合")) ;

				return ;
			}
			CEditData::MapData &data = g_EditData->getMapData(index) ;
			data.image = image ;

			ui->treeView->setCurrentIndex(id->index()) ;
		}
		else {
			p->image = image ;
			g_EditData->update() ;
		}

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
	if ( fileName.indexOf(kFileExt_JSON) > 0 ) {
		g_Setting->setFileSaveDir(fileName) ;
	}
}

void MainWindow::restoreSettings()
{
	ui->spinBox_grid_w_img->setValue(g_Setting->getImageGridSize().width()) ;
	ui->spinBox_grid_h_img->setValue(g_Setting->getImageGridSize().height()) ;
	ui->spinBox_grid_w_map->setValue(g_Setting->getMapGridSize().width()) ;
	ui->spinBox_grid_h_map->setValue(g_Setting->getMapGridSize().height()) ;

	restoreState(g_Setting->getMainWindowState()) ;
	restoreGeometry(g_Setting->getMainWindowGeometry()) ;
}

void MainWindow::setSpaceSize()
{
	m_frameImageSpace = QSize(10, 451-70-281) ;
	m_frameMapSpace = QSize(10, 10) ;
	m_frameTreeSpace = QSize(10, 60) ;
	m_windowSpace = QSize(10, 26) ;
}

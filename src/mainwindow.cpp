#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"
#include "cimagelabel.h"
#include "cmaplabel.h"
#include "csavefile.h"
#include "ccomboboxdelegate.h"

#define kExecName		"MapEditor"
#define kVersion		0x00000000
#define kFileExt_JSON	".json"
#define kFileExt_XML	".xml"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	dataInit() ;

	CComboBoxDelegate *pDelegate = new CComboBoxDelegate(this) ;
	QStringList dispList ;
	dispList << trUtf8("開始位置") << trUtf8("終了位置") ;
	pDelegate->setDisplayList(dispList) ;
	ui->listView_point->setItemDelegate(pDelegate) ;

	setWindowTitle(kExecName);

	CImageLabel *pLabelImage = new CImageLabel(this) ;
	pLabelImage->show();
	ui->scrollArea_Image->setWidget(pLabelImage);
	g_EditData->setImageLabel(pLabelImage) ;

	CMapLabel *pLabelMap = new CMapLabel(this) ;
	pLabelMap->show();
	ui->scrollArea_Map->setWidget(pLabelMap) ;
	g_EditData->setMapLabel(pLabelMap) ;

	QTabWidget *tabWidget = new QTabWidget(this) ;
	tabWidget->insertTab(CEditData::kEditMode_Map, ui->frame_image, trUtf8("マップ")) ;
	tabWidget->insertTab(CEditData::kEditMode_Data, ui->frame_data, trUtf8("データ")) ;

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

	m_pActAddStartPoint = new QAction(trUtf8("開始位置"), this) ;
	m_pActAddEndPoint = new QAction(trUtf8("終了位置"), this) ;

	ui->action_Open->setShortcut(QKeySequence::Open);
	ui->action_Save->setShortcut(QKeySequence::Save);
	ui->action_SaveAs->setShortcut(QKeySequence::SaveAs);
	ui->action_Quit->setShortcut(QKeySequence::Quit) ;
#if 0
	ui->action_Square->setCheckable(true) ;
	ui->action_Quarter->setCheckable(true) ;
	if ( g_EditData->getViewMode() == CEditData::kViewMode_Square ) {
		ui->action_Square->setChecked(true) ;
		ui->action_Quarter->setChecked(false) ;
	}
	else {
		ui->action_Square->setChecked(false) ;
		ui->action_Quarter->setChecked(true) ;
	}
#endif
	m_pActUndo = g_EditData->getUndoStack()->createUndoAction(this, trUtf8("元に戻す")) ;
	m_pActUndo->setShortcuts(QKeySequence::Undo);
	m_pActRedo = g_EditData->getUndoStack()->createRedoAction(this, trUtf8("やり直す")) ;
	m_pActRedo->setShortcuts(QKeySequence::Redo);
	ui->menu_Edit->addAction(m_pActUndo) ;
	ui->menu_Edit->addAction(m_pActRedo) ;

	connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(slot_fileOpen())) ;
	connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(slot_fileSave())) ;
	connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(slot_fileSaveAs())) ;
	connect(ui->action_Quit, SIGNAL(triggered()), this, SLOT(close())) ;
#if 0
	connect(ui->action_Square, SIGNAL(triggered()), this, SLOT(slot_clickViewSquare())) ;
	connect(ui->action_Quarter, SIGNAL(triggered()), this, SLOT(slot_clickViewQuarter())) ;
#endif
	connect(ui->spinBox_grid_w_img, SIGNAL(valueChanged(int)), this, SLOT(slot_changeImageGridW(int))) ;
	connect(ui->spinBox_grid_h_img, SIGNAL(valueChanged(int)), this, SLOT(slot_changeImageGridH(int))) ;
	connect(ui->spinBox_grid_w_map, SIGNAL(valueChanged(int)), this, SLOT(slot_changeMapGridW(int))) ;
	connect(ui->spinBox_grid_h_map, SIGNAL(valueChanged(int)), this, SLOT(slot_changeMapGridH(int))) ;
	connect(ui->checkBox_unit, SIGNAL(clicked(bool)), this, SLOT(slot_clickCheckBoxUnit(bool))) ;
	connect(ui->checkBox_through, SIGNAL(clicked(bool)), this, SLOT(slot_clickCheckBoxThrough(bool))) ;
	connect(m_pSplitterMap, SIGNAL(splitterMoved(int,int)), this, SLOT(slot_splitterMoveMap(int, int))) ;
	connect(pLabelImage, SIGNAL(sig_changeSelectGridRect()), pLabelMap, SLOT(slot_changeSelectGridRect())) ;
	connect(pLabelImage, SIGNAL(sig_changeSelectGridRect()), this, SLOT(slot_changeSelectGridRect())) ;
	connect(this, SIGNAL(sig_keyPress(QKeyEvent*)), pLabelMap, SLOT(slot_keyPress(QKeyEvent*))) ;
	connect(this, SIGNAL(sig_keyRelease(QKeyEvent*)), pLabelMap, SLOT(slot_keyRelease(QKeyEvent*))) ;
	connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(slot_clickPushAdd())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(slot_clickPushDel())) ;
	connect(ui->listView_map, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clickedListViewMap(QModelIndex))) ;
	connect(ui->listView_treasure, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clickedListViewTreasure(QModelIndex))) ;
	connect(ui->listView_point, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clickedListViewPoint(QModelIndex))) ;
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slot_tabChanged(int))) ;
	connect(ui->pushButton_treasure_add, SIGNAL(clicked()), this, SLOT(slot_pushAddTreasure())) ;
	connect(ui->pushButton_treasure_del, SIGNAL(clicked()), this, SLOT(slot_pushDelTreasure())) ;
	connect(ui->pushButton_pos_add, SIGNAL(clicked()), this, SLOT(slot_pushAddPoint())) ;
	connect(ui->pushButton_pos_del, SIGNAL(clicked()), this, SLOT(slot_pushDelPoint())) ;
	connect(m_pActAddStartPoint, SIGNAL(triggered()), this, SLOT(slot_clickAddStartPoint())) ;
	connect(m_pActAddEndPoint, SIGNAL(triggered()), this, SLOT(slot_clickAddEndPoint())) ;
	connect(g_EditData->getUndoStack(), SIGNAL(indexChanged(int)), this, SLOT(slot_changeDataModified(int))) ;
}

MainWindow::~MainWindow()
{
	g_EditData->release() ;
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if ( !checkSaveFileChanged() ) {
		event->ignore();
		return ;
	}

//	g_Setting->setViewMode(g_EditData->getViewMode()) ;

	QSize size ;
	size.setWidth(ui->spinBox_grid_w_img->value()) ;
	size.setHeight(ui->spinBox_grid_h_img->value()) ;
	g_Setting->setImageGridSize(size) ;

	size.setWidth(ui->spinBox_grid_w_map->value()) ;
	size.setHeight(ui->spinBox_grid_h_map->value()) ;
	g_Setting->setMapGridSize(size) ;

	g_Setting->setScrollAreaMapGeometry(ui->scrollArea_Map->saveGeometry()) ;

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
											tr("All Files (*.*);;Image (*.png);;JSON (*"kFileExt_JSON");;XML (*"kFileExt_XML");;")) ;
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
											   tr("XML (*"kFileExt_XML");;JSON (*"kFileExt_JSON");;")) ;
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

	QSize frameDataSize = ui->frame_data->size() ;
	size = frameDataSize ;
	size.setWidth(size.width()-ui->listView_treasure->x() - 10);
	size.setHeight(frameDataSize.height()/2 - (230-212)-ui->listView_treasure->y()) ;
	ui->listView_treasure->resize(size);

	size.setHeight(frameDataSize.height()/2) ;
	ui->label_point->move(QPoint(ui->label_point->x(), size.height())) ;
	ui->pushButton_pos_add->move(QPoint(ui->pushButton_pos_add->x(), size.height()));
	ui->pushButton_pos_del->move(QPoint(ui->pushButton_pos_del->x(), size.height()));

	ui->listView_point->move(QPoint(ui->listView_point->x(), size.height() + (260-230))) ;
	ui->listView_point->resize(frameDataSize - QSize(ui->listView_point->x(), ui->listView_point->y()) - m_frameMapSpace) ;
}

// Map frame 幅変更
void MainWindow::slot_splitterMoveMap(int, int)
{
	QSize size ;
	size = ui->frame_map->size() - QSize(ui->scrollArea_Map->x(), ui->scrollArea_Map->y()) - m_frameMapSpace ;
	ui->scrollArea_Map->resize(size) ;
	size = ui->frame_tree->size() - QSize(ui->listView_map->x(), ui->listView_map->y()) - m_frameTreeSpace ;
	ui->listView_map->resize(size) ;

	int y = 410 - 391 + ui->listView_map->y() + ui->listView_map->height() ;
	ui->pushButton_add->move(ui->pushButton_add->x(), y) ;
	ui->pushButton_del->move(ui->pushButton_del->x(), y) ;

	slot_splitterMoveImage(0, 0) ;
}

// image grid w 変更
void MainWindow::slot_changeImageGridW(int val)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->imgGridSize.width() == val ) { return ; }

	p->imgGridSize.setWidth(val) ;
	g_EditData->updateImage();
}

// image grid h 変更
void MainWindow::slot_changeImageGridH(int val)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->imgGridSize.height() == val ) { return ; }

	p->imgGridSize.setHeight(val) ;
	g_EditData->updateImage();
}

// map grid w 変更
void MainWindow::slot_changeMapGridW(int val)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->mapGridSize.width() == val ) { return ; }

	p->mapGridSize.setWidth(val) ;
	g_EditData->updateMap();
}

// map grid h 変更
void MainWindow::slot_changeMapGridH(int val)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->mapGridSize.height() == val ) { return ; }

	p->mapGridSize.setHeight(val) ;
	g_EditData->updateMap();
}

// ユニット配置 チェックボックス変更
void MainWindow::slot_clickCheckBoxUnit(bool val)
{
	qDebug() << "slot_clickCheckBoxUnit" << val ;

	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	ui->checkBox_unit->setCheckState(val ? Qt::Checked : Qt::Unchecked) ;

	QPoint st = g_EditData->getSelStartGrid() ;
	QPoint end = g_EditData->getSelEndGrid() ;
	for ( int y = st.y() ; y <= end.y() ; y ++ ) {
		for ( int x = st.x() ; x <= end.x() ; x ++ ) {
			QPoint grid = QPoint(x, y) ;
			int index = p->getImageDataIndex(grid) ;
			if ( index >= 0 ) {
				CListModelMap::ImageData &data = p->getImageData(index) ;
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
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	ui->checkBox_through->setCheckState(val ? Qt::Checked : Qt::Unchecked) ;

	QPoint st = g_EditData->getSelStartGrid() ;
	QPoint end = g_EditData->getSelEndGrid() ;
	for ( int y = st.y() ; y <= end.y() ; y ++ ) {
		for ( int x = st.x() ; x <= end.x() ; x ++ ) {
			QPoint grid = QPoint(x, y) ;
			int index = p->getImageDataIndex(grid);
			if ( index >= 0 ) {
				CListModelMap::ImageData &data = p->getImageData(index) ;
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
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	bool bUnit = false, bThrough = false ;
	QPoint st = g_EditData->getSelStartGrid() ;
	QPoint end = g_EditData->getSelEndGrid() ;
	for ( int y = st.y() ; y <= end.y() ; y ++ ) {
		for ( int x = st.x() ; x <= end.x() ; x ++ ) {
			QPoint grid = QPoint(x, y) ;
			int index = p->getImageDataIndex(grid) ;
			if ( index < 0 ) { continue ; }
			CListModelMap::ImageData &data = p->getImageData(index) ;
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
	bool bOk = false ;
	QString str = QInputDialog::getText(this,
										trUtf8("New Map"),
										trUtf8("マップ名を入力してください"),
										QLineEdit::Normal,
										trUtf8("0x00000000"),
										&bOk) ;
	if ( !bOk )				{ return ; }
	if ( str.isEmpty() )	{ return ; }

	QSize mapGrid = QSize(ui->spinBox_grid_w_map->value(), ui->spinBox_grid_h_map->value()) ;
	QSize imgGrid = QSize(ui->spinBox_grid_w_img->value(), ui->spinBox_grid_h_img->value()) ;
	g_EditData->cmd_addMap(str, mapGrid, imgGrid) ;
}

// 削除 押下
void MainWindow::slot_clickPushDel()
{
	if ( !g_EditData->isSelectMap() ) { return ; }

	g_EditData->cmd_delMap(g_EditData->getSelMapIndex().row()) ;
}

// Map 選択
void MainWindow::slot_clickedListViewMap(QModelIndex index)
{
	qDebug() << "slot_clickedListViewMap" ;

	g_EditData->setSelMapIndex(index) ;

	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( p ) {
		ui->spinBox_grid_w_img->setValue(p->imgGridSize.width());
		ui->spinBox_grid_h_img->setValue(p->imgGridSize.height());
		ui->spinBox_grid_w_map->setValue(p->mapGridSize.width());
		ui->spinBox_grid_h_map->setValue(p->mapGridSize.height());

		g_EditData->update() ;

		ui->listView_treasure->setModel(p->pModelTreasure) ;
		ui->listView_point->setModel(p->pModelPoint) ;
	}
	else {
		ui->listView_treasure->setModel(NULL) ;
		ui->listView_point->setModel(NULL) ;
	}
}

// お宝リスト選択
void MainWindow::slot_clickedListViewTreasure(QModelIndex index)
{
	g_EditData->setSelTreasureIndex(index) ;
	g_EditData->setSelPointIndex(QModelIndex()) ;
}

// 開始、終了位置リスト 選択
void MainWindow::slot_clickedListViewPoint(QModelIndex index)
{
	g_EditData->setSelPointIndex(index) ;
	g_EditData->setSelTreasureIndex(QModelIndex()) ;
}

// タブ変更
void MainWindow::slot_tabChanged(int index)
{
	g_EditData->setEditMode(index) ;
	g_EditData->update();
}

// 宝追加 押下
void MainWindow::slot_pushAddTreasure()
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	g_EditData->cmd_addTreasure();
}

// 宝削除 押下
void MainWindow::slot_pushDelTreasure()
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( !g_EditData->isSelectTreasure() ) { return ; }

	g_EditData->cmd_delTreasure(ui->listView_treasure->currentIndex().row());
}

// 開始、終了位置 追加
void MainWindow::slot_pushAddPoint()
{
	QMenu menu(this) ;
	menu.addAction(m_pActAddStartPoint) ;
	menu.addAction(m_pActAddEndPoint) ;
	menu.exec(cursor().pos()) ;
}

// 開始、終了位置 削除
void MainWindow::slot_pushDelPoint()
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( !g_EditData->isSelectPoint() ) { return ; }

	g_EditData->cmd_DelPoint(ui->listView_point->currentIndex().row());
}

// 開始位置 追加
void MainWindow::slot_clickAddStartPoint()
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	qDebug() << "slot_clickAddStartPoint:" << p ;
	if ( !p ) { return ; }

	g_EditData->cmd_AddPoint(0) ;
}

// 終了位置 追加
void MainWindow::slot_clickAddEndPoint()
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	g_EditData->cmd_AddPoint(1) ;
}

// コマンド 変更
void MainWindow::slot_changeDataModified(int index)
{
	if ( m_undoIndex == index ) {
		setWindowTitle(tr(kExecName"[%1]").arg(m_strSaveFileName));
	}
	else {	// 編集してる
		setWindowTitle(tr(kExecName"*[%1]").arg(m_strSaveFileName));
	}
}

void MainWindow::slot_clickViewSquare()
{
	ui->action_Square->setChecked(true) ;
	ui->action_Quarter->setChecked(false) ;
	g_EditData->setViewMode(CEditData::kViewMode_Square);
}

void MainWindow::slot_clickViewQuarter()
{
	ui->action_Quarter->setChecked(true) ;
	ui->action_Square->setChecked(false) ;
	g_EditData->setViewMode(CEditData::kViewMode_Quarter);
}


// ファイルを開く
void MainWindow::fileOpen(QString &fileName)
{
	g_Setting->setFileOpenDir(fileName) ;

	if ( fileName.toLower().indexOf(".png") <= 0
	  && fileName.toLower().indexOf(kFileExt_JSON) <= 0
	  && fileName.toLower().indexOf(kFileExt_XML) <= 0 ) {
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

		CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
		if ( !p ) {
			int index = g_EditData->getModelMap()->addMap("0x00000000") ;
			if ( index < 0 ) {
				QMessageBox::warning(this, trUtf8("エラー"), trUtf8("未知の不具合")) ;
				return ;
			}
			CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(index) ;
			data.image = image ;
			data.imageName = fileName ;
			data.imgGridSize = QSize(ui->spinBox_grid_w_img->value(), ui->spinBox_grid_h_img->value()) ;
			data.mapGridSize = QSize(ui->spinBox_grid_w_map->value(), ui->spinBox_grid_h_map->value()) ;

			QModelIndex idx = g_EditData->getModelMap()->index(index) ;
			ui->listView_map->setCurrentIndex(idx);
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
		setWindowTitle(tr(kExecName"[%1]").arg(m_strSaveFileName));
	}
	else if ( fileName.toLower().indexOf(kFileExt_XML) > 0 ) {	// XMLファイル
		if ( !checkSaveFileChanged() ) {
			// キャンセルした
			return ;
		}

		dataInit() ;

		CSaveFileXml data ;
		if ( !data.read(fileName) ) {
			dataInit() ;

			QMessageBox::warning(this, trUtf8("エラー"), trUtf8("読み込みに失敗しました:%1").arg(fileName)) ;
			return ;
		}

		g_EditData->update();

		m_strSaveFileName = fileName ;
		setWindowTitle(tr(kExecName"[%1]").arg(m_strSaveFileName));
	}
}

// ファイル保存
void MainWindow::fileWrite(QString &fileName)
{
	if ( fileName.indexOf(kFileExt_JSON) > 0 ) {
		g_Setting->setFileSaveDir(fileName) ;
		m_strSaveFileName = fileName ;
		setWindowTitle(tr(kExecName"[%1]").arg(m_strSaveFileName));
		m_undoIndex = g_EditData->getUndoStack()->index() ;
	}
	else if ( fileName.indexOf(kFileExt_XML) > 0 ) {
		g_Setting->setFileSaveDir(fileName) ;

		CSaveFileXml data ;
		if ( !data.write(fileName) ) {
			QMessageBox::warning(this, trUtf8("エラー"), trUtf8("保存失敗")) ;
			return ;
		}
		m_strSaveFileName = fileName ;
		setWindowTitle(tr(kExecName"[%1]").arg(m_strSaveFileName));
		m_undoIndex = g_EditData->getUndoStack()->index() ;
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

	slot_splitterMoveMap(0, 0) ;

//	g_EditData->setViewMode(g_Setting->getViewMode()) ;
}

void MainWindow::setSpaceSize()
{
	m_frameImageSpace = QSize(10, 100) ;
	m_frameMapSpace = QSize(10, 10) ;
	m_frameTreeSpace = QSize(10, 60) ;
	m_windowSpace = QSize(10, 26) ;
}

void MainWindow::dataInit()
{
	if ( g_EditData->getModelMap() ) {
		delete g_EditData->getModelMap() ;
	}
	g_EditData->setModelMap(new CListModelMap(this)) ;
	ui->listView_map->setModel(g_EditData->getModelMap());

	if ( g_EditData->getUndoStack() ) {
		g_EditData->getUndoStack()->clear();
	}
	else {
		g_EditData->setUndoStack(new QUndoStack(this)) ;
	}

	m_undoIndex = 0 ;
}

bool MainWindow::checkSaveFileChanged()
{
	int index = g_EditData->getUndoStack()->index() ;
	if ( index != m_undoIndex ) {
		QMessageBox::StandardButton reply = QMessageBox::question(this, trUtf8("確認"), trUtf8("現在のファイルを保存しますか？"),
																  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) ;
		if ( reply == QMessageBox::Yes ) {		// 保存する
			slot_fileSave();
			m_undoIndex = index ;
		}
		else if ( reply == QMessageBox::Cancel ) {	// キャンセル
			return false ;
		}
	}
	return true ;
}







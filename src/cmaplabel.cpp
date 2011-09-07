#include <QDebug>
#include "cmaplabel.h"

CMapLabel::CMapLabel(QWidget *parent) :
    QLabel(parent)
{
	m_bPressCtrl = false ;
	m_nMapControllType = kMapControll_None ;
	m_pDropLabel = NULL ;

	m_pLayerMap = new QLabel(this) ;
	m_pLayerMark = new QLabel(this) ;
	makeDropLabel(QPoint(), QPoint(), QPoint()) ;
	m_pGridLabel = new CGridLabel(this) ;
	m_pGridLabel->show();
	m_pGridLabel->setMouseTracking(true) ;

	connect(m_pGridLabel, SIGNAL(sig_mousePress(QMouseEvent *)),	this, SLOT(slot_mousePress(QMouseEvent *))) ;
	connect(m_pGridLabel, SIGNAL(sig_mouseMove(QMouseEvent*)),		this, SLOT(slot_mouseMove(QMouseEvent*))) ;
	connect(m_pGridLabel, SIGNAL(sig_mouseRelease(QMouseEvent*)),	this, SLOT(slot_mouseRelease(QMouseEvent*))) ;
}

void CMapLabel::updateLabels()
{
	if ( !g_EditData->isSelectMap() ) { return ; }
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	qDebug() << "CMapLabel::updateLabels" ;

	m_pGridLabel->setGridSize(p->mapGridSize) ;
	m_pGridLabel->setGridNum(QPoint(50, 50)) ;
	m_pGridLabel->updateGrid() ;

	m_pLayerMap->resize(m_pGridLabel->size());
	m_pLayerMark->resize(m_pGridLabel->size());
	setMinimumSize(m_pGridLabel->size());

	slot_changeSelectGridRect() ;

	releaseTipLabel(m_TipLabel) ;
	releaseTipLabel(m_TreasureLabel) ;
	releaseTipLabel(m_PointLabel) ;

	if ( !p->image.isNull() ) {
		for ( int i = 0 ; i < p->gridDatas.size() ; i ++ ) {
			CListModelMap::GridData &data = p->gridDatas[i] ;
			addMapGrid(data.mapGrid, data.imageGrid) ;
		}
	}

	if ( g_EditData->getEditMode() == CEditData::kEditMode_Data ) {
		for ( int i = 0 ; i < p->pModelTreasure->rowCount(QModelIndex()) ; i ++ ) {
			slot_addTreasureLabel(i) ;
		}
		for ( int i = 0 ; i < p->pModelPoint->rowCount(QModelIndex()) ; i ++ ) {
			slot_addPointLabel(i) ;
		}
	}
}

void CMapLabel::slot_changeSelectGridRect()
{
	if ( g_EditData->getEditMode() == CEditData::kEditMode_Data ) {
		makeDropLabel(m_pDropLabel->pos(), g_EditData->getSelStartGrid(), g_EditData->getSelStartGrid()) ;
	}
	else {
		makeDropLabel(m_pDropLabel->pos(), g_EditData->getSelStartGrid(), g_EditData->getSelEndGrid()) ;
	}
}

void CMapLabel::slot_addTreasureLabel(int index)
{
	qDebug() << "slot_addTreasureLabel index:" << index ;

	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	QList<CListModelTreasure::TreasureData> &treasureDatas = p->pModelTreasure->getList() ;

	slot_removeTreasureLabel(index);

	TipLabel label ;
	label.first = QPoint(index, 0) ;
	label.second = makeLabel(treasureDatas.at(index).mapGrid, QSize(1, 1), QColor(0, 128, 0)) ;
	if ( label.second ) {
		m_TreasureLabel.append(label) ;
	}
}

void CMapLabel::slot_removeTreasureLabel(int index)
{
	for ( int i = 0 ; i < m_TreasureLabel.size() ; i ++ ) {
		if ( m_TreasureLabel[i].first.x() == index ) {
			TipLabel l = m_TreasureLabel.takeAt(i) ;
			delete l.second ;
			break ;
		}
	}
}

void CMapLabel::slot_addPointLabel(int index)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	QList<CListModelPoint::PointData> &pointDatas = p->pModelPoint->getList() ;

	slot_removePointLabel(index);

	TipLabel label ;
	label.first = QPoint(index, 0) ;
	label.second = makeLabel(pointDatas.at(index).mapGrid, QSize(1, 1), pointDatas.at(index).kind==0 ? QColor(0, 0, 128) : QColor(128, 0, 0)) ;
	if ( label.second ) {
		m_PointLabel.append(label) ;
	}
}

void CMapLabel::slot_removePointLabel(int index)
{
	for ( int i = 0 ; i < m_PointLabel.size() ; i ++ ) {
		if ( m_PointLabel[i].first.x() == index ) {
			TipLabel l = m_PointLabel.takeAt(i) ;
			delete l.second ;
			break ;
		}
	}
}

void CMapLabel::slot_mousePress(QMouseEvent *event)
{
	if ( !m_pDropLabel ) { return ; }

	if ( g_EditData->getEditMode() == CEditData::kEditMode_Map ) {
		if ( m_nMapControllType != kMapControll_None ) {
			event->ignore();
			return ;
		}

		switch ( event->button() ) {
		case Qt::LeftButton:
			m_nMapControllType = m_bPressCtrl ? kMapControll_AddMulti : kMapControll_Add ;
			break ;
		case Qt::RightButton:
			m_nMapControllType = m_bPressCtrl ? kMapControll_RemoveMulti : kMapControll_Remove ;
			break ;
		}

		qDebug() << "mousePress:" << m_nMapControllType ;

		switch ( m_nMapControllType ) {
		case kMapControll_AddMulti:
		case kMapControll_RemoveMulti:
			m_oldDropSize = m_pDropLabel->size() ;
			break ;
		}
	}
}

void CMapLabel::slot_mouseMove(QMouseEvent *event)
{
	if ( !m_pDropLabel ) { return ; }

	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	if ( g_EditData->getEditMode() == CEditData::kEditMode_Map ) {
		switch ( m_nMapControllType ) {
		case kMapControll_Add:
//			addMapGrid(event->pos()) ;
			g_EditData->cmd_AddMapGrid(event->pos()) ;
			break ;
		case kMapControll_Remove:
//			removeMapGrid(event->pos()) ;
			g_EditData->cmd_DelMapGrid(event->pos()) ;
			break ;
		case kMapControll_AddMulti:
		case kMapControll_RemoveMulti:
			{
				QPoint st, end ;
				g_EditData->posToGrid(st, m_pDropLabel->pos(), p->mapGridSize) ;
				end = event->pos() - m_pDropLabel->pos() ;
				end.setX((end.x() / m_oldDropSize.width() + 1) * m_oldDropSize.width()) ;
				end.setY((end.y() / m_oldDropSize.height() + 1) * m_oldDropSize.height()) ;
				end += m_pDropLabel->pos() ;
				end.setX(end.x()-p->mapGridSize.width()) ;
				end.setY(end.y()-p->mapGridSize.height()) ;
				g_EditData->posToGrid(end, end, p->mapGridSize) ;
				makeDropLabel(m_pDropLabel->pos(), st, end) ;
			}
			return ;
		}
	}

	QPoint pos ;
	g_EditData->posToGrid(pos, event->pos(), p->mapGridSize) ;
	g_EditData->gridToPos(pos, pos, p->mapGridSize) ;
	m_pDropLabel->move(pos) ;
}

void CMapLabel::slot_mouseRelease(QMouseEvent *event)
{
	int type = m_nMapControllType ;
	m_nMapControllType = kMapControll_None ;

	if ( !m_pDropLabel ) { return ; }

	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	if ( g_EditData->getEditMode() == CEditData::kEditMode_Map ) {
		switch ( type ) {
		case kMapControll_Add:
//			addMapGrid(event->pos()) ;
			g_EditData->cmd_AddMapGrid(event->pos()) ;
			break ;
		case kMapControll_Remove:
//			removeMapGrid(event->pos()) ;
			g_EditData->cmd_DelMapGrid(event->pos()) ;
			break ;
		case kMapControll_AddMulti:
		case kMapControll_RemoveMulti:
			{
				for ( int y = m_pDropLabel->pos().y() ; y < event->pos().y() ; y += m_oldDropSize.height() ) {
					for ( int x = m_pDropLabel->pos().x() ; x < event->pos().x() ; x += m_oldDropSize.width() ) {
						if ( type == kMapControll_AddMulti ) {
//							addMapGrid(QPoint(x, y)) ;
							g_EditData->cmd_AddMapGrid(QPoint(x, y)) ;
						}
						else if ( type == kMapControll_RemoveMulti ) {
//							removeMapGrid(QPoint(x, y)) ;
							g_EditData->cmd_DelMapGrid(QPoint(x, y)) ;
						}
					}
				}

				m_pDropLabel->resize(m_oldDropSize);
			}
			break ;
		}
	}
	else if ( g_EditData->getEditMode() == CEditData::kEditMode_Data ) {
		QPoint pos ;
		g_EditData->posToGrid(pos, event->pos(), p->mapGridSize) ;

		if ( g_EditData->isSelectTreasure() ) {
			qDebug() << "move Treasure index:" << g_EditData->getSelTreasureIndex().row() ;
			p->pModelTreasure->setData(g_EditData->getSelTreasureIndex(), pos, Qt::UserRole) ;

			g_EditData->gridToPos(pos, pos, p->mapGridSize) ;
			m_TreasureLabel[g_EditData->getSelTreasureIndex().row()].second->move(pos) ;
		}
		else if ( g_EditData->isSelectPoint() ) {
			p->pModelPoint->setData(g_EditData->getSelPointIndex(), pos, Qt::UserRole) ;

			g_EditData->gridToPos(pos, pos, p->mapGridSize) ;
			m_PointLabel[g_EditData->getSelPointIndex().row()].second->move(pos) ;
		}
	}
}

void CMapLabel::slot_keyPress(QKeyEvent *event)
{
	if ( event->key() == Qt::Key_Control ) {
		m_bPressCtrl = true ;
	}
}

void CMapLabel::slot_keyRelease(QKeyEvent *event)
{
	if ( event->key() == Qt::Key_Control ) {
		m_bPressCtrl = false ;
	}
}

int CMapLabel::getGridLabelIndex(QPoint grid)
{
	for ( int i = 0 ; i < m_TipLabel.size() ; i ++ ) {
		if ( m_TipLabel[i].first == grid ) {
			return i ;
		}
	}
	return -1 ;
}

void CMapLabel::addMapGrid(const QPoint basePos)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	qDebug() << "addMapGrid:" << basePos ;

	QPoint st_grid, end_grid, base_grid ;

	g_EditData->posToGrid(base_grid, basePos, p->mapGridSize) ;
	st_grid = g_EditData->getSelStartGrid() ;
	end_grid = g_EditData->getSelEndGrid() ;
	for ( int y = st_grid.y() ; y <= end_grid.y() ; y ++ ) {
		for ( int x = st_grid.x() ; x <= end_grid.x() ; x ++ ) {
			QPoint imgGrid = QPoint(x, y) ;
			QPoint mapGrid = base_grid + QPoint(x-st_grid.x(), y-st_grid.y()) ;
			addMapGrid(mapGrid, imgGrid) ;

			p->addGridData(mapGrid, imgGrid) ;
		}
	}
}

void CMapLabel::addMapGrid(const QPoint mapGrid, const QPoint imgGrid)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) {
		qDebug() << "CMapLabel::addMapGrid p==null" ;
		return ;
	}

	QPoint mapPos, imgPos ;
	QSize mapGridSize = p->mapGridSize ;
	QSize imgGridSize = p->imgGridSize ;
	g_EditData->gridToPos(mapPos, mapGrid, mapGridSize) ;
	g_EditData->gridToPos(imgPos, imgGrid, imgGridSize) ;

	QImage img = p->image.copy(imgPos.x(), imgPos.y(), imgGridSize.width(), imgGridSize.height()) ;

	int index = getGridLabelIndex(mapGrid) ;
	if ( index >= 0 ) {
		releaseTipLabel(m_TipLabel, index) ;
	}

	QLabel *pLabel = new QLabel(m_pLayerMap) ;
	pLabel->move(mapPos) ;
	pLabel->setPixmap(QPixmap::fromImage(img.scaled(mapGridSize))) ;
	pLabel->show();
	m_TipLabel.append(TipLabel(mapGrid, pLabel)) ;
}

void CMapLabel::removeMapGrid(const QPoint basePos)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) {
		qDebug() << "CMapLabel::removeMapGrid p==null" ;
		return ;
	}

	QPoint st_grid, end_grid, base_grid ;

	g_EditData->posToGrid(base_grid, basePos, p->mapGridSize) ;
	st_grid = g_EditData->getSelStartGrid() ;
	end_grid = g_EditData->getSelEndGrid() ;
	for ( int y = st_grid.y() ; y <= end_grid.y() ; y ++ ) {
		for ( int x = st_grid.x() ; x <= end_grid.x() ; x ++ ) {
			QPoint mapGrid = base_grid + QPoint(x-st_grid.x(), y-st_grid.y()) ;

			int index = getGridLabelIndex(mapGrid) ;
			if ( index >= 0 ) {
				releaseTipLabel(m_TipLabel, index) ;
			}

			p->removeGridData(mapGrid) ;
		}
	}
}

void CMapLabel::makeDropLabel(QPoint pos, QPoint gridSt, QPoint gridEnd)
{
	if ( !m_pDropLabel ) {
		m_pDropLabel = new QLabel(m_pLayerMark) ;
	}
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	QPoint st, end ;
	g_EditData->gridToPos(st, gridSt, p->mapGridSize) ;
	g_EditData->gridToPos(end, gridEnd, p->mapGridSize) ;
	g_EditData->posToGrid(pos, pos, p->mapGridSize) ;
	g_EditData->gridToPos(pos, pos, p->mapGridSize) ;
	end.setX(end.x() + p->mapGridSize.width()) ;
	end.setY(end.y() + p->mapGridSize.height()) ;

	m_pDropLabel->move(pos) ;
	QSize size = QSize(end.x()-st.x(), end.y()-st.y()) ;
	QImage img = QImage(size, QImage::Format_ARGB32) ;
	int rgba = QColor(255, 0, 0, 64).rgba() ;
	if ( g_EditData->getEditMode() == CEditData::kEditMode_Data ) {
		rgba = QColor(255, 255, 0, 64).rgba() ;
	}
	for ( int i = 0 ; i < img.width()*img.height() ; i ++ ) {
		img.setPixel(i%img.width(), i/img.width(), rgba) ;
	}
	m_pDropLabel->setPixmap(QPixmap::fromImage(img));
	m_pDropLabel->resize(size) ;
	m_pDropLabel->show();
}

void CMapLabel::releaseTipLabel(QList<TipLabel> &label, int index)
{
	if ( index < 0 ) {
		for ( int i = 0 ; i < label.size() ; i ++ ) {
			delete label[i].second ;
		}
		label.clear() ;
	}
	else {
		if ( index >= label.size() ) { return ; }

		TipLabel l = label.takeAt(index) ;
		delete l.second ;
	}
}

QLabel *CMapLabel::makeLabel(QPoint grid, QSize size, QColor col)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return NULL ; }

	QPoint pos ;
	g_EditData->gridToPos(pos, grid, p->mapGridSize) ;
	size.setWidth(size.width()*p->mapGridSize.width()) ;
	size.setHeight(size.height()*p->mapGridSize.height()) ;

	QImage img = QImage(size, QImage::Format_ARGB32) ;
	for ( int i = 0 ; i < img.width()*img.height() ; i ++ ) {
		img.setPixel(i%img.width(), i/img.width(), (col.rgb()&0x00ffffff) | 0x80000000) ;
	}

	QLabel *label = new QLabel(m_pLayerMark) ;
	label->move(pos) ;
	label->setPixmap(QPixmap::fromImage(img)) ;
	label->resize(size) ;
	label->show() ;
	return label ;
}






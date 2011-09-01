#include "cmaplabel.h"

CMapLabel::CMapLabel(QWidget *parent) :
    QLabel(parent)
{
	m_bPressCtrl = false ;
	m_nMapControllType = kMapControll_None ;
	m_pDropLabel = NULL ;

	m_pMapLabel = new QLabel(this) ;
	makeDropLabel(QPoint(), QPoint(), QPoint()) ;
	m_pGridLabel = new CGridLabel(this) ;
	m_pGridLabel->show();

	connect(m_pGridLabel, SIGNAL(sig_mousePress(QMouseEvent *)),	this, SLOT(slot_mousePress(QMouseEvent *))) ;
	connect(m_pGridLabel, SIGNAL(sig_mouseMove(QMouseEvent*)),		this, SLOT(slot_mouseMove(QMouseEvent*))) ;
	connect(m_pGridLabel, SIGNAL(sig_mouseRelease(QMouseEvent*)),	this, SLOT(slot_mouseRelease(QMouseEvent*))) ;
}

void CMapLabel::updateLabels()
{
	if ( !g_EditData->isSelectMap() ) { return ; }
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( p->image.isNull() ) { return ; }

	qDebug() << "CMapLabel::updateLabels" ;

	m_pGridLabel->setGridSize(p->mapGridSize) ;
	m_pGridLabel->setGridNum(QPoint(50, 50)) ;
	m_pGridLabel->updateGrid() ;

	m_pMapLabel->resize(m_pGridLabel->size());
	setMinimumSize(m_pGridLabel->size());

	slot_changeSelectGridRect() ;

	for ( int i = 0 ; i < m_TipLabel.size() ; i ++ ) {
		delete m_TipLabel[i].second ;
	}
	m_TipLabel.clear() ;

	for ( int i = 0 ; i < p->gridDatas.size() ; i ++ ) {
		CEditData::GridData &data = p->gridDatas[i] ;
		addMapGrid(data.mapGrid, data.imageGrid) ;
	}
}

void CMapLabel::slot_changeSelectGridRect()
{
	makeDropLabel(m_pDropLabel->pos(), g_EditData->getSelStartGrid(), g_EditData->getSelEndGrid()) ;
}

void CMapLabel::slot_mousePress(QMouseEvent *event)
{
	if ( !m_pDropLabel ) { return ; }

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

void CMapLabel::slot_mouseMove(QMouseEvent *event)
{
	if ( !m_pDropLabel ) { return ; }

	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	switch ( m_nMapControllType ) {
	case kMapControll_Add:
		addMapGrid(event->pos()) ;
		break ;
	case kMapControll_Remove:
		removeMapGrid(event->pos()) ;
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

	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	switch ( type ) {
	case kMapControll_Add:
		addMapGrid(event->pos()) ;
		break ;
	case kMapControll_Remove:
		removeMapGrid(event->pos()) ;
		break ;
	case kMapControll_AddMulti:
	case kMapControll_RemoveMulti:
		{
/*
			QPoint end ;
			end = event->pos() - m_pDropLabel->pos() ;
			end.setX((end.x() / m_oldDropSize.width() + 1) * m_oldDropSize.width()) ;
			end.setY((end.y() / m_oldDropSize.height() + 1) * m_oldDropSize.height()) ;
			end += m_pDropLabel->pos() ;
			end.setX(end.x()-p->mapGridSize.width()) ;
			end.setY(end.y()-p->mapGridSize.height()) ;
			g_EditData->posToGrid(end, end, p->mapGridSize) ;
*/
			for ( int y = m_pDropLabel->pos().y() ; y < event->pos().y() ; y += m_oldDropSize.height() ) {
				for ( int x = m_pDropLabel->pos().x() ; x < event->pos().x() ; x += m_oldDropSize.width() ) {
					if ( type == kMapControll_AddMulti ) {
						addMapGrid(QPoint(x, y)) ;
					}
					else if ( type == kMapControll_RemoveMulti ) {
						removeMapGrid(QPoint(x, y)) ;
					}
				}
			}

			m_pDropLabel->resize(m_oldDropSize);
		}
		break ;
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
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
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
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
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
		TipLabel label = m_TipLabel.takeAt(index) ;
		delete label.second ;
	}

	QLabel *pLabel = new QLabel(m_pMapLabel) ;
	pLabel->move(mapPos) ;
	pLabel->setPixmap(QPixmap::fromImage(img.scaled(mapGridSize))) ;
	pLabel->show();
	m_TipLabel.append(TipLabel(mapGrid, pLabel)) ;
}

void CMapLabel::removeMapGrid(const QPoint basePos)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
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
				TipLabel label = m_TipLabel.takeAt(index) ;
				delete label.second ;
			}

			p->removeGridData(mapGrid) ;
		}
	}
}

void CMapLabel::makeDropLabel(QPoint pos, QPoint gridSt, QPoint gridEnd)
{
	if ( !m_pDropLabel ) {
		m_pDropLabel = new QLabel(this) ;
	}
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
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
	for ( int i = 0 ; i < img.width()*img.height() ; i ++ ) {
		img.setPixel(i%img.width(), i/img.width(), QColor(255, 0, 0, 64).rgba()) ;
	}
	m_pDropLabel->setPixmap(QPixmap::fromImage(img));
	m_pDropLabel->resize(size) ;
	m_pDropLabel->show();
}

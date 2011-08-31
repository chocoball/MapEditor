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
	m_pGridLabel->setGridSize(g_Setting->getMapGridSize()) ;
	m_pGridLabel->setGridNum(QPoint(100, 100)) ;
	m_pGridLabel->updateGrid() ;

	m_pMapLabel->resize(m_pGridLabel->size());
	setMinimumSize(m_pGridLabel->size());
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
			g_EditData->posToGrid(st, m_pDropLabel->pos(), g_Setting->getMapGridSize()) ;
			end = event->pos() - m_pDropLabel->pos() ;
			end.setX((end.x() / m_oldDropSize.width() + 1) * m_oldDropSize.width()) ;
			end.setY((end.y() / m_oldDropSize.height() + 1) * m_oldDropSize.height()) ;
			end += m_pDropLabel->pos() ;
			end.setX(end.x()-g_Setting->getMapGridSize().width()) ;
			end.setY(end.y()-g_Setting->getMapGridSize().height()) ;
			g_EditData->posToGrid(end, end, g_Setting->getMapGridSize()) ;
			makeDropLabel(m_pDropLabel->pos(), st, end) ;
		}
		return ;
	}

	QPoint pos ;
	g_EditData->posToGrid(pos, event->pos(), g_Setting->getMapGridSize()) ;
	g_EditData->gridToPos(pos, pos, g_Setting->getMapGridSize()) ;
	m_pDropLabel->move(pos) ;
}

void CMapLabel::slot_mouseRelease(QMouseEvent *event)
{
	if ( !m_pDropLabel ) { return ; }

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
			QPoint end ;
			end = event->pos() - m_pDropLabel->pos() ;
			end.setX((end.x() / m_oldDropSize.width() + 1) * m_oldDropSize.width()) ;
			end.setY((end.y() / m_oldDropSize.height() + 1) * m_oldDropSize.height()) ;
			end += m_pDropLabel->pos() ;
			end.setX(end.x()-g_Setting->getMapGridSize().width()) ;
			end.setY(end.y()-g_Setting->getMapGridSize().height()) ;
			g_EditData->posToGrid(end, end, g_Setting->getMapGridSize()) ;

			for ( int y = m_pDropLabel->pos().y() ; y < event->pos().y() ; y += m_oldDropSize.height() ) {
				for ( int x = m_pDropLabel->pos().x() ; x < event->pos().x() ; x += m_oldDropSize.width() ) {
					if ( m_nMapControllType == kMapControll_AddMulti ) {
						addMapGrid(QPoint(x, y)) ;
					}
					else if ( m_nMapControllType == kMapControll_RemoveMulti ) {
						removeMapGrid(QPoint(x, y)) ;
					}
				}
			}

			m_pDropLabel->resize(m_oldDropSize);
		}
		break ;
	}

	m_nMapControllType = kMapControll_None ;
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
	for ( int i = 0 ; i < m_GridLabel.size() ; i ++ ) {
		if ( m_GridLabel[i].first == grid ) {
			return i ;
		}
	}
	return -1 ;
}

void CMapLabel::addMapGrid(const QPoint basePos)
{
	QPoint st_grid, end_grid, base_grid ;

	QImage *pImage = g_EditData->getImagePtr() ;
	QSize imgGridSize = g_Setting->getImageGridSize() ;

	g_EditData->posToGrid(base_grid, basePos, g_Setting->getMapGridSize()) ;
	st_grid = g_EditData->getSelStartGrid() ;
	end_grid = g_EditData->getSelEndGrid() ;
	for ( int y = st_grid.y() ; y <= end_grid.y() ; y ++ ) {
		for ( int x = st_grid.x() ; x <= end_grid.x() ; x ++ ) {

			QPoint imgGrid = QPoint(x, y) ;
			QPoint mapGrid = base_grid + QPoint(x-st_grid.x(), y-st_grid.y()) ;
			QPoint imgPos, mapPos ;
			g_EditData->gridToPos(imgPos, imgGrid, imgGridSize) ;
			g_EditData->gridToPos(mapPos, mapGrid, g_Setting->getMapGridSize()) ;
			QImage img = pImage->copy(imgPos.x(), imgPos.y(), imgGridSize.width(), imgGridSize.height()).scaled(g_Setting->getMapGridSize()) ;

			int index = getGridLabelIndex(mapGrid) ;
			if ( index >= 0 ) {
				GridLabel label = m_GridLabel.takeAt(index) ;
				delete label.second ;
			}

			QLabel *pLabel = new QLabel(m_pMapLabel) ;
			pLabel->move(mapPos) ;
			pLabel->setPixmap(QPixmap::fromImage(img)) ;
			pLabel->show();
			m_GridLabel.push_back(GridLabel(mapGrid, pLabel)) ;

//TODO			g_EditData->addGridData(mapGrid, imgGrid, 0) ;
		}
	}
}

void CMapLabel::removeMapGrid(const QPoint basePos)
{
	QPoint st_grid, end_grid, base_grid ;

	g_EditData->posToGrid(base_grid, basePos, g_Setting->getMapGridSize()) ;
	st_grid = g_EditData->getSelStartGrid() ;
	end_grid = g_EditData->getSelEndGrid() ;
	for ( int y = st_grid.y() ; y <= end_grid.y() ; y ++ ) {
		for ( int x = st_grid.x() ; x <= end_grid.x() ; x ++ ) {
			QPoint mapGrid = base_grid + QPoint(x-st_grid.x(), y-st_grid.y()) ;

			int index = getGridLabelIndex(mapGrid) ;
			if ( index >= 0 ) {
				GridLabel label = m_GridLabel.takeAt(index) ;
				delete label.second ;
			}

//TODO			g_EditData->removeGridData(mapGrid) ;
		}
	}
}

void CMapLabel::makeDropLabel(QPoint pos, QPoint gridSt, QPoint gridEnd)
{
	if ( !m_pDropLabel ) {
		m_pDropLabel = new QLabel(this) ;
	}

	QPoint st, end ;
	g_EditData->gridToPos(st, gridSt, g_Setting->getMapGridSize()) ;
	g_EditData->gridToPos(end, gridEnd, g_Setting->getMapGridSize()) ;
	g_EditData->posToGrid(pos, pos, g_Setting->getMapGridSize()) ;
	g_EditData->gridToPos(pos, pos, g_Setting->getMapGridSize()) ;
	end.setX(end.x() + g_Setting->getMapGridSize().width()) ;
	end.setY(end.y() + g_Setting->getMapGridSize().height()) ;

	m_pDropLabel->move(pos) ;
	QSize size = QSize(end.x()-st.x(), end.y()-st.y()) ;
	QImage img = QImage(QSize(4, 4), QImage::Format_ARGB32) ;
	for ( int i = 0 ; i < img.width()*img.height() ; i ++ ) {
		img.setPixel(i%img.width(), i/img.width(), QColor(255, 0, 0, 64).rgba()) ;
	}
	m_pDropLabel->setPixmap(QPixmap::fromImage(img.scaled(size)));
	m_pDropLabel->resize(size) ;
	m_pDropLabel->show();
}

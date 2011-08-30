#include "cmaplabel.h"

CMapLabel::CMapLabel(QWidget *parent) :
    QLabel(parent)
{
	m_bMousePress = false ;
	m_pDropLabel = NULL ;

	m_pMapLabel = new QLabel(this) ;
	makeDropLabel(QPoint()) ;
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
}

void CMapLabel::slot_changeSelectGridRect()
{
	makeDropLabel(m_pDropLabel->pos()) ;
}

void CMapLabel::slot_mousePress(QMouseEvent *event)
{
	if ( event->button() == Qt::LeftButton ) {
		m_bMousePress = true ;
	}
}

void CMapLabel::slot_mouseMove(QMouseEvent *event)
{
	if ( !m_pDropLabel ) { return ; }

	QPoint pos ;
	g_EditData->posToGrid(pos, event->pos(), g_Setting->getMapGridSize()) ;
	g_EditData->gridToPos(pos, pos, g_Setting->getMapGridSize()) ;
	m_pDropLabel->move(pos) ;

	if ( m_bMousePress ) {
		addMapGrid(event->pos()) ;
	}
}

void CMapLabel::slot_mouseRelease(QMouseEvent *event)
{
	if ( m_bMousePress ) {
		addMapGrid(event->pos()) ;
	}
	m_bMousePress = false ;
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

			g_EditData->addGridData(mapGrid, imgGrid, 0) ;
		}
	}
}

void CMapLabel::makeDropLabel(QPoint pos)
{
	if ( !m_pDropLabel ) {
		m_pDropLabel = new QLabel(this) ;
	}

	QPoint st, end ;
	g_EditData->gridToPos(st, g_EditData->getSelStartGrid(), g_Setting->getMapGridSize()) ;
	g_EditData->gridToPos(end, g_EditData->getSelEndGrid(), g_Setting->getMapGridSize()) ;
	g_EditData->posToGrid(pos, pos, g_Setting->getMapGridSize()) ;
	g_EditData->gridToPos(pos, pos, g_Setting->getMapGridSize()) ;
	end.setX(end.x() + g_Setting->getMapGridSize().width()) ;
	end.setY(end.y() + g_Setting->getMapGridSize().height()) ;

	m_pDropLabel->move(pos) ;
	QSize size = QSize(end.x()-st.x(), end.y()-st.y()) ;
	QImage img = QImage(size, QImage::Format_ARGB32) ;
	QPixmap pix = QPixmap::fromImage(img) ;
	QPainter painter(&pix) ;
	painter.setPen(QColor(255, 0, 0, 64)) ;
	painter.fillRect(0, 0, size.width(), size.height(), QColor(255, 0, 0, 64));
	painter.end() ;
	m_pDropLabel->setPixmap(pix);
	m_pDropLabel->resize(size) ;
	m_pDropLabel->show();
}

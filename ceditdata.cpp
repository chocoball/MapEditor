#include "include.h"

CEditData::CEditData()
{
	m_pImageLabel = NULL ;
	m_pMapLabel = NULL ;
	m_selStartGrid.setX(-1) ;
	m_selEndGrid.setX(-1) ;
}

void CEditData::update()
{
	updateMap() ;
	updateImage() ;
}

void CEditData::updateMap()
{
	if ( !m_pMapLabel ) { return ; }

	int gridW = g_Setting->getGridW() ;
	int gridH = g_Setting->getGridH() ;
	QImage img = QImage(gridW*100, gridH*100, QImage::Format_ARGB32) ;
	QColor col(0, 0, 0) ;
	for ( int y = 0 ; y < img.height() ; y += gridH ) {
		for ( int x = 0 ; x < img.width() ; x ++ ) {
			img.setPixel(x, y, col.rgba()) ;
		}
	}
	for ( int x = 0 ; x < img.width() ; x += gridW ) {
		for ( int y = 0 ; y < img.height() ; y ++ ) {
			img.setPixel(x, y, col.rgba()) ;
		}
	}
	QPixmap pix = QPixmap::fromImage(img) ;
	m_pMapLabel->setPixmap(pix) ;
	m_pMapLabel->resize(img.size());
}

void CEditData::updateImage()
{
	if ( !m_pImageLabel ) { return ; }

	int w = m_Image.width() / g_Setting->getGridW() ;
	int h = m_Image.height() / g_Setting->getGridH() ;
	m_gridData.reserve(w*h) ;

	int gridW = g_Setting->getGridW() ;
	int gridH = g_Setting->getGridH() ;
	QImage image = getImage() ;
	QImage img = image ;
	for ( int y = 0 ; y < image.height() ; y += gridH ) {
		for ( int x = 0 ; x < image.width() ; x ++ ) {
			img.setPixel(x, y, 0) ;
		}
	}
	for ( int x = 0 ; x < image.width() ; x += gridW ) {
		for ( int y = 0 ; y < image.height() ; y ++ ) {
			img.setPixel(x, y, 0) ;
		}
	}
	QPixmap pix = QPixmap::fromImage(img) ;
	QPoint st, end ;
	if ( gridToPos(st, m_selStartGrid) && gridToPos(end, m_selEndGrid) ) {
		QPainter painter(&pix) ;
		painter.setPen(QColor(255, 0, 0, 128)) ;
		painter.fillRect(st.x(), st.y(), end.x()+g_Setting->getGridW() - st.x(), end.y()+g_Setting->getGridH() - st.y(), QColor(255, 0, 0, 128));
		painter.end() ;
	}
	m_pImageLabel->setPixmap(pix) ;
	m_pImageLabel->resize(m_Image.size());
}

bool CEditData::gridToPos(QPoint &ret, const QPoint &grid)
{
	if ( grid.x() < 0 || grid.x() >= (int)(m_Image.width()/g_Setting->getGridW()) ) { return false ; }
	if ( grid.y() < 0 || grid.y() >= (int)(m_Image.height()/g_Setting->getGridH()) ) { return false ; }

	ret.setX(grid.x() * g_Setting->getGridW());
	ret.setY(grid.y() * g_Setting->getGridH());
	return true ;
}

bool CEditData::posToGrid(QPoint &ret, const QPoint &pos)
{
	if ( pos.x() < 0 || pos.x() >= m_Image.width() ) { return false ; }
	if ( pos.y() < 0 || pos.y() >= m_Image.height() ) { return false ; }

	ret.setX(pos.x() / g_Setting->getGridW());
	ret.setY(pos.y() / g_Setting->getGridH());
	return true ;
}

bool CEditData::indexToGrid(QPoint &ret, int index)
{
	int w = m_Image.width() / g_Setting->getGridW() ;
	int h = m_Image.height() / g_Setting->getGridH() ;
	if ( index < 0 || index >= w*h ) { return false ; }

	ret.setX(index % w) ;
	ret.setY(index / w) ;
	return true ;
}

int CEditData::gridToIndex(const QPoint grid)
{
	int w = m_Image.width() / g_Setting->getGridW() ;
	int h = m_Image.height() / g_Setting->getGridH() ;
	if ( grid.x() < 0 || grid.x() >= w ) { return -1 ; }
	if ( grid.y() < 0 || grid.y() >= h ) { return -1 ; }

	return grid.y() * w + grid.x() ;
}

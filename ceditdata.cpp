#include "include.h"
#include "cmaplabel.h"

CEditData::CEditData()
{
	m_pImageLabel = NULL ;
	m_pMapLabel = NULL ;
}

void CEditData::update()
{
	updateMap() ;
	updateImage() ;
}

void CEditData::updateMap()
{
	qDebug() << "pMapLabel:" << m_pMapLabel ;
#if 1
	if ( !m_pMapLabel ) { return ; }
	m_pMapLabel->updateLabels() ;

	int gridW = g_Setting->getMapGridSize().width() ;
	int gridH = g_Setting->getMapGridSize().height() ;
	QImage img = QImage(gridW*100, gridH*100, QImage::Format_ARGB32) ;
	m_pMapLabel->setPixmap(QPixmap::fromImage(img)) ;
	m_pMapLabel->resize(img.size());

#else
	int gridW = g_Setting->getMapGridSize().width() ;
	int gridH = g_Setting->getMapGridSize().height() ;
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
#endif
}

void CEditData::updateImage()
{
	if ( !m_pImageLabel ) { return ; }

	QSize gridSize = g_Setting->getImageGridSize() ;
	int w = m_Image.width() / gridSize.width() ;
	int h = m_Image.height() / gridSize.height() ;
	m_gridData.reserve(w*h) ;

	int gridW = gridSize.width() ;
	int gridH = gridSize.height() ;
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
	if ( gridToPos(st, m_selStartGrid, gridSize) && gridToPos(end, m_selEndGrid, gridSize) ) {
		QPainter painter(&pix) ;
		painter.setPen(QColor(255, 0, 0, 128)) ;
		painter.fillRect(st.x(), st.y(), end.x()+gridW - st.x(), end.y()+gridH - st.y(), QColor(255, 0, 0, 128));
		painter.end() ;
	}
	m_pImageLabel->setPixmap(pix) ;
	m_pImageLabel->resize(m_Image.size());
}

bool CEditData::gridToPos(QPoint &ret, const QPoint &grid, const QSize &gridSize)
{
	ret.setX(grid.x() * gridSize.width());
	ret.setY(grid.y() * gridSize.height());
	return true ;
}

bool CEditData::posToGrid(QPoint &ret, const QPoint &pos, const QSize &gridSize)
{
	ret.setX(pos.x() / gridSize.width());
	ret.setY(pos.y() / gridSize.height());
	return true ;
}

bool CEditData::indexToGrid(QPoint &ret, int index, const QSize &gridSize)
{
	int w = m_Image.width() / gridSize.width() ;
	int h = m_Image.height() / gridSize.height() ;
	if ( index < 0 || index >= w*h ) { return false ; }

	ret.setX(index % w) ;
	ret.setY(index / w) ;
	return true ;
}

int CEditData::gridToIndex(const QPoint grid, const QSize &gridSize)
{
	int w = m_Image.width() / gridSize.width() ;
	int h = m_Image.height() / gridSize.height() ;
	if ( grid.x() < 0 || grid.x() >= w ) { return -1 ; }
	if ( grid.y() < 0 || grid.y() >= h ) { return -1 ; }

	return grid.y() * w + grid.x() ;
}

#include "cgridlabel.h"

CGridLabel::CGridLabel(QWidget *parent) :
    QLabel(parent)
{
	m_viewMode = CEditData::kViewMode_Square ;
	m_mag = 1 ;
}

void CGridLabel::updateGrid()
{
	switch ( m_viewMode ) {
	case CEditData::kViewMode_Square:
		{
			int gridW = m_gridSize.width() ;
			int gridH = m_gridSize.height() ;
			if ( gridW < 1 || gridH < 1 ) { return ; }
			int w = gridW*m_gridNum.x() * m_mag ;
			int h = gridH*m_gridNum.y() * m_mag ;
			if ( w < 1 || h < 1 ) { return ; }
			QImage img = QImage(w, h, QImage::Format_ARGB32) ;
			QColor col(0, 0, 0) ;
			for ( int y = 0 ; y < img.height() ; y += gridH * m_mag ) {
				for ( int x = 0 ; x < img.width() ; x ++ ) {
					img.setPixel(x, y, col.rgba()) ;
				}
			}
			for ( int x = 0 ; x < img.width() ; x += gridW * m_mag ) {
				for ( int y = 0 ; y < img.height() ; y ++ ) {
					img.setPixel(x, y, col.rgba()) ;
				}
			}
			QPixmap pix = QPixmap::fromImage(img) ;
			setPixmap(pix) ;
			resize(pix.size());
		}
		break ;
	case CEditData::kViewMode_Quarter:
		{
			int gridW = m_gridSize.width() ;
			int gridH = m_gridSize.height() ;
			if ( gridW < 1 || gridH < 1 ) { return ; }
			QSize worldSize = QSize(gridW*m_gridNum.x(), gridH*m_gridNum.y()) * m_mag ;
			if ( worldSize.width() < 1 || worldSize.height() < 1 ) { return ; }
			QSize gridSize = m_gridSize * m_mag ;
			QImage img = QImage(worldSize.width(), worldSize.height(), QImage::Format_ARGB32) ;
			QPixmap pix = QPixmap::fromImage(img) ;

			QPainter painter(&pix) ;
			painter.setPen(QColor(0, 0, 0)) ;
			for ( int i = 0 ; i <= m_gridNum.x() ; i ++ ) {
				QPoint p0 = g_EditData->gridToQuarter(QPoint(i, 0), gridSize, worldSize) ;
				QPoint p1 = g_EditData->gridToQuarter(QPoint(i, m_gridNum.y()), gridSize, worldSize) ;
				painter.drawLine(p0, p1) ;
			}
			for ( int i = 0 ; i <= m_gridNum.y() ; i ++ ) {
				QPoint p0 = g_EditData->gridToQuarter(QPoint(0, i), gridSize, worldSize) ;
				QPoint p1 = g_EditData->gridToQuarter(QPoint(m_gridNum.x(), i), gridSize, worldSize) ;
				painter.drawLine(p0, p1) ;
			}
			painter.end() ;

			setPixmap(pix) ;
			resize(img.size()) ;
		}
		break ;
	}
}

void CGridLabel::mousePressEvent(QMouseEvent *ev)
{
	emit sig_mousePress(ev);
}

void CGridLabel::mouseMoveEvent(QMouseEvent *ev)
{
	emit sig_mouseMove(ev);
}

void CGridLabel::mouseReleaseEvent(QMouseEvent *ev)
{
	emit sig_mouseRelease(ev);
}

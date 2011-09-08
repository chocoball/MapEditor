#include "cgridlabel.h"

CGridLabel::CGridLabel(QWidget *parent) :
    QLabel(parent)
{
	m_viewMode = CEditData::kViewMode_Square ;
}

void CGridLabel::updateGrid()
{
	switch ( m_viewMode ) {
	case CEditData::kViewMode_Square:
		{
			int gridW = m_gridSize.width() ;
			int gridH = m_gridSize.height() ;
			if ( gridW < 1 || gridH < 1 ) { return ; }
			int w = gridW*m_gridNum.x() ;
			int h = gridH*m_gridNum.y() ;
			if ( w < 1 || h < 1 ) { return ; }
			QImage img = QImage(w, h, QImage::Format_ARGB32) ;
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
			setPixmap(pix) ;
			resize(img.size());
		}
		break ;
	case CEditData::kViewMode_Quarter:
		{
			int gridW = m_gridSize.width() ;
			int gridH = m_gridSize.height() ;
			if ( gridW < 1 || gridH < 1 ) { return ; }
			QSize size = QSize(gridW*m_gridNum.x(), gridH*m_gridNum.y()) ;
			if ( size.width() < 1 || size.height() < 1 ) { return ; }
			QImage img = QImage(size.width(), size.height(), QImage::Format_ARGB32) ;
			QPixmap pix = QPixmap::fromImage(img) ;
			QPainter painter(&pix) ;
			painter.setPen(QColor(0, 0, 0)) ;
			for ( int i = 0 ; i <= m_gridSize.width() ; i ++ ) {
				QPoint p0 = g_EditData->gridToQuarter(QPoint(i, 0), m_gridSize, size) ;
				QPoint p1 = g_EditData->gridToQuarter(QPoint(i, m_gridSize.height()), m_gridSize, size) ;
				painter.drawLine(p0, p1) ;
			}
			for ( int i = 0 ; i <= m_gridSize.height() ; i ++ ) {
				QPoint p0 = g_EditData->gridToQuarter(QPoint(0, i), m_gridSize, size) ;
				QPoint p1 = g_EditData->gridToQuarter(QPoint(m_gridSize.width(), i), m_gridSize, size) ;
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

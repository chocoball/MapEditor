#include "cgridlabel.h"

CGridLabel::CGridLabel(QWidget *parent) :
    QLabel(parent)
{
}

void CGridLabel::updateGrid()
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

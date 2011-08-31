#include "cgridlabel.h"

CGridLabel::CGridLabel(QWidget *parent) :
    QLabel(parent)
{
	setMouseTracking(true) ;
}

void CGridLabel::updateGrid()
{
	int gridW = m_gridSize.width() ;
	int gridH = m_gridSize.height() ;
	int w = gridW*m_gridNum.x() ;
	int h = gridH*m_gridNum.y() ;
	if ( w < kScreenW ) { w = kScreenW + 1 ; }
	if ( h < kScreenH ) { h = kScreenH + 1 ; }
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

	// 画面サイズ
	col = QColor(255, 255, 0) ;
	for ( int i = 0 ; i < kScreenW ; i ++ ) {
		img.setPixel(i, kScreenH, col.rgba()) ;
	}
	for ( int i = 0 ; i < kScreenH ; i ++ ) {
		img.setPixel(kScreenW, i, col.rgba()) ;
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

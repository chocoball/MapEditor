#include "include.h"

CEditData::CEditData()
{
	m_pImageLabel = NULL ;
}

void CEditData::updateMap()
{
	updateImage() ;
}

void CEditData::updateImage()
{
	if ( !m_pImageLabel ) { return ; }

	QImage img = m_Image ;
	int gridW = g_Setting->getGridW() ;
	int gridH = g_Setting->getGridH() ;
	for ( int y = 0 ; y < m_Image.height() ; y += gridH ) {
		for ( int x = 0 ; x < m_Image.width() ; x ++ ) {
			img.setPixel(x, y, 0) ;
		}
	}
	for ( int x = 0 ; x < m_Image.width() ; x += gridW ) {
		for ( int y = 0 ; y < m_Image.height() ; y ++ ) {
			img.setPixel(x, y, 0) ;
		}
	}
	m_pImageLabel->setPixmap(QPixmap::fromImage(img)) ;
	m_pImageLabel->resize(m_Image.size()) ;
	qDebug() << "image size:" << m_Image.size() ;
}

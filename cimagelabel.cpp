#include "cimagelabel.h"
#include "include.h"

CImageLabel::CImageLabel(QWidget *parent) :
    QLabel(parent)
{
}

CImageLabel::~CImageLabel()
{
}

void CImageLabel::mousePressEvent(QMouseEvent *ev)
{
	m_selStart = ev->pos() ;
	g_EditData->posToGrid(m_selGridSt, m_selStart, g_Setting->getImageGridSize()) ;
}

void CImageLabel::mouseMoveEvent(QMouseEvent *ev)
{
	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd, g_Setting->getImageGridSize()) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;
	g_EditData->updateImage() ;
}

void CImageLabel::mouseReleaseEvent(QMouseEvent *ev)
{
	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd, g_Setting->getImageGridSize()) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;
	g_EditData->updateImage() ;

	emit sig_changeSelectGridRect() ;
}





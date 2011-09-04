#include "cimagelabel.h"
#include "include.h"

CImageLabel::CImageLabel(QWidget *parent) :
    QLabel(parent)
{
	setAcceptDrops(true) ;
}

CImageLabel::~CImageLabel()
{
}

void CImageLabel::mousePressEvent(QMouseEvent *ev)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( ev->pos().x() < 0 || ev->pos().y() < 0 ) { return ; }

	m_selStart = ev->pos() ;
	g_EditData->posToGrid(m_selGridSt, m_selStart, p->imgGridSize) ;
}

void CImageLabel::mouseMoveEvent(QMouseEvent *ev)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( ev->pos().x() < 0 || ev->pos().y() < 0 ) { return ; }

	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd, p->imgGridSize) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;
	g_EditData->updateImage() ;
}

void CImageLabel::mouseReleaseEvent(QMouseEvent *ev)
{
	CEditData::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( ev->pos().x() < 0 || ev->pos().y() < 0 ) { return ; }

	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd, p->imgGridSize) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;
	g_EditData->updateImage() ;

	emit sig_changeSelectGridRect() ;
}


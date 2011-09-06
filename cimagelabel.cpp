#include "cimagelabel.h"
#include "include.h"

CImageLabel::CImageLabel(QWidget *parent) :
    QLabel(parent)
{
	setAcceptDrops(true) ;

	m_pSelectLabel = new QLabel(this) ;
	m_pGridLabel = new CGridLabel(this) ;

	connect(m_pGridLabel, SIGNAL(sig_mousePress(QMouseEvent *)),	this, SLOT(slot_mousePress(QMouseEvent *))) ;
	connect(m_pGridLabel, SIGNAL(sig_mouseMove(QMouseEvent*)),		this, SLOT(slot_mouseMove(QMouseEvent*))) ;
	connect(m_pGridLabel, SIGNAL(sig_mouseRelease(QMouseEvent*)),	this, SLOT(slot_mouseRelease(QMouseEvent*))) ;
}

CImageLabel::~CImageLabel()
{
}

void CImageLabel::updateLabels()
{
	if ( !g_EditData->isSelectMap() ) { return ; }
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	setPixmap(QPixmap::fromImage(p->image)) ;

	m_pGridLabel->setGridSize(p->imgGridSize) ;
	m_pGridLabel->setGridNum(QPoint(p->image.width()/p->imgGridSize.width(), p->image.height()/p->imgGridSize.height())) ;
	m_pGridLabel->updateGrid() ;

	setMinimumSize(p->image.size());

	changeSelectGridRect() ;
}

void CImageLabel::changeSelectGridRect()
{
	if ( !g_EditData->isSelectMap() ) { return ; }
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	if ( !m_pSelectLabel ) {
		m_pSelectLabel = new QLabel(this) ;
	}

	QPoint st, end, pos ;
	g_EditData->gridToPos(st, m_selGridSt, p->imgGridSize) ;
	g_EditData->gridToPos(end, m_selGridEnd, p->imgGridSize) ;
	g_EditData->posToGrid(pos, m_selStart, p->imgGridSize) ;
	g_EditData->gridToPos(pos, pos, p->imgGridSize) ;
	end.setX(end.x() + p->imgGridSize.width()) ;
	end.setY(end.y() + p->imgGridSize.height()) ;

	m_pSelectLabel->move(pos) ;
	QSize size = QSize(end.x()-st.x(), end.y()-st.y()) ;
	QImage img = QImage(size, QImage::Format_ARGB32) ;
	for ( int i = 0 ; i < img.width()*img.height() ; i ++ ) {
		img.setPixel(i%img.width(), i/img.width(), QColor(255, 0, 0, 64).rgba()) ;
	}
	m_pSelectLabel->setPixmap(QPixmap::fromImage(img));
	m_pSelectLabel->resize(size) ;
	m_pSelectLabel->show();
}

void CImageLabel::slot_mousePress(QMouseEvent *ev)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( ev->pos().x() < 0 || ev->pos().y() < 0 ) { return ; }

	m_selStart = ev->pos() ;
	g_EditData->posToGrid(m_selGridSt, m_selStart, p->imgGridSize) ;
}

void CImageLabel::slot_mouseMove(QMouseEvent *ev)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( ev->pos().x() < 0 || ev->pos().y() < 0 ) { return ; }

	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd, p->imgGridSize) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;
	g_EditData->updateImage() ;
}

void CImageLabel::slot_mouseRelease(QMouseEvent *ev)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }
	if ( ev->pos().x() < 0 || ev->pos().y() < 0 ) { return ; }

	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd, p->imgGridSize) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;

	changeSelectGridRect();
	emit sig_changeSelectGridRect() ;
}


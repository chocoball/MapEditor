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
	QPoint grid ;
	bool bDrop = false ;
	if ( g_EditData->posToGrid(grid, ev->pos()) ) {
		QPoint st = g_EditData->getSelStartGrid() ;
		QPoint end = g_EditData->getSelEndGrid() ;
		if ( grid.x() >= st.x() && grid.x() <= end.x()
			 && grid.y() >= st.y() && grid.y() <= end.y() ) {
			bDrop = true ;
		}
	}
	if ( bDrop ) {
		startDrag(ev->pos()) ;
	}
	else {
		m_selStart = ev->pos() ;
		g_EditData->posToGrid(m_selGridSt, m_selStart) ;
	}
}

void CImageLabel::mouseMoveEvent(QMouseEvent *ev)
{
	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;
	g_EditData->updateImage() ;
}

void CImageLabel::mouseReleaseEvent(QMouseEvent *ev)
{
	m_selEnd = ev->pos() ;
	g_EditData->posToGrid(m_selGridEnd, m_selEnd) ;
	g_EditData->setSelStartGrid(m_selGridSt) ;
	g_EditData->setSelEndGrid(m_selGridEnd) ;
	g_EditData->updateImage() ;
}

void CImageLabel::startDrag(QPoint pos)
{
	QPoint st, end, st_grid, end_grid ;
	st_grid = g_EditData->getSelStartGrid() ;
	end_grid = g_EditData->getSelEndGrid() ;
	g_EditData->gridToPos(st, st_grid) ;
	g_EditData->gridToPos(end, end_grid) ;
	end += QPoint(g_Setting->getGridW(), g_Setting->getGridH()) ;
	QImage img = g_EditData->getImagePtr()->copy(st.x(), st.y(), end.x()-st.x(), end.y()-st.y()) ;
	QPixmap pix = QPixmap::fromImage(img) ;

	QByteArray itemData ;
	QDataStream stream(&itemData, QIODevice::WriteOnly) ;
	stream << st_grid << end_grid ;

	QMimeData *mimeData = new QMimeData ;
	mimeData->setData("editor/selected-image", itemData);

	QPainter painter ;
	painter.begin(&pix) ;
	painter.fillRect(pix.rect(), QColor(127, 127, 127, 127));
	painter.end() ;

	QDrag *drag = new QDrag(this) ;
	drag->setMimeData(mimeData);
	drag->setPixmap(pix);
	drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) ;
}




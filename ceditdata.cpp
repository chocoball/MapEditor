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
	if ( !m_pMapLabel ) { return ; }
	m_pMapLabel->updateLabels() ;
}

void CEditData::updateImage()
{
#if 1
	if ( !m_pImageLabel ) { return ; }
	MapData *p = getSelectMapData() ;
	if ( !p ) { return ; }
//	if ( p->image.isNull() ) { return ; }

	QSize gridSize = p->imgGridSize ;

	int gridW = gridSize.width() ;
	int gridH = gridSize.height() ;
	QImage &image = p->image ;

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
	m_pImageLabel->resize(image.size());
#endif
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

QStandardItem *CEditData::addMap(QString id)
{
	QStandardItem *item = new QStandardItem(id) ;
	QStandardItem *root = m_model.invisibleRootItem() ;
	root->appendRow(item) ;

	MapDataGroup data ;
	data.first = item ;
	MapData &map = data.second ;
	map.mapGridSize = g_Setting->getMapGridSize() ;
	map.imgGridSize = g_Setting->getImageGridSize() ;
	m_mapDataGroupList.append(data) ;

	return item ;
}
void CEditData::delMap(QStandardItem *item)
{
	int index = getMapDataIndex(item) ;
	if ( index >= 0 ) {
		delMapData(index) ;
	}

	QStandardItem *root = m_model.invisibleRootItem() ;
	root->removeRow(item->index().row());
}


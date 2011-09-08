#include "include.h"
#include "cmaplabel.h"
#include "cimagelabel.h"

CEditData::CEditData()
{
	m_pImageLabel = NULL ;
	m_pMapLabel = NULL ;

	m_pModelMap = NULL ;
	m_pUndoStack = NULL ;
}

void CEditData::release()
{
	if ( m_pModelMap ) {
		delete m_pModelMap ;
		m_pModelMap = NULL ;
	}
	if ( m_pUndoStack ) {
		delete m_pUndoStack ;
		m_pUndoStack = NULL ;
	}
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
	if ( !m_pImageLabel ) { return ; }
	m_pImageLabel->updateLabels() ;
}

QPoint CEditData::gridToPos(const QPoint &grid, const QSize &gridSize)
{
	QPoint ret ;
	ret.setX(grid.x() * gridSize.width());
	ret.setY(grid.y() * gridSize.height());
	return ret ;
}

QPoint CEditData::gridToQuarter(const QPoint &grid, const QSize &gridSize, const QSize &worldSize)
{
	double fsin = sin(45.0*kPI/180.0) ;
	double fcos = cos(45.0*kPI/180.0) ;

	QPoint pos = gridToPos(grid, gridSize) ;
	QPoint center = QPoint(worldSize.width()/2, worldSize.height()/2) ;
	QPoint ret = pos - center ;

	pos = QPoint(ret.x()*fcos - ret.y()*fsin, ret.x()*fsin + ret.y()*fcos) ;
	pos.setY(pos.y()*fsin) ;
	ret = center + pos ;
	return ret ;
}

QPoint CEditData::quarterToGrid(const QPoint &pos, const QSize &gridSize, const QSize &worldSize)
{
	double fsin = sin(-45.0*kPI/180.0) ;
	double fcos = cos(-45.0*kPI/180.0) ;

	QPoint center = QPoint(worldSize.width()/2, worldSize.height()/2) ;
	QPoint ret = pos - center ;

	ret.setY(ret.y()*fsin) ;
	ret = QPoint(ret.x()*fcos - ret.y()*fsin, ret.x()*fsin + ret.y()*fcos) ;
	ret = center + ret ;
	ret = posToGrid(ret, gridSize) ;
	return ret ;
}

QPoint CEditData::posToGrid(const QPoint &pos, const QSize &gridSize)
{
	QPoint ret ;
	ret.setX(pos.x() / gridSize.width());
	ret.setY(pos.y() / gridSize.height());
	return ret ;
}


#include "include.h"
#include "cmaplabel.h"
#include "cimagelabel.h"

CEditData::CEditData()
{
	m_pImageLabel = NULL ;
	m_pMapLabel = NULL ;

	m_pModelMap = NULL ;
}

void CEditData::release()
{
	if ( m_pModelMap ) {
		delete m_pModelMap ;
		m_pModelMap = NULL ;
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


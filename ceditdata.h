#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QtGui>
#include <QSortFilterProxyModel>
#include "include.h"
#include "clistmodelmap.h"

class CMapLabel ;
class CImageLabel ;

class CEditData
{
public:
	enum {
		kEditMode_Map = 0,
		kEditMode_Data = 1
	} ;

private:
    CEditData() ;

public:
	static CEditData *getInstance()
	{
		static CEditData ins ;
		return &ins ;
	}

	void release() ;
	void update() ;
	void updateMap() ;
	void updateImage() ;

	bool gridToPos(QPoint &ret, const QPoint &grid, const QSize &gridSize) ;
	bool posToGrid(QPoint &ret, const QPoint &pos, const QSize &gridSize) ;

	bool isSelectMap()		{ return getSelMapIndex().isValid() ; }
	bool isSelectTreasure()	{ return getSelTreasureIndex().isValid() ; }
	bool isSelectPoint()	{ return getSelPointIndex().isValid() ; }

	CListModelMap::MapData *getSelectMapData()
	{
		if ( !isSelectMap() ) { return NULL ; }

		QModelIndex modelIndex = getSelMapIndex() ;
		return &m_pModelMap->getMap(modelIndex.row()) ;
	}

	CListModelTreasure::TreasureData *getSelectTreasureData()
	{
		if ( !isSelectTreasure() ) { return NULL ; }

		CListModelMap::MapData *p = getSelectMapData() ;
		if ( !p ) { return NULL ; }

		return &p->pModelTreasure->getTreasure(getSelTreasureIndex().row()) ;
	}

	CListModelPoint::PointData *getSelectPointData()
	{
		if ( !isSelectPoint() ) { return NULL ; }

		CListModelMap::MapData *p = getSelectMapData() ;
		if ( !p ) { return NULL ; }

		return &p->pModelPoint->getPoint(getSelPointIndex().row()) ;
	}

	kAccessor(QPoint, m_selStartGrid, SelStartGrid)
	kAccessor(QPoint, m_selEndGrid, SelEndGrid)
	kAccessor(CImageLabel*, m_pImageLabel, ImageLabel)
	kAccessor(CMapLabel*, m_pMapLabel, MapLabel)
	kAccessor(QModelIndex, m_selectMapIndex, SelMapIndex)
	kAccessor(QModelIndex, m_selectTreasureIndex, SelTreasureIndex)
	kAccessor(QModelIndex, m_selectPointIndex, SelPointIndex)
	kAccessor(int, m_editMode, EditMode)
	kAccessor(CListModelMap*, m_pModelMap, ModelMap)

private:
};

#define g_EditData CEditData::getInstance()


#endif // CEDITDATA_H

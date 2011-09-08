#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QtGui>
#include <QSortFilterProxyModel>
#include <QUndoStack>
#include "include.h"
#include "clistmodelmap.h"
#include "command.h"

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

	// マップ追加 コマンド
	void cmd_addMap(QString mapName, QSize mapGrid, QSize imgGrid)
	{
		m_pUndoStack->push(new Command_AddMap(mapName, mapGrid, imgGrid)) ;
	}
	// マップ削除 コマンド
	void cmd_delMap(int row)
	{
		m_pUndoStack->push(new Command_DelMap(row)) ;
	}
	// お宝追加 コマンド
	void cmd_addTreasure()
	{
		m_pUndoStack->push(new Command_AddTreasure()) ;
	}
	// お宝削除 コマンド
	void cmd_delTreasure(int row)
	{
		m_pUndoStack->push(new Command_DelTreasure(row)) ;
	}
	// 位置追加 コマンド
	void cmd_AddPoint(int kind)
	{
		m_pUndoStack->push(new Command_AddPoint(kind)) ;
	}
	// 位置削除 コマンド
	void cmd_DelPoint(int row)
	{
		m_pUndoStack->push(new Command_DelPoint(row)) ;
	}
	// マップグリッド追加 コマンド
	void cmd_AddMapGrid(QPoint basePos)
	{
		m_pUndoStack->push(new Command_AddMapGrid(basePos)) ;
	}
	// マップグリッド削除 コマンド
	void cmd_DelMapGrid(QPoint basePos)
	{
		m_pUndoStack->push(new Command_DelMapGrid(basePos)) ;
	}
	// お宝移動 コマンド
	void cmd_MoveTreasure(QPoint mapGrid, int index)
	{
		m_pUndoStack->push(new Command_MoveTreasure(mapGrid, index)) ;
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
	kAccessor(QUndoStack*, m_pUndoStack, UndoStack)

private:
};

#define g_EditData CEditData::getInstance()


#endif // CEDITDATA_H

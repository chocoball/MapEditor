#include <QDebug>
#include "command.h"
#include "ceditdata.h"
#include "cmaplabel.h"

/****************************************************
  マップ追加
****************************************************/
Command_AddMap::Command_AddMap(QString mapName, QSize mapGrid, QSize imgGrid) :
	QUndoCommand(QObject::trUtf8("マップ追加"))
{
	m_mapName = mapName ;
	m_mapGrid = mapGrid ;
	m_imgGrid = imgGrid ;
}

void Command_AddMap::redo()
{
	m_row = g_EditData->getModelMap()->addMap(m_mapName) ;
	if ( m_row >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_row) ;
		pData->mapGridSize = m_mapGrid ;
		pData->imgGridSize = m_imgGrid ;
	}
}

void Command_AddMap::undo()
{
	g_EditData->getModelMap()->removeMap(m_row) ;
}

/****************************************************
  マップ削除
****************************************************/
Command_DelMap::Command_DelMap(int row) :
	QUndoCommand(QObject::trUtf8("マップ削除"))
{
	m_row = row ;
}

void Command_DelMap::redo()
{
	m_mapData = *g_EditData->getModelMap()->getMap(m_row) ;
	m_treasureDatas = m_mapData.pModelTreasure->getList() ;
	m_pointDatas = m_mapData.pModelPoint->getList() ;
	g_EditData->getModelMap()->removeMap(m_row) ;
}

void Command_DelMap::undo()
{
	m_row = g_EditData->getModelMap()->addMap(m_mapData, &m_treasureDatas, &m_pointDatas, m_row) ;
}

/****************************************************
  お宝追加
****************************************************/
Command_AddTreasure::Command_AddTreasure() :
	QUndoCommand(QObject::trUtf8("お宝追加"))
{
	m_mapRow = g_EditData->getSelMapIndex().row() ;
	m_treasureRow = -1 ;
}

void Command_AddTreasure::redo()
{
	if ( m_mapRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_treasureRow = pData->pModelTreasure->addTreasure(QPoint(0, 0), 1) ;
		if ( m_treasureRow >= 0 ) {
			g_EditData->getMapLabel()->slot_addTreasureLabel(m_treasureRow) ;
		}
	}
}

void Command_AddTreasure::undo()
{
	if ( m_mapRow >= 0 && m_treasureRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
		pData->pModelTreasure->removeTreasure(m_treasureRow) ;
		g_EditData->getMapLabel()->slot_removeTreasureLabel(m_treasureRow) ;
	}
}

/****************************************************
  お宝削除
****************************************************/
Command_DelTreasure::Command_DelTreasure(int row) :
	QUndoCommand(QObject::trUtf8("お宝削除"))
{
	m_mapRow = g_EditData->getSelMapIndex().row() ;
	m_treasureRow = row ;
}

void Command_DelTreasure::redo()
{
	if ( m_mapRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_treasureData = *pData->pModelTreasure->getTreasure(m_treasureRow) ;

		pData->pModelTreasure->removeTreasure(m_treasureRow) ;
		g_EditData->getMapLabel()->slot_removeTreasureLabel(m_treasureRow) ;
	}
}

void Command_DelTreasure::undo()
{
	if ( m_mapRow >= 0 && m_treasureRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_treasureRow = pData->pModelTreasure->addTreasure(m_treasureData.mapGrid, m_treasureData.num) ;
		if ( m_treasureRow >= 0 ) {
			g_EditData->getMapLabel()->slot_addTreasureLabel(m_treasureRow) ;
		}
	}
}

/****************************************************
  位置追加
****************************************************/
Command_AddPoint::Command_AddPoint(int kind) :
	QUndoCommand(QObject::trUtf8("位置追加"))
{
	m_mapRow = g_EditData->getSelMapIndex().row() ;
	m_pointRow = -1 ;
	m_kind = kind ;
}

void Command_AddPoint::redo()
{
	if ( m_mapRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_pointRow = pData->pModelPoint->addPoint(QPoint(), m_kind) ;
		if ( m_pointRow >= 0 ) {
			g_EditData->getMapLabel()->slot_addPointLabel(m_pointRow) ;
		}
	}
}

void Command_AddPoint::undo()
{
	if ( m_mapRow >= 0 && m_pointRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;

		pData->pModelPoint->removePoint(m_pointRow) ;
		g_EditData->getMapLabel()->slot_removePointLabel(m_pointRow);
	}
}

/****************************************************
  位置削除
****************************************************/
Command_DelPoint::Command_DelPoint(int row) :
	QUndoCommand(QObject::trUtf8("位置削除"))
{
	m_mapRow = g_EditData->getSelMapIndex().row() ;
	m_pointRow = row ;
}

void Command_DelPoint::redo()
{
	if ( m_mapRow >= 0 && m_pointRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_pointData = *pData->pModelPoint->getPoint(m_pointRow) ;
		pData->pModelPoint->removePoint(m_pointRow) ;
		g_EditData->getMapLabel()->slot_removePointLabel(m_pointRow);
	}
}

void Command_DelPoint::undo()
{
	if ( m_mapRow >= 0 ) {
		CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;

		m_pointRow = pData->pModelPoint->addPoint(m_pointData.mapGrid, m_pointData.kind, m_pointRow) ;
		if ( m_pointRow >= 0 ) {
			g_EditData->getMapLabel()->slot_addPointLabel(m_pointRow) ;
		}
	}
}

/****************************************************
  マップグリッド追加
****************************************************/
Command_AddMapGrid::Command_AddMapGrid(QPoint basePos) :
	QUndoCommand(QObject::trUtf8("マップグリッド追加"))
{
	m_mapRow = -1 ;
	if ( !g_EditData->getSelectMapData() ) { return ; }

	m_mapRow = g_EditData->getSelMapIndex().row() ;
	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;

	m_basePos = basePos ;
	m_mapGridSize = pData->mapGridSize ;
	m_imgGridSize = pData->imgGridSize ;
	m_stGrid = g_EditData->getSelStartGrid() ;
	m_endGrid = g_EditData->getSelEndGrid() ;
}

void Command_AddMapGrid::redo()
{
	if ( m_mapRow < 0 ) { return ; }
	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
	g_EditData->getMapLabel()->addMapGrid(m_basePos, m_stGrid, m_endGrid, m_mapGridSize, m_imgGridSize, *pData) ;
}

void Command_AddMapGrid::undo()
{
	if ( m_mapRow < 0 ) { return ; }
	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
	g_EditData->getMapLabel()->removeMapGrid(m_basePos, m_stGrid, m_endGrid, m_mapGridSize, *pData) ;
}

/****************************************************
  マップグリッド削除
****************************************************/
Command_DelMapGrid::Command_DelMapGrid(QPoint basePos) :
	QUndoCommand(QObject::trUtf8("マップグリッド削除"))
{
	m_mapRow = -1 ;
	if ( !g_EditData->getSelectMapData() ) { return ; }

	m_mapRow = g_EditData->getSelMapIndex().row() ;
	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;

	m_basePos = basePos ;
	m_mapGridSize = pData->mapGridSize ;
	m_imgGridSize = pData->imgGridSize ;
	m_stGrid = g_EditData->getSelStartGrid() ;
	m_endGrid = g_EditData->getSelEndGrid() ;
}

void Command_DelMapGrid::redo()
{
	if ( m_mapRow < 0 ) { return ; }

	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
	QPoint baseGrid = g_EditData->posToGrid(m_basePos, m_mapGridSize * g_EditData->getDispMagMap()) ;

	m_grids.clear();
	for ( int y = m_stGrid.y() ; y <= m_endGrid.y() ; y ++ ) {
		for ( int x = m_stGrid.x() ; x <= m_endGrid.x() ; x ++ ) {
			QPoint mapGrid = baseGrid + QPoint(x-m_stGrid.x(), y-m_stGrid.y()) ;

			int index = g_EditData->getMapLabel()->getGridLabelIndex(mapGrid) ;
			if ( index >= 0 ) {
				g_EditData->getMapLabel()->releaseMapTipLabel(index) ;
			}
			index = pData->getGridDataIndex(mapGrid) ;
			if ( index >= 0 ) {
				CListModelMap::GridData d = pData->getGridData(index) ;
				m_grids.append(d) ;
			}
			pData->removeGridData(mapGrid) ;
		}
	}
}

void Command_DelMapGrid::undo()
{
	if ( m_mapRow < 0 ) { return ; }
	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;

	for ( int i = 0 ; i < m_grids.size() ; i ++ ) {
		CListModelMap::GridData grid = m_grids[i] ;
		g_EditData->getMapLabel()->addMapGrid(m_basePos, grid.imageGrid, grid.imageGrid, m_mapGridSize, m_imgGridSize, *pData) ;
	}
}

/****************************************************
  お宝移動
****************************************************/
Command_MoveTreasure::Command_MoveTreasure(QPoint mapGrid, int treasureIndex) :
	QUndoCommand(QObject::trUtf8("お宝移動"))
{
	m_mapRow = -1 ;
	if ( !g_EditData->getSelectMapData() ) { return ; }

	m_mapRow = g_EditData->getSelMapIndex().row() ;
	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;

	m_mapGrid = mapGrid ;
	m_treasureIndex = treasureIndex ;
	m_mapGridSize = pData->mapGridSize ;
	m_oldGrid = pData->pModelTreasure->getTreasure(treasureIndex)->mapGrid ;
}

void Command_MoveTreasure::redo()
{
	if ( m_mapRow < 0 ) { return ; }

	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
	pData->pModelTreasure->setData(pData->pModelTreasure->index(m_treasureIndex), m_mapGrid, Qt::UserRole) ;

	QPoint pos = g_EditData->gridToPos(m_mapGrid, m_mapGridSize * g_EditData->getDispMagMap()) ;
	g_EditData->getMapLabel()->moveTreasureLabel(m_treasureIndex, pos) ;
}

void Command_MoveTreasure::undo()
{
	if ( m_mapRow < 0 ) { return ; }

	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
	pData->pModelTreasure->setData(pData->pModelTreasure->index(m_treasureIndex), m_oldGrid, Qt::UserRole) ;

	QPoint pos = g_EditData->gridToPos(m_oldGrid, m_mapGridSize * g_EditData->getDispMagMap()) ;
	g_EditData->getMapLabel()->moveTreasureLabel(m_treasureIndex, pos) ;
}

/****************************************************
  位置移動
****************************************************/
Command_MovePoint::Command_MovePoint(QPoint mapGrid, int pointIndex) :
	QUndoCommand(QObject::trUtf8("位置移動"))
{
	m_mapRow = -1 ;
	if ( !g_EditData->getSelectMapData() ) { return ; }

	m_mapRow = g_EditData->getSelMapIndex().row() ;
	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;

	m_mapGrid = mapGrid ;
	m_pointIndex = pointIndex ;
	m_mapGridSize = pData->mapGridSize ;
	m_oldGrid = pData->pModelPoint->getPoint(pointIndex)->mapGrid ;
}

void Command_MovePoint::redo()
{
	if ( m_mapRow < 0 ) { return ; }

	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
	pData->pModelPoint->setData(pData->pModelPoint->index(m_pointIndex), m_mapGrid, Qt::UserRole) ;

	QPoint pos = g_EditData->gridToPos(m_mapGrid, m_mapGridSize * g_EditData->getDispMagMap()) ;
	g_EditData->getMapLabel()->movePointLabel(m_pointIndex, pos) ;
}

void Command_MovePoint::undo()
{
	if ( m_mapRow < 0 ) { return ; }

	CListModelMap::MapData *pData = g_EditData->getModelMap()->getMap(m_mapRow) ;
	pData->pModelPoint->setData(pData->pModelPoint->index(m_pointIndex), m_oldGrid, Qt::UserRole) ;

	QPoint pos = g_EditData->gridToPos(m_oldGrid, m_mapGridSize * g_EditData->getDispMagMap()) ;
	g_EditData->getMapLabel()->movePointLabel(m_pointIndex, pos) ;
}

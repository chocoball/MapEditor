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
	qDebug() << "Command_AddMap::redo" ;

	m_row = g_EditData->getModelMap()->addMap(m_mapName) ;
	if ( m_row >= 0 ) {
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_row) ;
		data.mapGridSize = m_mapGrid ;
		data.imgGridSize = m_imgGrid ;
	}
}

void Command_AddMap::undo()
{
	qDebug() << "Command_AddMap::undo" ;
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
	m_mapData = g_EditData->getModelMap()->getMap(m_row) ;
	m_treasureDatas = m_mapData.pModelTreasure->getList() ;
	m_pointDatas = m_mapData.pModelPoint->getList() ;
	g_EditData->getModelMap()->removeMap(m_row) ;
}

void Command_DelMap::undo()
{
	m_row = g_EditData->getModelMap()->addMap(m_mapData, &m_treasureDatas, &m_pointDatas) ;
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
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_treasureRow = data.pModelTreasure->addTreasure(QPoint(0, 0), 1) ;
		if ( m_treasureRow >= 0 ) {
			g_EditData->getMapLabel()->slot_addTreasureLabel(m_treasureRow) ;
		}
	}
}

void Command_AddTreasure::undo()
{
	if ( m_mapRow >= 0 && m_treasureRow >= 0 ) {
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;
		data.pModelTreasure->removeTreasure(m_treasureRow) ;
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
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_treasureData = data.pModelTreasure->getTreasure(m_treasureRow) ;

		data.pModelTreasure->removeTreasure(m_treasureRow) ;
		g_EditData->getMapLabel()->slot_removeTreasureLabel(m_treasureRow) ;
	}
}

void Command_DelTreasure::undo()
{
	if ( m_mapRow >= 0 && m_treasureRow >= 0 ) {
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_treasureRow = data.pModelTreasure->addTreasure(m_treasureData.mapGrid, m_treasureData.num) ;
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
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_pointRow = data.pModelPoint->addPoint(QPoint(), m_kind) ;
		if ( m_pointRow >= 0 ) {
			g_EditData->getMapLabel()->slot_addPointLabel(m_pointRow) ;
		}
	}
}

void Command_AddPoint::undo()
{
	if ( m_mapRow >= 0 && m_pointRow >= 0 ) {
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;

		data.pModelPoint->removePoint(m_pointRow) ;
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
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;
		m_pointData = data.pModelPoint->getPoint(m_pointRow) ;
		data.pModelPoint->removePoint(m_pointRow) ;
		g_EditData->getMapLabel()->slot_removePointLabel(m_pointRow);
	}
}

void Command_DelPoint::undo()
{
	if ( m_mapRow >= 0 ) {
		CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;

		m_pointRow = data.pModelPoint->addPoint(m_pointData.mapGrid, m_pointData.kind) ;
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
	m_bAdd = false ;

	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	m_mapRow = g_EditData->getSelMapIndex().row() ;
	m_basePos = basePos ;
	m_mapGridSize = p->mapGridSize ;
	m_stGrid = g_EditData->getSelStartGrid() ;
	m_endGrid = g_EditData->getSelEndGrid() ;
	m_bAdd = true ;
}

void Command_AddMapGrid::redo()
{
	if ( !m_bAdd ) { return ; }

	CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;

	for ( int y = m_stGrid.y() ; y <= m_stGrid.y() ; y ++ ) {
		for ( int x = m_endGrid.x() ; x <= m_endGrid.x() ; x ++ ) {
			QPoint imgGrid = QPoint(x, y) ;
			QPoint mapGrid = m_basePos + QPoint(x-m_stGrid.x(), y-m_stGrid.y()) ;
			g_EditData->getMapLabel()->addMapGrid(mapGrid, imgGrid) ;

			data.addGridData(mapGrid, imgGrid) ;
		}
	}
}

void Command_AddMapGrid::undo()
{
	if ( !m_bAdd ) { return ; }

	CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;

	for ( int y = m_stGrid.y() ; y <= m_stGrid.y() ; y ++ ) {
		for ( int x = m_endGrid.x() ; x <= m_endGrid.x() ; x ++ ) {
			QPoint mapGrid = m_basePos + QPoint(x-m_stGrid.x(), y-m_stGrid.y()) ;

			int index = g_EditData->getMapLabel()->getGridLabelIndex(mapGrid) ;
			if ( index >= 0 ) {
				g_EditData->getMapLabel()->releaseMapTipLabel(index) ;
			}

			data.removeGridData(mapGrid) ;
		}
	}
}

/****************************************************
  マップグリッド削除
****************************************************/
Command_DelMapGrid::Command_DelMapGrid(QPoint basePos) :
	QUndoCommand(QObject::trUtf8("マップグリッド削除"))
{
	m_bDel = false ;

	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) { return ; }

	m_mapRow = g_EditData->getSelMapIndex().row() ;
	m_basePos = basePos ;
	m_mapGridSize = p->mapGridSize ;
	m_stGrid = g_EditData->getSelStartGrid() ;
	m_endGrid = g_EditData->getSelEndGrid() ;
	m_bDel = true ;
}

void Command_DelMapGrid::redo()
{
	if ( !m_bDel ) { return ; }

	CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;

	for ( int y = m_stGrid.y() ; y <= m_stGrid.y() ; y ++ ) {
		for ( int x = m_endGrid.x() ; x <= m_endGrid.x() ; x ++ ) {
			QPoint mapGrid = m_basePos + QPoint(x-m_stGrid.x(), y-m_stGrid.y()) ;

			int index = g_EditData->getMapLabel()->getGridLabelIndex(mapGrid) ;
			if ( index >= 0 ) {
				g_EditData->getMapLabel()->releaseMapTipLabel(index) ;
			}

			data.removeGridData(mapGrid) ;
		}
	}
}

void Command_DelMapGrid::undo()
{
	if ( !m_bDel ) { return ; }

	CListModelMap::MapData &data = g_EditData->getModelMap()->getMap(m_mapRow) ;

	for ( int y = m_stGrid.y() ; y <= m_stGrid.y() ; y ++ ) {
		for ( int x = m_endGrid.x() ; x <= m_endGrid.x() ; x ++ ) {
			QPoint imgGrid = QPoint(x, y) ;
			QPoint mapGrid = m_basePos + QPoint(x-m_stGrid.x(), y-m_stGrid.y()) ;
			g_EditData->getMapLabel()->addMapGrid(mapGrid, imgGrid) ;

			data.addGridData(mapGrid, imgGrid) ;
		}
	}
}



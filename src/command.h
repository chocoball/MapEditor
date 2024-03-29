#ifndef COMMAND_H
#define COMMAND_H

#include <QtGui>
#include "clistmodelmap.h"

// マップ追加コマンド
class Command_AddMap : public QUndoCommand
{
public:
	Command_AddMap(QString mapName, QSize mapGrid, QSize imgGrid) ;

	void undo() ;
	void redo() ;

private:
	QString					m_mapName ;
	QSize					m_mapGrid ;
	QSize					m_imgGrid ;
	int						m_row ;
} ;

// マップ削除コマンド
class Command_DelMap : public QUndoCommand
{
public:
	Command_DelMap(int row) ;

	void undo() ;
	void redo() ;

private:
	CListModelMap::MapData					m_mapData ;
	QList<CListModelTreasure::TreasureData>	m_treasureDatas ;
	QList<CListModelPoint::PointData>		m_pointDatas ;
	int										m_row ;
} ;

// お宝追加コマンド
class Command_AddTreasure : public QUndoCommand
{
public:
	Command_AddTreasure() ;

	void undo() ;
	void redo() ;

private:
	int						m_mapRow ;
	int						m_treasureRow ;
};

// お宝削除コマンド
class Command_DelTreasure : public QUndoCommand
{
public:
	Command_DelTreasure(int row) ;

	void undo() ;
	void redo() ;

private:
	int									m_mapRow ;
	int									m_treasureRow ;
	CListModelTreasure::TreasureData	m_treasureData ;
};

// 位置追加コマンド
class Command_AddPoint : public QUndoCommand
{
public:
	Command_AddPoint(int kind) ;

	void undo() ;
	void redo() ;

private:
	int			m_mapRow ;
	int			m_pointRow ;
	int			m_kind ;
} ;

// 位置削除コマンド
class Command_DelPoint : public QUndoCommand
{
public:
	Command_DelPoint(int row) ;

	void undo() ;
	void redo() ;

private:
	int							m_mapRow ;
	int							m_pointRow ;
	CListModelPoint::PointData	m_pointData ;
} ;

// マップグリッド追加
class Command_AddMapGrid : public QUndoCommand
{
public:
	Command_AddMapGrid(QPoint basePos) ;

	void undo() ;
	void redo() ;

private:
	QPoint					m_basePos ;
	QSize					m_mapGridSize ;
	QSize					m_imgGridSize ;
	QPoint					m_stGrid ;
	QPoint					m_endGrid ;
	int						m_mapRow ;
};

// マップグリッド削除
class Command_DelMapGrid : public QUndoCommand
{
public:
	Command_DelMapGrid(QPoint basePos) ;

	void undo() ;
	void redo() ;

private:
	QPoint							m_basePos ;
	QSize							m_mapGridSize ;
	QSize							m_imgGridSize ;
	QPoint							m_stGrid ;
	QPoint							m_endGrid ;
	int								m_mapRow ;
	QList<CListModelMap::GridData>	m_grids ;
};

// お宝移動
class Command_MoveTreasure : public QUndoCommand
{
public:
	Command_MoveTreasure(QPoint mapGrid, int treasureIndex) ;

	void undo() ;
	void redo() ;

private:
	int					m_mapRow ;
	int					m_treasureIndex ;
	QPoint				m_mapGrid ;
	QSize				m_mapGridSize ;
	QPoint				m_oldGrid ;
};

// 位置移動
class Command_MovePoint : public QUndoCommand
{
public:
	Command_MovePoint(QPoint mapGrid, int pointIndex) ;

	void undo() ;
	void redo() ;

private:
	int					m_mapRow ;
	int					m_pointIndex ;
	QPoint				m_mapGrid ;
	QSize				m_mapGridSize ;
	QPoint				m_oldGrid ;
};


#endif // COMMAND_H

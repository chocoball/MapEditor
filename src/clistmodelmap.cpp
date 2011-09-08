#include "clistmodelmap.h"
#include <QDebug>

CListModelMap::CListModelMap(QObject *parent) :
    QAbstractListModel(parent)
{
}

CListModelMap::~CListModelMap()
{
	for ( int i = 0 ; i < m_mapDatas.size() ; i ++ ) {
		delete m_mapDatas[i].pModelTreasure ;
		delete m_mapDatas[i].pModelPoint ;
	}
}

QVariant CListModelMap::data(const QModelIndex &index, int role) const
{
	int row = index.row() ;
	if ( row < 0 || row >= m_mapDatas.size() ) {
		return QVariant() ;
	}
	if ( role == Qt::EditRole || role == Qt::DisplayRole ) {
		return QVariant(m_mapDatas.at(row).mapName) ;
	}
	return QVariant() ;
}

int CListModelMap::rowCount(const QModelIndex &parent) const
{
	return m_mapDatas.size() ;
}

Qt::ItemFlags CListModelMap::flags(const QModelIndex &index) const
{
	if ( !index.isValid() ) {
		return Qt::ItemIsEnabled ;
	}
	return QAbstractListModel::flags(index) | Qt::ItemIsEditable ;
}

bool CListModelMap::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int row = index.row() ;
	if ( row < 0 || row >= m_mapDatas.size() ) { return false ; }

	if ( index.isValid() && role == Qt::EditRole ) {
		m_mapDatas[row].mapName = value.toString() ;
		emit dataChanged(index, index) ;
		return true ;
	}
	return false ;
}

bool CListModelMap::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(QModelIndex(), row, row+count-1) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		m_mapDatas.insert(row, MapData()) ;
	}
	endInsertRows() ;
	return true ;
}

bool CListModelMap::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(QModelIndex(), row, row+count-1) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		MapData data = m_mapDatas.takeAt(row) ;
		delete data.pModelTreasure ;
		delete data.pModelPoint ;
	}
	endRemoveRows() ;
	return true ;
}

int CListModelMap::addMap(QString str, int row)
{
	if ( row < 0 ) {
		row = rowCount(QModelIndex()) ;
	}
	insertRows(row, 1, QModelIndex()) ;

	QModelIndex index = this->index(row, 0, QModelIndex()) ;
	if ( !index.isValid() ) { return -1 ; }

	if ( !setData(index, str, Qt::EditRole) ) {
		return -1 ;
	}
	m_mapDatas[row].pModelTreasure = new CListModelTreasure(this) ;
	m_mapDatas[row].pModelPoint = new CListModelPoint(this) ;
	return row ;
}

int CListModelMap::addMap(MapData &data, QList<CListModelTreasure::TreasureData> *pTreasureDatas, QList<CListModelPoint::PointData> *pPointDatas, int row)
{
	row = addMap(data.mapName, row) ;
	if ( row < 0 ) { return -1 ; }
	MapData &newMap = m_mapDatas[row] ;
	newMap.image = data.image ;
	newMap.imageName = data.imageName ;
	newMap.imgGridSize = data.imgGridSize ;
	newMap.mapGridSize = data.mapGridSize ;
	newMap.gridDatas = data.gridDatas ;
	newMap.imgDatas = data.imgDatas ;
	if ( pTreasureDatas ) {
		for ( int i = 0 ; i < pTreasureDatas->size() ; i ++ ) {
			newMap.pModelTreasure->addTreasure(pTreasureDatas->at(i).mapGrid, pTreasureDatas->at(i).num) ;
		}
	}
	if ( pPointDatas ) {
		for ( int i = 0 ; i < pPointDatas->size() ; i ++ ) {
			newMap.pModelPoint->addPoint(pPointDatas->at(i).mapGrid, pPointDatas->at(i).kind) ;
		}
	}
	return row ;
}

bool CListModelMap::removeMap(int row)
{
	removeRows(row, 1, QModelIndex()) ;
	return true ;
}


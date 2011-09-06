#include "clistmodelmap.h"
#include <QDebug>

CListModelMap::CListModelMap(QObject *parent) :
    QAbstractListModel(parent)
{
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

int CListModelMap::addMap(QString str)
{
	int row = rowCount(QModelIndex()) ;
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

bool CListModelMap::removeMap(int row)
{
	removeRows(row, 1, QModelIndex()) ;
	return true ;
}


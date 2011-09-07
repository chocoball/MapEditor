#include "clistmodeltreasure.h"

CListModelTreasure::CListModelTreasure(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant CListModelTreasure::data(const QModelIndex &index, int role) const
{
	int row = index.row() ;
	if ( row < 0 || row >= m_datas.size() ) { return QVariant() ; }
	if ( role == Qt::EditRole ) {
		const TreasureData &d = m_datas.at(row) ;
		return trUtf8("%1").arg(d.num) ;
	}
	else if ( role == Qt::DisplayRole ) {
		const TreasureData &d = m_datas.at(row) ;
		return trUtf8("x:%1 y:%2 個数:%3").arg(d.mapGrid.x()).arg(d.mapGrid.y()).arg(d.num) ;
	}
	return QVariant() ;
}

int CListModelTreasure::rowCount(const QModelIndex &/*parent*/) const
{
	return m_datas.size() ;
}

Qt::ItemFlags CListModelTreasure::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable ;
}

bool CListModelTreasure::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int row = index.row() ;
	if ( row < 0 || row >= m_datas.size() || !index.isValid() ) {
		return false ;
	}
	if ( role == Qt::EditRole ) {
		m_datas[row].num = value.toInt() ;
		emit dataChanged(index, index) ;
		return true ;
	}
	else if ( role == Qt::UserRole ) {	// grid
		m_datas[row].mapGrid = value.toPoint() ;
		emit dataChanged(index, index) ;
		return true ;
	}

	return false ;
}

bool CListModelTreasure::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(QModelIndex(), row, row+count-1) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		m_datas.insert(row, TreasureData()) ;
	}
	endInsertRows() ;
	return true ;
}

bool CListModelTreasure::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(QModelIndex(), row, row+count-1) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		m_datas.removeAt(row) ;
	}
	endRemoveRows() ;
	return true ;
}

int CListModelTreasure::addTreasure(QPoint mapGrid, int num)
{
	int row = rowCount(QModelIndex()) ;
	insertRows(row, 1, QModelIndex()) ;
	QModelIndex index = this->index(row) ;
	if ( !index.isValid() ) { return -1 ; }
	if ( !setData(index, num, Qt::EditRole) ) {
		return -1 ;
	}
	if ( !setData(index, mapGrid, Qt::UserRole) ) {
		return -1 ;
	}
	return row ;
}

void CListModelTreasure::removeTreasure(int row)
{
	removeRows(row, 1, QModelIndex()) ;
}


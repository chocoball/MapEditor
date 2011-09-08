#include "clistmodelpoint.h"
#include <QDebug>

CListModelPoint::CListModelPoint(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant CListModelPoint::data(const QModelIndex &index, int role) const
{
	int row = index.row() ;
	if ( row < 0 || row >= m_datas.size() || !index.isValid() ) { return QVariant() ; }

	if ( role == Qt::DisplayRole ) {
		QString str ;
		switch ( m_datas[row].kind ) {
		case 0:
			str = trUtf8("開始位置") ;
			break ;
		case 1:
			str = trUtf8("終了位置") ;
			break ;
		}
		str += QString("x:%1 y:%2").arg(m_datas[row].mapGrid.x()).arg(m_datas[row].mapGrid.y()) ;
		return str ;
	}
	else if ( role == Qt::EditRole ) {
		return m_datas[row].kind ;
	}
	return QVariant() ;
}

int CListModelPoint::rowCount(const QModelIndex &parent) const
{
	return m_datas.size() ;
}

Qt::ItemFlags CListModelPoint::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable ;
}

bool CListModelPoint::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int row = index.row() ;
	if ( row < 0 || row >= m_datas.size() ) { return false ; }
	if ( role == Qt::EditRole ) {
		m_datas[row].kind = value.toInt() ;
		emit dataChanged(index, index) ;
		return true ;
	}
	else if ( role == Qt::UserRole ) {
		m_datas[row].mapGrid = value.toPoint() ;
		emit dataChanged(index, index) ;
		return true ;
	}
	return false ;
}

bool CListModelPoint::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(QModelIndex(), row, row+count-1) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		m_datas.insert(row, PointData()) ;
	}
	endInsertRows();
	return true ;
}

bool CListModelPoint::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(QModelIndex(), row, row+count-1) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		m_datas.removeAt(row) ;
	}
	endRemoveRows() ;
	return true ;
}

int CListModelPoint::addPoint(QPoint mapGrid, int kind, int row)
{
	if ( row < 0 ) {
		row = rowCount(QModelIndex()) ;
	}
	insertRows(row, 1, QModelIndex()) ;
	QModelIndex index = this->index(row) ;

	qDebug() << "addPoint index valid:" << index.isValid() ;

	if ( !setData(index, kind, Qt::EditRole) ) { return -1 ; }
	if ( !setData(index, mapGrid, Qt::UserRole) ) { return -1 ; }
	return row ;
}

void CListModelPoint::removePoint(QPoint mapGrid)
{
	for ( int i = 0 ; i < m_datas.size() ; i ++ ) {
		if ( m_datas.at(i).mapGrid == mapGrid ) {
			removePoint(i) ;
			break ;
		}
	}
}

void CListModelPoint::removePoint(int index)
{
	removeRows(index, 1, QModelIndex()) ;
}





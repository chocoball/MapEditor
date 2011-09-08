#ifndef CLISTMODELTREASURE_H
#define CLISTMODELTREASURE_H

#include <QAbstractListModel>
#include <QPoint>

class CListModelTreasure : public QAbstractListModel
{
public:
	typedef struct _tagTreasureData {
		QPoint		mapGrid ;
		int			num ;

		_tagTreasureData()
		{
			num = 0 ;
		}
	} TreasureData ;

    Q_OBJECT
public:
    explicit CListModelTreasure(QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const ;
	int rowCount(const QModelIndex &parent) const ;
	Qt::ItemFlags flags(const QModelIndex &index) const ;
	bool setData(const QModelIndex &index, const QVariant &value, int role) ;
	bool insertRows(int row, int count, const QModelIndex &parent) ;
	bool removeRows(int row, int count, const QModelIndex &parent) ;

	int addTreasure(QPoint mapGrid, int num) ;
	void removeTreasure(int row) ;

	TreasureData &getTreasure(int row) { return m_datas[row] ; }
	QList<TreasureData> &getList() { return m_datas ; }

	int getTreasureIndex(QPoint mapGrid)
	{
		for ( int i = 0 ; i < m_datas.size() ; i ++ ) {
			if ( m_datas[i].mapGrid == mapGrid ) { return i ; }
		}
		return -1 ;
	}

signals:

public slots:

private:
	QList<TreasureData>	m_datas ;
};

#endif // CLISTMODELTREASURE_H

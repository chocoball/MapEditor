#ifndef CLISTMODELTREASURE_H
#define CLISTMODELTREASURE_H

#include <QAbstractListModel>
#include <QPoint>

class CListModelTreasure : public QAbstractListModel
{
public:
	typedef struct _tagTreasureData {
		QModelIndex mapIndex ;
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

	int addTreasure(QModelIndex mapIndex, QPoint mapGrid) ;
	void removeTreasure(int row) ;

	TreasureData &getTreasure(int row) { return m_datas[row] ; }
	QList<TreasureData> &getList() { return m_datas ; }

signals:

public slots:

private:
	QList<TreasureData>	m_datas ;
};

#endif // CLISTMODELTREASURE_H

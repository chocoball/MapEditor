#ifndef CLISTMODELPOINT_H
#define CLISTMODELPOINT_H

#include <QAbstractListModel>
#include <QPoint>

class CListModelPoint : public QAbstractListModel
{
public:
	typedef struct _tagPointData {
		QPoint		mapGrid ;
		int			kind ;

		_tagPointData()
		{
			kind = 0 ;
		}
	} PointData ;

    Q_OBJECT
public:
    explicit CListModelPoint(QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const ;
	int rowCount(const QModelIndex &parent) const ;
	Qt::ItemFlags flags(const QModelIndex &index) const ;
	bool setData(const QModelIndex &index, const QVariant &value, int role) ;
	bool insertRows(int row, int count, const QModelIndex &parent) ;
	bool removeRows(int row, int count, const QModelIndex &parent) ;

	int addPoint(QPoint mapGrid, int kind, int row = -1) ;
	void removePoint(QPoint mapGrid) ;
	void removePoint(int index) ;

	PointData &getPoint(int index) { return m_datas[index] ; }
	QList<PointData> &getList() { return m_datas ; }

signals:

public slots:

private:
	QList<PointData>	m_datas ;
};

#endif // CLISTMODELPOINT_H

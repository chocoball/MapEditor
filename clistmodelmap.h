#ifndef CLISTMODELMAP_H
#define CLISTMODELMAP_H

#include <QAbstractListModel>
#include <QImage>
#include "clistmodeltreasure.h"
#include "clistmodelpoint.h"

class CListModelMap : public QAbstractListModel
{
public:
	typedef struct _tagGridData {
		QPoint			mapGrid ;
		QPoint			imageGrid ;

		_tagGridData() {}
		_tagGridData(QPoint mapGrid, QPoint imgGrid)
		{
			this->mapGrid = mapGrid ;
			this->imageGrid = imgGrid ;
		}
	} GridData ;

	typedef struct _tagImageData {
		QPoint			grid ;
		bool			bUnitable ;	// ユニット配置可能なら true
		bool			bThrough ;	// 敵通行可能なら true

		_tagImageData() {}
		_tagImageData(QPoint grid, bool bUnit, bool bThrough)
		{
			this->grid = grid ;
			this->bUnitable = bUnit ;
			this->bThrough = bThrough ;
		}
	} ImageData ;

	typedef struct _tagMapData {
		QString				mapName ;
		QImage				image ;
		QSize				imgGridSize, mapGridSize ;

		QList<GridData>		gridDatas ;
		QList<ImageData>	imgDatas ;

		CListModelTreasure	*pModelTreasure ;
		CListModelPoint		*pModelPoint ;

		_tagMapData()
		{
			pModelTreasure = NULL ;
			pModelPoint = NULL ;
		}

		// GridData -------------------------------------------------------
		void addGridData(QPoint mapGrid, QPoint imgGrid)
		{
			removeGridData(mapGrid) ;
			gridDatas.append(GridData(mapGrid, imgGrid)) ;
		}

		int getGridDataIndex(QPoint mapGrid)
		{
			for ( int i = 0 ; i < gridDatas.size() ; i ++ ) {
				if ( gridDatas[i].mapGrid == mapGrid ) { return i ; }
			}
			return -1 ;
		}

		GridData &getGridData(int index)
		{
			return gridDatas[index] ;
		}

		void removeGridData(QPoint mapGrid)
		{
			int i = getGridDataIndex(mapGrid) ;
			if ( i < 0 ) { return ; }
			gridDatas.removeAt(i) ;
		}

		// ImageData ------------------------------------------------------
		void addImageData(QPoint imgGrid, bool bUnit, bool bThrough)
		{
			removeImageData(imgGrid) ;
			imgDatas.append(ImageData(imgGrid, bUnit, bThrough)) ;
		}

		int getImageDataIndex(QPoint imgGrid)
		{
			for ( int i = 0 ; i < imgDatas.size() ; i ++ ) {
				if ( imgDatas[i].grid == imgGrid ) { return i ; }
			}
			return -1 ;
		}

		ImageData &getImageData(int index)
		{
			return imgDatas[index] ;
		}

		void removeImageData(QPoint imgGrid)
		{
			int i = getImageDataIndex(imgGrid) ;
			if ( i < 0 ) { return ; }
			imgDatas.removeAt(i) ;
		}
		// ----------------------------------------------------------------
	} MapData ;

    Q_OBJECT
public:
    explicit CListModelMap(QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const ;
	int rowCount(const QModelIndex &parent) const ;
	Qt::ItemFlags flags(const QModelIndex &index) const ;
	bool setData(const QModelIndex &index, const QVariant &value, int role) ;
	bool insertRows(int row, int count, const QModelIndex &parent) ;
	bool removeRows(int row, int count, const QModelIndex &parent) ;

	int addMap(QString str) ;
	bool removeMap(int row) ;

	MapData &getMap(int row) { return m_mapDatas[row] ; }

signals:

public slots:

private:
	QList<MapData>		m_mapDatas ;
} ;

#endif // CLISTMODELMAP_H

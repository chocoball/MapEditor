#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QtGui>
#include "include.h"

class CMapLabel ;

class CEditData
{
public:
	typedef struct _tagGridData {
		QPoint			mapGrid ;
		QPoint			imageGrid ;
		int				nTreasure ;

		_tagGridData() {}
		_tagGridData(QPoint mapGrid, QPoint imgGrid)
		{
			this->mapGrid = mapGrid ;
			this->imageGrid = imgGrid ;
			this->nTreasure = 0 ;
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

	typedef struct _tagTreasureData {
		QPoint			mapGrid ;
		int				num ;
		QStandardItem	*id ;

		_tagTreasureData()
		{
			num = 0 ;
			id = 0 ;
		}
		_tagTreasureData(QPoint grid, int num, QStandardItem *id)
		{
			this->mapGrid = grid ;
			this->num = num ;
			this->id = id ;
		}
	} TreasureData ;

	typedef struct _tagMapData {
		QImage				image ;
		QSize				mapGridSize, imgGridSize ;
		QList<GridData>		gridDatas ;
		QList<ImageData>	imgDatas ;
		QList<TreasureData>	treasureDatas ;

		_tagMapData() {}

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

		// TreasureData ---------------------------------------------------
		void addTreasureData(QPoint mapGrid, int num, QStandardItem *id)
		{
			removeTreasureData(mapGrid) ;
			treasureDatas.append(TreasureData(mapGrid, num, id)) ;
		}
		int getTreasureDataIndex(QStandardItem *id)
		{
			for ( int i = 0 ; i < treasureDatas.size() ; i ++ ) {
				if ( treasureDatas[i].id == id ) { return i ; }
			}
			return -1 ;
		}
		int getTreasureDataIndex(QPoint mapGrid)
		{
			for ( int i = 0 ; i < treasureDatas.size() ; i ++ ) {
				if ( treasureDatas[i].mapGrid == mapGrid ) { return i ; }
			}
			return -1 ;
		}
		TreasureData &getTreasureData(int index)
		{
			return treasureDatas[index] ;
		}
		void removeTreasureData(QPoint mapGrid)
		{
			int i = getTreasureDataIndex(mapGrid) ;
			if ( i < 0 ) { return ; }
			treasureDatas.removeAt(i) ;
		}
		void removeTreasureData(QStandardItem *id)
		{
			int i = getTreasureDataIndex(id) ;
			if ( i < 0 ) { return ; }
			treasureDatas.removeAt(i) ;
		}
		// ----------------------------------------------------------------

	} MapData ;

	typedef QStandardItem*				MapDataID ;
	typedef QPair<MapDataID, MapData>	MapDataGroup ;

private:
    CEditData() ;

public:
	static CEditData *getInstance()
	{
		static CEditData ins ;
		return &ins ;
	}

	void update() ;
	void updateMap() ;
	void updateImage() ;

	bool gridToPos(QPoint &ret, const QPoint &grid, const QSize &gridSize) ;
	bool posToGrid(QPoint &ret, const QPoint &pos, const QSize &gridSize) ;

	QStandardItemModel &getModel() { return m_model ; }

	int getMapDataIndex(MapDataID id)
	{
		for ( int i = 0 ; i < m_mapDataGroupList.size() ; i ++ ) {
			if ( m_mapDataGroupList[i].first == id ) { return i ; }
		}
		return -1 ;
	}
	MapData &getMapData(int index)
	{
		return m_mapDataGroupList[index].second ;
	}
	void delMapData(int index)
	{
		if ( index < 0 || index >= m_mapDataGroupList.size() ) { return ; }
		m_mapDataGroupList.removeAt(index) ;
	}

	QStandardItem *addMap(QString id) ;
	void delMap(QStandardItem *item) ;

	bool isSelectMap()
	{
		return getSelModelIndex().isValid() ;
	}
	bool isSelectTreasure()
	{
		return (getSelModelIndex().internalPointer() != m_model.invisibleRootItem()) ? true : false ;
	}

	MapData *getSelectMapData()
	{
		if ( !isSelectMap() ) { return NULL ; }

		QModelIndex modelIndex = getSelModelIndex() ;
		while ( modelIndex.internalPointer() != m_model.invisibleRootItem() ) { modelIndex = modelIndex.parent() ; }
		QStandardItem *p = m_model.itemFromIndex(modelIndex) ;
		if ( !p ) { return NULL ; }

		int index = getMapDataIndex(p) ;
		if ( index < 0 ) { return NULL ; }
		return &getMapData(index) ;
	}
	QStandardItem *getSelectMapItem()
	{
		QModelIndex modelIndex = getSelModelIndex() ;
		while ( modelIndex.internalPointer() != m_model.invisibleRootItem() ) { modelIndex = modelIndex.parent() ; }
		return m_model.itemFromIndex(modelIndex) ;
	}

	kAccessor(QPoint, m_selStartGrid, SelStartGrid)
	kAccessor(QPoint, m_selEndGrid, SelEndGrid)
	kAccessor(QLabel*, m_pImageLabel, ImageLabel)
	kAccessor(CMapLabel*, m_pMapLabel, MapLabel)
	kAccessor(QModelIndex, m_selectModelIndex, SelModelIndex)

private:
	QList<MapDataGroup>		m_mapDataGroupList ;
	QStandardItemModel		m_model ;
};

#define g_EditData CEditData::getInstance()


#endif // CEDITDATA_H

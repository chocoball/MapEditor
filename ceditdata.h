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

	typedef struct {
		QList<GridData>		gridDatas ;
		QList<ImageData>	imgDatas ;

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
			gridDatas.takeAt(i) ;
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
			imgDatas.takeAt(i) ;
		}
		// ----------------------------------------------------------------
	} EditData ;

	typedef QStandardItem*				EditDataID ;
	typedef QPair<EditDataID, EditData>	EditDataGroup ;
	typedef QList<EditDataGroup>		EditDataGroupList ;

private:
    CEditData() ;

public:
	static CEditData *getInstance()
	{
		static CEditData ins ;
		return &ins ;
	}

	QImage getImage() { return m_Image ; }
	QImage *getImagePtr() { return &m_Image ; }
	void setImage(QImage &image) { m_Image = image ; }

	void update() ;
	void updateMap() ;
	void updateImage() ;

	bool gridToPos(QPoint &ret, const QPoint &grid, const QSize &gridSize) ;
	bool posToGrid(QPoint &ret, const QPoint &pos, const QSize &gridSize) ;
	bool indexToGrid(QPoint &ret, int index, const QSize &gridSize) ;
	int gridToIndex(const QPoint grid, const QSize &gridSize) ;

	int getEditDataIndex(EditDataID id)
	{
		for ( int i = 0 ; i < m_editDataGroups.size() ; i ++ ) {
			EditDataGroup &group = m_editDataGroups[i] ;
			if ( group.first == id ) { return i ; }
		}
		return -1 ;
	}

	EditData &getEditData(int index)
	{
		return m_editDataGroups[index].second ;
	}

	kAccessor(QPoint, m_selStartGrid, SelStartGrid)
	kAccessor(QPoint, m_selEndGrid, SelEndGrid)
	kAccessor(QLabel*, m_pImageLabel, ImageLabel)
	kAccessor(CMapLabel*, m_pMapLabel, MapLabel)

private:
	QImage				m_Image ;

	EditDataGroupList	m_editDataGroups ;
};

#define g_EditData CEditData::getInstance()

#endif // CEDITDATA_H

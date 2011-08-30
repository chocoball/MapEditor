#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QtGui>
#include "include.h"

class CMapLabel ;

class CEditData
{
public:
	typedef struct {
		QPoint			mapGrid ;
		QPoint			imageGrid ;
		int				data ;
	} GridData ;

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

	void addGridData(QPoint mapGrid, QPoint imgGrid, int data)
	{
		removeGridData(mapGrid) ;

		GridData d ;
		d.mapGrid = mapGrid ;
		d.imageGrid = imgGrid ;
		d.data = data ;
		m_gridData.push_back(d) ;
	}
	int getGridDataIndex(QPoint mapGrid)
	{
		for ( int i = 0 ; i < m_gridData.size() ; i ++ ) {
			if ( m_gridData[i].mapGrid == mapGrid ) { return i ; }
		}
		return -1 ;
	}
	GridData &getGridData(int index)
	{
		return m_gridData[index] ;
	}

	void removeGridData(QPoint mapGrid)
	{
		int idx = getGridDataIndex(mapGrid) ;
		if ( idx >= 0 ) {
			m_gridData.takeAt(idx) ;
		}
	}

	kGetterSetter(QPoint, m_selStartGrid, SelStartGrid)
	kGetterSetter(QPoint, m_selEndGrid, SelEndGrid)
	kGetterSetter(QLabel*, m_pImageLabel, ImageLabel)
	kGetterSetter(CMapLabel*, m_pMapLabel, MapLabel)

private:
	QImage			m_Image ;

	QList<GridData>	m_gridData ;
};

#define g_EditData CEditData::getInstance()

#endif // CEDITDATA_H

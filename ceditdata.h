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

	typedef struct {
		QPoint			grid ;
		bool			bUnitable ;	// ユニット配置可能なら true
		bool			bThrough ;	// 敵通行可能なら true
	} ImageData ;

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

	// GridData -------------------------------------------------------
	void addGridData(QPoint mapGrid, QPoint imgGrid, int data) ;
	int getGridDataIndex(QPoint mapGrid) ;
	GridData &getGridData(int index) ;
	void removeGridData(QPoint mapGrid) ;
	// ----------------------------------------------------------------

	// ImageData ------------------------------------------------------
	void addImageData(QPoint imgGrid, bool bUnit, bool bThrough) ;
	int getImageDataIndex(QPoint imgGrid) ;
	ImageData &getImageData(int index) ;
	void removeImageData(QPoint imgGrid) ;
	// ----------------------------------------------------------------

	kGetterSetter(QPoint, m_selStartGrid, SelStartGrid)
	kGetterSetter(QPoint, m_selEndGrid, SelEndGrid)
	kGetterSetter(QLabel*, m_pImageLabel, ImageLabel)
	kGetterSetter(CMapLabel*, m_pMapLabel, MapLabel)

private:
	QImage				m_Image ;

	QList<GridData>		m_gridData ;
	QList<ImageData>	m_imgData ;
};

#define g_EditData CEditData::getInstance()

#endif // CEDITDATA_H

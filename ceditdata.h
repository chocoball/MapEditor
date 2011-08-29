#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QtGui>

class CEditData
{
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

	QPoint getSelStartGrid() { return m_selStartGrid ; }
	void setSelStartGrid(QPoint pos) { m_selStartGrid = pos ; }
	QPoint getSelEndGrid() { return m_selEndGrid ; }
	void setSelEndGrid(QPoint pos) { m_selEndGrid = pos ; }

	void setImageLabel(QLabel *p) { m_pImageLabel = p ; }

	void updateMap() ;
	void updateImage() ;

	bool gridToPos(QPoint &ret, const QPoint &grid) ;
	bool posToGrid(QPoint &ret, const QPoint &pos) ;
	bool indexToGrid(QPoint &ret, int index) ;
	int gridToIndex(const QPoint grid) ;

private:
	QImage		m_Image ;
	QLabel		*m_pImageLabel ;
	QList<int>	m_gridData ;
	QList<int>	m_selectGridIndeces ;
	QPoint		m_selStartGrid, m_selEndGrid ;
};

#define g_EditData CEditData::getInstance()

#endif // CEDITDATA_H

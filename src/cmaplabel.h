#ifndef CMAPLABEL_H
#define CMAPLABEL_H

#include <QLabel>
#include "cgridlabel.h"

typedef QPair<QPoint, QLabel *>	TipLabel ;

class CMapLabel : public QLabel
{
    Q_OBJECT
public:
	enum {
		kMapControll_None = 0,
		kMapControll_Add,
		kMapControll_Remove,
		kMapControll_AddMulti,
		kMapControll_RemoveMulti
	} ;

public:
	explicit CMapLabel(QWidget *parent = 0);

	void updateLabels() ;

	int getGridLabelIndex(QPoint grid) ;

	void addMapGrid(const QPoint mapGrid, const QPoint imgGrid) ;
	void addMapGrid(const QPoint mapPos, const QPoint imgStGrid, const QPoint imgEndGrid,
					const QSize mapGridSize, const QSize imgGridSize, CListModelMap::MapData &data) ;
	void addMapGrid(const QPoint mapPos, const QPoint mapGrid, QImage img) ;
	void removeMapGrid(const QPoint mapPos, const QPoint imgStGrid, const QPoint imgEndGrid,
					   const QSize mapGridSize, CListModelMap::MapData &data) ;

	void moveTreasureLabel(const int index, const QPoint pos) ;
	void movePointLabel(const int index, const QPoint pos) ;

	void releaseMapTipLabel(int index = -1)
	{
		releaseTipLabel(m_TipLabel, index) ;
	}

signals:

public slots:
	void slot_changeSelectGridRect() ;

	void slot_addTreasureLabel(int index) ;
	void slot_removeTreasureLabel(int index) ;

	void slot_addPointLabel(int index) ;
	void slot_removePointLabel(int index) ;

	void slot_mousePress(QMouseEvent *event) ;
	void slot_mouseMove(QMouseEvent *event) ;
	void slot_mouseRelease(QMouseEvent *event) ;

	void slot_keyPress(QKeyEvent*) ;
	void slot_keyRelease(QKeyEvent*) ;

protected:

private:
	int getGridLabelIndex(int x, int y)
	{
		return getGridLabelIndex(QPoint(x, y)) ;
	}

//	void addMapGrid(const QPoint basePos) ;

	void makeDropLabel(QPoint pos, QPoint gridSt, QPoint gridEnd) ;

	void releaseTipLabel(QList<TipLabel> &label, int index = -1) ;

	QLabel *makeLabel(QPoint grid, QSize size, QColor col) ;

private:
	bool				m_bPressCtrl ;
	int					m_nMapControllType ;
	bool				m_bMousePressLeft, m_bMousePressRight ;
	QList<TipLabel>		m_TipLabel ;
	QList<TipLabel>		m_TreasureLabel ;
	QList<TipLabel>		m_PointLabel ;

	QLabel				*m_pLayerMap ;
	QLabel				*m_pLayerMark ;
	QLabel				*m_pDropLabel ;
	CGridLabel			*m_pGridLabel ;
	QSize				m_oldDropSize ;
	QPoint				m_oldDropPos ;
};

#endif // CMAPLABEL_H

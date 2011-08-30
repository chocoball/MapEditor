#ifndef CMAPLABEL_H
#define CMAPLABEL_H

#include <QLabel>
#include "cgridlabel.h"

typedef QPair<QPoint, QLabel *>	GridLabel ;

class CMapLabel : public QLabel
{
    Q_OBJECT
public:
	explicit CMapLabel(QWidget *parent = 0);

	void updateLabels() ;
signals:

public slots:
	void slot_changeSelectGridRect() ;
	void slot_mousePress(QMouseEvent *event) ;
	void slot_mouseMove(QMouseEvent *event) ;
	void slot_mouseRelease(QMouseEvent *event) ;

protected:

private:
	int getGridLabelIndex(int x, int y)
	{
		return getGridLabelIndex(QPoint(x, y)) ;
	}

	int getGridLabelIndex(QPoint grid) ;

	void addMapGrid(const QPoint mapPos) ;
	void makeDropLabel(QPoint pos) ;

private:
	bool				m_bMousePress ;
	QList<GridLabel>	m_GridLabel ;
	QLabel				*m_pDropLabel ;
	CGridLabel			*m_pGridLabel ;
	QLabel				*m_pMapLabel ;
};

#endif // CMAPLABEL_H

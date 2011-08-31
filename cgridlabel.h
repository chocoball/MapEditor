#ifndef CGRIDLABEL_H
#define CGRIDLABEL_H

#include <QLabel>
#include "include.h"

class CGridLabel : public QLabel
{
    Q_OBJECT
public:
	explicit CGridLabel(QWidget *parent = 0);

	void updateGrid() ;

	kGetterSetter(QSize, m_gridSize, GridSize)
	kGetterSetter(QPoint, m_gridNum, GridNum)

signals:
	void sig_mousePress(QMouseEvent *) ;
	void sig_mouseMove(QMouseEvent *) ;
	void sig_mouseRelease(QMouseEvent *) ;

public slots:

protected:
	void mousePressEvent(QMouseEvent *ev) ;
	void mouseMoveEvent(QMouseEvent *ev) ;
	void mouseReleaseEvent(QMouseEvent *ev) ;
};

#endif // CGRIDLABEL_H

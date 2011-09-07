#ifndef CIMAGELABEL_H
#define CIMAGELABEL_H

#include <QLabel>
#include "cgridlabel.h"

class CImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CImageLabel(QWidget *parent = 0);
	~CImageLabel() ;

	void updateLabels() ;
	void changeSelectGridRect() ;

signals:
	void sig_changeSelectGridRect() ;

public slots:
	void slot_mousePress(QMouseEvent *ev) ;
	void slot_mouseMove(QMouseEvent *ev) ;
	void slot_mouseRelease(QMouseEvent *ev) ;

protected:

private:
	QPoint		m_selStart, m_selEnd ;
	QPoint		m_selGridSt, m_selGridEnd ;

	QLabel		*m_pSelectLabel ;
	CGridLabel	*m_pGridLabel ;
};

#endif // CIMAGELABEL_H

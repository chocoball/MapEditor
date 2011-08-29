#ifndef CIMAGELABEL_H
#define CIMAGELABEL_H

#include <QLabel>

class CImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CImageLabel(QWidget *parent = 0);
	~CImageLabel() ;

signals:

public slots:

protected:
	void mousePressEvent(QMouseEvent *ev) ;
	void mouseMoveEvent(QMouseEvent *ev) ;
	void mouseReleaseEvent(QMouseEvent *ev) ;

private:
	void startDrag(QPoint pos) ;

private:
	QPoint		m_selStart, m_selEnd ;
	QPoint		m_selGridSt, m_selGridEnd ;
};

#endif // CIMAGELABEL_H

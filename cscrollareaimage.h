#ifndef CSCROLLAREAIMAGE_H
#define CSCROLLAREAIMAGE_H

#include <QScrollArea>

class CScrollAreaImage : public QScrollArea
{
    Q_OBJECT
public:
	explicit CScrollAreaImage(QWidget *parent = 0);

signals:

public slots:

protected:
	void dragEnterEvent(QDragEnterEvent *event) ;
	void dropEvent(QDropEvent *event) ;
};

#endif // CSCROLLAREAIMAGE_H

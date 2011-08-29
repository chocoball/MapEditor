#ifndef CMAPLABEL_H
#define CMAPLABEL_H

#include <QLabel>

class CMapLabel : public QLabel
{
    Q_OBJECT
public:
	explicit CMapLabel(QWidget *parent = 0);

signals:

public slots:

protected:
	void dropEvent(QDropEvent *event) ;
};

#endif // CMAPLABEL_H

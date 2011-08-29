#include "cmaplabel.h"
#include "include.h"

CMapLabel::CMapLabel(QWidget *parent) :
    QLabel(parent)
{
	setAcceptDrops(true) ;
}

void CMapLabel::dropEvent(QDropEvent *event)
{
	qDebug() << event->mimeData()->formats() ;

	if ( event->mimeData()->hasFormat("editor/selected-image") ) {
		QPixmap pix ;
		QRect rect ;
		int scale ;
		QPoint pos ;
		int index ;

		QByteArray itemData = event->mimeData()->data("editor/selected-image");
		QDataStream stream( &itemData, QIODevice::ReadOnly ) ;
		stream >> rect >> scale >> index ;

		pos = event->pos() ;

		event->accept();
	}
	else {
		event->ignore();
	}
}

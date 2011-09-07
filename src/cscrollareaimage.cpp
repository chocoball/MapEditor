#include "cscrollareaimage.h"
#include "include.h"

CScrollAreaImage::CScrollAreaImage(QWidget *parent) :
	QScrollArea(parent)
{
	setAcceptDrops(true) ;
}

void CScrollAreaImage::dragEnterEvent(QDragEnterEvent *event)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) {
		event->ignore();
		return ;
	}
	event->accept();
}

void CScrollAreaImage::dropEvent(QDropEvent *event)
{
	CListModelMap::MapData *p = g_EditData->getSelectMapData() ;
	if ( !p ) {
		event->ignore();
		return ;
	}

	QList<QUrl> urls = event->mimeData()->urls() ;

	if ( urls.size() <= 0 ) { return ; }

	QString fileName = urls[0].toLocalFile() ;

	QImage image ;
	if ( !image.load(fileName) ) {
		QMessageBox::warning(this, trUtf8("エラー"), trUtf8("読み込みに失敗しました:%1").arg(fileName)) ;
		return ;
	}

	p->image = image ;
	p->imageName = fileName ;
	g_EditData->update();
}

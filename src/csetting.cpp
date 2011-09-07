#include "csetting.h"

#define kKeyFileOpenDir		"fileOpenDir"
#define kKeyFileSaveDir		"fileSaveDir"
#define kKeyWindowGeometry	"geometry"
#define kKeyWindowState		"state"
#define kKeyImageGridSize	"img_grid_size"
#define kKeyMapGridSize		"map_grid_size"


CSetting::CSetting()
{
	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("Global");
	m_fileOpenDir = settings.value(kKeyFileOpenDir, "./").toString() ;
	m_fileSaveDir = settings.value(kKeyFileSaveDir, "./").toString() ;
	m_imgGridSize = settings.value(kKeyImageGridSize, QSize(16, 16)).toSize() ;
	m_mapGridSize = settings.value(kKeyMapGridSize, QSize(90, 90)).toSize() ;
	settings.endGroup();

	settings.beginGroup("MainWindow") ;
	m_MainWindowGeometry	= settings.value(kKeyWindowGeometry).toByteArray() ;
	m_MainWindowState		= settings.value(kKeyWindowState).toByteArray() ;
	settings.endGroup();

	settings.beginGroup("SplitterMap") ;
	m_SplitterMapGeometry	= settings.value(kKeyWindowGeometry).toByteArray() ;
	m_SplitterMapState		= settings.value(kKeyWindowState).toByteArray() ;
	settings.endGroup();

	settings.beginGroup("LabelImage") ;
	m_LabelImageGeometry	= settings.value(kKeyWindowGeometry).toByteArray() ;
	m_LabelImageState		= settings.value(kKeyWindowState).toByteArray() ;
	settings.endGroup();

	settings.beginGroup("ScrollAreaMap") ;
	m_ScrollAreaMapGeometry	= settings.value(kKeyWindowGeometry).toByteArray() ;
	settings.endGroup();
}

CSetting::~CSetting()
{
}

void CSetting::writeSetting()
{
	qDebug() << "write settings" << qApp->applicationDirPath() << "/settnig.ini" ;

	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("Global");
	settings.setValue(kKeyFileOpenDir, m_fileOpenDir) ;
	settings.setValue(kKeyFileSaveDir, m_fileSaveDir) ;
	settings.setValue(kKeyImageGridSize, m_imgGridSize) ;
	settings.setValue(kKeyMapGridSize, m_mapGridSize) ;
	settings.endGroup();

	settings.beginGroup("MainWindow") ;
	settings.setValue(kKeyWindowGeometry, m_MainWindowGeometry) ;
	settings.setValue(kKeyWindowState, m_MainWindowState) ;
	settings.endGroup();

	settings.beginGroup("SplitterMap") ;
	settings.setValue(kKeyWindowGeometry, m_SplitterMapGeometry) ;
	settings.setValue(kKeyWindowState, m_SplitterMapState) ;
	settings.endGroup();

	settings.beginGroup("LabelImage") ;
	settings.setValue(kKeyWindowGeometry, m_LabelImageGeometry) ;
	settings.setValue(kKeyWindowState, m_LabelImageState) ;
	settings.endGroup();

	settings.beginGroup("ScrollAreaMap") ;
	settings.setValue(kKeyWindowGeometry, m_ScrollAreaMapGeometry) ;
	settings.endGroup();
}

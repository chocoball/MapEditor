#include "csetting.h"

#define kKeyFileOpenDir		"fileOpenDir"
#define kKeyFileSaveDir		"fileSaveDir"
#define kKeyJsonFileSaveDir	"jsonFileSaveDir"
#define kKeyWindowGeometry	"geometry"
#define kKeyWindowState		"state"
#define kKeyWindowSize		"size"
#define kKeyImageGridSize	"img_grid_size"
#define kKeyMapGridSize		"map_grid_size"
#define kKeyViewMode		"view_mode"


CSetting::CSetting()
{
	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("Global");
	m_fileOpenDir		= settings.value(kKeyFileOpenDir, "./").toString() ;
	m_fileSaveDir		= settings.value(kKeyFileSaveDir, "./").toString() ;
	m_jsonFileSaveDir	= settings.value(kKeyJsonFileSaveDir, "./").toString() ;
	m_imgGridSize		= settings.value(kKeyImageGridSize, QSize(16, 16)).toSize() ;
	m_mapGridSize		= settings.value(kKeyMapGridSize, QSize(90, 90)).toSize() ;
	m_viewMode			= settings.value(kKeyViewMode, 0).toInt() ;
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

	settings.beginGroup("FrameMap") ;
	m_frameMapSize = settings.value(kKeyWindowSize, QSize(406, 451)) ;
	settings.endGroup();
	settings.beginGroup("FrameTree") ;
	m_frameTreeSize = settings.value(kKeyWindowSize, QSize(121, 441)) ;
	settings.endGroup();
	settings.beginGroup("FrameImage") ;
	m_frameImageSize = settings.value(kKeyWindowSize) ;
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
	settings.setValue(kKeyJsonFileSaveDir, m_jsonFileSaveDir) ;
	settings.setValue(kKeyImageGridSize, m_imgGridSize) ;
	settings.setValue(kKeyMapGridSize, m_mapGridSize) ;
	settings.setValue(kKeyViewMode, m_viewMode) ;
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

	settings.beginGroup("FrameMap") ;
	settings.setValue(kKeyWindowSize, m_frameMapSize) ;
	settings.endGroup();

	settings.beginGroup("FrameTree") ;
	settings.setValue(kKeyWindowSize, m_frameTreeSize) ;
	settings.endGroup();

	settings.beginGroup("FrameImage") ;
	settings.setValue(kKeyWindowSize, m_frameImageSize) ;
	settings.endGroup();
}

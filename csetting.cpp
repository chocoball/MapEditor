#include "csetting.h"

#define kKeyFileOpenDir		"fileOpenDir"
#define kKeyFileSaveDir		"fileSaveDir"
#define kKeyWindowGeometry	"geometry"
#define kKeyWindowState		"state"
#define kKeyGridWidth		"grid_w"
#define kKeyGridHeight		"grid_h"


CSetting::CSetting()
{
	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("Global");
	m_fileOpenDir = settings.value(kKeyFileOpenDir, "./").toString() ;
	m_fileSaveDir = settings.value(kKeyFileSaveDir, "./").toString() ;
	m_gridW = settings.value(kKeyGridWidth, 16).toInt() ;
	m_gridH = settings.value(kKeyGridHeight, 16).toInt() ;
	settings.endGroup();

	settings.beginGroup("MainWindow") ;
	m_MainWindowGeometry	= settings.value(kKeyWindowGeometry).toByteArray() ;
	m_MainWindowState		= settings.value(kKeyWindowState).toByteArray() ;
	settings.endGroup();

	settings.beginGroup("SplitterMap") ;
	m_SplitterMapGeometry	= settings.value(kKeyWindowGeometry).toByteArray() ;
	m_SplitterMapState		= settings.value(kKeyWindowState).toByteArray() ;
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
	settings.setValue(kKeyGridWidth, m_gridW) ;
	settings.setValue(kKeyGridHeight, m_gridH) ;
	settings.endGroup();

	settings.beginGroup("MainWindow") ;
	settings.setValue(kKeyWindowGeometry, m_MainWindowGeometry) ;
	settings.setValue(kKeyWindowState, m_MainWindowState) ;
	settings.endGroup();

	settings.beginGroup("SplitterMap") ;
	settings.setValue(kKeyWindowGeometry, m_SplitterMapGeometry) ;
	settings.setValue(kKeyWindowState, m_SplitterMapState) ;
	settings.endGroup();
}

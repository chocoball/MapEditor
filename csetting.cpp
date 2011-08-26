#include "csetting.h"

#define kKeyFileOpen	"fileOpenDir"


CSetting::CSetting()
{
	QSettings settings(qApp->applicationDirPath() + "/settnig.ini", QSettings::IniFormat) ;

	settings.beginGroup("Global");
	m_fileOpenDir = settings.value(kKeyFileOpen, "./").toString() ;
	settings.endGroup();
}

CSetting::~CSetting()
{
	QSettings settings(qApp->applicationDirPath() + "/settnig.ini", QSettings::IniFormat) ;
	settings.beginGroup("Global");
	settings.setValue(kKeyFileOpen, m_fileOpenDir) ;
	settings.endGroup();
}

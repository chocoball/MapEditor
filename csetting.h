#ifndef CSETTING_H
#define CSETTING_H

#include <QtGui>
#include "include.h"

class CSetting
{
private:
	CSetting();
	~CSetting() ;

public:
	static CSetting *getInstance()
	{
		static CSetting ins ;
		return &ins ;
	}

	void writeSetting() ;

	QString getFilePath() { return QString(qApp->applicationDirPath() + "/settnig.ini") ; }
	QSettings::Format getFileFormat() { return QSettings::IniFormat ; }

	kGetterSetter(QString, m_fileOpenDir, FileOpenDir)
	kGetterSetter(QString, m_fileSaveDir, FileSaveDir)
	kGetterSetter(QByteArray, m_MainWindowGeometry, MainWindowGeometry)
	kGetterSetter(QByteArray, m_MainWindowState, MainWindowState)
	kGetterSetter(QByteArray, m_SplitterMapGeometry, SplitterMapGeometry)
	kGetterSetter(QByteArray, m_SplitterMapState, SplitterMapState)
	kGetterSetter(QByteArray, m_LabelImageGeometry, LabelImageGeometry)
	kGetterSetter(QByteArray, m_LabelImageState, LabelImageState)
	kGetterSetter(QByteArray, m_ScrollAreaMapGeometry, ScrollAreaMapGeometry)

	kGetterSetter(QSize, m_imgGridSize, ImageGridSize)
	kGetterSetter(QSize, m_mapGridSize, MapGridSize)
};

#define g_Setting CSetting::getInstance()

#endif // CSETTING_H

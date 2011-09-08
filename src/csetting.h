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

	kAccessor(QString, m_fileOpenDir, FileOpenDir)
	kAccessor(QString, m_fileSaveDir, FileSaveDir)
	kAccessor(QByteArray, m_MainWindowGeometry, MainWindowGeometry)
	kAccessor(QByteArray, m_MainWindowState, MainWindowState)
	kAccessor(QByteArray, m_SplitterMapGeometry, SplitterMapGeometry)
	kAccessor(QByteArray, m_SplitterMapState, SplitterMapState)
	kAccessor(QByteArray, m_LabelImageGeometry, LabelImageGeometry)
	kAccessor(QByteArray, m_LabelImageState, LabelImageState)
	kAccessor(QByteArray, m_ScrollAreaMapGeometry, ScrollAreaMapGeometry)

	kAccessor(QSize, m_imgGridSize, ImageGridSize)
	kAccessor(QSize, m_mapGridSize, MapGridSize)

	kAccessor(int, m_viewMode, ViewMode)
};

#define g_Setting CSetting::getInstance()

#endif // CSETTING_H

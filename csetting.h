#ifndef CSETTING_H
#define CSETTING_H

#include <QtGui>

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

	QString getFileOpenDir()					{ return m_fileOpenDir ; }
	void setFileOpenDir(QString &str)			{ m_fileOpenDir = str ; }

	QString getFileSaveDir()					{ return m_fileSaveDir ; }
	void setFileSaveDir(QString &str)			{ m_fileSaveDir = str ; }

	QByteArray getMainWindowGeometry()			{ return m_MainWindowGeometry ; }
	void setMainWindowGeometry(QByteArray val)	{ m_MainWindowGeometry = val ; }
	QByteArray getMainWindowState()				{ return m_MainWindowState ; }
	void setMainWindowState(QByteArray val)		{ m_MainWindowState = val ; }

	int getGridW()								{ return m_gridW ; }
	void setGridW(int w)						{ m_gridW = w ; }
	int getGridH()								{ return m_gridH ; }
	void setGridH(int h)						{ m_gridH = h ; }

	QString getFilePath() { return QString(qApp->applicationDirPath() + "/settnig.ini") ; }
	QSettings::Format getFileFormat() { return QSettings::IniFormat ; }

private:
	QString		m_fileOpenDir ;
	QString		m_fileSaveDir ;
	QByteArray	m_MainWindowGeometry ;
	QByteArray	m_MainWindowState ;
	int			m_gridW, m_gridH ;
};

#define g_Setting CSetting::getInstance()

#endif // CSETTING_H

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

	QString getFileOpenDir() { return m_fileOpenDir ; }
	void setFileOpenDir(QString str) { m_fileOpenDir = str ; }

private:
	QString		m_fileOpenDir ;
};

#define g_Setting CSetting::getInstance()

#endif // CSETTING_H

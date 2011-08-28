#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QtGui>

class CEditData
{
private:
    CEditData() ;

public:
	static CEditData *getInstance()
	{
		static CEditData ins ;
		return &ins ;
	}

	QImage getImage() { return m_Image ; }
	void setImage(QImage &image) { m_Image = image ; }

	void setImageLabel(QLabel *p) { m_pImageLabel = p ; }

	void updateMap() ;
	void updateImage() ;

private:
	QImage		m_Image ;
	QLabel		*m_pImageLabel ;
	QList<int>	m_selectGridIndeces ;
};

#define g_EditData CEditData::getInstance()

#endif // CEDITDATA_H

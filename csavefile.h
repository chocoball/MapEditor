#ifndef CSAVEFILE_H
#define CSAVEFILE_H

#include <QString>

class CSaveFile
{
public:
    CSaveFile();

	virtual bool write(QString &fileName) { return false ; }
	virtual bool read(QString &fileName) { return false ; }
};

class CSaveFileJson : public CSaveFile
{
public:
	CSaveFileJson() ;

	bool write(QString &fileName) ;
	bool read(QString &fileName) ;
} ;

#endif // CSAVEFILE_H

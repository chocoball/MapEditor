#include "csavefile.h"
#include "include.h"

/*****************************************
  基底クラス
*****************************************/
CSaveFile::CSaveFile()
{
}

/*****************************************
  JSON 吐き出しクラス
*****************************************/
CSaveFileJson::CSaveFileJson()
{
}

bool CSaveFileJson::write(QString &fileName)
{
	return true ;
}

bool CSaveFileJson::read(QString &fileName)
{
	return true ;
}

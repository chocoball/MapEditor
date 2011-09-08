#ifndef CSAVEFILE_H
#define CSAVEFILE_H

#include <QString>
#include <QDomDocument>
#include "clistmodelmap.h"

// base class
class CSaveFile
{
public:
    CSaveFile();

	virtual bool write(QString &fileName) = 0 ;
	virtual bool read(QString &fileName) = 0 ;
};

// json 吐き出し class
class CSaveFileJson : public CSaveFile
{
public:
	CSaveFileJson() ;

	bool write(QString &fileName) ;
	bool read(QString &fileName) ;

private:
	bool toStringMap(QString &str) ;
	bool toStringGridImage(QString &str, const CListModelMap::MapData &data) ;
	bool toStringGridData(QString &str, const CListModelMap::MapData &data) ;
	bool toStringTreasure(QString &str, const CListModelMap::MapData &data) ;
	bool toStringPoint(QString &str, const CListModelMap::MapData &data) ;
} ;

// xml 吐き出し class
class CSaveFileXml : public CSaveFile
{
public:
	CSaveFileXml() ;

	bool write(QString &fileName) ;
	bool read(QString &fileName) ;

private:
	bool makeXml_Header(QDomDocument &doc, QDomElement &root) ;
	bool makeXml_Map(QDomDocument &doc, QDomElement &root) ;
	bool makeXml_MapData(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData) ;
	bool makeXml_Image(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData) ;
	bool makeXml_Treasure(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData) ;
	bool makeXml_Point(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData) ;

	bool readMap(QDomElement &root, int mapNum) ;
	bool readMapData(CListModelMap::MapData &mapData, QDomNode node) ;
	bool readImageData(CListModelMap::MapData &mapData, QDomNode node) ;
	bool readTreasure(CListModelMap::MapData &mapData, QDomNode node) ;
	bool readPoint(CListModelMap::MapData &mapData, QDomNode node) ;

private:
	static const char *kId_Header ;
	static const char *kId_Root ;
	static const char *kId_Map ;
	static const char *kId_MapGridSize ;
	static const char *kId_ImageGridSize ;
	static const char *kId_MapData ;
	static const char *kId_GridData ;
	static const char *kId_MapGrid ;
	static const char *kId_ImageGrid ;
	static const char *kId_ImageData ;
	static const char *kId_Treasure ;
	static const char *kId_Point ;
	static const char *kId_TreasureData ;
	static const char *kId_PointData ;

	static const char *kAttr_Version ;
	static const char *kAttr_MapNum ;
	static const char *kAttr_MapName ;
	static const char *kAttr_ImageName ;
	static const char *kAttr_X ;
	static const char *kAttr_Y ;
	static const char *kAttr_W ;
	static const char *kAttr_H ;
	static const char *kAttr_Num ;
	static const char *kAttr_Unit ;
	static const char *kAttr_Through ;
	static const char *kAttr_Kind ;

	static const char *kXml_Version ;
} ;

#endif // CSAVEFILE_H

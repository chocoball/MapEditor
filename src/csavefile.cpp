#include <QRegExp>
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
	QString str ;

	if ( !toStringMap(str) ) { return false ; }

//	str = str.replace(QRegExp("(\\s|\\t|\\n)"), "") ;

	QFile file(fileName) ;
	if ( !file.open(QFile::WriteOnly) ) {
		return false ;
	}
	file.write(str.toAscii()) ;
	file.close() ;
	return true ;
}

bool CSaveFileJson::read(QString &fileName)
{
	return true ;
}

bool CSaveFileJson::toStringMap(QString &str)
{
	str += "[\n" ;
	QList<CListModelMap::MapData> &mapDatas = g_EditData->getModelMap()->getList() ;
	for ( int i = 0 ; i < mapDatas.size() ; i ++ ) {
		CListModelMap::MapData &data = mapDatas[i] ;
		QSize gridNum = data.getMaxGridNum() ;

		str += "\t{\n" ;
		str += "\t\t\"id\": " + data.mapName + ",\n" ;
		str += QString("\t\t\"grid_num\": [%1, %2],\n").arg(gridNum.width()).arg(gridNum.height()) ;
		str += QString("\t\t\"map_grid_size\": [%1, %2],\n").arg(data.mapGridSize.width()).arg(data.mapGridSize.height()) ;
		str += QString("\t\t\"img_grid_size\": [%1, %2],\n").arg(data.imgGridSize.width()).arg(data.imgGridSize.height()) ;
		str += QString("\t\t\"image_name\": \"%1.png\",\n").arg(QFileInfo(data.imageName).baseName()) ;
#if 1
		str += "\t\t\"grid\": [\n" ;
		for ( int y = 0 ; y < gridNum.height() ; y ++ ) {
			for ( int x = 0 ; x < gridNum.width() ; x ++ ) {
				int index = data.getGridDataIndex(QPoint(x, y)) ;
				if ( index >= 0 ) {
					CListModelMap::GridData &grid = data.getGridData(index) ;
					int bUnit = 0 ;
					int bThrough = 0 ;
					index = data.getImageDataIndex(grid.imageGrid) ;
					if ( index >= 0 ) {
						bUnit = data.getImageData(index).bUnitable ;
						bThrough = data.getImageData(index).bThrough ;
					}

					str += "\t\t\t{ " ;
					str += QString("\"img_grid\": [%1, %2]").arg(grid.imageGrid.x()).arg(grid.imageGrid.y()) ;
					if ( bUnit ) {
						str += ", \"bUnit\": 1" ;
					}
					if ( bThrough ) {
						str += ", \"bThrough\": 1" ;
					}
					index = data.pModelPoint->getPointIndex(grid.mapGrid) ;
					if ( index >= 0 ) {
						str += QString(", \"kind\": %1").arg(data.pModelPoint->getPoint(index).kind+1) ;
					}
					index = data.pModelTreasure->getTreasureIndex(grid.mapGrid) ;
					if ( index >= 0 ) {
						str += QString(", \"treasure\": %1").arg(data.pModelTreasure->getTreasure(index).num) ;
					}

					str += " }" ;
				}
				else {
					str += "\t\t\t{ \"img_grid\": [0, 0] }" ;
				}
				if ( y != gridNum.height()-1 || x != gridNum.width()-1 ) {
					str += "," ;
				}
				str += "\n" ;
			}
		}
		str += "\t\t]\n" ;
#else
		if ( !toStringGridImage(str, data) ) { return false ; }
		if ( !toStringGridData(str, data) ) { return false ; }
		if ( !toStringTreasure(str, data) ) { return false ; }
		if ( !toStringPoint(str, data) ) { return false ; }
#endif
		str += "\t}" ;
		if ( i < mapDatas.size()-1 ) {
			str += "," ;
		}
		str += "\n" ;
	}
	str += "]\n" ;
	return true ;
}

bool CSaveFileJson::toStringGridImage(QString &str, const CListModelMap::MapData &data)
{
	str += "\t\t\"grid_image\": [\n" ;

	str += "\t\t],\n" ;
	return true ;
}

bool CSaveFileJson::toStringGridData(QString &str, const CListModelMap::MapData &data)
{
	return true ;
}

bool CSaveFileJson::toStringTreasure(QString &str, const CListModelMap::MapData &data)
{
	return true ;
}

bool CSaveFileJson::toStringPoint(QString &str, const CListModelMap::MapData &data)
{
	return true ;
}



/*****************************************
  XML 吐き出しクラス
*****************************************/
const char *CSaveFileXml::kId_Header		= "MAP_XML" ;
const char *CSaveFileXml::kId_Root			= "MAP_ROOT" ;
const char *CSaveFileXml::kId_Map			= "MAP" ;
const char *CSaveFileXml::kId_MapGridSize	= "MAPGRID_SIZE" ;
const char *CSaveFileXml::kId_ImageGridSize	= "IMAGEGRID_SIZE" ;
const char *CSaveFileXml::kId_MapData		= "MAP_DATA" ;
const char *CSaveFileXml::kId_GridData		= "GRID_DATA" ;
const char *CSaveFileXml::kId_MapGrid		= "MAP_GRID" ;
const char *CSaveFileXml::kId_ImageGrid		= "IMAGE_GRID" ;
const char *CSaveFileXml::kId_ImageData		= "IMAGE_DATA" ;
const char *CSaveFileXml::kId_Treasure		= "TREASURE" ;
const char *CSaveFileXml::kId_Point			= "POINT" ;
const char *CSaveFileXml::kId_TreasureData	= "TREASUREDATA" ;
const char *CSaveFileXml::kId_PointData		= "POINTDATA" ;

const char *CSaveFileXml::kAttr_Version		= "MAP_VERSION" ;
const char *CSaveFileXml::kAttr_MapNum		= "MAP_NUM" ;
const char *CSaveFileXml::kAttr_MapName		= "MAP_NAME" ;
const char *CSaveFileXml::kAttr_ImageName	= "IMAGE_NAME" ;
const char *CSaveFileXml::kAttr_X			= "X" ;
const char *CSaveFileXml::kAttr_Y			= "Y" ;
const char *CSaveFileXml::kAttr_W			= "W" ;
const char *CSaveFileXml::kAttr_H			= "H" ;
const char *CSaveFileXml::kAttr_Num			= "NUM" ;
const char *CSaveFileXml::kAttr_Unit		= "UNIT" ;
const char *CSaveFileXml::kAttr_Through		= "THTOUGH" ;
const char *CSaveFileXml::kAttr_Kind		= "KIND" ;

const char *CSaveFileXml::kXml_Version		= "0x00000001" ;


CSaveFileXml::CSaveFileXml()
{
}

bool CSaveFileXml::write(QString &fileName)
{
	QDomDocument doc(kId_Header) ;
	QDomElement root = doc.createElement(kId_Root) ;

	if ( !makeXml_Header(doc, root) ) { return false ; }
	if ( !makeXml_Map(doc, root) ) { return false ; }

	doc.appendChild(root) ;

	QFile file(fileName) ;
	if ( !file.open(QFile::WriteOnly) ) {
		return false ;
	}
	file.write(doc.toString(4).toAscii()) ;
	file.close() ;
	return true ;
}

bool CSaveFileXml::read(QString &fileName)
{
	QDomDocument doc ;
	QFile file(fileName) ;
	if ( !file.open(QFile::ReadOnly) ) {
		return false ;
	}
	doc.setContent(&file) ;

	if ( doc.doctype().name() != kId_Header ) { return false ; }

	QDomElement root = doc.documentElement() ;
	if ( root.isNull() ) { return false ; }
	if ( root.nodeName() != kId_Root ) { return false ; }

	QDomNamedNodeMap nodeMap = root.attributes() ;
	if ( nodeMap.isEmpty() ) { return false ; }
	int mapNum = nodeMap.namedItem(kAttr_MapNum).toAttr().value().toInt() ;

	if ( !readMap(root, mapNum) ) { return false ; }

	return true ;
}

bool CSaveFileXml::makeXml_Header(QDomDocument &doc, QDomElement &root)
{
Q_UNUSED(doc)
	root.setAttribute(kAttr_Version, kXml_Version) ;
	root.setAttribute(kAttr_MapNum, g_EditData->getModelMap()->getList().size()) ;
	return true ;
}

bool CSaveFileXml::makeXml_Map(QDomDocument &doc, QDomElement &root)
{
	QList<CListModelMap::MapData> &mapDatas = g_EditData->getModelMap()->getList() ;

	for ( int i = 0 ; i < mapDatas.size() ; i ++ ) {
		const CListModelMap::MapData &mapData = mapDatas.at(i) ;

		QDomElement elmMap = doc.createElement(kId_Map) ;
		elmMap.setAttribute(kAttr_MapName, mapData.mapName) ;
		elmMap.setAttribute(kAttr_ImageName, mapData.imageName) ;
		root.appendChild(elmMap) ;

		QDomElement elmMapGrid = doc.createElement(kId_MapGridSize) ;
		QDomElement elmImageGrid = doc.createElement(kId_ImageGridSize) ;
		elmMap.appendChild(elmMapGrid) ;
		elmMap.appendChild(elmImageGrid) ;

		elmMapGrid.setAttribute(kAttr_W, mapData.mapGridSize.width());
		elmMapGrid.setAttribute(kAttr_H, mapData.mapGridSize.height());
		elmImageGrid.setAttribute(kAttr_W, mapData.imgGridSize.width());
		elmImageGrid.setAttribute(kAttr_H, mapData.imgGridSize.height());

		if ( !makeXml_MapData(doc, elmMap, mapData) ) { return false ; }
		if ( !makeXml_Image(doc, elmMap, mapData) ) { return false ; }
		if ( !makeXml_Treasure(doc, elmMap, mapData) ) { return false ; }
		if ( !makeXml_Point(doc, elmMap, mapData) ) { return false ; }
	}

	return true ;
}

bool CSaveFileXml::makeXml_MapData(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData)
{
	QDomElement elmMapData = doc.createElement(kId_MapData) ;
	root.appendChild(elmMapData) ;

	const QList<CListModelMap::GridData> &gridDatas = mapData.gridDatas ;
	elmMapData.setAttribute(kAttr_Num, gridDatas.size()) ;
	for ( int i = 0 ; i < gridDatas.size() ; i ++ ) {
		const CListModelMap::GridData gridData = gridDatas.at(i) ;

		QDomElement elmGridData = doc.createElement(kId_GridData) ;

		QDomElement elmMapGrid = doc.createElement(kId_MapGrid) ;
		elmMapGrid.setAttribute(kAttr_X, gridData.mapGrid.x()) ;
		elmMapGrid.setAttribute(kAttr_Y, gridData.mapGrid.y()) ;
		QDomElement elmImageGrid = doc.createElement(kId_ImageGrid) ;
		elmImageGrid.setAttribute(kAttr_X, gridData.imageGrid.x()) ;
		elmImageGrid.setAttribute(kAttr_Y, gridData.imageGrid.y()) ;

		elmGridData.appendChild(elmMapGrid) ;
		elmGridData.appendChild(elmImageGrid) ;
		elmMapData.appendChild(elmGridData) ;
	}
	return true ;
}

bool CSaveFileXml::makeXml_Image(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData)
{
	QDomElement elmImageData = doc.createElement(kId_ImageData) ;
	root.appendChild(elmImageData) ;

	const QList<CListModelMap::ImageData> &imgDatas = mapData.imgDatas ;
	elmImageData.setAttribute(kAttr_Num, imgDatas.size());
	for ( int i = 0 ; i < imgDatas.size() ; i ++ ) {
		const CListModelMap::ImageData &imgData = imgDatas.at(i) ;
		QDomElement elmGrid = doc.createElement(kId_ImageGrid) ;
		elmGrid.setAttribute(kAttr_X, imgData.grid.x()) ;
		elmGrid.setAttribute(kAttr_Y, imgData.grid.y()) ;
		elmGrid.setAttribute(kAttr_Unit, imgData.bUnitable) ;
		elmGrid.setAttribute(kAttr_Through, imgData.bThrough) ;

		elmImageData.appendChild(elmGrid) ;
	}

	return true ;
}

bool CSaveFileXml::makeXml_Treasure(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData)
{
	QDomElement elmTreasure = doc.createElement(kId_Treasure) ;
	root.appendChild(elmTreasure) ;

	const QList<CListModelTreasure::TreasureData> &treasureDatas = mapData.pModelTreasure->getList() ;
	elmTreasure.setAttribute(kAttr_Num, treasureDatas.size()) ;
	for ( int i = 0 ; i < treasureDatas.size() ; i ++ ) {
		const CListModelTreasure::TreasureData &data = treasureDatas.at(i) ;

		QDomElement elmData = doc.createElement(kId_TreasureData) ;
		elmData.setAttribute(kAttr_X, data.mapGrid.x()) ;
		elmData.setAttribute(kAttr_Y, data.mapGrid.y()) ;
		elmData.setAttribute(kAttr_Num, data.num) ;

		elmTreasure.appendChild(elmData) ;
	}
	return true ;
}

bool CSaveFileXml::makeXml_Point(QDomDocument &doc, QDomElement &root, const CListModelMap::MapData &mapData)
{
	QDomElement elmPoint = doc.createElement(kId_Point) ;
	root.appendChild(elmPoint) ;

	const QList<CListModelPoint::PointData> &pointDatas = mapData.pModelPoint->getList() ;
	elmPoint.setAttribute(kAttr_Num, pointDatas.size()) ;
	for ( int i = 0 ; i < pointDatas.size() ; i ++ ) {
		const CListModelPoint::PointData data = pointDatas.at(i) ;

		QDomElement elmData = doc.createElement(kId_PointData) ;
		elmData.setAttribute(kAttr_X, data.mapGrid.x()) ;
		elmData.setAttribute(kAttr_Y, data.mapGrid.y()) ;
		elmData.setAttribute(kAttr_Kind, data.kind) ;

		elmPoint.appendChild(elmData) ;
	}

	return true ;
}

bool CSaveFileXml::readMap(QDomElement &root, int mapNum)
{
	QDomNode n = root.firstChild() ;
	for ( int i = 0 ; i < mapNum ; i ++ ) {
		if ( n.isNull() ) { return false ; }

		QDomNamedNodeMap nodeMap = n.attributes() ;
		if ( nodeMap.namedItem(kAttr_MapName).isNull()
		  || nodeMap.namedItem(kAttr_ImageName).isNull() ) {
			return false ;
		}

		int row = g_EditData->getModelMap()->addMap(nodeMap.namedItem(kAttr_MapName).toAttr().value()) ;
		if ( row < 0 ) { return false ; }

		CListModelMap::MapData &mapData = g_EditData->getModelMap()->getMap(row) ;

		mapData.imageName = nodeMap.namedItem(kAttr_ImageName).toAttr().value() ;
		if ( !mapData.imageName.isEmpty() ) {
			if ( !mapData.image.load(mapData.imageName) ) { return false ; }
		}

		QDomNode node = n.namedItem(kId_MapGridSize) ;
		if ( node.isNull() ) { return false ; }
		mapData.mapGridSize.setWidth(node.attributes().namedItem(kAttr_W).toAttr().value().toInt()) ;
		mapData.mapGridSize.setHeight(node.attributes().namedItem(kAttr_H).toAttr().value().toInt()) ;
		node = n.namedItem(kId_ImageGridSize) ;
		if ( node.isNull() ) { return false ; }
		mapData.imgGridSize.setWidth(node.attributes().namedItem(kAttr_W).toAttr().value().toInt()) ;
		mapData.imgGridSize.setHeight(node.attributes().namedItem(kAttr_H).toAttr().value().toInt()) ;

		if ( !readMapData(mapData, n.namedItem(kId_MapData)) ) { return false ; }
		if ( !readImageData(mapData, n.namedItem(kId_ImageData)) ) { return false ; }
		if ( !readTreasure(mapData, n.namedItem(kId_Treasure)) ) { return false ; }
		if ( !readPoint(mapData, n.namedItem(kId_Point)) ) { return false ; }

		n = n.nextSibling() ;
	}
	return true ;
}

bool CSaveFileXml::readMapData(CListModelMap::MapData &mapData, QDomNode node)
{
	if ( node.isNull() ) { return false ; }

	int mapDataNum = node.attributes().namedItem(kAttr_Num).toAttr().value().toInt() ;

	node = node.firstChild() ;
	for ( int i = 0 ; i < mapDataNum ; i ++ ) {
		if ( node.isNull() ) { return false ; }

		QDomNode n ;
		QPoint mapGrid, imgGrid ;
		QDomNamedNodeMap attrs ;

		n = node.namedItem(kId_MapGrid) ;
		if ( n.isNull() ) { return false ; }
		attrs = n.attributes() ;
		mapGrid.setX(attrs.namedItem(kAttr_X).toAttr().value().toInt()) ;
		mapGrid.setY(attrs.namedItem(kAttr_Y).toAttr().value().toInt()) ;

		n = node.namedItem(kId_ImageGrid) ;
		if ( n.isNull() ) { return false ; }
		attrs = n.attributes() ;
		imgGrid.setX(attrs.namedItem(kAttr_X).toAttr().value().toInt()) ;
		imgGrid.setY(attrs.namedItem(kAttr_Y).toAttr().value().toInt()) ;

		mapData.addGridData(mapGrid, imgGrid) ;

		node = node.nextSibling() ;
	}
	return true ;
}

bool CSaveFileXml::readImageData(CListModelMap::MapData &mapData, QDomNode node)
{
	if ( node.isNull() ) { return false ; }

	int imgDataNum = node.attributes().namedItem(kAttr_Num).toAttr().value().toInt() ;

	node = node.firstChild() ;
	for ( int i = 0 ; i < imgDataNum ; i ++ ) {
		if ( node.isNull() ) { return false ; }

		QPoint grid ;
		bool bUnit, bThrough ;
		QDomNamedNodeMap attrs = node.attributes() ;
		if ( attrs.isEmpty() ) { return false ; }

		if ( attrs.namedItem(kAttr_X).toAttr().isNull() ) { return false ; }
		if ( attrs.namedItem(kAttr_Y).toAttr().isNull() ) { return false ; }
		if ( attrs.namedItem(kAttr_Unit).toAttr().isNull() ) { return false ; }
		if ( attrs.namedItem(kAttr_Through).toAttr().isNull() ) { return false ; }

		grid.setX(attrs.namedItem(kAttr_X).toAttr().value().toInt()) ;
		grid.setY(attrs.namedItem(kAttr_Y).toAttr().value().toInt()) ;
		bUnit = attrs.namedItem(kAttr_Unit).toAttr().value().toInt() ;
		bThrough = attrs.namedItem(kAttr_Through).toAttr().value().toInt() ;

		mapData.addImageData(grid, bUnit, bThrough) ;

		node = node.nextSibling() ;
	}
	return true ;
}

bool CSaveFileXml::readTreasure(CListModelMap::MapData &mapData, QDomNode node)
{
	if ( node.isNull() ) { return false ; }

	int treasureNum = node.attributes().namedItem(kAttr_Num).toAttr().value().toInt() ;

	node = node.firstChild() ;
	for ( int i = 0 ; i < treasureNum ; i ++ ) {
		if ( node.isNull() ) { return false ; }

		QPoint grid ;
		int num ;
		QDomNamedNodeMap attrs = node.attributes() ;
		if ( attrs.isEmpty() ) { return false ; }

		if ( attrs.namedItem(kAttr_X).toAttr().isNull() ) { return false ; }
		if ( attrs.namedItem(kAttr_Y).toAttr().isNull() ) { return false ; }
		if ( attrs.namedItem(kAttr_Num).toAttr().isNull() ) { return false ; }

		grid.setX(attrs.namedItem(kAttr_X).toAttr().value().toInt()) ;
		grid.setY(attrs.namedItem(kAttr_Y).toAttr().value().toInt()) ;
		num = attrs.namedItem(kAttr_Num).toAttr().value().toInt() ;

		int row = mapData.pModelTreasure->addTreasure(grid, num) ;
		if ( row < 0 ) { return false ; }

		node = node.nextSibling() ;
	}
	return true ;
}

bool CSaveFileXml::readPoint(CListModelMap::MapData &mapData, QDomNode node)
{
	if ( node.isNull() ) { return false ; }

	int pointNum = node.attributes().namedItem(kAttr_Num).toAttr().value().toInt() ;

	node = node.firstChild() ;
	for ( int i = 0 ; i < pointNum ; i ++ ) {
		if ( node.isNull() ) { return false ; }

		QPoint grid ;
		int kind ;
		QDomNamedNodeMap attrs = node.attributes() ;
		if ( attrs.isEmpty() ) { return false ; }

		if ( attrs.namedItem(kAttr_X).toAttr().isNull() ) { return false ; }
		if ( attrs.namedItem(kAttr_Y).toAttr().isNull() ) { return false ; }
		if ( attrs.namedItem(kAttr_Kind).toAttr().isNull() ) { return false ; }

		grid.setX(attrs.namedItem(kAttr_X).toAttr().value().toInt()) ;
		grid.setY(attrs.namedItem(kAttr_Y).toAttr().value().toInt()) ;
		kind = attrs.namedItem(kAttr_Kind).toAttr().value().toInt() ;

		int row = mapData.pModelPoint->addPoint(grid, kind) ;
		if ( row < 0 ) { return false ; }

		node = node.nextSibling() ;
	}
	return true ;
}

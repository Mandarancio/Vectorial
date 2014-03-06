/*
 * NSVGParser.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef NSVGPARSER_H_
#define NSVGPARSER_H_
#include <string>

#include "VSVGImage.h"
#include "VSVGShape.h"
#include "../rapidXML/rapidxml.hpp"

class VSVGParser {

public:
	VSVGParser();
	virtual ~VSVGParser();

	VSVGImage * parse(std::string fileName, Unit unit, float dpi);
private:
	void parseXML(rapidxml::xml_node<> * first,VSVGImage *image);
	VSVGShape*  parseRect(rapidxml::xml_node<>*node);
	VSVGShape*  parsePath(rapidxml::xml_node<>*node);
	VSVGShape*  parseArc(rapidxml::xml_node<>*node);

	void parseStyle(std::string style,VSVGShape* shape);
	rapidxml::xml_document<> * xmlDoc;
};

#endif /* NSVGPARSER_H_ */

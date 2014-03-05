/*
 * main.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */
#include <string>
#include <iostream>

#include "SVGGraphicalEntity.h"
#include "GameScene.h"

#include "nanosvg/nanosvg.h"

int main(int argc, char**argv) {
	std::string filename = "./rsc/easy.svg";
	std::cout<<filename<<"\n";
	SVGGraphicalEntity * svgEntity = new SVGGraphicalEntity("test",
			nsvgParseFromFile(filename.c_str(), "px", 90.0f));
//	svgEntity->setRasterizedMode(true);
	std::cout<<"SVG Entity done\n";
	GameScene *scene = new GameScene("test");
	std::cout<<"Scene done\n";
	scene->addEntity(svgEntity);
	std::cout<<"Entity added\n";
	scene->run();
	std::cout<<"exit\n";
	return 0;
}


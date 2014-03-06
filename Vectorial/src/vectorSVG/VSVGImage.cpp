/*
 * VSVGImage.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#include "VSVGImage.h"

VSVGImage::VSVGImage() {
	height=0;
	width=0;
}

VSVGImage::~VSVGImage() {
	for (unsigned int i=0;i<shapes.size();i++){
		delete shapes[i];
	}
	shapes.clear();
}

std::vector<VSVGShape*> VSVGImage::getShapes() const
{
    return shapes;
}

float VSVGImage::getWidth() const
{
    return width;
}

void VSVGImage::setWidth(float width)
{
    this->width = width;
}

float VSVGImage::getHeight() const
{
    return height;
}

void VSVGImage::setHeight(float height)
{
    this->height = height;
}

void VSVGImage::addShape(VSVGShape * shape){
	shapes.push_back(shape);
}

float VSVGImage::getDpi() const
{
    return dpi;
}

std::string VSVGImage::getFilename() const
{
    return filename;
}

Unit VSVGImage::getUnit() const
{
    return unit;
}

void VSVGImage::setDpi(float dpi)
{
    this->dpi = dpi;
}

void VSVGImage::setFilename(std::string filename)
{
    this->filename = filename;
}

void VSVGImage::setUnit(Unit unit)
{
    this->unit = unit;
}


void VSVGImage::render(SDL_Surface * surface){
	for (unsigned int i=0;i<shapes.size();i++){
		shapes[i]->render(surface);
	}
}



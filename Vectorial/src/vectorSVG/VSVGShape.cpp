/*
 * VSVGShape.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#include "VSVGShape.h"
#include "../graphics/VPlainColor.h"

VSVGShape::VSVGShape() {
	fill=new VSVGPlainColor(0);
	stroke=new VSVGPlainColor(0);
	strokeWidth=1;
}

VSVGShape::~VSVGShape() {
	for (unsigned int i=0;i<paths.size();i++){
		delete paths[i];
	}
	paths.clear();
}

SDL_Rect VSVGShape::getBounds() const
{
    return bounds;
}

VSVGPaint * VSVGShape::getFill() const
{
    return fill;
}

std::vector<VSVGPath*> VSVGShape::getPaths() const
{
    return paths;
}

void VSVGShape::addPath(VSVGPath *path)
{

	paths.push_back(path);
}

VSVGPaint * VSVGShape::getStroke() const
{
    return stroke;
}

float VSVGShape::getStrokeWidth() const
{
    return strokeWidth;
}

void VSVGShape::setBounds(SDL_Rect bounds)
{
    this->bounds = bounds;
}

void VSVGShape::setFill(VSVGPaint * fill)
{
    this->fill = fill;
}

void VSVGShape::setStroke(VSVGPaint* stroke)
{
    this->stroke = stroke;
}

void VSVGShape::setStrokeWidth(float strokeWidth)
{
    this->strokeWidth = strokeWidth;
}

std::string VSVGShape::getId() const
{
    return id;
}

void VSVGShape::setId(std::string id)
{
    this->id = id;
}


void VSVGShape::render(SDL_Surface * surface){
	for (unsigned int i=0;i<paths.size();i++){
		if (paths[i]!=NULL)
			paths[i]->render(surface,strokeWidth,stroke,fill);
	}
}


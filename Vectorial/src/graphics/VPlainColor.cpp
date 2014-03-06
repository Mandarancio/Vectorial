/*
 * VSVGPlainColor.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#include "VPlainColor.h"

VSVGPlainColor::VSVGPlainColor() {
	color=0;
}

VSVGPlainColor::~VSVGPlainColor(){}

Uint32 VSVGPlainColor::getColor()
{
	return color;
}

Uint32 VSVGPlainColor::getColorAt(float x,float y)
{
	return color;
}

VSVGPlainColor::VSVGPlainColor(Uint32 color){
	this->color=color;
}

void VSVGPlainColor::setColor(Uint32 color)
{

    this->color = color;
}



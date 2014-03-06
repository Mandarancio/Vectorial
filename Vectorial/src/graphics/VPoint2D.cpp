/*
 * Point2D.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#include "VPoint2D.h"
#include <SDL/SDL_gfxPrimitives.h>

VPoint2D::VPoint2D(float x,float y) {
	this->x=x;
	this->y=y;
}
VPoint2D::VPoint2D(){
	x=0;
	y=0;
}
VPoint2D::~VPoint2D() {
}

void VPoint2D::render(SDL_Surface * surface,float strokeWidth,VSVGPaint * stroke,VSVGPaint * fill){
	filledCircleColor(surface,x,y,4,fill->getColor());
	aacircleColor(surface,x,y,4,fill->getColor());
	aacircleColor(surface,x,y,4,stroke->getColor());
}

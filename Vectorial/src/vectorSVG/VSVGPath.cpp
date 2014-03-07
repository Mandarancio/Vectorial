/*
 * VSVGPath.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#include "VSVGPath.h"
#include <SDL/SDL_gfxPrimitives.h>

#include <iostream>

VSVGPath::VSVGPath() {
	closed=false;
	totalPoints=0;
	vx=0;
	vy=0;
}

VSVGPath::~VSVGPath() {
	for (unsigned int i=0;i<curves.size();i++){
		delete curves[i];
	}
	curves.clear();
}

SDL_Rect VSVGPath::getBounds() const
{
    return bounds;
}

bool VSVGPath::isClosed() const
{
    return closed;
}

std::vector<VBezier*> VSVGPath::getCourves() const
{
    return curves;
}

void VSVGPath::setBounds(SDL_Rect bounds)
{
    this->bounds = bounds;
}

void VSVGPath::setClosed(bool closed)
{
    this->closed = closed;
}


void VSVGPath::addCourve(VBezier* courve)
{
	this->curves.push_back(courve);
	computePolygon();
}

void VSVGPath::render(SDL_Surface * surface,float strokeWidth,VSVGPaint * stroke,VSVGPaint * fill){
	filledPolygonColor(surface,vx,vy,totalPoints,0x333333FF);
	aapolygonColor(surface,vx,vy,totalPoints,0x000000FF);

	for (unsigned int i=0;i<curves.size();i++){
		if (curves[i]!=0)
			curves[i]->render(surface,strokeWidth,stroke,fill);
	}
}

void VSVGPath::computePolygon(){
	int n=curves.size();

	std::cout<<"n: "<<n<<"\n";
	totalPoints=n*precision+1;
	vx=new Sint16[totalPoints];
	vy=new Sint16[totalPoints];

	Sint16 *x,*y;
	for (int i=0;i<n;i++){
		x=curves[i]->getVx();
		y=curves[i]->getVy();
		for (int j=0;j<precision;j++){
			vx[i*precision+j]=x[j];
			vy[i*precision+j]=y[j];
		}

	}
	vx[n*precision]=vx[0];
	vy[n*precision]=vy[0];
}

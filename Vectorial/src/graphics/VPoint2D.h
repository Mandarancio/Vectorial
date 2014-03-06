/*
 * Point2D.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef POINT2D_H_
#define POINT2D_H_

#include <SDL/SDL.h>
#include "VPaint.h"

class VPoint2D {
public:
	VPoint2D(float x,float y);
	VPoint2D();
	virtual ~VPoint2D();
	float x;
	float y;

    void render(SDL_Surface * surface,float strokeWidth,VSVGPaint  * stroke,VSVGPaint * fill);

};

#endif /* POINT2D_H_ */

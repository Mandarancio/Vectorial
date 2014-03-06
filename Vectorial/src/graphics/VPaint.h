/*
 * VSVGPaint.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef VSVGPAINT_H_
#define VSVGPAINT_H_
#include <SDL/SDL.h>
#include <string>

class VSVGPaint {
public:
	virtual Uint32 getColor()=0;
	virtual Uint32 getColorAt(float x,float y)=0;
};

#endif /* VSVGPAINT_H_ */

/*
 * VSVGPlainColor.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef VSVGPLAINCOLOR_H_
#define VSVGPLAINCOLOR_H_

#include "VPaint.h"

class VSVGPlainColor: public VSVGPaint {
public:
	VSVGPlainColor();
	VSVGPlainColor(Uint32 color);
	virtual ~VSVGPlainColor();
	Uint32 getColor();
	Uint32 getColorAt(float x,float y);
	void setColor(Uint32 color);
private:
	Uint32 color;
};

#endif /* VSVGPLAINCOLOR_H_ */

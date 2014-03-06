/*
 * VSVGImage.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef VSVGIMAGE_H_
#define VSVGIMAGE_H_

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "VSVGShape.h"

enum Unit {
	PX,IN,MM,CM
};

class VSVGImage {
public:
	VSVGImage();
	virtual ~VSVGImage();
    std::vector<VSVGShape*> getShapes() const;
    float getWidth() const;
    void setWidth(float width);
    float getHeight() const;
    void setHeight(float height);

    void addShape(VSVGShape * shape);
    float getDpi() const;
    std::string getFilename() const;
    Unit getUnit() const;
    void setDpi(float dpi);
    void setFilename(std::string filename);
    void setUnit(Unit unit);

    void render(SDL_Surface *surface);
private:
	float width;
	float height;
	std::vector<VSVGShape*> shapes;
	float dpi;
	Unit unit;
	std::string filename;
};

#endif /* VSVGIMAGE_H_ */

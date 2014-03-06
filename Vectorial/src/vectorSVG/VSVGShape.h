/*
 * VSVGShape.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef VSVGSHAPE_H_
#define VSVGSHAPE_H_

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include "../graphics/VPaint.h"
#include "VSVGPath.h"

class VSVGShape {
public:
	VSVGShape();
	virtual ~VSVGShape();
    SDL_Rect getBounds() const;
    VSVGPaint * getFill() const;
    std::vector<VSVGPath*> getPaths() const;
    void addPath(VSVGPath * path);
    VSVGPaint * getStroke() const;
    float getStrokeWidth() const;
    void setBounds(SDL_Rect bounds);
    void setFill(VSVGPaint * fill);
    void setStroke(VSVGPaint * stroke);
    void setStrokeWidth(float strokeWidth);
    std::string getId() const;
    void setId(std::string id);

    void render(SDL_Surface *surface);
private:
    std::string id;
	VSVGPaint * fill;
	VSVGPaint * stroke;
	float strokeWidth;
	SDL_Rect bounds;
	std::vector<VSVGPath*> paths;
};

#endif /* VSVGSHAPE_H_ */

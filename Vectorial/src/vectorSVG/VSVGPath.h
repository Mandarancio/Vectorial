/*
 * VSVGPath.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef VSVGPATH_H_
#define VSVGPATH_H_

#include <SDL/SDL.h>
#include <vector>
#include "../graphics/VPaint.h"
#include "../graphics/VBezier.h"

class VSVGPath {
public:
	VSVGPath();
	virtual ~VSVGPath();
    SDL_Rect getBounds() const;
    bool isClosed() const;
    std::vector<VBezier*> getCourves() const;
    void setBounds(SDL_Rect bounds);
    void setClosed(bool closed);
    void addCourve(VBezier* courve);

    void render(SDL_Surface * surface,float strokeWidth,VSVGPaint * stroke,VSVGPaint * fill);

private:
    void computePolygon();
	bool closed;
	int totalPoints;
	std::vector<VBezier*> curves;
	SDL_Rect bounds;
	Sint16 *vx,*vy;
};

#endif /* VSVGPATH_H_ */

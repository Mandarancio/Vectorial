/*
 * Bezier.h
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#ifndef BEZIER_H_
#define BEZIER_H_
#include "VPoint2D.h"
#include <SDL/SDL.h>

const int precision=20;

class VBezier {
public:
	VBezier(VPoint2D p1, VPoint2D c1, VPoint2D p2, VPoint2D c2);
	virtual ~VBezier();
    VPoint2D getC1() const;
    VPoint2D getC2() const;
    VPoint2D getP1() const;
    VPoint2D getP2() const;
    void setC1(VPoint2D c1);
    void setC2(VPoint2D c2);
    void setP1(VPoint2D p1);
    void setP2(VPoint2D p2);

    void render(SDL_Surface * surface,float strokeWidth,VSVGPaint * stroke,VSVGPaint * fill);
    Sint16* getVx();
    Sint16* getVy();

private:
    VPoint2D computePoint(float t);
    void computePolygon();
    Sint16 vx[precision];
    Sint16 vy[precision];
	VPoint2D p1;
	VPoint2D c1;
	VPoint2D c2;
	VPoint2D p2;
};

#endif /* BEZIER_H_ */

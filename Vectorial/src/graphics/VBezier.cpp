/*
 * Bezier.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#include "VBezier.h"
#include "VPlainColor.h"

#include <SDL/SDL_gfxPrimitives.h>

#define DEBUG true

VBezier::VBezier(VPoint2D p1, VPoint2D c1, VPoint2D p2, VPoint2D c2) {
	this->p1 = p1;
	this->p2 = p2;
	this->c1 = c1;
	this->c2 = c2;
	computePolygon();
}

VBezier::~VBezier() {
	// TODO Auto-generated destructor stub
}

VPoint2D VBezier::getC1() const {
	return c1;
}

VPoint2D VBezier::getC2() const {
	return c2;
}

VPoint2D VBezier::getP1() const {
	return p1;
}

VPoint2D VBezier::getP2() const {
	return p2;
}

void VBezier::setC1(VPoint2D c1) {
	this->c1 = c1;
}

void VBezier::setC2(VPoint2D c2) {
	this->c2 = c2;
}

void VBezier::setP1(VPoint2D p1) {
	this->p1 = p1;
}

void VBezier::setP2(VPoint2D p2) {
	this->p2 = p2;
}

void VBezier::render(SDL_Surface * surface, float strokeWidth,
		VSVGPaint * stroke, VSVGPaint * fill) {
	if (DEBUG) {
		VSVGPlainColor * color = new VSVGPlainColor(0xCCCCCCFF);
		p1.render(surface, 1, color, color);
		p2.render(surface, 1, color, color);
		c1.render(surface, 1, color, new VSVGPlainColor(0));
		c2.render(surface, 1, color, new VSVGPlainColor(0));
		aalineColor(surface, p1.x, p1.y, c1.x, c1.y, color->getColor());
		aalineColor(surface, p2.x, p2.y, c2.x, c2.y, color->getColor());
	}
}

void VBezier::computePolygon() {
	float dt;
	int i;

	dt = 1.0 / (precision - 1);
	VPoint2D point;

	for (i = 0; i < precision; i++) {
		point = computePoint(i * dt);
		vx[i] = point.x;
		vy[i] = point.y;
	}
}

Sint16 *VBezier::getVx() {
	return vx;
}

Sint16 * VBezier::getVy() {
	return vy;
}

VPoint2D VBezier::computePoint(float t) {
	float ax, bx, cx;
	float ay, by, cy;
	float tSquared, tCubed;
	VPoint2D result;
	VPoint2D cp[4] = { p1, c1, c2, p2 };

	/* calcolo dei coefficienti del polinomio */

	cx = 3.0 * (cp[1].x - cp[0].x);
	bx = 3.0 * (cp[2].x - cp[1].x) - cx;
	ax = cp[3].x - cp[0].x - cx - bx;

	cy = 3.0 * (cp[1].y - cp[0].y);
	by = 3.0 * (cp[2].y - cp[1].y) - cy;
	ay = cp[3].y - cp[0].y - cy - by;

	/* calcolo del punto della curva in relazione a t */

	tSquared = t * t;
	tCubed = tSquared * t;

	result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + cp[0].x;
	result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + cp[0].y;

	return result;
}


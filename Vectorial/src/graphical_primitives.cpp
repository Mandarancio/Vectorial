/*
 * graphical_primitives.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */
#include "graphical_primitives.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <math.h>
#include <iostream>

#define DEBUG true
#define STD_DEBUG false

Point2D PointOnCubicBezier(Point2D* cp, float t);

void drawPixel(SDL_Surface *surface, int x, int y, unsigned char *buffer) {

	drawPixel(surface, x, y, getColor(buffer));
}

void drawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	Uint8 *target_pixel = (Uint8 *) surface->pixels + y * surface->pitch
			+ x * 4;
	*(Uint32 *) target_pixel = pixel;
}

void drawCircle(SDL_Surface *surface, int cx, int cy, int radius,
		Uint32 pixel) {
	aacircleColor(surface, cx, cy, radius, pixel);
}

void fillCircle(SDL_Surface *surface, int cx, int cy, int radius,
		Uint32 pixel) {
	filledCircleColor(surface, cx, cy, radius, pixel);
}

Uint32 getColor(unsigned char *buffer) {
	Uint32 value = 0;

	value |= buffer[3] << 24;
	value |= buffer[0] << 16;
	value |= buffer[1] << 8;
	value |= buffer[2];

	return value;
}

Uint32 rgba2argb(Uint32 color) {

	return
	// Source is in format: 0xAARRGGBB 0xRRGGBBAA
	((color & 0xFF000000) >> 8) | //__RR_____
			((color & 0x00FF0000) >> 8) | //___BB___
			((color & 0x0000FF00) >> 8) | //_____GG_
			((color & 0x000000FF) << 24); //AA______
	// Return value is in format:  0xAARRGGB
}

Uint32 uint2Uint32(unsigned int color) {

	Uint32 r, g, b, a;
	b = ((color & 0x000000FF));
	g = ((color & 0x0000FF00) >> 8);
	r = ((color & 0x00FF0000) >> 16);
	a = ((color & 0xFF000000) >> 24);

	return a | r << 8 | g << 16 | b << 24;
}

unsigned char * getBuffer(Uint32 color) {
	unsigned char *buffer = new unsigned char[4];
	buffer[3] = color >> 24; //a [0]
	buffer[2] = color; //b [3]
	buffer[1] = color >> 8; //g [2]
	buffer[0] = color >> 16; // [1]

	return buffer;
}

void drawLine(SDL_Surface *surface, int x0, int y0, int x1, int y1,
		Uint32 color) {
	aalineColor(surface, x0, y0, x1, y1, color);
}

void drawRectangle(SDL_Surface *surface, int x, int y, int w, int h,
		Uint32 color) {
	drawLine(surface, x, y, x + w, y, color);
	drawLine(surface, x + w, y, x + w, y + h, color);
	drawLine(surface, x + w, y + h, x, y + h, color);
	drawLine(surface, x, y + h, x, y, color);
}

void drawPath(SDL_Surface * surface, NSVGpath * path, Uint32 stroke,
		Uint32 fill) {
	//first staff
	//	bool closed = path->closed;

	Path *shape = computePath(path);

	int tot = shape->size;
	Point2D * points = shape->points;

	Sint16 * vx = new Sint16[tot + 1];
	Sint16 * vy = new Sint16[tot + 1];
	for (int i = 0; i < tot; i++) {
		vx[i] = points[i].x;
		vy[i] = points[i].y;
	}

	filledPolygonColor(surface, vx, vy, tot, fill);
	aapolygonColor(surface, vx, vy, tot, stroke);

	if (DEBUG) {
		Uint32 color = 0x9999DDFF;
		for (int i = 0; i < path->npts - 1; i += 3) {
			float *p = &path->pts[i * 2];
			Point2D* cp = new Point2D[4];
			cp[0].x = p[0];
			cp[0].y = p[1];
			cp[1].x = p[2];
			cp[1].y = p[3];
			cp[2].x = p[4];
			cp[2].y = p[5];
			cp[3].x = p[6];
			cp[3].y = p[7];

			fillCircle(surface, cp[0].x, cp[0].y, 4, color);
			drawCircle(surface, cp[1].x, cp[1].y, 3, color);
			drawCircle(surface, cp[2].x, cp[2].y, 3, color);
			fillCircle(surface, cp[3].x, cp[3].y, 4, color);
			drawLine(surface, cp[0].x, cp[0].y, cp[1].x, cp[1].y, color);
			drawLine(surface, cp[2].x, cp[2].y, cp[3].x, cp[3].y, color);

		}
	}
}

Point2D PointOnCubicBezier(Point2D* cp, float t) {
	float ax, bx, cx;
	float ay, by, cy;
	float tSquared, tCubed;
	Point2D result;

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

Point2D * computeBezier(Point2D * cp, int n) {
	float dt;
	int i;

	dt = 1.0 / (n - 1);
	Point2D * points = new Point2D[n];

	for (i = 0; i < n; i++) {
		points[i] = PointOnCubicBezier(cp, i * dt);
	}

	return points;

}

Path * computePath(NSVGpath * path) {
	int n = PRECISION;
	int end=path->npts-1;

	int tot = 0;
	if (path->closed) {
		tot = n * ( path->npts / 3) ;
	} else{
		tot = n * (path->npts / 3 -1);
		end-=3;
	}Point2D * cp;
	float* p;
	Point2D * pointpath = new Point2D[tot];
	int i = 0;

	for (; i < end; i += 3) {
		p = &path->pts[i * 2];
		cp = new Point2D[4];
		cp[0].x = p[0];
		cp[0].y = p[1];
		cp[1].x = p[2];
		cp[1].y = p[3];
		cp[2].x = p[4];
		cp[2].y = p[5];
		cp[3].x = p[6];
		cp[3].y = p[7];
		Point2D * curve = computeBezier(cp, n);
		if (STD_DEBUG) {
			std::cout << cp[0].x << "," << cp[0].y << "\n";
			std::cout << cp[1].x << "," << cp[1].y << "\n";
			std::cout << cp[2].x << "," << cp[2].y << "\n";
			std::cout << cp[3].x << "," << cp[3].y << "\n\n";
		}
		for (int j = 0; j < n; j++) {
			pointpath[i / 3 * n + j] = curve[j];
		}
	}

	pointpath[tot - 1] = pointpath[0];
	Path* result = new Path();
	result->size = tot;
	result->points = pointpath;
	return result;
}

void drawPolygon(SDL_Surface *surface, Point2D * points, int size,
		Uint32 stroke, Uint32 fill) {
	int n = size;
	Sint16 * vx = new Sint16[n];
	Sint16 * vy = new Sint16[n];
	for (int i = 0; i < n; i++) {
		vx[i] = points[i].x;
		vy[i] = points[i].y;
	}

	filledPolygonColor(surface, vx, vy, n, fill);
	aapolygonColor(surface, vx, vy, n, stroke);
}

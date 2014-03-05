/*
 * graphical_primitives.h
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */

#ifndef GRAPHICAL_PRIMITIVES_H_
#define GRAPHICAL_PRIMITIVES_H_

#include <SDL/SDL.h>
#include "nanosvg/nanosvg.h"

#define PRECISION 12;

typedef struct {
	float x;
	float y;
} Point2D;

struct Path {
	Uint32 fill, stroke;
	Point2D * points;
	int size;
	struct Path * next;
};


Uint32 getColor(unsigned char *buffer);
Uint32 rgba2argb(Uint32 color);
Uint32 uint2Uint32(unsigned int color);
unsigned char * getBuffer(Uint32 color);

void drawPixel(SDL_Surface *surface, int x, int y, unsigned char *buffer);
void drawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void drawCircle(SDL_Surface *surface, int n_cx, int n_cy, int radius,
		Uint32 pixel);
void fillCircle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel);
void drawLine(SDL_Surface *surface, int x0, int y0, int x1, int y1,
		Uint32 color);
void drawRectangle(SDL_Surface *surface, int x, int y, int w, int h,
		Uint32 color);
void drawPath(SDL_Surface *surface, NSVGpath * path, Uint32 stroke, Uint32 fill);
Point2D * computeBezier(Point2D * cp, int n);
Path * computePath(NSVGpath * path);
void drawPolygon(SDL_Surface *surface,Point2D * points,int size, Uint32 stroke, Uint32 fill);

#endif /* GRAPHICAL_PRIMITIVES_H_ */

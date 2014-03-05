/*
 * SimpleGraphicalEntity.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */

#include "SVGGraphicalEntity.h"
#include <math.h>

#include "nanosvg/nanosvgrast.h"

#include <iostream>

#define DEBUG true

SVGGraphicalEntity::SVGGraphicalEntity(std::string name, NSVGimage* svgImage) :
		GraphicalEntity(name) {
	this->svgImage_ = svgImage;
	this->convertSVG();
	rasterized_ = false;
	w_ = svgImage_->width;
	h_ = svgImage_->height;
	image_ = 0;
}

SVGGraphicalEntity::~SVGGraphicalEntity() {
	delete svgImage_;
	delete image_;
}

void SVGGraphicalEntity::render(SDL_Surface * screen) {
	if (rasterized_) {
		renderRasterized(screen);
	} else {
		renderVectorial(screen);
	}
}

void SVGGraphicalEntity::setPosition(int cx, int cy) {
	cx_ = cx;
	cy_ = cy;
}

void SVGGraphicalEntity::renderRasterized(SDL_Surface *screen) {
	if (svgImage_ != NULL) {
		for (int j = 0; j < h_; j++) {

			for (int i = 0; i < w_; ++i) {
				unsigned char *d = image_ + (j * w_ + i) * 4;
				drawPixel(screen, i, j, d);
			}
		}
	}
}

void SVGGraphicalEntity::renderVectorial(SDL_Surface *screen) {
	if (paths != NULL) {
		Path * path = this->paths;
		do {
			drawPolygon(screen,path->points,path->size,path->stroke,path->fill);
			path = path->next;
		} while (path != NULL);
	}
	if (DEBUG) {
		NSVGshape * shape = this->svgImage_->shapes;
		do {
			drawPath(screen, shape->paths, 0,
					0);
			shape = shape->next;
		} while (shape != NULL);
	}
}

void SVGGraphicalEntity::rasterize() {

	struct NSVGrasterizer *rast = nsvgCreateRasterizer();
	if (rast == NULL) {
		std::cout << "Could not init rasterizer.\n";
	}

	image_ = (unsigned char*) malloc(w_ * h_ * 4);
	if (image_ == NULL) {
		std::cout << "Could not alloc image buffer.\n";
	}
	nsvgRasterize(rast, svgImage_, 0, 0, 1, image_, w_, h_, w_ * 4);
}

void SVGGraphicalEntity::convertSVG() {
	if (svgImage_ != NULL) {

		NSVGshape * shape = svgImage_->shapes;
		Path * old = 0;
		Path * current = 0;
		do {
			current = computePath(shape->paths);
			current->fill = uint2Uint32(shape->fill.color);
			current->stroke = uint2Uint32(shape->stroke.color);
			if (old != 0)
				old->next = current;
			if (old == 0)
				paths = current;
			old = current;
			shape = shape->next;
		} while (shape != NULL);
	}
}

int SVGGraphicalEntity::getWidth() const {
	return w_;
}

int SVGGraphicalEntity::getHeight() const {
	return h_;
}

bool SVGGraphicalEntity::isRasterizedMode() const {
	return rasterized_;
}

void SVGGraphicalEntity::setRasterizedMode(bool mode) {
	rasterized_ = mode;
	if (mode && image_ == 0) {
		if (DEBUG)
			std::cout << "rasterize first time\n";
		rasterize();
	}
}

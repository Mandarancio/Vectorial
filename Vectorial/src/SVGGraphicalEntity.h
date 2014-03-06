///*
// * SimpleGraphicalEntity.h
// *
// *  Created on: Mar 4, 2014
// *      Author: maferrar
// */
//
//#ifndef SIMPLEGRAPHICALENTITY_H_
//#define SIMPLEGRAPHICALENTITY_H_
//
//#include "GraphicalEntity.h"
//#include "nanosvg/nanosvg.h"
//#include "graphical_primitives.h"
//
//
//class SVGGraphicalEntity: public GraphicalEntity {
//public:
//	SVGGraphicalEntity(std::string name, NSVGimage * svgImage);
//	virtual ~SVGGraphicalEntity();
//	void render(SDL_Surface * screen);
//	void setPosition(int cx, int cy);
//    int getWidth() const;
//    int getHeight() const;
//    bool isRasterizedMode() const;
//    void setRasterizedMode(bool mode);
//private:
//    void renderVectorial(SDL_Surface * screen);
//    void renderRasterized(SDL_Surface *screen);
//	void rasterize();
//	void convertSVG();
//	int w_,h_;
//	int cx_, cy_;
//	Path * paths;
//	NSVGimage *svgImage_;
//	unsigned char * image_;
//	bool rasterized_;
//
//};
//
//#endif /* SIMPLEGRAPHICALENTITY_H_ */

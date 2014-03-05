#ifndef NANOSVG_H
#define NANOSVG_H

#define NSVG_PAINT_NONE 0
#define NSVG_PAINT_COLOR 1
#define NSVG_PAINT_LINEAR_GRADIENT 2
#define NSVG_PAINT_RADIAL_GRADIENT 3

#define NSVG_SPREAD_PAD 0
#define NSVG_SPREAD_REFLECT 1
#define NSVG_SPREAD_REPEAT 2
#include <SDL/SDL.h>

struct NSVGgradientStop {
	Uint32 color;
	float offset;
};

struct NSVGgradient {
	float xform[6];
	char spread;
	float fx, fy;
	int nstops;
	struct NSVGgradientStop stops[1];
};

struct NSVGpaint {
	char type;
	union {
		Uint32 color;
		struct NSVGgradient* gradient;
	};
};

struct NSVGpath {
	float* pts; // Cubic bezier points: x0,y0, [cpx1,cpx1,cpx2,cpy2,x1,y1], ...
	int npts; // Total number of bezier points.
	char closed; // Flag indicating if shapes should be treated as closed.
	float bounds[4]; // Tight bounding box of the shape [minx,miny,maxx,maxy].
	struct NSVGpath* next; // Pointer to next path, or NULL if last element.
};

struct NSVGshape {
	struct NSVGpaint fill; // Fill paint
	struct NSVGpaint stroke; // Stroke paint
	float strokeWidth; // Stroke width (scaled)
	float bounds[4]; // Tight bounding box of the shape [minx,miny,maxx,maxy].
	struct NSVGpath* paths; // Linked list of paths in the image.
	struct NSVGshape* next; // Pointer to next shape, or NULL if last element.
};

struct NSVGimage {
	float width; // Width of the image.
	float height; // Height of the image.
	struct NSVGshape* shapes; // Linked list of shapes in the image.
};

// Parses SVG file from a file, returns SVG image as paths.
struct NSVGimage* nsvgParseFromFile(const char* filename, const char* units,
		float dpi);

// Parses SVG file from a null terminated string, returns SVG image as paths.
struct NSVGimage* nsvgParse(char* input, const char* units, float dpi);

// Deletes list of paths.
void nsvgDelete(struct NSVGimage* image);

#endif // NANOSVG_H

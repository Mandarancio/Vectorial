#ifndef NANOSVGRAST_H
#define NANOSVGRAST_H


struct NSVGrasterizer* nsvgCreateRasterizer();

void nsvgRasterize(struct NSVGrasterizer* r, struct NSVGimage* image, float tx,
		float ty, float scale, unsigned char* dst, int w, int h, int stride);

// Deletes rasterizer context.
void nsvgDeleteRasterizer(struct NSVGrasterizer*);

#endif // NANOSVGRAST_H

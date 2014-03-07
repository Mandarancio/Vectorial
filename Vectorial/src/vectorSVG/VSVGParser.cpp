/*
 * NSVGParser.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: maferrar
 */

#include "VSVGParser.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#include "VSVGPath.h"
#include "../graphics/VPoint2D.h"
#include "../graphics/VBezier.h"

//STYLE CONST
#define OPACITY "opacity"
#define FILL "fill"
#define FILLOPACITY "fill-opacity"
#define STROKE "stroke"
#define STROKEOPACITY "stroke-opacity"
#define STROKEWIDTH "stroke-width"

//ELEMENT TYPES
#define RECT "rect"
#define PATH "path"

#define DEBUG true
#define PI (3.14159265358979323846264338327f)

//struct NamedColor namedColors[] = {
//		{ "red", 0xFF0000FF }, { "green",0x008000FF },
//		{ "blue", 0x0000FFFF }, { "yellow", 0xFFFF00FF },
//		{	"cyan", 0x00FFFFFF }, { "magenta", 0xFF00FFFF },
//		{ "black", 0x000000FF }, { "grey", 0x808080FF },
//		{ "gray",	0x808080FF }, { "white",0xFFFFFFFF },
//		{ "none", 0x00000000 } };


std::vector<std::string> &split(const std::string &s, char delim, std::vector<
		std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

static float vmag(float x, float y) {
	return sqrtf(x * x + y * y);
}

static float vecrat(float ux, float uy, float vx, float vy) {
	return (ux * vx + uy * vy) / (vmag(ux, uy) * vmag(vx, vy));
}

static float vecang(float ux, float uy, float vx, float vy) {
	float r = vecrat(ux, uy, vx, vy);
	if (r < -1.0f)
		r = -1.0f;
	if (r > 1.0f)
		r = 1.0f;
	return ((ux * vy < uy * vx) ? -1.0f : 1.0f) * acosf(r);
}

static void xformPoint(float* dx, float* dy, float x, float y, float* t) {
	*dx = x * t[0] + y * t[2] + t[4];
	*dy = x * t[1] + y * t[3] + t[5];
}

static void xformVec(float* dx, float* dy, float x, float y, float* t) {
	*dx = x * t[0] + y * t[2];
	*dy = x * t[1] + y * t[3];
}

std::vector<VBezier*> parseArc(VPoint2D p1, VPoint2D p2, float rx, float ry,
		float rotx, int fa, int fs, bool relative) {
	if (relative) {
		p2.x += p1.x;
		p2.y += p1.y;
	}

	std::vector<VBezier*> result;

	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float d = sqrtf(dx * dx + dy * dy);
	if (d < 1e-6f || rx < 1e-6f || ry < 1e-6f) {
		result.push_back(new VBezier(p1, p1, p2, p2));
		return result;
	}

	float sinrx = sinf(rotx);
	float cosrx = cosf(rotx);

	// Convert to center point parameterization.
	// http://www.w3.org/TR/SVG11/implnote.html#ArcImplementationNotes
	// 1) Compute x1', y1'
	float x1p = cosrx * dx / 2.0f + sinrx * dy / 2.0f;
	float y1p = -sinrx * dx / 2.0f + cosrx * dy / 2.0f;

	d = (x1p * x1p) / (rx * rx) + (y1p * y1p) / (ry * ry);
	if (d > 1) {
		d = sqrtf(d);
		rx *= d;
		ry *= d;
	}

	// 2) Compute cx', cy'
	float s = 0.0f;
	float sa = (rx * rx) * (ry * ry) - (rx * rx) * (y1p * y1p) - (ry * ry)
			* (x1p * x1p);
	float sb = (rx * rx) * (y1p * y1p) + (ry * rx) * (x1p * x1p);
	if (sa < 0.0f)
		sa = 0.0f;
	if (sb > 0.0f)
		s = sqrtf(sa / sb);
	if (fa == fs)
		s = -s;
	float cxp = s * rx * y1p / ry;
	float cyp = s * -ry * x1p / rx;

	// 3) Compute cx,cy from cx',cy'
	float cx = (p1.x + p2.x) / 2.0f + cosrx * cxp - sinrx * cyp;
	float cy = (p1.y + p2.y) / 2.0f + sinrx * cxp + cosrx * cyp;

	// 4) Calculate theta1, and delta theta.
	float ux = (x1p - cxp) / rx;
	float uy = (y1p - cyp) / ry;
	float vx = (-x1p - cxp) / rx;
	float vy = (-y1p - cyp) / ry;
	float a1 = vecang(1.0f, 0.0f, ux, uy); // Initial angle
	float da = vecang(ux, uy, vx, vy); // Delta angle

	if (fa) {
		// Choose large arc
		if (da > 0.0f)
			da = da - 2 * PI;
		else
			da = 2 * PI + da;
	}

	float t[6];
	// Approximate the arc using cubic spline segments.
	t[0] = cosrx;
	t[1] = sinrx;
	t[2] = -sinrx;
	t[3] = cosrx;
	t[4] = cx;
	t[5] = cy;
	// Split arc into max 90 degree segments.
	float ndivs = (int) (fabsf(da) / (PI * 0.5f) + 0.5f);
	float hda = (da / (float) ndivs) / 2.0f;
	float kappa = fabsf(4.0f / 3.0f * (1.0f - cosf(hda)) / sinf(hda));
	if (da < 0.0f)
		kappa = -kappa;
	float a, px = p1.x, py = p1.y, ptanx, ptany;
	float x, y, tanx, tany;

	for (int i = 0; i <= ndivs; i++) {
		a = a1 + da * (i / (float) ndivs);
		dx = cosf(a);
		dy = sinf(a);
		xformPoint(&x, &y, dx * rx, dy * ry, t); // position
		xformVec(&tanx, &tany, -dy * rx * kappa, dx * ry * kappa, t); // tangent
		if (i > 0) {
			result.push_back(new VBezier(VPoint2D(px, py), VPoint2D(px + ptanx, py
					+ ptany), VPoint2D(x, y), VPoint2D(x - tanx, y - tany)));
		}

		px = x;
		py = y;
		ptanx = tanx;
		ptany = tany;
	}

	return result;
}

std::vector<VBezier *> parseCmd(std::vector<std::string> cmds, VBezier* last) {
	std::vector<VBezier *> result;
	bool relative = true;

	if (cmds[0][0] >= 'A' && cmds[0][0] < 'Z')
		relative = false;
	char cmd = cmds[0][0];
	cmd = tolower(cmd);
	std::vector<double> points;

	for (unsigned int i = 1; i < cmds.size(); i++) {
		if (cmds[i].find(',') != std::string::npos) {
			std::vector<std::string> tmp = split(cmds[i], ',');
			for (unsigned int j = 0; j < tmp.size(); j++) {
				points.push_back(atof(tmp[j].c_str()));
			}
		} else
			points.push_back(atof(cmds[i].c_str()));
	}

	VPoint2D lastPoint = last->getP2();
	if (cmd == 'm') {

		for (unsigned int i = 0; i < points.size(); i += 2) {
			VPoint2D p(points[i], points[i + 1]);
			if (relative) {
				p.x += lastPoint.x;
				p.y += lastPoint.y;
			}
			lastPoint = p;
			result.push_back(new VBezier(p, p, p, p));
		}
	} else if (cmd == 'l') {
		for (unsigned int i = 0; i < points.size(); i += 2) {
			VPoint2D p(points[i], points[i + 1]);
			if (relative) {
				p.x += lastPoint.x;
				p.y += lastPoint.y;
			}
			result.push_back(new VBezier(lastPoint, lastPoint, p, p));
			lastPoint = p;
		}
	} else if (cmd == 'v') {
		for (unsigned int i = 0; i < points.size(); i++) {
			VPoint2D p(lastPoint.x, points[i]);
			if (relative)
				p.y += lastPoint.y;
			result.push_back(new VBezier(lastPoint, lastPoint, p, p));
			lastPoint = p;
		}
	} else if (cmd == 'h') {
		for (unsigned int i = 0; i < points.size(); i++) {
			VPoint2D p(points[i], lastPoint.y);
			if (relative)
				p.x += lastPoint.x;
			result.push_back(new VBezier(lastPoint, lastPoint, p, p));
			lastPoint = p;
		}
	} else if (cmd == 'c' && points.size() >= 6) {
		for (unsigned int i = 0; i < points.size(); i += 6) {
			VPoint2D c1(points[i + 0], points[i + 1]);
			VPoint2D c2(points[i + 2], points[i + 3]);
			VPoint2D p2(points[i + 4], points[i + 5]);
			if (relative) {
				c1.x += lastPoint.x;
				c1.y += lastPoint.y;
				c2.x += lastPoint.x;
				c2.y += lastPoint.y;
				p2.x += lastPoint.x;
				p2.y += lastPoint.y;
			}
			result.push_back(new VBezier(lastPoint, c1, p2, c2));
			lastPoint = p2;

		}
	} else if (cmd == 's' && points.size() >= 4) {
		VPoint2D lc = last->getC2();
		for (unsigned int i = 0; i < points.size(); i += 4) {

			VPoint2D c2(points[i + 0], points[i + 1]);
			VPoint2D p2(points[i + 2], points[i + 3]);

			VPoint2D c1(lastPoint.x - lc.x, lastPoint.y - lc.y);

			if (relative) {
				c2.x += lastPoint.x;
				c2.y += lastPoint.y;
				p2.x += lastPoint.x;
				p2.y += lastPoint.y;
			}
			result.push_back(new VBezier(lastPoint, c1, p2, c2));
			lastPoint = p2;
			lc = c2;
		}
	} else if (cmd == 'q' && points.size() >= 4) {
		for (unsigned int i = 0; i < points.size(); i += 4) {
			VPoint2D c(points[i + 0], points[i + 1]);
			VPoint2D p2(points[i + 2], points[i + 3]);

			if (relative) {
				c.x += lastPoint.x;
				c.y += lastPoint.y;
				p2.x += lastPoint.x;
				p2.y += lastPoint.y;
			}
			result.push_back(new VBezier(lastPoint, c, p2, c));
			lastPoint = p2;
		}
	} else if (cmd == 't') {
		VPoint2D lc = last->getC2();

		for (unsigned int i = 0; i < points.size(); i += 2) {
			VPoint2D p2(points[i + 0], points[i + 1]);
			VPoint2D c(lastPoint.x - lc.x, lastPoint.y - lc.y);

			if (relative) {
				p2.x += lastPoint.x;
				p2.y += lastPoint.y;
			}
			result.push_back(new VBezier(lastPoint, c, p2, c));
			lastPoint = p2;
			lc = c;
		}
	} else if (cmd == 'a' && points.size() > 6) { //rx ry x-axis-rotation large-arc-flag sweep-flag x y
		float rx = points[0];
		float ry = points[1];
		float xRot = points[2];
		int largeFlag = points[3];
		int sweepFlag = points[4];
		VPoint2D p2(points[5], points[6]);

		return parseArc(lastPoint, p2, rx, ry, xRot, largeFlag, sweepFlag,
				relative);
	}

	return result;
}

VSVGParser::VSVGParser() {
	xmlDoc = new rapidxml::xml_document<>();
}

VSVGParser::~VSVGParser() {
	delete xmlDoc;
}

VSVGImage *VSVGParser::parse(std::string fileName, Unit unit, float dpi) {
	VSVGImage * image = new VSVGImage();
	image->setFilename(fileName);
	image->setDpi(dpi);
	image->setUnit(unit);

	std::ifstream stream;
	stream.open(fileName.c_str());
	if (DEBUG)
		std::cout<<"file opened\n";
	if (stream.is_open()) {
		std::string data;
		std::string line;
		while (getline(stream, line)) {
			data += line + "\n";
		}
		stream.close();
		//		std::cout<<data.c_str();
		xmlDoc->parse<0> (const_cast<char*> (data.c_str())); // 0 means default parse flags
		rapidxml::xml_node<> *node = xmlDoc->first_node();
		if (node->first_attribute("width") && node->first_attribute("height")) {
			image->setWidth(atof(node->first_attribute("width")->value()));
			image->setHeight(atof(node->first_attribute("height")->value()));
		}
		parseXML(node, image);
		xmlDoc->clear();
	}

	return image;
}

void VSVGParser::parseXML(rapidxml::xml_node<> * first, VSVGImage *image) {
	rapidxml::xml_node<> *node = first;
	do {
		if (strcmp(node->name(), RECT) == 0) {
			VSVGShape * shape = parseRect(node);
			image->addShape(shape);
		} else if (strcmp(node->name(), PATH) == 0) {
			image->addShape(parsePath(node));
		} else if (node->first_node() != NULL)
			parseXML(node->first_node(), image);
		node = node->next_sibling();
	} while (node != NULL);
}

VSVGShape *VSVGParser::parsePath(rapidxml::xml_node<> *node) {
	VSVGShape * shape = new VSVGShape();
	std::string id(node->first_attribute("id")->value());
	shape->setId(id);
	parseStyle(std::string(node->first_attribute("style")->value()), shape);

	std::string cmdLine = node->first_attribute("d")->value();

	std::vector<std::string> cmds = split(cmdLine, ' ');
	std::vector<std::string> cmd;
	VSVGPath * path = new VSVGPath();

	VPoint2D point(0, 0);
	VBezier* last = new VBezier(point, point, point, point);
	for (unsigned int i = 0; i < cmds.size(); i++) {
		if (cmd.size() > 0) {
			if (cmds[i].length() == 1) {
				if (cmds[i][0] < '0' || cmds[i][0] > '9') {
					std::vector<VBezier*> b = parseCmd(cmd, last);
					for (unsigned int i = 0; i < b.size(); i++) {
						path->addCourve(b[i]);
						last = b[i];
					}

					cmd.clear();
				}
			}
		}
		cmd.push_back(cmds[i]);
	}

	if (cmd.size() > 0) {
		if (cmd.size() == 1 && (cmd[0][0] == 'z' || cmd[0][0] == 'Z'))
			path->setClosed(true);
		else {
			std::vector<VBezier*> b = parseCmd(cmd, last);
			for (unsigned int i = 0; i < b.size(); i++) {
				path->addCourve(b[i]);
			}
		}
	}
	shape->addPath(path);
	return shape;
}

VSVGShape *VSVGParser::parseArc(rapidxml::xml_node<> *node) {
	return 0;
}

VSVGShape *VSVGParser::parseRect(rapidxml::xml_node<> *node) {
	VSVGShape * shape = new VSVGShape();
	float x, y;
	float w, h;
	x = atof(node->first_attribute("x")->value());
	y = atof(node->first_attribute("y")->value());
	w = atof(node->first_attribute("width")->value());
	h = atof(node->first_attribute("height")->value());
	std::string id(node->first_attribute("id")->value());

	shape->setId(id);
	parseStyle(std::string(node->first_attribute("style")->value()), shape);

	VSVGPath *path = new VSVGPath();
	if (node->first_attribute("rx") == NULL && node->first_attribute("ry")
			== NULL) {
		VBezier * side = new VBezier(VPoint2D(x, y), VPoint2D(x + w / 4, y),
				VPoint2D(x + w, y), VPoint2D(x + 3 * w / 4, y));
		path->addCourve(side);
		side = new VBezier(VPoint2D(x + w, y), VPoint2D(x + w, y + h / 4),
				VPoint2D(x + w, y + h), VPoint2D(x + w, y + 3 * h / 4));
		path->addCourve(side);
		side = new VBezier(VPoint2D(x + w, y + h), VPoint2D(x + 3 * w / 4, y + h),
				VPoint2D(x, y + h), VPoint2D(x + w / 4, y + h));
		path->addCourve(side);
		side = new VBezier(VPoint2D(x, y + h), VPoint2D(x, y + 3 * h / 4),
				VPoint2D(x, y), VPoint2D(x, y + h / 4));
		path->addCourve(side);
	}

	path->setClosed(true);
	shape->addPath(path);

	return shape;
}

void VSVGParser::parseStyle(std::string style, VSVGShape *shape) {
	std::vector<std::string> x = split(style, ';');
	std::vector<std::string> pair;
	float opacity = 1;
	std::string fill, stroke;
	float strOpacity = 1, fillOpacity = 1;
	float strWidth = 0;

	for (unsigned int i = 0; i < x.size(); i++) {
		pair = split(x[i], ':');
		if (pair.size() == 2) {
			if (pair[0].compare(OPACITY) == 0) {
				opacity = atof(pair[1].c_str());
			} else if (pair[0].compare(FILL) == 0) {
				fill = pair[1];
			} else if (pair[0].compare(FILLOPACITY) == 0) {
				fillOpacity = atof(pair[1].c_str());
			} else if (pair[0].compare(STROKE) == 0) {
				stroke = pair[1];
			} else if (pair[0].compare(STROKEWIDTH) == 0) {
				strWidth = atof(pair[1].c_str());
			} else if (pair[0].compare(STROKEOPACITY) == 0) {
				strOpacity = atof(pair[1].c_str());
			}
		}
	}

	strOpacity *= opacity;
	fillOpacity *= opacity;

	std::cout << "FILL " << fill << "," << fillOpacity << "\n";
	std::cout << "STROKE " << stroke << "," << strOpacity << "," << strWidth<< "\n";
}

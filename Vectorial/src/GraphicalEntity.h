/*
 * GraphicalEntity.h
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */

#ifndef GRAPHICALENTITY_H_
#define GRAPHICALENTITY_H_

#include "Entity.h"
#include <SDL/SDL.h>

class GraphicalEntity : public Entity {
public:
	GraphicalEntity(std::string name);
	virtual ~GraphicalEntity();

	virtual void render(SDL_Surface * screen)=0;
};

#endif /* GRAPHICALENTITY_H_ */

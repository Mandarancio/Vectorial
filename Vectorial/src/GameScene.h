/*
 * GameScene.h
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "GraphicalEntity.h"
#include <vector>
#include <SDL/SDL.h>

class GameScene: public Entity {
public:
	GameScene(std::string name);
	virtual ~GameScene();

	void addEntity(GraphicalEntity*e);
	void run();
private:
	std::vector<GraphicalEntity*> entities_;
	SDL_Surface * screen_;

	bool init();
};

#endif /* GAMESCENE_H_ */

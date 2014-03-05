/*
 * GameScene.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */

#include "GameScene.h"
#include <iostream>

GameScene::GameScene(std::string name) :
	Entity(name, GRAPHICAL) {

}

GameScene::~GameScene() {
	for (u_int i = 0; i < entities_.size(); i++) {
		delete entities_[i];
	}
	entities_.clear();
}

void GameScene::addEntity(GraphicalEntity * e) {
	entities_.push_back(e);
}

void GameScene::run() {
	std::cout << "\nRun scene " << this->getName() << "\n";
	if (init()) {
		std::cout << "Init DONE\n";
		std::cout << "Screen: " << screen_->w << "," << screen_->h << "\n";
		for (;;) {
			SDL_Flip(screen_);
			SDL_LockSurface(screen_);
			SDL_FillRect(screen_,&screen_->clip_rect,0xFFFFFFFF);
			for (u_int i = 0; i < entities_.size(); i++) {
				entities_[i]->render(screen_);
			}
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				//TODO key event handling
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					return;
				}
			}
			SDL_UnlockSurface(screen_);
		}
	}

}

bool GameScene::init() {
	screen_ = SDL_SetVideoMode(800, 800, 32, SDL_ANYFORMAT);
	if (screen_ != NULL)
		return true;
	SDL_Quit();
	return false;

}

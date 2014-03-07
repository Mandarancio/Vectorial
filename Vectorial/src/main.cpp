/*
 * main.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */
#include <string>
#include <iostream>
#include <SDL/SDL.h>

#include "vectorSVG/VSVGImage.h"
#include "vectorSVG/VSVGParser.h"

int main() {
	std::cout<<"make parser\n";
	VSVGParser * parser = new VSVGParser();
	std::cout<<"start svg parsing\n";
	VSVGImage *image=parser->parse("rsc/easy.svg", PX, 90);
	std::cout<<"parsed, rendering\n";
	SDL_Surface * screen_ = SDL_SetVideoMode(image->getWidth(), image->getHeight(), 32, SDL_ANYFORMAT);
	if (screen_ != NULL) {
		for (;;) {
			SDL_Flip( screen_);
			SDL_LockSurface(screen_);
			SDL_FillRect(screen_, &screen_->clip_rect, 0xFFFFFFFF);
			image->render(screen_);
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					SDL_Quit();
					return 0;
				}
			}
			SDL_UnlockSurface(screen_);
		}
	}
	return 1;
}

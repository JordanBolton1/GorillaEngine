//system libs
#include <iostream>

//External Libs
#include <SDL/SDL.h>

//engine libs
#include "GWindow.h"

//--- smart pointers delete themselves when there is no owner reference ---
// shared pointer = shares ownership across all references
// unique pointer = does not share ownership with anything
// weak pointer = this has no ownership over any references

// source variables
std::unique_ptr<GWindow> m_window;


//source functions
bool Initialise() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) !=0) {
		std::cout << "fail to init sdl: " << SDL_GetError() << std::endl;
	}


	//creating thwe window object
	m_window = std::make_unique<GWindow>();

	//creating sdl window
	if (!m_window->CreateWindow({ "Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720}))
		return false;


	return true;
}

void Cleanup() {	
	//exit sdl and deallocate all initialises sdl components
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	//initialise the engine
	if (!Initialise()) {
		Cleanup();
		return -1;
	}

	
	//keep game open while window is open
	while (!m_window->IsPendingClose()) {
		//TO DO
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				m_window->CloseWindow();
			}
		}
	}



	//clean up the engine
	Cleanup();

	std::cout << "closing app" << std::endl;

	return 0;
}
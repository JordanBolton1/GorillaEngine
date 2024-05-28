#pragma once

//system libs
#include <iostream>
#include <string>


struct GSWindowParams {
	//default constructor
	GSWindowParams() {
		title = "Gorrila Engine Window";
		x = y = 0;
		w = 1280;
		h = 720;
		vsync = false;
		fullscreen = false;
	}

	//setting constructor
	GSWindowParams(std::string title, int x, int y, unsigned  int w, unsigned int h) : 
		title(title), x(x), y(y), w(w), h(h), vsync(false),fullscreen(false) {

	}
	//title of the window
	std::string title;
	//position of window
	int x, y;
	//width and height of the window
	unsigned int w, h;
	//vsync enable
	bool vsync;
	//fullscreen enable
	bool fullscreen;

};

struct SDL_Window;

class GWindow {
public:
	GWindow();
	~GWindow();

	//create the window with parameters
	bool CreateWindow(const GSWindowParams& params);

	//close the window
	void CloseWindow() { m_shouldClose = true; }

	//check if the window has been set to close
	bool IsPendingClose() { return m_shouldClose; }

private:
	//a ref to the window in sdl
	SDL_Window* m_sdlWindow;

	//window parameters
	GSWindowParams m_params;

	//determine if the window should close
	bool m_shouldClose;
};

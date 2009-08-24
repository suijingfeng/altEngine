#include "include.h"

#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
public:
	Engine()
	{
	}

	void init(void *param1, void *param2);
	void render();
	void step();
	bool mousepos(int x, int y, int deltax, int deltay);
	void keystroke(char *key, bool pressed);
	void resize(int width, int height);
	void destroy();
	~Engine()
	{
	}
private:
	Bsp			map;
	Graphics	gfx;
	Sound		audio;
	Entity_list	entity_list;
	Frame		camera;
	Keyboard	keyboard;
};

#endif


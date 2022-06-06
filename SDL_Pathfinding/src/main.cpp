#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "SDL_SimpleApp.h"
#include "ScenePathFindingMouse.h"
#include "ScenePathFindingA.h"
#include "ScenePathFindingEnemies.h"

using namespace std;

int main(int argc, char ** argv)
{
	bool quit = false;
	SDL_Event event;
	int this_tick = 0;
	int next_tick = 1000/FRAMES_PER_SEC;

	SDL_SimpleApp *app = SDL_SimpleApp::Instance();

	Scene *curr_scene = new ScenePathFindingMouse;
	app->setWindowTitle(curr_scene->getTitle());

	
	if (argc > 1) {
		cout << argv[1] << endl; 
		exit(0);
	}

	while (!quit)
	{
		// Uncomment this to control effective frame rate 
		this_tick = SDL_GetTicks();
		if (this_tick < next_tick)
		{
			SDL_Delay(next_tick - this_tick);
		}
		next_tick = this_tick + (1000 / FRAMES_PER_SEC);

		// run app frame by frame
		event = app->run(curr_scene);

		/* Keyboard events */
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_1)
			{
				delete(curr_scene);
				curr_scene = new ScenePathFindingMouse;
				app->setWindowTitle(curr_scene->getTitle());
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_2)
			{
				delete(curr_scene);
				curr_scene = new ScenePathFindingA;
				app->setWindowTitle(curr_scene->getTitle());
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_3)
			{
				delete(curr_scene);
				curr_scene = new ScenePathFindingEnemies;
				app->setWindowTitle(curr_scene->getTitle());
			}
			if ((event.key.keysym.scancode == SDL_SCANCODE_Q) || (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			{
				quit = true;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_F)
			{
				app->setFullScreen();
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}

	}

	return 0;
}
/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "ObjLoader.h"
#include "SoftwareRenderer.h"
#include "Camera.h"
#include <iostream>


//Screen dimension constants


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

enum controls {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SHOOT
};


class player {
public:
	float forward;
	float backward;
	float left;
	float right;
	bool shoot;
};


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}



int main(int argc, char* args[])
{
	//Start up SDL and create window
	int px = 0;
	int py = 0;
	float s = 100.0;

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			Entity cube = loadEntity("Entity/cube.obj");
			//Entity w = loadEntity("Entity/church.obj");
			//Entity p1 = loadEntity("Entity/p1.obj");
			//Entity sphere = loadEntity("Entity/sphere.obj");
			//Entity map = loadEntity("Entity/MAPHOUSE.obj");
			//Entity c2 = loadEntity("Entity/c2.obj");
			DisplayList list;
			//list.insert(p1);
			//list.insert(sphere);
			list.insert(cube);
			//list.insert(map);
			list = scale(s, list);
			Camera c;
			c.fov = 90;
			c.pos.y = 0;
			c.pos.x = 0;
			c.pos.z = 400.0;
			c.frontDirection.z = 1;
			c.setResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
			float moveX = 0;
			float moveY = 0;
			float moveZ = 0;
			while (!quit)
			{

				bool states[5] = { false, false, false, false, false };
				while (SDL_PollEvent(&e)) {

					if (e.type == SDL_QUIT)
						quit = true;
				}
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_UP])
				{
					states[FORWARD] = true;
					c.translateZ(-10);
				}
				if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					states[BACKWARD] = true;
					c.translateZ(10);
				}
				if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					states[LEFT] = true;
					c.rotateY(1);
				}
				if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					states[RIGHT] = true;
					c.rotateY(-1);
				}
				if (currentKeyStates[SDL_SCANCODE_SPACE])
				{
					states[SHOOT] = true;
				}



				int count = 0;
				for (int i = 0; i < sizeof(states); i++)
					//	printf("%d", states[i]);
				{
					std::cout << states[i];

					if (states[i] == true) {
						count++;
					}
				}
				printf("\n");
				printf("%d", count);
				printf("\n");
				Uint64 start = SDL_GetPerformanceCounter();

				// Do event loop

				// Do physics loop

				// Do rendering loop

				Uint64 end = SDL_GetPerformanceCounter();

				float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

				// Cap to 30 FPS
				SDL_Delay(floor(33.333f - elapsedMS));

				printf("Camera parameters: angle (x, y, z): %f %f %f\n pos (x, y, z): %f %f %f direction vec X:%f Y:%f Z:%f\n",
				c.angle.x*100, c.angle.y*100, c.angle.z*100,
				c.pos.x, c.pos.y, c.pos.z,
				c.frontDirection.x, c.frontDirection.y, c.frontDirection.z);
				//Clear screen
				//Update screen
				DisplayList temp = list;
				//temp = applyDelta(c, temp);
				c.cameraRender(temp, gRenderer);
				//render(gRenderer, temp);
				//renderWireframe(gRenderer, temp);
				//c.translateX(moveX);
				SDL_RenderPresent(gRenderer);
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
				SDL_RenderClear(gRenderer);
				SDL_Delay(1);

			}
		}
	}
		//Free resources and close SDL
		close();
		return 0;
}


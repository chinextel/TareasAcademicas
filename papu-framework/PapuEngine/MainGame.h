#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "GLS_Program.h"
#include "Sprite.h"
#include <vector>
//#include "GLTexture.h"

enum class GameState
{
	PLAY, EXIT
};


class MainGame
{
private:
	int _witdh;
	int _height;
	float _time;
	SDL_Window* _window;
	void init();
	//Sprite _sprite;
	void procesInput();
	GLS_Program _program;
	//GLTexture _texture;
	vector<Sprite*> _sprites;

public:
	MainGame();
	~MainGame();
	GameState _gameState;
	void initShaders();
	void run();
	void draw();
	void update();
};


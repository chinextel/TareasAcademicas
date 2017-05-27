#include "MainGame.h"
#include "Sprite.h"
#include "ImageLoader.h"
#include <iostream>
#include <stdio.h>      // NULL 
#include <stdlib.h>     // srand, rand 
#include <time.h>       // time 
using namespace std;

void MainGame::run() {
	srand(time(NULL));
	init();
	
	/*
	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1, -1, 1, 1, "Textures/Hongo.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0, -1, 1, 1, "Textures/Hongo.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1, 0, 1, 1, "Textures/Hongo.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0, 0, 1, 1, "Textures/Hongo.png");
	*/
	
	//_sprite.init(-1,-1,1,1, "Textures/Hongo.png");
	//_texture = ImageLoader::loadPNG("Textures/Hongo.png");
	update();
}
void MainGame::init() {
	
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Papu engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _witdh, _height, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	
	GLenum error = glewInit();
	if (error != GLEW_OK) {

	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	initShaders();
}

void MainGame::initShaders() {
	_program.compileShaders("Shaders/colorShaderVert.txt",
		"Shaders/colorShaderFrag.txt");
	_program.addAtribute("vertexPosition");
	_program.addAtribute("vertexColor");
	_program.addAtribute("vertexUV");
	_program.linkShader();
}


void MainGame::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program.use();

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _texture.id);

	GLuint imageLocation = _program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);

	GLuint timeLocation = 
		_program.getUniformLocation("time");
	
	glUniform1f(timeLocation,_time);
	_time += 0.0008;
	
	
	/*
		Si el residuo de dividir _time entre un numero es 1, entramos y creamos un nuevo sprite.
		Luego, creamos una variable r que puede tomar valores de 0 a 3. Dado el valor de r, creara
		un nuevo sprite en un de las 4 posiciones. Ademas, se puede verificar en consola cuando se
		crea un nuevo sprite.

		PD: en el if "_time" lo multiplico por 1000, luego ese resultado lo convierto en un int 
		y saco el residuo con 8000 para que no se creen los sprites tan rapido
	*/
	if ( (int(_time*1000)) % 8000 == 1)
	{
		int r = rand() % 4;

		switch (r)
		{
		case 0:
			_sprites.push_back(new Sprite());
			_sprites.back()->init(-1, -1, 1, 1, "Textures/Hongo.png");
			break;
		case 1:
			_sprites.push_back(new Sprite());
			_sprites.back()->init(0, -1, 1, 1, "Textures/Hongo.png");
			break;
		case 2:
			_sprites.push_back(new Sprite());
			_sprites.back()->init(-1, 0, 1, 1, "Textures/Hongo.png");
			break;
		case 3:
			_sprites.push_back(new Sprite());
			_sprites.back()->init(0, 0, 1, 1, "Textures/Hongo.png");
			break;

		}
		cout << "Se ha creado el sprite: " << _sprites.size()<< endl;
		
	}
	
	

	//_sprite.draw();
	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
		
	}

	_program.unuse();
	SDL_GL_SwapWindow(_window);
}

void MainGame::procesInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
			break;
		}
	}

}

void MainGame::update() {

	while (_gameState != GameState::EXIT) {
		procesInput();
		draw();
		
	}
}


MainGame::MainGame(): _window(nullptr), 
					  _witdh(800),
					  _height(600),
					  _gameState(GameState::PLAY),
					  _time(0)
{

}


MainGame::~MainGame()
{
}

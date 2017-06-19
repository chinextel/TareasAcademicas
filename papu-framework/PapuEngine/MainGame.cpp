#include "MainGame.h"
#include "ImageLoader.h"
#include <iostream>
#include "ResourceManager.h"
#include "PapuEngine.h"
#include <random>
#include <ctime>

using namespace std;

void MainGame::run() {
	init();
	update();
}

void MainGame::init() {
	Papu::init();
	_window.create("Engine", _witdh, _height, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initLevel();
	initShaders();
}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	_player = new Player();
	_currenLevel = 0;
	_player->init(1.0f, _levels[_currenLevel]->getPlayerPosition(), &_inputManager);
	_humans.push_back(_player);
	_spriteBacth.init();
	_numBala = 0; //inicializamos numBala en 0 (por defecto dispara marios si hacemos click izquierdo) 
	
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int>randPosX(
		1, _levels[_currenLevel]->getWidth()-2);
	std::uniform_int_distribution<int>randPosY(
		1, _levels[_currenLevel]->getHeight()-2);

	for (size_t i = 0; i < _levels[_currenLevel]->getNumHumans(); i++)
	{
		_humans.push_back(new Human());
		glm::vec2 pos(randPosX(randomEngine)*TILE_WIDTH, 
							randPosY(randomEngine)*TILE_WIDTH);
		_humans.back()->init(1.0f, pos);
	}
	
	const std::vector<glm::vec2>& zombiePosition =
		_levels[_currenLevel]->getZombiesPosition();
	
	for (size_t i = 0; i < zombiePosition.size(); i++)
	{
		_zombies.push_back(new Zombie());
		_zombies.back()->init(1.3f, zombiePosition[i]);
	}
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

	/*GLuint timeLocation = 
		_program.getUniformLocation("time");

	glUniform1f(timeLocation,_time);*/

	GLuint pLocation =
		_program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1,GL_FALSE, &(cameraMatrix[0][0]));

	GLuint imageLocation = _program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);

	_spriteBacth.begin();
	_levels[_currenLevel]->draw();
	
	for (size_t i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_spriteBacth);
	}
	
	for (size_t i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->draw(_spriteBacth); 
	}
	//dibujamos las balas en draw
	for (int i = 0; i < _bullets.size(); i++)
	{
		
		_bullets[i].draw(_spriteBacth, _numBala);
	}
	
	
	
	_spriteBacth.end();
	_spriteBacth.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_program.unuse();
	_window.swapBuffer();
}

void MainGame::procesInput() {
	SDL_Event event;
	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(event.motion.x,event.motion.y);
			break;
			case  SDL_KEYUP:
				_inputManager.releaseKey(event.key.keysym.sym);
				break;
			case  SDL_KEYDOWN:
				_inputManager.pressKey(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(event.button.button);
				break;
		}

		/*if (_inputManager.isKeyPressed(SDLK_w)) {
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED));
		}
		if (_inputManager.isKeyPressed(SDLK_s)) {
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAMERA_SPEED));
		}
		if (_inputManager.isKeyPressed(SDLK_a)) {
			_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0));
		}
		if (_inputManager.isKeyPressed(SDLK_d)) {
			_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0));
		}*/
		if (_inputManager.isKeyPressed(SDLK_q)) {
			_camera.setScale(_camera.getScale() + SCALE_SPEED);
		}
		if (_inputManager.isKeyPressed(SDLK_e)) {
			_camera.setScale(_camera.getScale() - SCALE_SPEED);
		}
		//si hacemos click izquierdo dispara marios hacia donde esta el mouse y todos los disparos son marios
		if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			_numBala = 0; 
			glm::vec2 mouseCoords =  _camera.convertScreenToWorl(_inputManager.getMouseCoords());
			cout << mouseCoords.x << " " << mouseCoords.y << endl;

			//glm::vec2 playerPosition(0, 0);
			glm::vec2 playerPosition = _player->getPosition();
			glm::vec2 direction = mouseCoords - playerPosition;
			direction = glm::normalize(direction);

			
			_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);

			
		}
		//si presionamos "1" dispara balones hacia donde esta el mouse y todos los disparos son balones
		if (_inputManager.isKeyPressed(SDLK_1)) {
			_numBala = 1;
			glm::vec2 mouseCoords = _camera.convertScreenToWorl(_inputManager.getMouseCoords());
			cout << mouseCoords.x << " " << mouseCoords.y << endl;

			glm::vec2 playerPosition = _player->getPosition();
			glm::vec2 direction = mouseCoords - playerPosition;
			direction = glm::normalize(direction);
		
			_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
		}
		//si presionamos "2" dispara bombas hacia donde esta el mouse y todos los disparos son bombas
		if (_inputManager.isKeyPressed(SDLK_2)) {
			_numBala = 2;
			glm::vec2 mouseCoords = _camera.convertScreenToWorl(_inputManager.getMouseCoords());
			cout << mouseCoords.x << " " << mouseCoords.y << endl;

			glm::vec2 playerPosition = _player->getPosition();
			glm::vec2 direction = mouseCoords - playerPosition;
			direction = glm::normalize(direction);
			
			_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
		}
		//si presionamos "3" dispara misiles hacia donde esta el mouse y todos los disparos son misiles
		if (_inputManager.isKeyPressed(SDLK_3)) {
			_numBala = 3;
			glm::vec2 mouseCoords = _camera.convertScreenToWorl(_inputManager.getMouseCoords());
			cout << mouseCoords.x << " " << mouseCoords.y << endl;

			glm::vec2 playerPosition = _player->getPosition();
			glm::vec2 direction = mouseCoords - playerPosition;
			direction = glm::normalize(direction);
			
			_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
		}
		//si presionamos "4" dispara pokebolas hacia donde esta el mouse y todos los disparos son pokebolas
		if (_inputManager.isKeyPressed(SDLK_4)) {
			_numBala = 4;
			glm::vec2 mouseCoords = _camera.convertScreenToWorl(_inputManager.getMouseCoords());
			cout << mouseCoords.x << " " << mouseCoords.y << endl;

			glm::vec2 playerPosition = _player->getPosition();
			glm::vec2 direction = mouseCoords - playerPosition;
			direction = glm::normalize(direction);
			
			_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
		}



	}
}

void MainGame::update() {

	while (_gameState != GameState::EXIT) {
		procesInput();
		draw();
		_camera.update();
		_time += 0.002f;
		updateAgents();
		_camera.setPosition(_player->getPosition());
		//actualiza las balas constantemente
		for (int i = 0; i < _bullets.size(); i++)
		{
			_bullets[i].update();
		}
		
	}
}

void MainGame::updateAgents() {
	for (size_t i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currenLevel]->getLevelData(),
			_humans,_zombies);
	}
	
	for (size_t i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currenLevel]->getLevelData(),
			_humans, _zombies);


		for (size_t j = 1; j < _humans.size(); j++)
		{
			if (_zombies[i]->collideWithAgent(_humans[j])) {

			}
		}
	}
}

MainGame::MainGame(): 
					  _witdh(800),
					  _height(600),
					  _gameState(GameState::PLAY),
					  _time(0),
					  _player(nullptr)
{
	_camera.init(_witdh, _height);
}


MainGame::~MainGame()
{
}

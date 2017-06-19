#include "Bullet.h"
#include "ResourceManager.h"
#include "GLTexture.h"
//al metodo draw le agregamos un parametro llamado "numBala"
void Bullet::draw(SpriteBacth& spriteBatch, int numBala) {
	Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//static GLTexture texture = ResourceManager::getTexture("Textures/Paper_Mario_.png");
	static GLTexture texture;
	/*Hacemos un switch para numBala en donde si es 1, dispara balones, si es 2 dispara bombas,
	  si es 3 dispara misiles, si es 4 dispara pokebolas y por defecto dispara marios (que vendria
	  a ser cuando numBala es 0)*/
	switch (numBala)
	{
	case 1:
		texture = ResourceManager::getTexture("Textures/balon.png");
		break;
	case 2:
	    texture = ResourceManager::getTexture("Textures/bomba.png");
		break;
	case 3:
		texture = ResourceManager::getTexture("Textures/misil.png");
		break;
	case 4:
	    texture = ResourceManager::getTexture("Textures/pokebola.png");
		break;
	default:
		texture = ResourceManager::getTexture("Textures/Paper_Mario_.png");
		break;
	}
	
	glm::vec4 posAnSize = glm::vec4(_position.x, _position.y, 30, 30);
	spriteBatch.draw(posAnSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update() {
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0) {
		return true;
	}
	return false;
}

Bullet::Bullet(glm::vec2 po, glm::vec2 dir, float speed, int lifeTime)
{
	_lifeTime = lifeTime;
	_position = po;
	_direction = dir;
	_speed = speed;
}


Bullet::~Bullet()
{
}

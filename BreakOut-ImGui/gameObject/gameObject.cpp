#include"gameObject.h"

GameObject::GameObject()
	:position(0.0f), size(1.0f), velocity(0.0f), rotation(0.0f), color(1.0f),
	sprite(), isSolid(false), destroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite,
	glm::vec3 color, glm::vec2 velocity) 
	: position(pos), size(size), sprite(sprite), color(color), velocity(velocity),
	rotation(0.0f), isSolid(false), destroyed(false) {}

void GameObject::draw(spriteRenderer& renderer) {
	renderer.drawSprite(this->sprite, this->position, this->size, this->rotation,
		this->color);
}


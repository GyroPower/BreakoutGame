#include"game.h"
#include"../GUI/GUI.h"
//#include"../gameObject/ballObject/ballObject.h"

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float PLAYER_VELOCITY(500.0f);
const float BALL_RADIUS = 12.5f;
unsigned int nrParticles = 500;
float shakeTime = 0.0f;

spriteRenderer* renderer;
GameObject* player;
ballObject* ball;
GUI* gui;


Game::Game(unsigned int width, unsigned int height) 
	:width(width), keys(), keysProcessed(), height(height), state(GAME_MENU),
	level(1),show_keys_pressed(false)
{

}

Game::~Game() {
	delete renderer;
	delete ball;
	delete player;
}

void Game::init() {
	gui = new GUI(this->width,this->height);

	ResourceManager::loadShader("shaders/spriteRenderer/spriteRenderer.vs",
		"shaders/spriteRenderer/spriteRenderer.frag", "spriteRenderer");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
		static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);

	glm::vec2 playerPos = glm::vec2(
		this->width / 2.0f - PLAYER_SIZE.x / 2.0f,
		this->height - PLAYER_SIZE.y);

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
		-BALL_RADIUS * 2.0f);

	ResourceManager::getShader("spriteRenderer").use().setInt("textureImg", 0);
	ResourceManager::getShader("spriteRenderer").use().setMat4("projection",
		projection);

	ResourceManager::loadTexture("resources/textures/player/paddle.png", true, "playerPaddle");
	ResourceManager::loadTexture("resources/textures/ball/awesomeface.png", true, "ball");
	ResourceManager::loadTexture("resources/textures/background/background.jpg", false, "background");
	ResourceManager::loadTexture("resources/textures/blocks/block.png", false, "block");
	ResourceManager::loadTexture("resources/textures/blocks/block_solid.png", false, "block_solid");

	renderer = new spriteRenderer(ResourceManager::getShader("spriteRenderer"));
	
	player = new GameObject(playerPos, PLAYER_SIZE,
		ResourceManager::getTexture("playerPaddle"));

	ball = new ballObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::getTexture("ball"));

	GameLevel one; one.load("levels/1.lvl", this->width, this->height/2);
	GameLevel two; two.load("levels/2.lvl", this->width, this->height/2);
	GameLevel three; three.load("levels/3.lvl", this->width, this->height/2);
	GameLevel four; four.load("levels/4.lvl", this->width, this->height/2 );

	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);


}


void Game::renderGUI() {

	if(this->state==GAME_MENU)
		gui->mainMenuWindow(this->state,this->level);
	if(this->show_keys_pressed)
		gui->showKeysPressed(this->show_keys_pressed);
}

void Game::render() {
	if (this->state == GAME_MENU || this->state == GAME_ACTIVE) {


		Texture2D backgroundTex = ResourceManager::getTexture("background");

		renderer->drawSprite(backgroundTex, glm::vec2(0.0f),
			glm::vec2(this->width, this->height), 0.0f);

		this->levels[this->level - 1].draw(*renderer);

		player->draw(*renderer);
	}
}

void Game::processInput(float dt) {
	//if (ImGui::IsWindowFocused()) {
		
	
	if (this->keys[GLFW_KEY_ESCAPE] && !this->keysProcessed[GLFW_KEY_ESCAPE] 
		&& this->state == GAME_ACTIVE) {
		this->keysProcessed[GLFW_KEY_ESCAPE] = true;

		this->state = GAME_MENU;
	}
	if (this->keys[GLFW_KEY_ESCAPE] && !this->keysProcessed[GLFW_KEY_ESCAPE] &&
		this->state == GAME_MENU)
	{
		this->keysProcessed[GLFW_KEY_ESCAPE] = true;
		this->state = GAME_ACTIVE;

	}

	if (this->state == GAME_ACTIVE) {
		float velocity = PLAYER_VELOCITY * dt;

		if (this->keys[GLFW_KEY_D]) {
			if (player->position.x <= this->width - player->size.x) {
				player->position.x += velocity;
					
			}
		}

		if (this->keys[GLFW_KEY_A]) {
			if (player->position.x >= 0.0f) {
					
				player->position.x -= velocity;
			}
		}

		if (this->keys[GLFW_KEY_T] && !this->keysProcessed[GLFW_KEY_T])
		{
			this->show_keys_pressed = !this->show_keys_pressed;
			this->keysProcessed[GLFW_KEY_T] = true;
		}

	}
	//}
}

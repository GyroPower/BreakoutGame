#include"game.h"
#include"../GUI/GUI.h"
#include<tuple>
#include<vector>
#include<random>
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

//a tuple to give us info for the collision detection
typedef std::tuple<bool, Direction, glm::vec2> Collision;


void show_demo() {

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("demo of move", NULL,flags)) {
		ImGui::Text("Some text");

	}
	ImGui::End();
}

Direction vectorDirection(glm::vec2 target) {
	glm::vec2 compass[]{
		glm::vec2(0.0f,1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f,-1.0f),
		glm::vec2(-1.0f,0.0f)
	};

	float max = 0.0f;

	unsigned int best_match = -1;

	for (unsigned int i = 0; i < 4; i++) {
		float dot_product = glm::dot(glm::normalize(target), compass[i]);

		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}

	return (Direction)best_match;
}

static bool checkCollisions(GameObject& one, GameObject& two) {
	bool collisionX = one.position.x + one.size.x >= two.position.x &&
		two.position.x + two.size.x >= one.position.x;

	bool collisionY = one.position.y = one.size.y >= two.position.y &&
		two.position.y + two.size.y >= one.position.y;

	return collisionX && collisionY;
}

static Collision checkCollision(ballObject& one, GameObject& two) {
	glm::vec2 center(one.position + one.radius);
	glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);

	//gui->showVec2Pos("center of ball", center);

	glm::vec2 aabb_center(
		two.position.x + aabb_half_extents.x,
		two.position.y + aabb_half_extents.y
	);


	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;
	difference = closest - center;

	//gui->showVec2Pos("closest vector in check collision", center);

	if (glm::length(difference) <= one.radius)
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f));
}

Game::Game(unsigned int width, unsigned int height) 
	:width(width), keys(), keysProcessed(), height(height), state(GAME_MENU),
	level(1),show_keys_pressed(false),close(false),lives(3)
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
		gui->mainMenuWindow(this->state,this->level,this->close);
	if(this->show_keys_pressed)
		gui->showKeysPressed(this->show_keys_pressed);
	
	if (this->state == GAME_ACTIVE) {
		//gui->showVec2Pos("player pos", player->position);
		gui->showOneVariable("Lives", std::to_string(this->lives),false,1,1,1.5f,0.5);
	}
}

void Game::render() {
	if (this->state == GAME_MENU || this->state == GAME_ACTIVE) {


		Texture2D backgroundTex = ResourceManager::getTexture("background");

		renderer->drawSprite(backgroundTex, glm::vec2(0.0f),
			glm::vec2(this->width, this->height), 0.0f);

		this->levels[this->level - 1].draw(*renderer);

		player->draw(*renderer);
		ball->draw(*renderer);
		
	}
}

void Game::update(float dt, GLFWwindow* window) {

	if (this->state == GAME_ACTIVE) {

		this->doCollisions();
		ball->move(dt, this->width);
		//gui->showVec2("ball pos", ball->position);
		if (ball->position.y >= this->height) {
			this->lives -= 1;

			if (this->lives == 0) {
				this->resetLevel();
				this->state = GAME_MENU;
			}
			this->resetPlayer();
		}
	}
	if (this->close)
		glfwSetWindowShouldClose(window, true);
}

void Game::resetLevel() {
	std::string level_to_load = "levels/" + std::to_string(this->level) + ".lvl";
	this->levels[this->level - 1].load(level_to_load.c_str(), this->width, this->height / 2);

}

void Game::resetPlayer() {
	player->position = glm::vec2(this->width / 2 - player->size.x / 2,
		this->height - player->size.y);
	ball->position = glm::vec2(player->position.x + player->size.x / 2 - ball->radius,
		-ball->radius * 2.0f);
	ball->velocity = INITIAL_BALL_VELOCITY;
	ball->stuck = true;
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
				if(ball->stuck)
					ball->position.x += velocity;
			}
		}

		if (this->keys[GLFW_KEY_A]) {
			if (player->position.x >= 0.0f) {
					
				player->position.x -= velocity;
				if(ball->stuck)
					ball->position.x -= velocity;
			}
		}

		if (this->keys[GLFW_KEY_SPACE]) {
			ball->stuck = false;

		}

		if (this->keys[GLFW_KEY_T] && !this->keysProcessed[GLFW_KEY_T])
		{
			this->show_keys_pressed = !this->show_keys_pressed;
			this->keysProcessed[GLFW_KEY_T] = true;
		}

	}
	//}
}

void Game::doCollisions() {
	int actualLevel = this->level - 1;
	for (GameObject& box : this->levels[actualLevel].bricks) {
		if (!box.destroyed) {

			Collision collision = checkCollision(*ball, box);
			
			if (std::get<0>(collision)) {
				if (!box.isSolid) {

					box.destroyed = true;
				}
				


				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);

				//if (!(ball->passThrought && !box.isSolid)) {

					if (dir == LEFT || dir == RIGHT) {
						ball->velocity.x = -ball->velocity.x;

						float penetration = ball->radius - std::abs(diff_vector.x);

						if (dir == LEFT)
							ball->position.x += penetration;
						else
							ball->position.x -= penetration;
					}
					else {
						ball->velocity.y = -ball->velocity.y;

						float penetration = ball->radius - std::abs(diff_vector.y);

						if (dir == UP)
							ball->position.y -= penetration;
						else
							ball->position.y += penetration;
					}
				//}

			}
			

		}
	}

	Collision player_col = checkCollision(*ball, *player);

	if (!ball->stuck && std::get<0>(player_col)) {
		float centerBoard = player->position.x + player->size.x / 2;
		float distance = (ball->position.x + ball->radius) - centerBoard;
		float percentage = distance / (player->size.x / 2.0f);

		float strength = 2.0f;

		glm::vec2 oldVelocity = ball->velocity;
		ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		ball->velocity.y = -1.0f * std::abs(ball->velocity.y);
		ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
		ball->stuck = ball->sticky;
	}
}
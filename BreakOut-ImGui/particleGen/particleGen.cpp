#include"particleGen.h"

GenParticle::GenParticle(Shader& shader, Texture2D& texture, unsigned int nrParticles)
 : lastUnusedParticle(0),VAO(0) {

	this->shader = shader;
	this->texture = texture;
	this->nrParticles = nrParticles;

	for (int i = 0; i < this->nrParticles; i++) {
		this->particles.push_back(particle());
	}

}

void GenParticle::respawnParticle(particle& particle, GameObject& gameObject, glm::vec2 offset) {

	float random = ((rand() % 100) - 50) / 10.0f;

	
	float rcolor = 0.5f + ((rand() % 100) / 100);

	particle.position = gameObject.position + random + offset;
	particle.color = glm::vec4(glm::vec3(rcolor), 1.0f);
	particle.life = 1.0f;
	particle.velocity = gameObject.velocity * 0.1f;
}


int GenParticle::firstUnusedParticle() {
	for (int i = this->lastUnusedParticle; i < this->nrParticles; i++) {
		if (this->particles[i].life <= 0) {
			this->lastUnusedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < this->nrParticles; i++) {
		if (this->particles[i].life <= 0) {
			this->lastUnusedParticle = i;
			return i;
		}
	}

	this->lastUnusedParticle = 0;
	return 0;

}

void GenParticle::updateParticle(float dt, GameObject& gameObject, glm::vec2 offset,
	unsigned int new_nrParticles) {

	for (unsigned int i = 0; i < new_nrParticles; i++) {
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], gameObject, offset);
	}

	for (unsigned int i = 0; i < this->nrParticles; i++) {
		particle& part = this->particles[i];
		part.life -= dt;

		if (part.life > 0.0f) {
			part.position -= part.velocity * dt;
			part.color.a -= dt * 2.5f;
		}
	}

}


void GenParticle::render() {
	
	if (this->VAO == 0)
		this->initRenderData();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.use();
	for (particle& part : this->particles) {

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(part.position, 0.0f));
		this->shader.setMat4("model", model);
		//this->shader.setVec2("offset", part.position);
		this->shader.setVec4("color", part.color);

		this->texture.Bind();

		glBindVertexArray(this->VAO);

		glDrawArrays(GL_TRIANGLES,0,6);
		glBindVertexArray(0);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GenParticle::initRenderData() {

	unsigned int VBO;

	/*float vertices[] = {
		1.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f,  0.0f, 0.0f,
		1.0f, 1.0f,  1.0f, 1.0f,

		1.0f, 0.0f,  1.0f, 0.0f,
		1.0f, 1.0f,  1.0f, 1.0f,
		0.0f ,1.0f,  0.0f, 1.0f,
	};*/

	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}
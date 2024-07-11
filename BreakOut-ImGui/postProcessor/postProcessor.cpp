#include"postProcessor.h"


PostProcessor::PostProcessor(Shader& shader, unsigned int width, unsigned int height)
	: postProcessingShader(shader), width(width), height(height), shake(false),
	confuse(false), chaos(false)
{
	glGenFramebuffers(1, &this->MSFBO);
	glGenRenderbuffers(1, &this->RBO);
	glGenFramebuffers(1, &this->FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, this->width, this->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
		this->RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	this->texture.Generate(this->width, this->height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		this->texture.id, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->initRenderData();

	this->postProcessingShader.use().setInt("scene", 0);

	float offset = 1.0f / 300.0f;

	float offsets[9][3]{
		{ -offset, offset},
		{ 0.0f, offset},
		{ offset, offset},
		{ -offset, 0.0f},
		{ 0.0f, 0.0f },
		{ offset, 0.0f },
		{ -offset, -offset },
		{ 0.0f, -offset },
		{ offset, -offset }
	};

	glUniform2fv(glGetUniformLocation(this->postProcessingShader.id, "offsets"), 9, (float*)offsets);

	int edgeKernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};

	glUniform1iv(glGetUniformLocation(this->postProcessingShader.id, "edgeKernel"), 9, edgeKernel);

	float blurKernerl[9] = {
		1.0f / 16.0f , 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f , 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f , 2.0f / 16.0f, 1.0f / 16.0f
	};

	glUniform1fv(glGetUniformLocation(this->postProcessingShader.id, "blurKernel"), 9, blurKernerl);
}

void PostProcessor::beginRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::render(float time) {
	this->postProcessingShader.use();
	this->postProcessingShader.setFloat("time", time);
	this->postProcessingShader.setBool("confuse", this->confuse);
	this->postProcessingShader.setBool("chaos", this->chaos);
	this->postProcessingShader.setBool("shake", this->shake);

	glActiveTexture(GL_TEXTURE0);
	this->texture.Bind();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::endRender() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width,
		this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::initRenderData() {

	unsigned int VBO;

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
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
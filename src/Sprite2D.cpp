#include "Sprite2D.h"

Sprite2D::Sprite2D(Shader shader) {
    this->shader = shader;
    this->initRenderData();
}

Sprite2D::~Sprite2D() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Sprite2D::drawSprite(const Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (scale, rotation, then translation)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad and resize
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    this->shader.setMat4("model", model);
    this->shader.setVec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite2D::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = { 
        // vertex   // texture
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // step by 4 as going to pass vec4 (xy coords of vertex + texture)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // unbind vao
}
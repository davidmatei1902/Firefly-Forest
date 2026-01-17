#include "Firefly.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

using namespace m1;

Firefly::Firefly() {
    // random initialization in constructor
    this->centerPosition = glm::vec3(0, 0, 0);

    this->radius = 3.0f + (rand() % 15);
    this->angle = (rand() % 360) * 3.14f / 180.0f;
    this->speed = 0.3f + ((rand() % 100) / 100.0f) * 0.5f;
    this->heightOffset = 1.5f + (rand() % 4);

    // random yellow-green color
    this->color = glm::vec3(0.6f + (rand() % 40) / 100.0f, 0.9f, 0.1f);
}

Firefly::~Firefly() {}

void Firefly::Update(float deltaTimeSeconds) {
    // increase angle
    angle += speed * deltaTimeSeconds;

    // calculate circle position
    float x = centerPosition.x + sin(angle) * radius;
    float z = centerPosition.z + cos(angle) * radius;

    // y-axis oscillation for natural look
    float y = centerPosition.y + heightOffset + sin(angle * 2.0f) * 0.5f;

    this->position = glm::vec3(x, y, z);
}

void Firefly::Render(const std::unordered_map<std::string, Mesh*>& meshes, Shader* shader,
    const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

    if (!shader || !shader->GetProgramID()) return;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // small spheres (20cm)

    RenderSimpleMesh(meshes.at("sphere"), shader, modelMatrix, color, viewMatrix, projectionMatrix);
}

void Firefly::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
    const glm::vec3& color, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    if (!mesh) return;

    shader->Use();

    // send matrices
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // send color
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    // set object type to 0 (static/no wind)
    glUniform1i(glGetUniformLocation(shader->program, "u_object_type"), 0);

    mesh->Render();
}
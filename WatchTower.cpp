#include "WatchTower.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace m1;

WatchTower::WatchTower(glm::vec3 pos) : position(pos) {}
WatchTower::~WatchTower() {}

void WatchTower::Render(const std::unordered_map<std::string, Mesh*>& meshes, Shader* shader,
    const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

    if (!shader || !shader->GetProgramID()) return;

    // tower legs
    glm::vec3 offsets[4] = {
        glm::vec3(-1.0f, 0, -1.0f), glm::vec3(1.0f, 0, -1.0f),
        glm::vec3(-1.0f, 0, 1.0f),  glm::vec3(1.0f, 0, 1.0f)
    };

    for (int i = 0; i < 4; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        // translate to position + offset + height adjustment
        modelMatrix = glm::translate(modelMatrix, position + offsets[i] + glm::vec3(0, 3.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 6.5f, 0.5f));
        RenderSimpleMesh(meshes.at("cylinder"), shader, modelMatrix, glm::vec3(0.3f, 0.2f, 0.1f), viewMatrix, projectionMatrix);
    }

    // platform
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0, 5.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.2f, 0.4f, 2.2f));
        RenderSimpleMesh(meshes.at("box"), shader, modelMatrix, glm::vec3(0.35f, 0.25f, 0.15f), viewMatrix, projectionMatrix);
    }

    // roof
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0, 8.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f, 1.0f, 1.8f));
        RenderSimpleMesh(meshes.at("cone"), shader, modelMatrix, glm::vec3(0.2f, 0.1f, 0.05f), viewMatrix, projectionMatrix);
    }
}

void WatchTower::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
    const glm::vec3& color, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    if (!mesh) return;

    shader->Use();

    // send matrices
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // send color
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    // set object type to 0 (static, no wind)
    glUniform1i(glGetUniformLocation(shader->program, "u_object_type"), 0);

    mesh->Render();
}
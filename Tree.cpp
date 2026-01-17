#include "Tree.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace m1;

Tree::Tree(glm::vec3 pos) : position(pos) {

}

Tree::~Tree() {}

void Tree::Render(const std::unordered_map<std::string, Mesh*>& meshes,
    Shader* shader,
    const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

    if (!shader || !shader->GetProgramID()) return;

    // trunk (cylinder)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0, 1.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 4.0f, 0.8f));

        glm::vec3 trunkColor = glm::vec3(0.4f, 0.25f, 0.1f); // brown

        // type = 1 (trunk -> tapering), bendStrength = 0.0 (static)
        RenderSimpleMesh(meshes.at("cylinder"), shader, modelMatrix, trunkColor, 1, 0.0f, viewMatrix, projectionMatrix);
    }

    // foliage (layers of boxes)
    float currentY = startHeight;
    float currentSpan = baseSpan;

    for (int i = 0; i < numLayers; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position + glm::vec3(0, currentY, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(currentSpan, layerThickness, constantDepth));

        // green gradient (darker at base, lighter at top)
        glm::vec3 leafColor = glm::vec3(0.0f, 0.4f + (i * 0.06f), 0.0f);

        // wind amplitude selection
        float amplitude = 0.0f;

        if (i < 2)
            amplitude = 0.01f;      // base: very small movement
        else if (i < 4)
            amplitude = 0.03f;      // middle: medium movement
        else
            amplitude = 0.08f;      // top: visible movement

        // type = 2 (foliage -> sinusoidal wind)
        RenderSimpleMesh(meshes.at("box"), shader, modelMatrix, leafColor, 2, amplitude, viewMatrix, projectionMatrix);

        // update position and size for next layer
        currentY += (layerThickness + spacing);
        currentSpan -= 0.3f;
    }
}

void Tree::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
    const glm::vec3& color, int objectType, float bendStrength,
    const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

    if (!mesh) return;

    shader->Use();

    // send standard matrices
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // send color
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    // send custom deformation variables
    glUniform1i(glGetUniformLocation(shader->program, "u_object_type"), objectType);
    glUniform1f(glGetUniformLocation(shader->program, "u_bend_strength"), bendStrength);

    // send tree height for Y normalization
    glUniform1f(glGetUniformLocation(shader->program, "u_tree_height"), 4.0f);

    mesh->Render();
}
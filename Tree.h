#pragma once
#include "components/simple_scene.h"

namespace m1 {
    class Tree {
    public:
        Tree(glm::vec3 position);
        ~Tree();

        void Render(const std::unordered_map<std::string, Mesh*>& meshes,
            Shader* shader,
            const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    private:
        glm::vec3 position;

        int numLayers = 6;
        float layerThickness = 0.35f;
        float spacing = 0.15f;
        float startHeight = 1.2f;
        float baseSpan = 2.0f;
        float constantDepth = 0.4f;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
            const glm::vec3& color, int objectType, float bendStrength,
            const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    };
}
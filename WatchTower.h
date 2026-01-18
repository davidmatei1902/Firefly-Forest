#pragma once
#include "components/simple_scene.h"

namespace m1 {
    class WatchTower {
    public:
        WatchTower(glm::vec3 position);
        ~WatchTower();

        void Render(const std::unordered_map<std::string, Mesh*>& meshes, Shader* shader,
            const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Texture2D *texture);

    private:
        glm::vec3 position;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
            const glm::vec3& color, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Texture2D *texture);
    };
}
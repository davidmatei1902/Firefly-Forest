#pragma once
#include "components/simple_scene.h"

namespace m1 {
    class Firefly {
    public:
        Firefly(); // default constructor
        ~Firefly();

        // calculate new position (orbital movement)
        void Update(float deltaTimeSeconds);

        // render function
        void Render(const std::unordered_map<std::string, Mesh*>& meshes, Shader* shader,
            const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    private:
        // physical properties
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 centerPosition; // pivot point (e.g. 0,0,0)

        // movement variables
        float angle;
        float radius;
        float speed;
        float heightOffset;

        // helper
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
            const glm::vec3& color, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    };
}
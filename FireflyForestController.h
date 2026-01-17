#pragma once
#include "components/simple_scene.h"
#include "Camera.h" 
#include "Tree.h"
#include "WatchTower.h"
#include "Firefly.h"

namespace m1
{
    class FireflyForestController : public gfxc::SimpleScene
    {
    public:
        FireflyForestController();
        ~FireflyForestController();
        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

    protected:
        game::Camera* camera;
        glm::mat4 projectionMatrix;

        // Entitatile scenei
        std::vector<Tree*> trees;
        WatchTower* watchTower;
        std::vector<Firefly*> fireflies;

        // Ceata
        float fogMaxDistance = 50.0f;
        glm::vec3 fogColor = glm::vec3(0.15f, 0.15f, 0.2f);
        bool renderFog = true;
    };
}
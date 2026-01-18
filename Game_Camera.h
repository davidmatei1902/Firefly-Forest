#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace game
{
    class Game_Camera
    {
    public:
        Game_Camera();
        Game_Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up);
        ~Game_Camera();

        // Update Game_Camera
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up);

        void MoveForward(float distance);
        void TranslateForward(float distance);
        void TranslateUpward(float distance);
        void TranslateRight(float distance);

        void RotateFirstPerson_OX(float angle);
        void RotateFirstPerson_OY(float angle);
        void RotateFirstPerson_OZ(float angle);

        void RotateThirdPerson_OX(float angle);
        void RotateThirdPerson_OY(float angle);
        void RotateThirdPerson_OZ(float angle);

        glm::mat4 GetViewMatrix();
        glm::vec3 GetTargetPosition();

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}  // namespace game

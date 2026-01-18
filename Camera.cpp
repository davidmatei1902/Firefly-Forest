#include "Camera.h"

namespace game
{
    Camera::Camera()
    {
        position = glm::vec3(0, 5, 15);
        forward = glm::vec3(0, 0, 1);

        forward = glm::normalize(glm::vec3(0, 0, 0) - position); // look at origin

        up = glm::vec3(0, 1, 0);
        right = glm::vec3(1, 0, 0);
        distanceToTarget = 2;
    }

    Camera::Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
    {
        Set(position, center, up);
    }

    Camera::~Camera() {}

    void Camera::Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
    {
        this->position = position;
        forward = glm::normalize(center - position);
        right = glm::cross(forward, up);
        this->up = glm::cross(right, forward);
    }

    void Camera::MoveForward(float distance)
    {
        position += glm::normalize(forward) * distance;
    }

    void Camera::TranslateForward(float distance)
    {
        position += glm::normalize(glm::vec3(forward.x, 0, forward.z)) * distance;
    }

    void Camera::TranslateUpward(float distance)
    {
        position += glm::normalize(glm::vec3(up.x, up.y, 0)) * distance;
    }

    void Camera::TranslateRight(float distance)
    {
        position += glm::normalize(glm::vec3(right.x, 0, right.z)) * distance;
    }

    void Camera::RotateFirstPerson_OX(float angle)
    {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, right);
        forward = glm::normalize(glm::vec3(rotation * glm::vec4(forward, 0.0f)));
        up = glm::normalize(glm::vec3(rotation * glm::vec4(up, 0.0f)));
    }

    void Camera::RotateFirstPerson_OY(float angle)
    {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
        forward = glm::normalize(glm::vec3(rotation * glm::vec4(forward, 0.0f)));
        right = glm::normalize(glm::vec3(rotation * glm::vec4(right, 0.0f)));
        up = glm::normalize(glm::cross(right, forward));
    }

    void Camera::RotateFirstPerson_OZ(float angle)
    {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, forward);
        right = glm::normalize(glm::vec3(rotation * glm::vec4(right, 0.0f)));
        up = glm::normalize(glm::vec3(rotation * glm::vec4(up, 0.0f)));
    }

    void Camera::RotateThirdPerson_OX(float angle)
    {
        TranslateForward(distanceToTarget);
        RotateFirstPerson_OX(angle);
        TranslateForward(-distanceToTarget);
    }

    void Camera::RotateThirdPerson_OY(float angle)
    {
        TranslateForward(distanceToTarget);
        RotateFirstPerson_OY(angle);
        TranslateForward(-distanceToTarget);
    }

    void Camera::RotateThirdPerson_OZ(float angle)
    {
        TranslateForward(distanceToTarget);
        RotateFirstPerson_OZ(angle);
        TranslateForward(-distanceToTarget);
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(position, position + forward, up);
    }

    glm::vec3 Camera::GetTargetPosition()
    {
        return position + forward * distanceToTarget;
    }
}  // namespace game

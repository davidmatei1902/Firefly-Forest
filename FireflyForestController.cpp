#include "FireflyForestController.h"

#include <vector>
#include <string>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace m1;

FireflyForestController::FireflyForestController()
{
    camera = nullptr;
    watchTower = nullptr;
}

FireflyForestController::~FireflyForestController()
{
    if (camera) delete camera;
    if (watchTower) delete watchTower;

    for (auto t : trees) delete t;
    trees.clear();

    for (auto f : fireflies) delete f;
    fireflies.clear();
}

void FireflyForestController::Init()
{
    // initialize camera
    camera = new game::Camera();
    camera->Set(glm::vec3(0, 8, 25), glm::vec3(0, 4, 0), glm::vec3(0, 1, 0));

    // load meshes
    {
        string path = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives");
        Mesh* mesh = new Mesh("box"); mesh->LoadMesh(path, "box.obj"); meshes[mesh->GetMeshID()] = mesh;
        mesh = new Mesh("cone"); mesh->LoadMesh(path, "cone.obj"); meshes[mesh->GetMeshID()] = mesh;
        mesh = new Mesh("cylinder"); mesh->LoadMesh(path, "cylinder.obj"); meshes[mesh->GetMeshID()] = mesh;
        mesh = new Mesh("plane"); mesh->LoadMesh(path, "plane50.obj"); meshes[mesh->GetMeshID()] = mesh;
        mesh = new Mesh("sphere"); mesh->LoadMesh(path, "sphere.obj"); meshes[mesh->GetMeshID()] = mesh;
    }

    // load shader
    {
        Shader* shader = new Shader("ForestShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // projection
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // tower
    watchTower = new WatchTower(glm::vec3(0, 0, 0));

    // forest generation
    for (int i = 0; i < 50; i++) {
        // generate coordinates between -30 and 30
        float x = (rand() % 600) / 10.0f - 30.0f;
        float z = (rand() % 600) / 10.0f - 30.0f;

        // simple check: if too close to tower (center), skip
        if (abs(x) < 4.0f && abs(z) < 4.0f) {
            continue;
        }

        trees.push_back(new Tree(glm::vec3(x, 0, z)));
    }

    // fireflies
    for (int i = 0; i < 20; i++) {
        fireflies.push_back(new Firefly());
    }
}

void FireflyForestController::FrameStart()
{
    glClearColor(fogColor.r, fogColor.g, fogColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void FireflyForestController::Update(float deltaTimeSeconds)
{
    Shader* shader = shaders["ForestShader"];
    if (!shader || !shader->GetProgramID()) return;

    shader->Use();

    // global settings
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(camera->position));
    glUniform1i(glGetUniformLocation(shader->program, "u_render_fog"), renderFog ? 1 : 0);
    glUniform1f(glGetUniformLocation(shader->program, "fog_max_dist"), fogMaxDistance);
    glUniform3fv(glGetUniformLocation(shader->program, "fog_color"), 1, glm::value_ptr(fogColor));

    // time uniform
    glUniform1f(glGetUniformLocation(shader->program, "Time"), (float)Engine::GetElapsedTime());

    // render terrain
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        // color: dark forest green
        RenderSimpleMesh(meshes["plane"], shader, modelMatrix, glm::vec3(0.05f, 0.15f, 0.05f));
    }

    // tower
    if (watchTower) {
        watchTower->Render(meshes, shader, camera->GetViewMatrix(), projectionMatrix);
    }

    // trees
    for (auto tree : trees) {
        tree->Render(meshes, shader, camera->GetViewMatrix(), projectionMatrix);
    }

    // fireflies
    for (auto firefly : fireflies) {
        firefly->Update(deltaTimeSeconds);
        firefly->Render(meshes, shader, camera->GetViewMatrix(), projectionMatrix);
    }
}

void FireflyForestController::FrameEnd() {}

void FireflyForestController::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    glUniform1i(glGetUniformLocation(shader->program, "u_object_type"), 0);
    glUniform1f(glGetUniformLocation(shader->program, "u_bend_strength"), 0.0f);

    mesh->Render();
}

void FireflyForestController::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 10.0f;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (window->KeyHold(GLFW_KEY_W)) camera->TranslateForward(speed * deltaTime);
        if (window->KeyHold(GLFW_KEY_S)) camera->TranslateForward(-speed * deltaTime);
        if (window->KeyHold(GLFW_KEY_A)) camera->TranslateRight(-speed * deltaTime);
        if (window->KeyHold(GLFW_KEY_D)) camera->TranslateRight(speed * deltaTime);
        if (window->KeyHold(GLFW_KEY_Q)) camera->TranslateUpward(-speed * deltaTime);
        if (window->KeyHold(GLFW_KEY_E)) camera->TranslateUpward(speed * deltaTime);
    }
}

void FireflyForestController::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F) renderFog = !renderFog;
}

void FireflyForestController::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivity = 0.001f;
        camera->RotateFirstPerson_OX(-deltaY * sensivity);
        camera->RotateFirstPerson_OY(-deltaX * sensivity);
    }
}
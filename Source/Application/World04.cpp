#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>

#define INTERLEAVE

namespace nc
{
    bool World04::Initialize()
    {
        /*m_program = GET_RESOURCE(Program, "Shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "Textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);*/
        auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/cube.obj", glm::vec3{ 0 }, glm::vec3{-90, 0, 0});

        return true;
    }


    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        
        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();

        ImGui::Begin("Light Controls");
        ImGui::ColorEdit3("Ambient Color", &ambientLight[0], 0.1f);
        ImGui::DragFloat3("Light Position", &lightPosition[0]);
        ImGui::ColorEdit3("Light Color", &lightColor[0], 0.1f);
        ImGui::End();

        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? m_speed * +dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? m_speed * +dt : 0;

        m_time += dt;


        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();

        // Model Matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
        material->GetProgram()->SetUniform("light.position", lightPosition);
        material->GetProgram()->SetUniform("light.color", lightColor);
        material->GetProgram()->SetUniform("ambientLight", ambientLight);

        // View matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);

        //Projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / float(ENGINE.GetSystem<Renderer>()->GetHeight()), .01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);
        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
       
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw(GL_TRIANGLES);
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}

#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace nc
{
    bool World04::Initialize()
    {
        /*auto material = GET_RESOURCE(Material, "Materials/multi.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/plane.obj");
        m_transform.position.y = -1.0f;*/
        //m_model->Load("models/plane.obj", glm::vec3{ 0 }, glm::vec3{-90, 0, 0});
        m_material = GET_RESOURCE(Material, "materials/squirrel.mtrl");
        m_model = std::make_shared<Model>();
        m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });

        for (int i = 0; i < 3; i++) {
            m_lights[i].type = light_t::eType::Point;
            m_lights[i].position = glm::vec3{ randomf(-5, 5), randomf(1, 8), randomf(-5, 5) };
            m_lights[i].direction = glm::vec3{ 0, -1, 0 };
            m_lights[i].color = glm::rgbColor(glm::vec3{ randomf(), 1, 1});
            m_lights[i].intensity = 1;
            m_lights[i].range = 5;
            m_lights[i].innerAngle = 10.0f;
            m_lights[i].outerAngle = 30.0f;
        }

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

        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientLight));
        ImGui::Separator();

        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), m_selected == i)) m_selected = i;
        }
        ImGui::End();

        ImGui::Begin("Light Controls");
        const char* types[] = { "Point", "Directional", "Spot" };
        ImGui::Combo("Type", (int*)(&m_lights[m_selected].type), types, 3);

        if (m_lights[m_selected].type != light_t::Directional) ImGui::DragFloat3("Light Position", &m_lights[m_selected].position[0]);
        if (m_lights[m_selected].type != light_t::Point) ImGui::DragFloat3("Light Direction", &m_lights[m_selected].direction[0], 0.1f);
        if (m_lights[m_selected].type == light_t::Spot) {
            ImGui::DragFloat("Inner Angle", &m_lights[m_selected].innerAngle, 1, 0, m_lights[m_selected].outerAngle);
            ImGui::DragFloat("Outer Angle", &m_lights[m_selected].outerAngle, 1, m_lights[m_selected].innerAngle, 90);
        }

        ImGui::ColorEdit3("Light Color", glm::value_ptr(m_lights[m_selected].color), 0.1f);
        ImGui::DragFloat("Intensity", &m_lights[m_selected].intensity, 0.1f, 0, 10);
        if (m_lights[m_selected].type != light_t::Directional) ImGui::DragFloat("Range", &m_lights[m_selected].range, 0.1f, 0.1f, 100);
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientLight), 0.1f);
        ImGui::End();

        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? m_speed * +dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? m_speed * +dt : 0;

        m_time += dt;


        m_material->ProcessGui();
        m_material->Bind();


        // Model Matrix
        for (int i = 0; i < 3; i++) {
            std::string name = "lights[" + std::to_string(i) + "]";
            m_material->GetProgram()->SetUniform(name + ".position", m_lights[i].position);
            m_material->GetProgram()->SetUniform(name + ".direction", glm::normalize(m_lights[i].direction));
            m_material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
            m_material->GetProgram()->SetUniform(name + ".color", m_lights[i].color);
            m_material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
            m_material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
            m_material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
            m_material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
        }

        m_material->GetProgram()->SetUniform("ambientLight", ambientLight);


        m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
        // View matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        m_material->GetProgram()->SetUniform("view", view);

        //Projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / float(ENGINE.GetSystem<Renderer>()->GetHeight()), .01f, 100.0f);
        m_material->GetProgram()->SetUniform("projection", projection);
        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
       
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_material->Bind();
        m_model->Draw();
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}

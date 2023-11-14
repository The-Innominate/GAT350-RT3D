#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_framingbuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        return true;
    }


    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

       /* xGrayscale Effect
          xColor Tinting Effect
          xInvert Effect
            Grain Effect
            Scanline Effect
            Custom Effect(chosen by the student)*/
        //Set post process gui
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect)) {
            if (effect) m_params |= INVERT_MASK;
            else m_params &= ~INVERT_MASK;
        }
        effect = m_params & GREYSCALE_MASK;
        if (ImGui::Checkbox("Greyscale", &effect)) {
            if (effect) m_params |= GREYSCALE_MASK;
            else m_params &= ~GREYSCALE_MASK; 
        }
        effect = m_params & COLORTINT_MASK;
        ImGui::ColorEdit3("Color Tint", glm::value_ptr(m_colorTint), 0.01f);
        if (ImGui::Checkbox("Color Tint", &effect)) {
			if (effect) m_params |= COLORTINT_MASK;
			else m_params &= ~COLORTINT_MASK;
		}
        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect)) {
			if (effect) m_params |= GRAIN_MASK;
			else m_params &= ~GRAIN_MASK;
		}
        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect)) {
            if (effect) m_params |= SCANLINE_MASK;
			else m_params &= ~SCANLINE_MASK;
        }
        effect = m_params & CUSTOM_MASK;
        if (ImGui::Checkbox("Custom", &effect)) {
			if (effect) m_params |= CUSTOM_MASK;
            else m_params &= ~CUSTOM_MASK;
        }
        
        

        ImGui::End();

        //Set post process shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program) {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("tint", m_colorTint);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // **** PASS 1 ****
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewPort(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame(glm::vec3{ 0, 0, 1 });
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // **** PASS 2 ****
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewPort();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        // post renderer
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}

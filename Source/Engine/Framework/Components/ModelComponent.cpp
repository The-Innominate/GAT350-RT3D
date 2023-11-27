#include "ModelComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"
#include "Core/Core.h"

namespace nc
{
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize()
	{
		//if (!modelName.empty()) m_model = GET_RESOURCE(Model, modelName);
		if (!modelName.empty())
		{
			model = GET_RESOURCE(Model, modelName);
			//ADD_RESOURCE(modelName, model);
		}
		if (model && !materialName.empty())
		{
			material = (GET_RESOURCE(Material, materialName));
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullFace);

		model->Draw();
	}

	void ModelComponent::ProcessGui()
	{
		(model) ? ImGui::Text("Model: %s", model->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogResource<Model>(model, "ModelTextureDialog", "Open Model", "Model file (*.obj;*.fbx){.obj,.fbx},.*");

		(material) ? ImGui::Text("Material: %s", material->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogResource<Material>(material, "MaterialTextureDialog", "Open Material", "Material file (*.mtrl){.mtrl},.*");

		ImGui::Checkbox("Cast Shadow", &castShadow);
		ImGui::Checkbox("Enable Depth", &enableDepth);
	}

	void ModelComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);

		READ_DATA(value, enableDepth);
		READ_DATA(value, castShadow);

		std::string cullfaceName;
		READ_NAME_DATA(value, "cullface", cullfaceName);
		if (StringUtils::IsEqualIgnoreCase(cullfaceName, "front")) cullFace = GL_FRONT;
		//if (StringUtils::IsEqualIgnoreCase(cullfaceName, "none")) cullFace = GL_NONE;
	}
}
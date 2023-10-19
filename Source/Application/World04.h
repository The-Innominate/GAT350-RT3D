#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "core/math/Transform.h"
#include <vector>

namespace nc
{
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;

		Transform m_transform;
		res_t<Model> m_model;

		glm::vec3 lightPosition = { 0.0f, 0.0f, 3.0f };
		glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
		glm::vec3 ambientLight = { 0.1f, 0.1f, 0.1f };
	};
}
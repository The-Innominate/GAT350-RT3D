#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "core/math/Transform.h"
#include <vector>


namespace nc
{
	class World05 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;

		float m_refraction{ 1 };
		glm::vec3 ambientLight = { 0.1f, 0.1f, 0.1f };
	};
}
#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "core/math/Transform.h"
#include <vector>


namespace nc
{
	class World07 : public World
	{
	public: 
		const uint32_t INVERT_MASK = (1 << 0);
		const uint32_t GREYSCALE_MASK = (1 << 1);
		const uint32_t COLORTINT_MASK = (1 << 2);
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;
		float m_blend{ 1 };

		uint32_t m_params{ 1 };

		float m_refraction{ 1 };
		glm::vec3 ambientLight = { 0.1f, 0.1f, 0.1f };
	};
}
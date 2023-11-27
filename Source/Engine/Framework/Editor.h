#pragma once

namespace nc {
	class Editor {
	public:
		void Update();
		void ProcessGui(class Scene* scene);
	private:
		bool m_active = false;
		class Object* m_selected = nullptr;

	};
}
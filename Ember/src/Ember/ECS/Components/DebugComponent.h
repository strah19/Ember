#ifndef DEBUG_COMPONENT_H
#define DEBUG_COMPONENT_H

#include "../../Renderer/2DRenderer.h"
#include "../../Core/EventHandlers/Input.h"
#include "../../Core/GuiCore/Button.h"
#include "../Entity.h"
#include "PositionComponent.h"

namespace ember {
	class DebugComponent : public Component {
	public:
		DebugComponent(Input* input);

		void Init();
		void Render();
		void Update();
	private:
		Input* input;
		PositionComponent* position;

		bool moving = false;
		int px = 0, py = 0;
		Button button;
	};
}

#endif // !DEBUG_COMPONENT_H

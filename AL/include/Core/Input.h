#ifndef INPUT_H
#define INPUT_H

#include "Core/Base.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"
#include <glm/glm.hpp>

namespace ale
{
class Input
{
  public:
	static bool isKeyPressed(KeyCode key);

	static bool isMouseButtonPressed(MouseCode button);
	static glm::vec2 getMousePosition();
	static float getMouseX();
	static float getMouseY();
};
} // namespace ale

#endif
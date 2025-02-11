#include "Scene/SceneCamera.h"
#include "Alpch.h"

namespace ale
{

void SceneCamera::updateSceneCamera(glm::vec3 &pos, glm::vec3 &rot)
{
	setPosition(pos);
	setRotation(rot);
	updateViewMatrix();
}

} // namespace ale
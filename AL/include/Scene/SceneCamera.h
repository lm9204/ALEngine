#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "Renderer/Camera.h"

namespace ale
{
// camera maybe extended by various types - orthographic, perspective
class SceneCamera : public Camera
{
  public:
	SceneCamera() = default;
	virtual ~SceneCamera() = default;
};
} // namespace ale

#endif
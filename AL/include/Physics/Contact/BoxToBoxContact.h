#pragma once

#include "Physics/Contact/Contact.h"

namespace ale
{
class BoxToBoxContact : public Contact
{
  public:
	static Contact *create(Fixture *fixtureA, Fixture *fixtureB, int32_t indexA, int32_t indexB);
	BoxToBoxContact(Fixture *fixtureA, Fixture *fixtureB, int32_t indexA, int32_t indexB);

	virtual glm::vec3 supportA(const ConvexInfo &box, glm::vec3 dir) override;
	virtual glm::vec3 supportB(const ConvexInfo &box, glm::vec3 dir) override;
	virtual void findCollisionPoints(const ConvexInfo &boxA, const ConvexInfo &boxB, CollisionInfo &collisionInfo,
									 EpaInfo &epaInfo, SimplexArray &simplexArray) override;
};
} // namespace ale
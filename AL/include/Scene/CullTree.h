#pragma once

#include "Renderer/Common.h"
#include "Scene/Entity.h"

namespace ale
{

#define NULL_NODE (-1)

struct CullSphere
{
	glm::vec3 center;
	float radius;

	CullSphere(glm::vec3 &center, float radius)
		: center(center), radius(radius) {};

	float getVolume()
	{
		return radius * radius * radius;
	}

	void combine(const CullSphere &sphere1, const CullSphere &sphere2)
	{
		glm::vec3 centerDiff = sphere2.center - sphere1.center;
		float distance = glm::length(centerDiff);

		// sphere1이 sphere2를 포함
		if (sphere1.radius >= distance + sphere2.radius)
		{
			center = sphere1.center;
			radius = sphere1.radius;
			return;
		}

		// sphere2가 sphere1을 포함
		if (sphere2.radius >= distance + sphere1.radius)
		{
			center = sphere2.center;
			radius = sphere2.radius;
			return;
		}

		radius = (distance + sphere1.radius + sphere2.radius) * 0.5f;

		// 새로운 중심: 기존 중심을 거리 비율로 보간하여 위치 지정
		center = sphere1.center;
		center += centerDiff * ((radius - sphere1.radius) / distance);
	}
};

struct FrustumPlane
{
	float distance;
	glm::vec3 normal;

	FrustumPlane() = default;

	// CCW
	FrustumPlane(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
		: normal(glm::normalize(glm::cross(p2 - p1, p3 - p1))), distance(glm::dot(normal, p1)) {};
};

enum class EFrustum
{
	OUTSIDE,
	INSIDE,
	INTERSECT,
};

struct Frustum
{
	// 0: near, 1: far, 2: left, 3: right, 4: up, 5: down
	FrustumPlane plane[6];

	EFrustum cullingSphere(const CullSphere &sphere) const
	{
		bool intersect = false;

		for (int32_t i = 0; i < 6; ++i)
		{
			float dotResult = glm::dot(plane[i].normal, sphere.center);
			float distance = plane[i].distance;
			if (dotResult > distance + sphere.radius)
			{
				return EFrustum::OUTSIDE;
			}
			else if (dotResult > distance - sphere.radius)
			{
				intersect = true;
			}
		}

		if (intersect == true)
		{
			return EFrustum::INTERSECT;
		}
		else
		{
			return EFrustum::INSIDE;
		}
	}
};

struct CullTreeNode
{
	bool isLeaf() const
	{
		return child1 == NULL_NODE;
	}

	CullSphere sphere;
	entt::entity entityHandle;

	union {
		int32_t parent;
		int32_t next;
	};
	int32_t child1;
	int32_t child2;
	int32_t height;
};

class CullTree
{
  public:
	// Dynamic Tree 생성
	CullTree();
	~CullTree() = default;

	void updateTree();
	void destroyNode(int32_t nodeId);
	void setScene(Scene *scene);
	void setRenderEnable(int32_t nodeId);
	void setRenderDisable(int32_t nodeId);
	void frustumCulling(const Frustum &frustum, int32_t nodeId);
	int32_t createNode(const CullSphere &sphere, entt::entity entityHandle);

	int32_t getRootNodeId();

	// void printDynamicTree(int32_t nodeId)

	// const AABB &getFatAABB(int32_t proxyId) const;

  private:
	// 가장 최적의 위치를 찾아 node 삽입, balance
	void freeNode(int32_t nodeId);
	void insertLeaf(int32_t leaf);
	void detachNode(int32_t nodeId);
	bool moveNode(int32_t nodeId, const CullSphere &newSphere);
	float getInsertionCost(const CullSphere &leafSphere, int32_t child, float inheritedCost);
	float getInsertionCostForLeaf(const CullSphere &leafSphere, int32_t child, float inheritedCost);
	int32_t balance(int32_t index);
	int32_t allocateNode();

	Scene *m_scene;
	int32_t m_root;
	int32_t m_freeNode;
	int32_t m_nodeCount;
	int32_t m_nodeCapacity;
	std::vector<CullTreeNode> m_nodes;
};

} // namespace ale

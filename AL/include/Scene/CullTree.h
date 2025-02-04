#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

namespace ale
{

#define NULL_NODE (-1)

struct CullSphere
{
	glm::vec3 center;
	float radius;

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

struct CullTreeNode
{
	bool isLeaf() const
	{
		return child1 == NULL_NODE;
	}

	CullSphere sphere;
	uint32_t handle;

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

	// 주어진 aabb와 userData로 node에 값 초기화, node 삽입
	int32_t createNode(const CullSphere &sphere, uint32_t handle);

	void destroyNode(int32_t nodeId);

	// proxyId에 해당하는 node 삭제 후, 적당한 위치로 다시 Insert
	bool moveNode(int32_t nodeId, const glm::vec3 &displacement);

	// 해당 노드부터 하위 노드들까지 render flag를 전부 true로 설정
	void setRenderEnable(int32_t nodeId);

	// 해당 노드부터 하위 노드들까지 render flag를 전부 false로 설정
	void setRenderDisable(int32_t nodeId);

	void frustumCulling(int32_t nodeId);
	
	// void printDynamicTree(int32_t nodeId)

	// const AABB &getFatAABB(int32_t proxyId) const;

  private:
	// 가장 최적의 위치를 찾아 node 삽입, balance
	void insertLeaf(int32_t leaf);
	void removeLeaf(int32_t leaf);
	void freeNode(int32_t nodeId);

	// node 삭제 (but 해당 leaf 재사용할 때 사용하는 함수)
	void detachNode(int32_t nodeId);

	// 트리가 쏠리지 않게 balance 맞춰줌
	int32_t balance(int32_t index);
	int32_t allocateNode();

	float getInsertionCostForLeaf(const CullSphere &leafSphere, int32_t child, float inheritedCost);
	float getInsertionCost(const CullSphere &leafSphere, int32_t child, float inheritedCost);

	int32_t m_root;
	int32_t m_freeNode;
	int32_t m_nodeCount;
	int32_t m_nodeCapacity;
	std::vector<CullTreeNode> m_nodes;
};

} // namespace ale

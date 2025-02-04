#pragma once

#include <glm/glm.hpp>

namespace ale
{

#define emptyNode (-1)

struct CullSphere
{
	glm::vec3 center;
	float radius;
};

struct CullTreeNode
{
	bool isLeaf() const
	{
		return child1 == emptyNode;
	}

	CullSphere sphere;
	uint32_t entityId;

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
	~CullTree();

	// 주어진 aabb와 userData로 node에 값 초기화, node 삽입
	int32_t createNode(const AABB &aabb, void *userData);

	// proxyId에 해당하는 node Destroy
	void destroyNode(int32_t proxyId);

	// proxyId에 해당하는 node 삭제 후, 적당한 위치로 다시 Insert
	bool moveNode(int32_t proxyId, const AABB &aabb, const glm::vec3 &displacement);

	// 해당 노드부터 하위 노드들까지 render flag를 전부 true로 설정
	void setRenderEnable();

	// 해당 노드부터 하위 노드들까지 render flag를 전부 false로 설정
	void setRenderDisable();

	void frustumCulling();

	// const AABB &getFatAABB(int32_t proxyId) const;

  private:
	// 가장 최적의 위치를 찾아 node 삽입, balance
	void insertLeaf(int32_t leaf);
	void freeNode(int32_t nodeId);

	// node 삭제
	void removeLeaf(int32_t leaf);

	// 트리가 쏠리지 않게 balance 맞춰줌
	int32_t balance(int32_t index);
	int32_t allocateNode();

	float getInsertionCostForLeaf(const AABB &leafAABB, int32_t child, float inheritedCost);
	float getInsertionCost(const AABB &leafAABB, int32_t child, float inheritedCost);

	int32_t m_root;
	int32_t m_freeNode;
	int32_t m_nodeCount;
	int32_t m_nodeCapacity;
	std::vector<CullTreeNode> m_nodes;
};

} // namespace ale

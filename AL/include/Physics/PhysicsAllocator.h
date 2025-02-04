#pragma once

#include "Memory/BlockAllocator.h"
#include "Memory/StackAllocator.h"

namespace ale
{
class PhysicsAllocator
{
  public:
	PhysicsAllocator() = default;
	~PhysicsAllocator() = default;

	static BlockAllocator m_blockAllocator;
	static StackAllocator m_stackAllocator;
};

} // namespace ale
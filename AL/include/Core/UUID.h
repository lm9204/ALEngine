#pragma once

#include <stdint.h>

namespace ale
{
class UUID
{
  public:
	UUID();
	UUID(uint64_t uuid);
	UUID(const UUID &other) = default;

	// UUID를 uint64_t로 쓰기 위해 재정의.
	operator uint64_t() const
	{
		return m_UUID;
	}

  private:
	uint64_t m_UUID;
};
} // namespace ale

namespace std
{
// UUID로 unordered map hashing을 쓰기 위해 정의.
template <typename T> struct hash;

// template specialization
template <> struct hash<ale::UUID>
{
	size_t operator()(const ale::UUID &uuid) const
	{
		return (uint64_t)uuid;
	}
};

} // namespace std
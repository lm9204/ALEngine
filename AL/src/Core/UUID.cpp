#include "alpch.h"

#include "Core/UUID.h"
#include <random>
#include <unordered_map>

namespace ale
{
// seed generator
static std::random_device s_RandomDevice;
// 64bit random number generator
static std::mt19937_64 s_Engine(s_RandomDevice());
// uniform distributor - range is same as input random number
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

UUID::UUID() : m_UUID(s_UniformDistribution(s_Engine))
{
}

UUID::UUID(uint64_t uuid) : m_UUID(uuid)
{
}
} // namespace ale
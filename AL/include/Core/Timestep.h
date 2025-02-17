#pragma once

#include "Core/Base.h"
#include "Renderer/Common.h"

using namespace std::chrono_literals;

namespace ale
{

namespace Chrono
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using Duration = std::chrono::duration<float, std::chrono::seconds::period>;
} // namespace Chrono
class Timestep
{
  public:
	Timestep();
	Timestep(std::chrono::duration<float, std::chrono::seconds::period> time);

	std::chrono::duration<float, std::chrono::seconds::period> getSeconds() const;
	std::chrono::duration<float, std::chrono::milliseconds::period> getMilliseconds() const;

	void print() const;
	float count() const;

	Timestep& operator=(const std::chrono::duration<float, std::chrono::seconds::period>& timestep);
	Timestep& operator-=(const Timestep& other);
	Timestep operator-(const Timestep& other) const;
	Timestep operator*(float factor) const;
	bool operator<=(const std::chrono::duration<float, std::chrono::seconds::period>& other) const;

	operator float() const { return m_Timestep.count(); }
	glm::vec3 operator*(const glm::vec3& other) const
	{
		auto ts = m_Timestep.count();
		return glm::vec3(ts * other.x, ts * other.y, ts * other.z);
	}

  private:
	std::chrono::duration<float, std::chrono::seconds::period> m_Timestep;
};
} // namespace ale

#endif
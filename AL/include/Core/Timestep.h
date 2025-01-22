#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <chrono>

#include "Core/Base.h"

using namespace std::chrono_literals;

namespace ale
{
class AL_API Timestep
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
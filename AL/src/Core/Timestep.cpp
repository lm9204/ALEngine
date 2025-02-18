#include "Core/Timestep.h"
#include "Renderer/Common.h"

namespace ale
{
Timestep::Timestep() : m_Timestep(0) {} // temp

Timestep::Timestep(std::chrono::duration<float, std::chrono::seconds::period> time) : m_Timestep(time) {}

Timestep& Timestep::operator=(const std::chrono::duration<float, std::chrono::seconds::period>& timestep)
{
	m_Timestep = timestep;
	return *this;
}

Timestep& Timestep::operator-=(const Timestep& other)
{
	m_Timestep = m_Timestep - other.m_Timestep;
	return *this;
}
Timestep Timestep::operator-(const Timestep& other) const { return m_Timestep - other.m_Timestep; }

Timestep Timestep::operator*(float factor) const { return Timestep(m_Timestep * factor); }

bool Timestep::operator<=(const std::chrono::duration<float, std::chrono::seconds::period>& other) const
{
	return (m_Timestep - other) <= 0ms;
}

std::chrono::duration<float, std::chrono::seconds::period> Timestep::getSeconds() const { return m_Timestep; }

std::chrono::duration<float, std::chrono::milliseconds::period> Timestep::getMiliSeconds() const
{
	return (std::chrono::duration<float, std::chrono::milliseconds::period>)m_Timestep;
}

void Timestep::print() const
{
	auto inMilliSeconds = getMiliSeconds();
	std::cout << "timestep in milli seconds: " << inMilliSeconds.count() << "ms\n";
	auto inSeconds = getSeconds();
	std::cout << "timestep in seconds: " << inSeconds.count() << "s\n";
}

float Timestep::count() const { return m_Timestep.count(); }
} //namespace ale
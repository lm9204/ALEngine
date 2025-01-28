#include "Renderer/SAComponent.h"

namespace ale
{

SAComponent::SAComponent() : m_Repeat(0), m_FirstKeyFrameTime(0), m_CurrentKeyFrameTime(0), m_LastKeyFrameTime(0), m_FrameCounter(0), m_SpeedFactor(1.0f)
{
	m_CurrentAnimation = nullptr;
}

void SAComponent::updateAnimation(const Timestep& timestep, uint32_t currentFrame)
{
	if (m_CurrentAnimation)
	{
		m_CurrentAnimation->uploadData(this->getData());
		m_Model->updateAnimations(m_CurrentAnimation, timestep, m_SpeedFactor, m_FrameCounter, currentFrame);
		this->setData(m_Model->getAnimCurrentFrame() ,m_CurrentAnimation->getData());
	}
}

struct SAData SAComponent::getData() const
{
	struct SAData data{};
	data.m_Repeat = m_Repeat;
	data.m_FirstKeyFrameTime = m_FirstKeyFrameTime;
	data.m_LastKeyFrameTime = m_LastKeyFrameTime;
	data.m_CurrentKeyFrameTime = m_CurrentKeyFrameTime;

	return data;
}

void SAComponent::setData(uint16_t currentFrame, const SAData& data)
{
	m_FrameCounter = currentFrame;

	m_Repeat = data.m_Repeat;
	m_FirstKeyFrameTime = data.m_FirstKeyFrameTime;
	m_LastKeyFrameTime = data.m_LastKeyFrameTime;
	m_CurrentKeyFrameTime = data.m_CurrentKeyFrameTime;
}

} //namespace ale
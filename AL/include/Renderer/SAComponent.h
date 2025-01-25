#ifndef SACOMPONENT_H
#define SACOMPONENT_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Model.h"
#include "Renderer/Animation/SkeletalAnimations.h"

namespace ale
{
class SAComponent
{
public:
	SAComponent();
	void updateAnimation(const Timestep& timestep, uint32_t currentFrame);
	struct SAData getData() const;
	void setData(uint16_t currentFrame, const SAData& data);

	SkeletalAnimation* m_CurrentAnimation;
	std::shared_ptr<Model> m_Model;

private:
	uint16_t m_FrameCounter;

	bool	m_Repeat;
	float	m_FirstKeyFrameTime;
	float	m_LastKeyFrameTime;
	float	m_CurrentKeyFrameTime;

	
};
} //namespace ale

#endif
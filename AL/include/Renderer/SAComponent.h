#ifndef SACOMPONENT_H
#define SACOMPONENT_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Model.h"
#include "Renderer/Animation/SkeletalAnimations.h"
#include "Renderer/AnimationStateManager.h"

namespace ale
{

class SAComponent
{
	using Bones = std::vector<Armature::Bone>;
public:
	SAComponent(std::shared_ptr<Model>& model);

	void start(std::string const& animation);
	void start(size_t index);
	void start() { start(0); };
	void stop() { m_CurrentAnimation->stop(); };
	void setRepeat(bool repeat);
	void setRepeatAll(bool repeat);
	bool isRunning() const { return m_CurrentAnimation->isRunning(); };
	bool willExpire(const Timestep& timestep) const { return m_CurrentAnimation->willExpire(timestep); };
	float getDuration(std::string const& animation) { return m_CurrentAnimation->getDuration(); };
	float getCurrentTime() { return m_CurrentAnimation->getCurrentTime(); };
	uint16_t getCurrentFrame() { return m_FrameCounter; };

	void updateAnimation(const Timestep& timestep, uint32_t currentFrame);
	struct SAData getData(unsigned int index = 0) const;
	void setData(uint16_t currentFrame, const SAData& data, unsigned int index);
	void setSpeedFactor(float factor);
	void setCurrentFrame(uint32_t currentFrame) { m_FrameCounter = currentFrame; };

	Bones blendBones(Bones& to, Bones& from, float blendFactor);
public:
	std::shared_ptr<AnimationStateManager> m_StateManager;

private:
	SAComponent();
	void init(SkeletalAnimation* animation);
	void blendUpdate(const Timestep& timestep, Armature::Skeleton& skeleton, uint32_t currentFrame);
	int getAnimIndex(const std::string& name);
	bool getAnimRepeat(SkeletalAnimation* animation);

private:
	SkeletalAnimation* m_CurrentAnimation;
	std::shared_ptr<SkeletalAnimations> m_Animations;
	std::shared_ptr<Armature::Skeleton> m_Skeleton;
	std::shared_ptr<Model> m_Model;

	float	m_SpeedFactor;
	uint32_t m_FrameCounter;
	std::vector<bool> m_Repeats;

	struct SAData m_Data[2];

};
} //namespace ale

#endif
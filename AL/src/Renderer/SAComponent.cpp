#include "Renderer/SAComponent.h"

namespace ale
{

SAComponent::SAComponent(std::shared_ptr<Model>& model) :
	m_SpeedFactor(1.0f)
{
	m_CurrentAnimation = nullptr;
	m_StateManager = std::make_shared<AnimationStateManager>();
	// Init Keyframe Data
	for (unsigned int i = 0; i < 2; ++i)
	{
		m_Data[i].m_CurrentKeyFrameTime = 0.0f;
		m_Data[i].m_FirstKeyFrameTime = 0.0f;
		m_Data[i].m_LastKeyFrameTime = 0.0f;
	}

	m_Model = model;
	if (m_Model->m_SkeletalAnimations)
	{
		m_Skeleton = model->getSkeleton();
		m_Animations = model->getAnimations();

		m_Repeats.resize(m_Animations->size());
		for (size_t i = 0; i < m_Animations->size(); ++i)
			m_Repeats[i] = false;
	}
}

void SAComponent::start(std::string const& animation)
{
	m_Animations->start(animation);
	init(&(*m_Animations)[animation]);
}

void SAComponent::start(size_t index)
{
	m_Animations->start(index);
	init(&(*m_Animations)[index]);
}

void SAComponent::setRepeat(bool repeat)
{
	if (m_CurrentAnimation)
	{
		int index = m_Animations->getIndex(m_CurrentAnimation->getName());
		if (index != -1)
			m_Repeats[index] = repeat;
	}
}

void SAComponent::setRepeatAll(bool repeat)
{
	for (size_t i = 0; i < m_Repeats.size(); ++i)
		m_Repeats[i] = repeat;
}

void SAComponent::init(SkeletalAnimation* animation)
{
	m_CurrentAnimation = animation;
	m_Data[0] = animation->getData();
	m_FrameCounter = m_Animations->getCurrentFrame();

	AnimationState* state = m_StateManager->getState(animation->getName());
	if (state)
		m_StateManager->currentState = *state;
}

void SAComponent::updateAnimation(const Timestep& timestep, uint32_t currentFrame)
{
	m_StateManager->update(timestep);
	if (m_StateManager->inTransition) // BLENDING-ANIMATION (2)
	{
		if (m_StateManager->currentState.animationName != m_CurrentAnimation->getName())
		{
			auto& prevAnim = (*m_Animations)[m_StateManager->prevState.animationName];
			setData(m_FrameCounter, prevAnim.getData(), 0);
			
			m_CurrentAnimation = &(*m_Animations)[m_StateManager->currentState.animationName];
			setData(m_FrameCounter, m_CurrentAnimation->getData(), 1);
		}
		blendUpdate(timestep, *m_Skeleton, currentFrame);
	}
	else if (m_StateManager->isTransitionFinish) // AFTER BLENDING CHANGED TO CURRENT-ANIMATION
	{
		m_Data[0] = m_Data[1];
		m_Data[1].m_CurrentKeyFrameTime = 0.0f;
		m_Data[1].m_FirstKeyFrameTime = 0.0f;
		m_Data[1].m_LastKeyFrameTime = 0.0f;
		m_StateManager->isTransitionFinish = false;
		m_CurrentAnimation = &(*m_Animations)[m_StateManager->currentState.animationName];
	}
	else if (m_CurrentAnimation && m_Model->m_SkeletalAnimations) // SINGLE-ANIMATION
	{
		m_CurrentAnimation->uploadData(this->getData(),
			m_Repeats[getAnimIndex(m_CurrentAnimation->getName())]);
		m_Animations->uploadData(m_CurrentAnimation, m_FrameCounter);
		m_Animations->update(timestep * m_SpeedFactor, *m_Skeleton, currentFrame);
		m_Skeleton->update();

		m_Model->setShaderData(m_Skeleton->m_ShaderData.m_FinalBonesMatrices);
		this->setData(m_Animations->getCurrentFrame() ,m_CurrentAnimation->getData(), 0);
	}
}

void SAComponent::blendUpdate(
	const Timestep& timestep,
	Armature::Skeleton& skeleton,
	uint32_t currentFrame
)
{
	auto& animA = (*m_Animations)[m_StateManager->prevState.animationName];
	auto& animB = (*m_Animations)[m_StateManager->currentState.animationName];
	animA.uploadData(getData(0), getAnimRepeat(&animA));
	animB.uploadData(getData(1), getAnimRepeat(&animB));

	float blendFactor = std::min(m_StateManager->transitionTime / m_StateManager->transitionDuration, 1.0f);
	Bones poseFrom, poseTo;

	m_Animations->uploadData(&animA, m_FrameCounter);
	m_Animations->update(timestep, *m_Skeleton, currentFrame);
	poseFrom = m_Skeleton->m_Bones;
	this->setData(m_FrameCounter, animA.getData(), 0); // prevState 애니메이션이 기존 애니메이션이므로 기존 애니메이션의 키프레임 데이터를 유지

	m_Animations->uploadData(&animB, m_FrameCounter);
	m_Animations->update(timestep, *m_Skeleton, currentFrame);
	poseTo = m_Skeleton->m_Bones;
	this->setData(currentFrame, animB.getData(), 1);

	m_Skeleton->m_Bones = blendBones(poseTo, poseFrom, blendFactor);
	m_Skeleton->update();
	
	m_Model->setShaderData(m_Skeleton->m_ShaderData.m_FinalBonesMatrices);
}

SAComponent::Bones SAComponent::blendBones(Bones& to, Bones& from, float blendFactor)
{
	if (to.size() != from.size())
	{
		AL_ERROR("SAComponent::blendBones(): Each Bones has different size! ({0}:{1})", to.size(), from.size());
		return to;
	}

	size_t numberOfBones = to.size();

	for (size_t boneIndex = 0; boneIndex < numberOfBones; ++boneIndex)
	{
		to[boneIndex].m_DeformedNodeTranslation =
			glm::mix(from[boneIndex].m_DeformedNodeTranslation,
					 to[boneIndex].m_DeformedNodeTranslation,
					 blendFactor);
		to[boneIndex].m_DeformedNodeRotation =
			glm::normalize(glm::slerp(from[boneIndex].m_DeformedNodeRotation,
					 to[boneIndex].m_DeformedNodeRotation,
					 blendFactor));
		to[boneIndex].m_DeformedNodeScale =
			glm::mix(from[boneIndex].m_DeformedNodeScale,
					 to[boneIndex].m_DeformedNodeScale,
					 blendFactor);
	}

	return to;
}

struct SAData SAComponent::getData(unsigned int index) const
{
	return m_Data[index];
}

void SAComponent::setData(uint16_t currentFrame, const SAData& data, unsigned int index)
{
	m_FrameCounter = currentFrame; //??

	m_Data[index].m_FirstKeyFrameTime = data.m_FirstKeyFrameTime;
	m_Data[index].m_LastKeyFrameTime = data.m_LastKeyFrameTime;
	m_Data[index].m_CurrentKeyFrameTime = data.m_CurrentKeyFrameTime;
}

void SAComponent::setSpeedFactor(float factor)
{
	if (factor > 1.0f || factor < 0.0f)
	{
		AL_WARN("SAComponent::setSpeedFactor(): Speed Factor Only range in 0.0f ~ 1.0f, {0}", factor);
		return;
	}

	m_SpeedFactor = factor;
}

int SAComponent::getAnimIndex(const std::string& name)
{
	return m_Animations->getIndex(name);
}

bool SAComponent::getAnimRepeat(SkeletalAnimation* animation)
{
	return m_Repeats[getAnimIndex(animation->getName())];
}

} //namespace ale
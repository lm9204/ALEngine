#include "Core/Timestep.h"
#include "Renderer/AnimationStateManager.h"

namespace ale
{

void AnimationStateManager::update(const Timestep& timestep)
{
	if (inTransition)
	{
		transitionTime += timestep;
		if (transitionTime >= transitionDuration)
			finishTransition();
	}
	else
		timeSinceLastTransition += timestep;

	processRequests();
	processTransitions();
}

void AnimationStateManager::processRequests()
{
	while (!m_RequestQueue.empty())
	{
		AnimationStateChangeRequest req = m_RequestQueue.front();
		m_RequestQueue.pop();

		for (auto& t : m_Transitions)
		{
			if ((t.fromState == "*" || t.fromState == currentState.stateName) &&
				t.toState == req.targetState)
			{
				if (t.condition || t.condition())
				{
					startTransition(t);
					break;
				}
			}
		}
	}
}

void AnimationStateManager::processTransitions()
{
	if (!currentState.interruptible && inTransition) return;

	for (auto& t : m_Transitions)
	{
		if ((t.fromState == "*" || t.fromState == currentState.stateName) &&
			t.toState != currentState.stateName &&
			t.condition && t.condition())
		{
			startTransition(t);
			break;
		}
	}
}

void AnimationStateManager::startTransition(const AnimationStateTransition& t)
{
	if (currentState.stateName == t.toState) return;

	inTransition = true;
	isTransitionFinish = false;

	timeSinceLastTransition = 0.0f;
	transitionTime = 0.0f;
	transitionDuration = t.blendTime;

	prevState = currentState;
	currentState = *getState(t.toState);

	AL_INFO("[AnimationStateManager] Transition:" + prevState.stateName + "->" + currentState.stateName);
}

void AnimationStateManager::finishTransition()
{
	inTransition = false;
	isTransitionFinish = true;

	transitionTime = 0.0f;
	transitionDuration = 0.0f;

	AL_INFO("[AnimationStateManager] Transition Finished:" + prevState.stateName + "->" + currentState.stateName);
}

AnimationState* AnimationStateManager::getState(const std::string& stateName)
{
	auto it = m_States.find(stateName);

	if (it != m_States.end())
		return &it->second;
	return nullptr;
}

bool AnimationStateManager::hasNoTransitionFor(float seconds) const
{
	return (timeSinceLastTransition >= seconds);
}

}; //namespace ale
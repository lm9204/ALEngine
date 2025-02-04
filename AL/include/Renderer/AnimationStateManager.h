#ifndef ANIMATIONSTATEMANAGER_H
#define ANIMATIONSTATEMANAGER_H

#include <queue>

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Model.h"

namespace ale
{

struct AnimationState
{
	std::string	stateName;
	std::string	animationName;
	bool		looping;
	bool		interruptible;
	float		defaultBlendTime;
};

struct AnimationStateTransition
{
	std::string	fromState;
	std::string	toState;
	std::function<bool()> condition;
	float blendTime;
};

struct AnimationStateChangeRequest
{
	std::string targetState; // -> AnimationState::stateName
};

class AnimationStateManager
{
public:
	AnimationState currentState;
	AnimationState prevState;
	bool inTransition;
	bool isTransitionFinish;
	float transitionTime = 0.0f;
	float transitionDuration = 0.0f;
	float timeSinceLastTransition = 0.0f;

public:
	AnimationStateManager() = default;
	AnimationState* getState(const std::string& stateName);
	void addState(const AnimationState& s) { m_States[s.stateName] = s; }
	void addTransition(const AnimationStateTransition& t) { m_Transitions.push_back(t); }
	void pushStateChangeRequest(const std::string& target) { m_RequestQueue.push({ target }); }
	void update(const Timestep& timestep);
	bool hasNoTransitionFor(float seconds) const;


private:
	void processRequests();
	void processTransitions();
	void startTransition(const AnimationStateTransition& t);
	void finishTransition();

private:
	std::unordered_map<std::string, AnimationState> m_States;
	std::vector<AnimationStateTransition> m_Transitions;
	std::queue<AnimationStateChangeRequest> m_RequestQueue;
};

} // namespace ale


#endif
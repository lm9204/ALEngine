#ifndef SKELETALANIMATION_H
#define SKELETALANIMATION_H

#include "Core/Timestep.h"
#include "Skeleton.h"

namespace ale
{
class Timestep;
class SkeletalAnimation
{
	public:
		enum class EPath
		{
			TRANSLATION,
			ROTATION,
			SCALE
		};

		enum class EInterpolationMethod
		{
			LINEAR,
			STEP,
			CUBICSPLINE
		};
		
		struct Channel
		{
			EPath m_Path;
			int m_SamplerIndex;
			int m_Node;
		};

		struct Sampler
		{
			std::vector<float> m_Timestamps;
			std::vector<glm::vec4> m_TRSoutputValuesToBeInterpolated;
			EInterpolationMethod m_Interpolation;
		};

	public:
		SkeletalAnimation(std::string const& name);

		void start();
		void stop();
		bool isRunning() const;
		bool willExpire(const Timestep& timestep) const;
		std::string const& getName() const { return m_Name; }
		void setRepeat(bool repeat) { m_Repeat = repeat; }
		void update(const Timestep& timestep, Armature::Skeleton& skeleton);
		float getDuration() const { return m_LastKeyFrameTime - m_FirstKeyFrameTime; }
		float getCurrentTime() const { return m_CurrentKeyFrameTime - m_FirstKeyFrameTime; }

		std::vector<SkeletalAnimation::Sampler> m_Samplers;
		std::vector<SkeletalAnimation::Channel> m_Channels;

		void setFirstKeyFrameTime(float firstKeyFrameTime) { m_FirstKeyFrameTime = firstKeyFrameTime; }
		void setLastKeyFrameTime(float lastKeyFrameTime) { m_LastKeyFrameTime = lastKeyFrameTime; }

	private:
		std::string m_Name;
		bool m_Repeat;

		float m_FirstKeyFrameTime;
		float m_LastKeyFrameTime;
		float m_CurrentKeyFrameTime = 0.0f;
};
}
#endif
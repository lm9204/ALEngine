#ifndef SKELETON_H
#define SKELETON_H

#include "Core/Base.h"
#include "alpch.h"
#include "Renderer/Common.h"
#include "Renderer/Animation/Bones.h"
#include <iostream>
#include <unordered_map>


namespace ale
{
	namespace Armature
	{
		static constexpr int NO_PARENT = -1;
		static constexpr int ROOT_JOINT = 0;

		struct ShaderData
		{
			std::vector<glm::mat4> m_FinalBonesMatrices;
		};

		// TODO : 최적화 ->  동적/정적 데이터 분리

		struct Bone
		{
			// TODO 동적/정적 데이터 분리사항
				// glm::mat4 m_BindPose{0.0f};
				// glm::mat4 m_InverseBindPose{0.0f};

			std::string m_Name;
			glm::mat4 m_InverseBindMatrix;

			// deformed data
			glm::vec3 m_DeformedNodeTranslation{0.0f};
			glm::quat m_DeformedNodeRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			glm::vec3 m_DeformedNodeScale{1.0f};

			glm::mat4 getDeformedBindMatrix()
			{
				return	glm::translate(glm::mat4(1.0f), m_DeformedNodeTranslation) *	// T
						glm::mat4(m_DeformedNodeRotation) *								// R
						glm::scale(glm::mat4(1.0f), m_DeformedNodeScale);				// S
			}

			int m_ParentBone;
			std::vector<int> m_Children;
		};

		struct Skeleton
		{
			void update();
			void updateBone(int16_t boneIndex);

			std::string								m_Name;
			std::vector<Bone>						m_Bones;
			std::unordered_map<std::string, int>	m_NodeNameToBoneIndex;

			ShaderData m_ShaderData;
			bool m_isAnimated = true;
		};
	};
}
#endif
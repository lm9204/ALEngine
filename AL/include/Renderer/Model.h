#ifndef MODEL_H
#define MODEL_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"
#include "Renderer/Animation/SkeletalAnimations.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
namespace ale
{
class ShaderResourceManager;

struct DrawInfo {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 finalBonesMatrices[MAX_BONES];
	ShaderResourceManager* shaderResourceManager;
	VkCommandBuffer commandBuffer;
	VkPipelineLayout pipelineLayout;
	std::vector<std::shared_ptr<Material>> materials;
	uint32_t currentFrame;
};

class Model
{
  public:
	static std::shared_ptr<Model> createModel(std::string path);
	static std::shared_ptr<Model> createBoxModel();
	static std::shared_ptr<Model> createSphereModel();
	static std::shared_ptr<Model> createPlaneModel();
	~Model() = default;
	void cleanup();

	void draw(DrawInfo& drawInfo);
	size_t getMeshCount() {return m_meshes.size();}
	std::vector<std::shared_ptr<Mesh>>& getMeshes() {return m_meshes;}
	std::vector<std::shared_ptr<Material>>& getMaterials() {return m_materials;}
	void updateMaterial(std::vector<std::shared_ptr<Material>> materials);
	void updateAnimations(SkeletalAnimation* animation, const Timestep& timestep, uint32_t prevImage, uint32_t currentImage);
	void setShaderData(const std::vector<glm::mat4>& shaderData);
	std::shared_ptr<SkeletalAnimations>& getAnimations();
	std::shared_ptr<Armature::Skeleton>& getSkeleton();
	bool m_SkeletalAnimations;
	
  private:
	Model() = default;

	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<std::shared_ptr<Material>> m_materials;
	// animation
	std::shared_ptr<SkeletalAnimations> m_Animations;
	std::shared_ptr<Armature::Skeleton> m_Skeleton;
	Armature::ShaderData m_ShaderData;

	struct VertexBoneData
	{
		std::vector<std::pair<int, float>> bones;
	};

	void initModel(std::string path, std::shared_ptr<Material> &defaultMaterial);
	void initBoxModel(std::shared_ptr<Material> &defaultMaterial);
	void initSphereModel(std::shared_ptr<Material> &defaultMaterial);
	void initPlaneModel(std::shared_ptr<Material> &defaultMaterial);
	void loadModel(std::string path, std::shared_ptr<Material> &defaultMaterial);
	void loadGLTFModel(std::string path, std::shared_ptr<Material> &defaultMaterial);
	void loadOBJModel(std::string path, std::shared_ptr<Material> &defaultMaterial);
	
	std::string getMaterialPath(std::string &path, std::string materialPath);
	std::shared_ptr<Material> processGLTFMaterial(aiMaterial *material, std::shared_ptr<Material> &defaultMaterial, std::string path);
	void processGLTFNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Material>> &materials);
	std::shared_ptr<Mesh> processGLTFMesh(aiMesh *mesh, const aiScene *scene, std::shared_ptr<Material> &material);
	void processGLTFSkeleton(const aiScene* scene);
	void collectAllBones(const aiScene* scene, std::vector<aiBone*>& outBones);
	void buildSkeletonBoneArray(const std::vector<aiBone*>& allAiBones);
	void loadBone(aiNode* node, int parentBoneIndex);
	void loadAnimations(const aiScene* scene);
	glm::mat4 convertMatrix(const aiMatrix4x4& m);


	void processOBJNode(aiNode *node, const aiScene *scene, std::shared_ptr<Material> &defaultMaterial);
	std::shared_ptr<Mesh> processOBJMesh(aiMesh *mesh, const aiScene *scene, std::shared_ptr<Material> &defaultMaterial);
};

} // namespace ale

#endif
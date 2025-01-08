#ifndef MODEL_H
#define MODEL_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
namespace ale
{
class Model
{
  public:
	static std::shared_ptr<Model> createModel(std::string path);
	static std::shared_ptr<Model> createBoxModel();
	static std::shared_ptr<Model> createSphereModel();
	static std::shared_ptr<Model> createPlaneModel();
	~Model() = default;
	void cleanup();

	void draw(VkCommandBuffer commandBuffer);

  private:
	Model() = default;

	std::vector<std::shared_ptr<Mesh>> m_meshes;

	void initModel(std::string path);
	void initBoxModel();
	void initSphereModel();
	void initPlaneModel();
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
};

} // namespace ale

#endif
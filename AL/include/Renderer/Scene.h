#ifndef SCENE_H
#define SCENE_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Object.h"
#include "Renderer/Material.h"

namespace ale
{

struct LightInfo {
	glm::vec3 lightPos;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	float intensity;
	float ambientStrength;
};

class AL_API Scene
{
public:
	static std::unique_ptr<Scene> createScene();
	~Scene() {}
	void cleanup();

	const std::vector< std::shared_ptr<Object> >& getObjects() { return m_objects; }
	LightInfo &getLightInfo() { return m_lightInfo; }
	glm::vec3 &getCamPos() { return m_cameraPos; }
	glm::vec3 &getCamFront() { return m_cameraFront; }
	glm::vec3 &getCamUp() { return m_cameraUp; }
	float &getCamPitch() { return m_cameraPitch; }
	float &getCamYaw() { return m_cameraYaw; }
	size_t getObjectCount() { return m_objectCount; }
	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix(VkExtent2D swapChainExtent);
	std::shared_ptr<Object> getLightObject() { return m_lightObject; }
	DefaultTextures &getDefaultTextures() { return m_defaultTextures; }

	void updateLightPos(glm::vec3 lightPos);
	void mouseButton(int button, int action, double x, double y);
	void mouseMove(double x, double y);
	void processInput(GLFWwindow* window);




private:
	Scene() {}

	DefaultTextures m_defaultTextures;

	std::shared_ptr<Model> m_boxModel;
	std::shared_ptr<Model> m_sphereModel;
	std::shared_ptr<Model> m_planeModel;
	std::shared_ptr<Model> m_vikingModel;
	std::shared_ptr<Model> m_catModel;
	std::shared_ptr<Model> m_backpackModel;

	std::shared_ptr<Texture> m_backpackAlbedo;
	std::shared_ptr<Texture> m_backpackNormal;
	std::shared_ptr<Texture> m_backpackRoughness;
	std::shared_ptr<Texture> m_backpackMetallic;
	std::shared_ptr<Texture> m_backpackAo;




	std::shared_ptr<Texture> m_vikingTexture;
	std::shared_ptr<Texture> m_sampleTexture;
	std::shared_ptr<Texture> m_catTexture;
	std::shared_ptr<Texture> m_karinaTexture;
	std::shared_ptr<Texture> m_defaultTexture;
	std::shared_ptr<Texture> m_defaultSingleChannelTexture;

	std::vector< std::shared_ptr<Object> > m_objects;
	std::shared_ptr<Object> m_lightObject;

	std::shared_ptr<Material> m_material;
	std::shared_ptr<Material> m_karinaMaterial;
	std::shared_ptr<Material> m_catMaterial;
	std::shared_ptr<Material> m_vikingMaterial;
	std::shared_ptr<Material> m_backpackMaterial;

	bool m_cameraControl { false };
	glm::vec2 m_prevMousePos { 0.0f, 0.0f };

    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 5.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
	float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };
	size_t m_objectCount;
	LightInfo m_lightInfo;

	void initScene();
};

} // namespace ale

#endif
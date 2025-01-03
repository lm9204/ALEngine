#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Base.h"
#include "Renderer/Buffer.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class AL_API Texture : public Buffer
{
  public:
	static std::shared_ptr<Texture> createTexture(std::string path);
	static std::shared_ptr<Texture> createDefaultTexture(glm::vec4 color);
	static std::shared_ptr<Texture> createDefaultSingleChannelTexture(float value);

	~Texture() = default;

	void cleanup();

	VkImageView getImageView()
	{
		return textureImageView;
	}
	VkSampler getSampler()
	{
		return textureSampler;
	}

  private:
	Texture() = default;

	uint32_t mipLevels;
	std::unique_ptr<ImageBuffer> m_imageBuffer;
	VkImageView textureImageView;
	VkSampler textureSampler;

	void initTexture(std::string path);
	void loadTexture(std::string path);
	void createTextureImageView();
	void createTextureSampler();

	void initDefaultTexture(glm::vec4 color);
	void createDefaultTextureImageView();
	void createDefaultTextureSampler();

	void initDefaultSingleChannelTexture(float value);
	void createDefaultSingleChannelTextureImageView();
	void createDefaultSingleChannelTextureSampler();
};

} // namespace ale

#endif
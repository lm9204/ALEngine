#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Base.h"
#include "Renderer/Buffer.h"
#include "Renderer/Common.h"
#include "Renderer/VulkanContext.h"

namespace ale
{
class Texture : public Buffer
{
  public:
	static std::shared_ptr<Texture> createTexture(std::string path);

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
};

} // namespace ale

#endif
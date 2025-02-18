#include "Renderer/VulkanUtil.h"

namespace ale
{
void VulkanUtil::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
							 VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
							 VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
{

	auto &context = VulkanContext::getContext();
	auto device = context.getDevice();

	// 이미지 객체를 만드는데 사용되는 구조체
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D; // 이미지의 차원을 설정
	imageInfo.extent.width = width;			// 이미지의 너비 지정
	imageInfo.extent.height = height;		// 이미지의 높이 지정
	imageInfo.extent.depth = 1;				// 이미지의 깊이 지정 (2D 이미지의 경우 depth는 1로 지정해야 함)
	imageInfo.mipLevels = mipLevels;		// 생성할 mipLevel의 개수 지정
	imageInfo.arrayLayers = 1;				// 생성할 이미지 레이어 수 (큐브맵의 경우 6개 생성)
	imageInfo.format = format;				// 이미지의 포맷을 지정하며, 채널 구성과 각 채널의 비트 수를 정의
	imageInfo.tiling = tiling; // 이미지를 GPU 메모리에 배치할 때 메모리 레이아웃을 결정하는 설정 (CPU에서도 접근
							   // 가능하게 할꺼냐, GPU에만 접근 가능하게 최적화 할거냐 결정)
	imageInfo.initialLayout =
		VK_IMAGE_LAYOUT_UNDEFINED;	// 이미지 초기 레이아웃 설정 (이미지가 메모리에 배치될 때 초기 상태를 정의)
	imageInfo.usage = usage;		// 이미지의 사용 용도 결정
	imageInfo.samples = numSamples; // 멀티 샘플링을 위한 샘플 개수
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // 이미지의 큐 공유 모드 설정 (VK_SHARING_MODE_EXCLUSIVE: 한 번에
													   // 하나의 큐 패밀리에서만 접근 가능한 단일 큐 모드)

	// 이미지 객체 생성
	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}

	// 이미지에 필요한 메모리 요구 사항을 조회
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	// 메모리 할당을 위한 구조체
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;										// 메모리 크기 설정
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties); // 메모리 유형과 속성 설정

	// 이미지를 위한 메모리 할당
	if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate image memory!");
	}

	// 이미지에 할당한 메모리 바인딩
	vkBindImageMemory(device, image, imageMemory, 0);
}

/*
	GPU와 buffer가 호환되는 메모리 유형중 properties에 해당하는 속성들을 갖는 메모리 유형 찾기
*/
uint32_t VulkanUtil::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	auto &context = VulkanContext::getContext();
	auto physicalDevice = context.getPhysicalDevice();

	// GPU에서 사용한 메모리 유형을 가져온다.
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		// typeFilter & (1 << i) : GPU의 메모리 유형중 버퍼와 호환되는 것인지 판단
		// memProperties.memoryTypes[i].propertyFlags & properties : GPU 메모리 유형의 속성이 properties와 일치하는지
		// 판단

		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			// 해당 메모리 유형 반환
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

// 이미지 뷰 생성
VkImageView VulkanUtil::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
										uint32_t mipLevels)
{
	auto &context = VulkanContext::getContext();
	auto device = context.getDevice();

	// 이미지 뷰 정보 생성
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;								// 이미지 핸들
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;			// 이미지 타입
	viewInfo.format = format;							// 이미지 포맷
	viewInfo.subresourceRange.aspectMask = aspectFlags; // 이미지 형식 결정 (color / depth / stencil 등)
	viewInfo.subresourceRange.baseMipLevel = 0;			// 렌더링할 mipmap 단계 설정
	viewInfo.subresourceRange.levelCount =
		mipLevels; // baseMipLevel 기준으로 몇 개의 MipLevel을 더 사용할지 설정 (실제 mipmap 만드는 건 따로 해줘야함)
	viewInfo.subresourceRange.baseArrayLayer = 0; // ImageView가 참조하는 이미지 레이어의 시작 위치 정의
	viewInfo.subresourceRange.layerCount = 1;	  // 스왑 체인에서 설정한 이미지 레이어 개수

	// 이미지 뷰 생성
	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image view!");
	}

	return imageView;
}

// depth image의 format 설정
VkFormat VulkanUtil::findDepthFormat()
{
	return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
							   VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

// Vulkan의 특정 format에 대해 GPU가 tiling의 features를 지원하는지 확인
VkFormat VulkanUtil::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
										 VkFormatFeatureFlags features)
{
	auto &context = VulkanContext::getContext();
	VkPhysicalDevice physicalDevice = context.getPhysicalDevice();

	// format 들에 대해 GPU가 tiling과 features를 지원하는지 확인
	for (VkFormat format : candidates)
	{
		// GPU가 format에 대해 지원하는 특성 가져오는 함수
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		// GPU가 지원하는 특성과 비교
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{ // VK_IMAGE_TILING_LINEAR의 특성 비교
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{ // VK_IMAGE_TILING_OPTIMAL의 특성 비교
			return format;
		}
	}
	throw std::runtime_error("failed to find supported format!");
}

// shader 파일인 SPIR-V 파일을 바이너리 형태로 읽어오는 함수
std::vector<char> VulkanUtil::readFile(const std::string &filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void VulkanUtil::insertImageMemoryBarrier(VkCommandBuffer cmdbuffer, VkImage image, VkAccessFlags srcAccessMask,
										  VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout,
										  VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask,
										  VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange)
{
	VkImageMemoryBarrier imageMemoryBarrier{};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.srcAccessMask = srcAccessMask;
	imageMemoryBarrier.dstAccessMask = dstAccessMask;
	imageMemoryBarrier.oldLayout = oldImageLayout;
	imageMemoryBarrier.newLayout = newImageLayout;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange = subresourceRange;

	vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}

VkCommandBuffer VulkanUtil::beginSingleTimeCommands(VkDevice device, VkCommandPool commandpool)
{
	// 커맨드 버퍼 할당을 위한 구조체
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level =
		VK_COMMAND_BUFFER_LEVEL_PRIMARY; // PRIMARY LEVEL 등록 (해당 커맨드 버퍼가 큐에 단독으로 제출될 수 있음)
	allocInfo.commandPool = commandpool; // 커맨드 풀 지정
	allocInfo.commandBufferCount = 1;	 // 커맨드 버퍼 개수 지정

	// 커맨드 버퍼 생성
	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	// 커맨드 버퍼 기록을 위한 정보 객체
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // 커맨드 버퍼를 1번만 제출

	// GPU에 필요한 작업을 모두 커맨드 버퍼에 기록하기 시작
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void VulkanUtil::endSingleTimeCommands(VkDevice device, VkQueue queue, VkCommandPool commandPool,
									   VkCommandBuffer commandBuffer)
{
	// 커맨드 버퍼 기록 중지
	vkEndCommandBuffer(commandBuffer);

	// 복사 커맨드 버퍼 제출 정보 객체 생성
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;			 // 커맨드 버퍼 개수
	submitInfo.pCommandBuffers = &commandBuffer; // 커맨드 버퍼 등록

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE); // 커맨드 버퍼 큐에 제출
	vkQueueWaitIdle(queue);								  // 그래픽스 큐 작업 종료 대기

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer); // 커맨드 버퍼 제거
}

VkSampler VulkanUtil::createSampler()
{
	VkSampler textureSampler;
	auto &context = VulkanContext::getContext();
	auto device = context.getDevice();
	auto physicalDevice = context.getPhysicalDevice();

	// GPU의 속성 정보를 가져오는 함수
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	// 샘플러 생성시 필요한 구조체
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;				   // 확대시 필터링 적용 설정 (현재 선형 보간 필터링 적용)
	samplerInfo.minFilter = VK_FILTER_LINEAR;				   // 축소시 필터링 적용 설정 (현재 선형 보간 필터링 적용)
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; // 텍스처 좌표계의 U축(너비)에서 범위를 벗어난 경우 래핑
															   // 모드 설정 (현재 반복 설정)
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; // 텍스처 좌표계의 V축(높이)에서 범위를 벗어난 경우 래핑
															   // 모드 설정 (현재 반복 설정)
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT; // 텍스처 좌표계의 W축(깊이)에서 범위를 벗어난 경우 래핑
															   // 모드 설정 (현재 반복 설정)
	samplerInfo.anisotropyEnable =
		VK_TRUE; // 이방성 필터링 적용 여부 설정 (경사진 곳이나 먼 곳의 샘플을 늘려 좀 더 정확한 값을 가져오는 방법)
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy; // GPU가 지원하는 최대의 이방성 필터링 제한 설정
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; // 래핑 모드가 VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
																// 일때 텍스처 외부 색 지정
	samplerInfo.unnormalizedCoordinates =
		VK_FALSE; // VK_TRUE로 설정시 텍스처 좌표가 0 ~ 1의 정규화된 좌표가 아닌 실제 텍셀의 좌표 범위로 바뀜
	samplerInfo.compareEnable =
		VK_FALSE; // 비교 연산 사용할지 결정 (보통 쉐도우 맵같은 경우 깊이 비교 샘플링에서 사용됨)
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;			// 비교 연산에 사용할 연산 지정
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; // mipmap 사용시 mipmap 간 보간 방식 결정 (현재 선형 보간)
	samplerInfo.minLod = 0.0f; // 최소 level을 0으로 설정 (가장 높은 해상도의 mipmap 을 사용가능하게 허용)
	samplerInfo.maxLod = 1.0f; // 최대 level을 mipLevel로 설정 (VK_LOD_CLAMP_NONE 설정시 제한 해제)
	samplerInfo.mipLodBias =
		0.0f; // Mipmap 레벨 오프셋(Bias)을 설정
			  // Mipmap을 일부러 더 높은(더 큰) 레벨로 사용하거나 낮은(더 작은) 레벨로 사용하고 싶을 때 사용.

	// 샘플러 생성
	if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
	return textureSampler;
}

VkDescriptorSetLayout VulkanUtil::createIconDescriptorSetLayout(VkDevice device, VkDescriptorPool descriptorPool)
{
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 0; // 샘플러를 binding 0으로 설정
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	// DescriptorSetLayout 생성
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1; // 샘플러만 포함
	layoutInfo.pBindings = &samplerLayoutBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create sampler-only descriptor set layout!");
	}
	return descriptorSetLayout;
}

VkDescriptorSet VulkanUtil::createIconDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool,
													VkDescriptorSetLayout descriptorSetLayout, VkImageView imageView,
													VkSampler sampler)
{
	VkDescriptorSet descriptorSet;
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout;

	if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor set!");
	}

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageView = imageView;
	imageInfo.sampler = sampler;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

	return descriptorSet;
}

ImTextureID VulkanUtil::createIconTexture(VkDevice device, VkDescriptorPool descriptorPool, VkImageView imageView,
										  VkSampler sampler)
{
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet descriptorSet;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 0; // 샘플러를 binding 0으로 설정
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	// DescriptorSetLayout 생성
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1; // 샘플러만 포함
	layoutInfo.pBindings = &samplerLayoutBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create sampler-only descriptor set layout!");
	}

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout;

	if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor set!");
	}

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageView = imageView;
	imageInfo.sampler = sampler;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

	return reinterpret_cast<ImTextureID>(descriptorSet);
}

void VulkanUtil::createCubeMapImage(uint32_t width, uint32_t height, uint32_t mipLevels,
									VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
									VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
									VkDeviceMemory &imageMemory)
{
	auto &context = VulkanContext::getContext();
	auto device = context.getDevice();

	// 큐브맵 이미지 객체를 만드는데 사용되는 구조체
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 6;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSamples;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

	// 이미지 객체 생성
	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create cube image!");
	}

	// 이미지에 필요한 메모리 요구 사항 조회
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	// 메모리 할당을 위한 구조체
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;										// 메모리 크기
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties); // 메모리 유형 설정

	// 이미지를 위한 메모리 할당
	if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate cube image memory!");
	}

	// 이미지에 메모리 바인딩
	vkBindImageMemory(device, image, imageMemory, 0);
}

VkImageView VulkanUtil::createCubeMapImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
											   uint32_t mipLevels)
{
	auto &context = VulkanContext::getContext();
	auto device = context.getDevice();

	// 큐브 맵 이미지 뷰 정보 생성
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;								// 이미지 핸들
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;		// 큐브 맵 타입
	viewInfo.format = format;							// 이미지 포맷
	viewInfo.subresourceRange.aspectMask = aspectFlags; // 이미지 형식 결정 (color / depth / stencil 등)
	viewInfo.subresourceRange.baseMipLevel = 0;			// 렌더링할 mipmap 단계 설정
	viewInfo.subresourceRange.levelCount = mipLevels;	// 사용 가능한 모든 MipLevel
	viewInfo.subresourceRange.baseArrayLayer = 0;		// 첫 번째 레이어부터 시작
	viewInfo.subresourceRange.layerCount = 6;			// 큐브 맵은 6개의 레이어를 사용

	// 큐브 맵 이미지 뷰 생성
	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create cube image view!");
	}

	return imageView;
}

} // namespace ale
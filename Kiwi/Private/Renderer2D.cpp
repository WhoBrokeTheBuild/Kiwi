#include <Kiwi/Renderer2D.hpp>
#include <Kiwi/IEmulator.hpp>
#include <Kiwi/Utility/Exception.hpp>

namespace kiwi {

#include "Render2D.vert.spv.hpp"
#include "Render2D.frag.spv.hpp"

Renderer2D::Renderer2D(IEmulator * emulator, Vec2u size, unsigned count /*= 2*/)
    : IRenderer(emulator)
    , _imageCount(count)
    , _imageSize(size)
{
    assert(_imageCount > 0);
    
    size_t imageBufferSize = _imageSize.x * _imageSize.y * _components;

    for (unsigned i = 0; i < _imageCount; ++i) {
        _imageList.push_back(List<uint8_t>(imageBufferSize));

        auto& image = _imageList.back();

        // Set image to black
        memset(image.data(), 0, image.size());

        // Set alpha values to opaque
        for (size_t i = 3; i < image.size(); i += 4) {
            image[i] = 0xFF;
        }
    }

    Log(KIWI_ANCHOR, "Renderer2D::ctor()");
}

void Renderer2D::initResources()
{
    Log(KIWI_ANCHOR, "Renderer2D::initResources()");

    IRenderer::initResources();
    
    VkResult vkResult;

    ///
    /// _vkImageBuffer, _vmaImageBufferAllocation, _imageBufferData
    ///

    VkBufferCreateInfo imageBufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = image().size(),
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VmaAllocationCreateInfo imageBufferAllocationCreateInfo = {
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    
    VmaAllocationInfo imageBufferAllocationInfo;

    vkResult = vmaCreateBuffer(
        _vmaAllocator,
        &imageBufferCreateInfo,
        &imageBufferAllocationCreateInfo,
        &_vkImageBuffer,
        &_vmaImageBufferAllocation,
        &imageBufferAllocationInfo
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vmaCreateBuffer failed: {}", vkResult);
    }

    _imageBufferData = imageBufferAllocationInfo.pMappedData;

    ///
    /// _vkImage, _vmaImageAllocation, _vkImageView, _vkSampler
    ///

    VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .extent = VkExtent3D{ _imageSize.x, _imageSize.y, 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    
    VmaAllocationCreateInfo imageAllocationCreateInfo = {
        .flags = 0,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    
    vkResult = vmaCreateImage(
        _vmaAllocator,
        &imageCreateInfo,
        &imageAllocationCreateInfo,
        &_vkImage,
        &_vmaImageAllocation,
        nullptr
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vmaCreateBuffer failed: {}", vkResult);
    }

    // TODO: Move to function?
    VkCommandBuffer commandBuffer = beginOneTimeCommandBuffer();

    VkImageMemoryBarrier imageMemoryBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = _vkImage,
        .subresourceRange = VkImageSubresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    _vkDeviceFuncs->vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier
    );

    endOneTimeCommandBuffer(commandBuffer);

    VkImageViewCreateInfo imageViewCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = _vkImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .subresourceRange = VkImageSubresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    vkResult = _vkDeviceFuncs->vkCreateImageView(
        _vkDevice,
        &imageViewCreateInfo,
        nullptr,
        &_vkImageView
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreateImageView failed: {}", vkResult);
    }

    VkSamplerCreateInfo samplerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .anisotropyEnable = VK_FALSE,
    };

    vkResult = _vkDeviceFuncs->vkCreateSampler(_vkDevice, &samplerCreateInfo, nullptr, &_vkSampler);
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreateSampler failed: {}", vkResult);
    }

    ///
    /// _vkVertexBuffer, _vmaVertexBufferAllocation
    ///

    const float vertexData[] = {
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,

         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
    };

    VkBufferCreateInfo vertexBufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(vertexData),
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    };

    VmaAllocationCreateInfo vertexBufferAllocationCreateInfo = {
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VmaAllocationInfo vertexBufferAllocationInfo;

    vkResult = vmaCreateBuffer(
        _vmaAllocator,
        &vertexBufferCreateInfo,
        &vertexBufferAllocationCreateInfo,
        &_vkVertexBuffer,
        &_vmaVertexBufferAllocation,
        &vertexBufferAllocationInfo
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vmaCreateBuffer failed: {}", vkResult);
    }

    memcpy(vertexBufferAllocationInfo.pMappedData, vertexData, sizeof(vertexData));

    ///
    /// _vkVertexShaderModule, _vkFragmentShaderModule
    ///

    VkShaderModuleCreateInfo vertexShaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = sizeof(Render2D_vert_spv),
        .pCode = reinterpret_cast<uint32_t *>(Render2D_vert_spv),
    };
    
    VkShaderModuleCreateInfo fragmentShaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = sizeof(Render2D_frag_spv),
        .pCode = reinterpret_cast<uint32_t *>(Render2D_frag_spv),
    };

    vkResult = _vkDeviceFuncs->vkCreateShaderModule(
        _vkDevice,
        &vertexShaderModuleCreateInfo,
        nullptr,
        &_vkVertexShaderModule
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreateShaderModule failed for 'Render2D.vert.spv': {}", vkResult);
    }
    
    vkResult = _vkDeviceFuncs->vkCreateShaderModule(
        _vkDevice,
        &fragmentShaderModuleCreateInfo,
        nullptr,
        &_vkFragmentShaderModule
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreateShaderModule failed for 'Render2D.frag.spv': {}", vkResult);
    }

    ///
    /// _vkDescriptorPool, _vkDescriptorSetLayout, _vkDescriptorSet
    ///

    Array<VkDescriptorPoolSize, 1> descriptorPoolSizeList = {
        VkDescriptorPoolSize{
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
        },
    };

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 1,
        .poolSizeCount = (uint32_t)descriptorPoolSizeList.size(),
        .pPoolSizes = descriptorPoolSizeList.data(),
    };

    vkResult = _vkDeviceFuncs->vkCreateDescriptorPool(
        _vkDevice,
        &descriptorPoolCreateInfo,
        nullptr,
        &_vkDescriptorPool
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreateDescriptorPool failed: {}", vkResult);
    }

    Array<VkDescriptorSetLayoutBinding, 1> descriptorSetLayoutBindingList = {
        VkDescriptorSetLayoutBinding{
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        },
    };

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = (uint32_t)descriptorSetLayoutBindingList.size(),
        .pBindings = descriptorSetLayoutBindingList.data(),
    };

    vkResult = _vkDeviceFuncs->vkCreateDescriptorSetLayout(
        _vkDevice,
        &descriptorSetLayoutCreateInfo,
        nullptr,
        &_vkDescriptorSetLayout
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreateDescriptorSetLayout failed: {}", vkResult);
    }

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = _vkDescriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &_vkDescriptorSetLayout,
    };

    vkResult = _vkDeviceFuncs->vkAllocateDescriptorSets(_vkDevice, &descriptorSetAllocateInfo, &_vkDescriptorSet);
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkAllocateDescriptorSets failed: {}", vkResult);
    }

    VkDescriptorImageInfo imageDescriptorInfo = {
        .sampler = _vkSampler,
        .imageView = _vkImageView,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, // TODO: Investigate
    };

    VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = _vkDescriptorSet,
        .dstBinding = 0,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo = &imageDescriptorInfo,
    };

    _vkDeviceFuncs->vkUpdateDescriptorSets(
        _vkDevice,
        1, &writeDescriptorSet,
        0, nullptr
    );

    ///
    /// _vkPipelineLayout, _vkPipeline
    ///

    Array<VkPipelineShaderStageCreateInfo, 2> shaderStageCreateInfoList = {
        VkPipelineShaderStageCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = _vkVertexShaderModule,
            .pName = "main",
        },
        VkPipelineShaderStageCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = _vkFragmentShaderModule,
            .pName = "main",
        },
    };

    Array<VkVertexInputBindingDescription, 1> vertexInputBindingList = {
        VkVertexInputBindingDescription{ 
            .binding = 0,
            .stride = 5 * sizeof(float),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        }
    };

    Array<VkVertexInputAttributeDescription, 2> vertexInputAttributeList = {
        // a_Position
        VkVertexInputAttributeDescription{
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = 0,
        },
        // a_TexCoord
        VkVertexInputAttributeDescription{
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = 3 * sizeof(float),
        },
    };

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = (uint32_t)vertexInputBindingList.size(),
        .pVertexBindingDescriptions = vertexInputBindingList.data(),
        .vertexAttributeDescriptionCount = (uint32_t)vertexInputAttributeList.size(),
        .pVertexAttributeDescriptions = vertexInputAttributeList.data(),
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    };

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1,
    };

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .lineWidth = 1.0f,
    };

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = _emulator->sampleCountFlagBits(),
    };

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
    };

    // TODO: Investigate
    Array<VkPipelineColorBlendAttachmentState, 1> colorBlendAttachmentList = {
        VkPipelineColorBlendAttachmentState{
            .colorWriteMask = 0xF,
        },
    };

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .attachmentCount = (uint32_t)colorBlendAttachmentList.size(),
        .pAttachments = colorBlendAttachmentList.data(),
    };

    Array<VkDynamicState, 2> dynamicStateList = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = (uint32_t)dynamicStateList.size(),
        .pDynamicStates = dynamicStateList.data(),
    };

    VkPushConstantRange pushConstantRange = {
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .offset = 0,
        .size = 2 * sizeof(Vec2),
    };

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &_vkDescriptorSetLayout,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &pushConstantRange,
    };

    vkResult = _vkDeviceFuncs->vkCreatePipelineLayout(
        _vkDevice,
        &pipelineLayoutCreateInfo,
        nullptr,
        &_vkPipelineLayout
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreatePipelineLayout failed: {}", vkResult);
    }

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = (uint32_t)shaderStageCreateInfoList.size(),
        .pStages = shaderStageCreateInfoList.data(),
        .pVertexInputState = &vertexInputStateCreateInfo,
        .pInputAssemblyState = &inputAssemblyStateCreateInfo,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        .pDepthStencilState = &depthStencilStateCreateInfo,
        .pColorBlendState = &colorBlendStateCreateInfo,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = _vkPipelineLayout,
        .renderPass = _emulator->defaultRenderPass(),
    };

    vkResult = _vkDeviceFuncs->vkCreateGraphicsPipelines(
        _vkDevice,
        nullptr,
        1, &pipelineCreateInfo,
        nullptr,
        &_vkPipeline
    );
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkCreateGraphicsPipelines failed: {}", vkResult);
    }

}

void Renderer2D::initSwapChainResources()
{
    IRenderer::initSwapChainResources();


}

void Renderer2D::releaseSwapChainResources()
{


    IRenderer::releaseSwapChainResources();
}

void Renderer2D::releaseResources()
{
    Log(KIWI_ANCHOR, "Renderer2D::releaseResources()");

    if (_vkPipeline) {
        _vkDeviceFuncs->vkDestroyPipeline(_vkDevice, _vkPipeline, nullptr);
        _vkPipeline = nullptr;
    }

    if (_vkPipelineLayout) {
        _vkDeviceFuncs->vkDestroyPipelineLayout(_vkDevice, _vkPipelineLayout, nullptr);
        _vkPipelineLayout = nullptr;
    }

    if (_vkDescriptorSetLayout) {
        _vkDeviceFuncs->vkDestroyDescriptorSetLayout(_vkDevice, _vkDescriptorSetLayout, nullptr);
        _vkDescriptorSetLayout = nullptr;
    }

    _vkDescriptorSet = nullptr;

    if (_vkDescriptorPool) {
        _vkDeviceFuncs->vkDestroyDescriptorPool(_vkDevice, _vkDescriptorPool, nullptr);
        _vkDescriptorPool = nullptr;
    }

    if (_vkFragmentShaderModule) {
        _vkDeviceFuncs->vkDestroyShaderModule(_vkDevice, _vkFragmentShaderModule, nullptr);
        _vkFragmentShaderModule = nullptr;
    }

    if (_vkVertexShaderModule) {
        _vkDeviceFuncs->vkDestroyShaderModule(_vkDevice, _vkVertexShaderModule, nullptr);
        _vkVertexShaderModule = nullptr;
    }

    if (_vmaVertexBufferAllocation) {
        vmaFreeMemory(_vmaAllocator, _vmaVertexBufferAllocation);
        _vmaVertexBufferAllocation = nullptr;
    }

    if (_vkVertexBuffer) {
        _vkDeviceFuncs->vkDestroyBuffer(_vkDevice, _vkVertexBuffer, nullptr);
        _vkVertexBuffer = nullptr;
    }

    if (_vkSampler) {
        _vkDeviceFuncs->vkDestroySampler(_vkDevice, _vkSampler, nullptr);
        _vkSampler = nullptr;
    }

    if (_vkImageView) {
        _vkDeviceFuncs->vkDestroyImageView(_vkDevice, _vkImageView, nullptr);
        _vkImageView = nullptr;
    }

    if (_vmaImageBufferAllocation) {
        vmaFreeMemory(_vmaAllocator, _vmaImageBufferAllocation);
        _vmaImageBufferAllocation = nullptr;
    }

    if (_vkImageBuffer) {
        _vkDeviceFuncs->vkDestroyBuffer(_vkDevice, _vkImageBuffer, nullptr);
        _vkImageBuffer = nullptr;
    }

    _imageBufferData = nullptr;

    if (_vmaImageAllocation) {
        vmaFreeMemory(_vmaAllocator, _vmaImageAllocation);
        _vmaImageAllocation = nullptr;
    }

    if (_vkImage) {
        _vkDeviceFuncs->vkDestroyImage(_vkDevice, _vkImage, nullptr);
        _vkImage = nullptr;
    }

    IRenderer::releaseResources();
}

void Renderer2D::startNextFrame()
{
    VkCommandBuffer commandBuffer = _emulator->currentCommandBuffer();

    memcpy(_imageBufferData, image().data(), image().size());

    VkImageMemoryBarrier imageMemoryBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED ,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = _vkImage,
        .subresourceRange = VkImageSubresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    _vkDeviceFuncs->vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier
    );

    VkBufferImageCopy bufferImageCopyRegion = {
        .imageSubresource = VkImageSubresourceLayers{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .imageOffset = VkOffset3D{ 0, 0, 0 },
        .imageExtent = VkExtent3D{ _imageSize.x, _imageSize.y, 1 },
    };

    _vkDeviceFuncs->vkCmdCopyBufferToImage(
        commandBuffer,
        _vkImageBuffer,
        _vkImage,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &bufferImageCopyRegion
    );
    
    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    _vkDeviceFuncs->vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier
    );

    Array<VkClearValue, 2> clearValues = {
        VkClearValue{
            .color = {{ 0.0f, 1.0f, 0.0f, 1.0f }},
        },
        VkClearValue{
            .depthStencil = { 1.0f, 0 },
        }
    };

    const QSize swapChainImageSize = _emulator->swapChainImageSize();
    VkRenderPassBeginInfo renderPassBeginInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = _emulator->defaultRenderPass(),
        .framebuffer = _emulator->currentFramebuffer(),
        .renderArea = VkRect2D{
            .extent = VkExtent2D{
                .width = (uint32_t)swapChainImageSize.width(),
                .height = (uint32_t)swapChainImageSize.height(),
            },
        },
        .clearValueCount = clearValues.size(),
        .pClearValues = clearValues.data(),
    };

    _vkDeviceFuncs->vkCmdBeginRenderPass(
        commandBuffer,
        &renderPassBeginInfo,
        VK_SUBPASS_CONTENTS_INLINE
    );

    VkViewport viewport = {
        .x = 0,
        .y = 0,
        .width = (float)swapChainImageSize.width(),
        .height = (float)swapChainImageSize.height(),
        .minDepth = 0,
        .maxDepth = 1,
    };
    _vkDeviceFuncs->vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {
        .offset = VkOffset2D{ 0, 0 },
        .extent = VkExtent2D{
            .width = (uint32_t)swapChainImageSize.width(),
            .height = (uint32_t)swapChainImageSize.height()
        },
    };
    _vkDeviceFuncs->vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    float zoom = std::min(
        (float)swapChainImageSize.width() / (float)_imageSize.x,
        (float)swapChainImageSize.height() / (float)_imageSize.y
    );

    Vec2 size = {
        (float)_imageSize.x / (float)swapChainImageSize.width() * zoom,
        (float)_imageSize.y / (float)swapChainImageSize.height() * zoom,
    };

    Vec2 offset = {
        (1.0f - size.x) * 0.5f,
        (1.0f - size.y) * 0.5f,
    };

    Vec2 pushConstants[] = { size, offset };

    _vkDeviceFuncs->vkCmdPushConstants(
        commandBuffer,
        _vkPipelineLayout,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(pushConstants), pushConstants
    );

    _vkDeviceFuncs->vkCmdBindPipeline(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        _vkPipeline
    );

    _vkDeviceFuncs->vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        _vkPipelineLayout,
        0,
        1, &_vkDescriptorSet,
        0, nullptr
    );

    VkDeviceSize vertexBufferOffset = 0;
    _vkDeviceFuncs->vkCmdBindVertexBuffers(
        commandBuffer,
        0, 1,
        &_vkVertexBuffer,
        &vertexBufferOffset
    );

    _vkDeviceFuncs->vkCmdDraw(commandBuffer, 6, 1, 0, 0);

    _vkDeviceFuncs->vkCmdEndRenderPass(commandBuffer);

    IRenderer::startNextFrame();
}

void Renderer2D::nextImage()
{
    _imageIndex = (_imageIndex + 1) % _imageCount;
}

} // namespace kiwi
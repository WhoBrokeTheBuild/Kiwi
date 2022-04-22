#include <Kiwi/Renderer.hpp>
#include <Kiwi/Emulator.hpp>
#include <Kiwi/Exception.hpp>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace kiwi {

static VKAPI_ATTR VkBool32 VKAPI_CALL _VulkanDebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
    void * userData)
{
    kiwi::Log("VkDebugUtilsMessenger", "{} {}",
        callbackData->pMessageIdName, 
        callbackData->pMessage);

    return VK_FALSE;
}

Renderer::Renderer(Emulator * emulator)
    : _emulator(emulator)
{

}

void Renderer::initResources()
{
    VkResult vkResult;

    _vkInstance = _emulator->vulkanInstance();
    _vkDevice = _emulator->device();
    _vkPhysicalDevice = _emulator->physicalDevice();

    _vkFuncs = _vkInstance->functions();
    _vkDeviceFuncs = _vkInstance->deviceFunctions(_vkDevice);

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = 
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = _VulkanDebugMessageCallback,
    };

    auto vkCreateDebugUtilsMessengerEXT = 
        (PFN_vkCreateDebugUtilsMessengerEXT)_vkInstance->getInstanceProcAddr("vkCreateDebugUtilsMessengerEXT");
    if (vkCreateDebugUtilsMessengerEXT) {
        vkCreateDebugUtilsMessengerEXT(
            _vkInstance->vkInstance(),
            &debugUtilsMessengerCreateInfo,
            nullptr,
            &_vkDebugUtilsMessenger
        );
    }

    VmaVulkanFunctions vmaFunctions = {
        .vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)_vkInstance->getInstanceProcAddr("vkGetInstanceProcAddr"),
        .vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)_vkInstance->getInstanceProcAddr("vkGetDeviceProcAddr"),
    };

    VmaAllocatorCreateInfo allocatorCreateInfo = {
        .flags = 0,
        .physicalDevice = _vkPhysicalDevice,
        .device = _vkDevice,
        .pVulkanFunctions = &vmaFunctions,
        .instance = _vkInstance->vkInstance(),
    };

    vkResult = vmaCreateAllocator(&allocatorCreateInfo, &_vmaAllocator);
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vmaCreateAllocator failed: {}", vkResult);
    }
}

void Renderer::initSwapChainResources()
{
    
}

void Renderer::releaseSwapChainResources()
{
    
}

void Renderer::releaseResources()
{
    if (_vmaAllocator) {
        vmaDestroyAllocator(_vmaAllocator);
        _vmaAllocator = nullptr;
    }
    
    auto vkDestroyDebugUtilsMessengerEXT = 
        (PFN_vkDestroyDebugUtilsMessengerEXT)_vkInstance->getInstanceProcAddr("vkDestroyDebugUtilsMessengerEXT");
    if (vkDestroyDebugUtilsMessengerEXT && _vkDebugUtilsMessenger) {
        vkDestroyDebugUtilsMessengerEXT(_vkInstance->vkInstance(), _vkDebugUtilsMessenger, nullptr);
        _vkDebugUtilsMessenger = nullptr;
    }
    
    _vkInstance = nullptr;
    _vkDevice = nullptr;
    _vkPhysicalDevice = nullptr;
    _vkFuncs = nullptr;
    _vkDeviceFuncs = nullptr;
}

void Renderer::startNextFrame()
{
    _emulator->frameReady();
    _emulator->requestUpdate();

    _emulator->doFrame();
}

VkCommandBuffer Renderer::beginOneTimeCommandBuffer()
{
    VkResult vkResult;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = _emulator->graphicsCommandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer commandBuffer;
    vkResult = _vkDeviceFuncs->vkAllocateCommandBuffers(_vkDevice, &commandBufferAllocateInfo, &commandBuffer);
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkAllocateCommandBuffers failed: {}", vkResult);
    }

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    _vkDeviceFuncs->vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Renderer::endOneTimeCommandBuffer(VkCommandBuffer commandBuffer)
{
    VkResult vkResult;

    _vkDeviceFuncs->vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
    };

    vkResult = _vkDeviceFuncs->vkQueueSubmit(_emulator->graphicsQueue(), 1, &submitInfo, nullptr);
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkQueueSubmit failed: {}", vkResult);
    }

    vkResult = _vkDeviceFuncs->vkQueueWaitIdle(_emulator->graphicsQueue());
    if (vkResult != VK_SUCCESS) {
        throw RuntimeError("vkQueueWaitIdle failed: {}", vkResult);
    }
    
    _vkDeviceFuncs->vkFreeCommandBuffers(_vkDevice, _emulator->graphicsCommandPool(), 1, &commandBuffer);
}

} // namespace kiwi
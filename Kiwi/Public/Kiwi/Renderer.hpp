#ifndef KIWI_RENDERER_HPP
#define KIWI_RENDERER_HPP

#include <Kiwi/Config.hpp>

KIWI_DISABLE_WARNINGS()

    #define VMA_STATS_STRING_ENABLED 0
    #include <vk_mem_alloc.h>

KIWI_ENABLE_WARNINGS()

#include <QVulkanWindowRenderer>

#include <QVulkanFunctions>
#include <QVulkanDeviceFunctions>

namespace kiwi {

class Emulator;

class KIWI_API Renderer : public QVulkanWindowRenderer
{
public:

    Renderer(Emulator * emulator);

    virtual ~Renderer() = default;

    void initResources() override;

    void initSwapChainResources() override;

    void releaseSwapChainResources() override;

    void releaseResources() override;

    void startNextFrame() override;

protected:

    Emulator * _emulator;

    QVulkanInstance * _vkInstance;

    VkDevice _vkDevice;

    VkPhysicalDevice _vkPhysicalDevice;

    QVulkanFunctions * _vkFuncs;

    QVulkanDeviceFunctions * _vkDeviceFuncs;

    VkDebugUtilsMessengerEXT _vkDebugUtilsMessenger;

    VmaAllocator _vmaAllocator;

    VkCommandBuffer beginOneTimeCommandBuffer();

    void endOneTimeCommandBuffer(VkCommandBuffer commandBuffer);

}; // class Renderer

} // namespace kiwi

#endif // KIWI_RENDERER_HPP
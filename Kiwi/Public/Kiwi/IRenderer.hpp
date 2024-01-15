#ifndef Kiwi_IRenderer_HPP
#define Kiwi_IRenderer_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/Utility/Log.hpp>

KIWI_DISABLE_WARNINGS()

    #define VMA_STATS_STRING_ENABLED 0
    #include <vma/vk_mem_alloc.h>

KIWI_ENABLE_WARNINGS()

#include <QVulkanWindowRenderer>

#include <QVulkanFunctions>
#include <QVulkanDeviceFunctions>

// TODO: Move
inline auto format_as(VkResult vkResult) {
    return fmt::underlying(vkResult);
}

namespace kiwi {

class IEmulator;

class KIWI_API IRenderer : public QVulkanWindowRenderer
{
public:

    IRenderer(IEmulator * emulator);

    virtual ~IRenderer() = default;

    void initResources() override;

    void initSwapChainResources() override;

    void releaseSwapChainResources() override;

    void releaseResources() override;

    void startNextFrame() override;

protected:

    IEmulator * _emulator;

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

#endif // Kiwi_IRenderer_HPP
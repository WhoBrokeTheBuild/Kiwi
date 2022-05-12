#ifndef KIWI_RENDERER_2D_HPP
#define KIWI_RENDERER_2D_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/Renderer.hpp>
#include <Kiwi/Containers.hpp>
#include <Kiwi/Math.hpp>

namespace kiwi {

class KIWI_API Renderer2D : public Renderer
{
public:

    Renderer2D(Emulator * emulator, Vec2u size, unsigned count = 2);
    
    void initResources() override;

    void initSwapChainResources() override;

    void releaseSwapChainResources() override;

    void releaseResources() override;

    void startNextFrame() override;

    void nextImage();

    inline Span<uint8_t> image() {
        return _imageList[_imageIndex];
    }

    inline Vec2u imageSize() const {
        return _imageSize;
    }

protected:

    unsigned _imageCount;

    unsigned _imageIndex = 0;

    Vec2u _imageSize;

    List<List<uint8_t>> _imageList;

    const unsigned _components = 4; // RGBA

    VkImage _vkImage = nullptr;

    VmaAllocation _vmaImageAllocation = nullptr;

    VkImageView _vkImageView = nullptr;
    
    VkBuffer _vkImageBuffer = nullptr;

    VmaAllocation _vmaImageBufferAllocation = nullptr;

    void * _imageBufferData = nullptr;

    VkSampler _vkSampler = nullptr;

    VkShaderModule _vkVertexShaderModule = nullptr;

    VkShaderModule _vkFragmentShaderModule = nullptr;

    VkBuffer _vkVertexBuffer = nullptr;

    VmaAllocation _vmaVertexBufferAllocation = nullptr;

    VkDescriptorPool _vkDescriptorPool = nullptr;

    VkDescriptorSetLayout _vkDescriptorSetLayout = nullptr;

    VkDescriptorSet _vkDescriptorSet = nullptr;

    VkPipelineLayout _vkPipelineLayout = nullptr;

    VkPipeline _vkPipeline = nullptr;

}; // class Renderer2D

} // namespace kiwi

#endif // KIWI_RENDERER_2D_HPP
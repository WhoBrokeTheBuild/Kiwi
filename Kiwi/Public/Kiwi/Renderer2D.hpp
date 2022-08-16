#ifndef KIWI_Renderer2D_HPP
#define KIWI_Renderer2D_HPP

#include <Kiwi/Config.hpp>
#include <Kiwi/IRenderer.hpp>
#include <Kiwi/Utility/Containers.hpp>
#include <Kiwi/Utility/Math.hpp>
#include <Kiwi/Utility/Log.hpp>

namespace kiwi {

class KIWI_API Renderer2D : public IRenderer
{
public:

    Renderer2D(IEmulator * emulator, Vec2u size, unsigned count = 2);

    virtual ~Renderer2D() {
        Log(KIWI_ANCHOR, "~Renderer2D()");
    }
    
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

    inline unsigned components() const {
        return _components;
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

#endif // KIWI_Renderer2D_HPP
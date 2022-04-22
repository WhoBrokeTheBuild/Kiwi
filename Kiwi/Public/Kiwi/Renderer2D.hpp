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

    Renderer2D(Emulator * emulator, Vec2u size);
    
    void initResources() override;

    void initSwapChainResources() override;

    void releaseSwapChainResources() override;

    void releaseResources() override;

    void startNextFrame() override;

    inline Vec2u size() const {
        return _size;
    }
    
    inline uint width() const {
        return _size.x;
    }
    
    inline uint height() const {
        return _size.y;
    }

    inline Span<uint8_t> image() {
        return _image;
    }

protected:

    Vec2u _size;

    List<uint8_t> _image;

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
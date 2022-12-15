#pragma once
#include "../IRenderer.hpp"
#include "../TextureCache.hpp"
#include "../PaletteCache.hpp"

#include <volk/volk.h>

#define VK_NO_PROTOTYPES 
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <optional>

class VulkanRenderer final : public IRenderer
{
public:
    explicit VulkanRenderer(TWindowHandleType window);
    ~VulkanRenderer();
    void Clear(u8 r, u8 g, u8 b) override;
    void StartFrame() override;
    void EndFrame() override;
    void SetTPage(u16 tPage) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void ToggleFilterScreen() override;
    void Draw(Prim_GasEffect& gasEffect) override;
    void Draw(Line_G2& line) override;
    void Draw(Line_G4& line) override;
    void Draw(Poly_G3& poly) override;
    void Draw(Poly_FT4& poly) override;
    void Draw(Poly_G4& poly) override;

private:
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    bool mFrameStarted = false;

    // TODO: Simply down the prim types so we don't need this
    u16 mGlobalTPage = 0;

    PaletteCache mPaletteCache;
    //DirectX9TextureCache mTextureCache;

    void initVulkan();

    void cleanupSwapChain();
    void cleanup();
    void recreateSwapChain();

    void createInstance();

    void setupDebugMessenger();

    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();

    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createTextureImage(const char* bmpName, std::unique_ptr<vk::raii::Image>& textureImage, std::unique_ptr<vk::raii::DeviceMemory>& deviceMemory);
    void createTextureImageView();
    void createTextureSampler();
    vk::raii::ImageView createImageView(vk::Image image, vk::Format format);
    std::pair<std::unique_ptr<vk::raii::Image>, std::unique_ptr<vk::raii::DeviceMemory>> createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);
    void transitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
    void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    std::pair<std::unique_ptr<vk::raii::Buffer>, std::unique_ptr<vk::raii::DeviceMemory>> createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
    vk::raii::CommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(vk::raii::CommandBuffer& commandBuffer);
    void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    void createCommandBuffers();
    void recordCommandBuffer(vk::raii::CommandBuffer& commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void updateUniformBuffer(uint32_t currentImage);
    void drawFrame();
    vk::raii::ShaderModule createShaderModule(const std::vector<char>& code);
    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(vk::raii::PhysicalDevice& vkDevice);
    bool isDeviceSuitable(vk::raii::PhysicalDevice& vkDevice);
    bool checkDeviceExtensionSupport(vk::raii::PhysicalDevice& vkDevice);
    QueueFamilyIndices findQueueFamilies(vk::raii::PhysicalDevice& vkDevice);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    static std::vector<char> readFile(const std::string& filename);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT /*messageSeverity*/, VkDebugUtilsMessageTypeFlagsEXT /*messageType*/, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* /*pUserData*/);

    std::unique_ptr<vk::raii::Context> mContext;
    std::unique_ptr<vk::raii::Instance> mInstance;
    std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> mDebugMessenger;
    std::unique_ptr<vk::raii::SurfaceKHR> mSurface;

    std::unique_ptr<vk::raii::PhysicalDevice> mPhysicalDevice;
    std::unique_ptr<vk::raii::Device> mDevice;

    std::unique_ptr<vk::raii::Queue> mGraphicsQueue;
    std::unique_ptr<vk::raii::Queue> mPresentQueue;

    std::unique_ptr<vk::raii::SwapchainKHR> mSwapChain;
    std::vector<vk::Image> mSwapChainImages;
    vk::Format mSwapChainImageFormat;
    vk::Extent2D mSwapChainExtent;
    std::vector<vk::raii::ImageView> mSwapChainImageViews;
    std::vector<vk::raii::Framebuffer> mSwapChainFramebuffers;

    std::unique_ptr<vk::raii::RenderPass> mRenderPass;
    std::unique_ptr<vk::raii::DescriptorSetLayout> mDescriptorSetLayout;
    std::unique_ptr<vk::raii::PipelineLayout> mPipelineLayout;
    std::unique_ptr<vk::raii::Pipeline> mGraphicsPipeline;

    std::unique_ptr<vk::raii::CommandPool> mCommandPool;

    std::unique_ptr<vk::raii::Image> mTextureImage;
    std::unique_ptr<vk::raii::DeviceMemory> mTextureImageMemory;

    std::unique_ptr<vk::raii::Image> mTextureImage2;
    std::unique_ptr<vk::raii::DeviceMemory> mTextureImageMemory2;

    std::unique_ptr<vk::raii::ImageView> mTextureImageView;
    std::unique_ptr<vk::raii::ImageView> mTextureImageView2;
    std::unique_ptr<vk::raii::Sampler> mTextureSampler;

    std::unique_ptr<vk::raii::Buffer> mVertexBuffer;
    std::unique_ptr<vk::raii::DeviceMemory> mVertexBufferMemory;
    std::unique_ptr<vk::raii::Buffer> mIndexBuffer;
    std::unique_ptr<vk::raii::DeviceMemory> mIndexBufferMemory;

    std::vector<std::unique_ptr<vk::raii::Buffer>> mUniformBuffers;
    std::vector<std::unique_ptr<vk::raii::DeviceMemory>> mUniformBuffersMemory;
    std::vector<void*> mUniformBuffersMapped;

    std::unique_ptr<vk::raii::DescriptorPool> mDescriptorPool;
    std::vector<vk::raii::DescriptorSet> mDescriptorSets;

    std::vector<std::unique_ptr<vk::raii::CommandBuffer>> mCommandBuffers;

    std::vector<vk::raii::Semaphore> mImageAvailableSemaphores;
    std::vector<vk::raii::Semaphore> mRenderFinishedSemaphores;
    std::vector<vk::raii::Fence> mInFlightFences;
    uint32_t mCurrentFrame = 0;
};

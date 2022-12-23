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

    enum PipelineIndex
    {
        eReverseBlending = 0,
        eAddBlending = 1,
    };
    void createGraphicsPipeline(PipelineIndex idx);
    void createFramebuffers();
    void createCommandPool();
    void createTextureSampler();
    vk::raii::ImageView createImageView(vk::Image image, vk::Format format);
    std::pair<std::unique_ptr<vk::raii::Image>, std::unique_ptr<vk::raii::DeviceMemory>> createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);
    void transitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
    void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void updateDescriptorSets();
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

    void DecreaseResourceLifetimes();

    u32 PreparePalette(AnimationPal& pCache);

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

    std::vector<std::unique_ptr<vk::raii::PipelineLayout>> mPipelineLayouts;
    std::vector<std::unique_ptr<vk::raii::Pipeline>> mGraphicsPipelines;

    std::unique_ptr<vk::raii::CommandPool> mCommandPool;

    class Texture final
    {
    public:
        enum Format
        {
            RGBA,
            Indexed,
        };

        explicit Texture(VulkanRenderer& renderer, u32 width, u32 height, void* pPixels, Format format)
            : mRenderer(renderer)
            , mFormat(format)
        {
            vk::DeviceSize imageSize = width * height * (mFormat == Format::Indexed ? 1 : 4);
            vk::Format imgFormat = format == Format::Indexed ? vk::Format::eR8Unorm : vk::Format::eR8G8B8A8Srgb;
            auto [stagingBuffer, stagingBufferMemory] = mRenderer.createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            void* data = stagingBufferMemory->mapMemory(0, imageSize);
            memcpy(data, pPixels, static_cast<std::size_t>(imageSize));
            stagingBufferMemory->unmapMemory();

            auto [textureImage, deviceMemory] = mRenderer.createImage(width, height, imgFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
            mImage = std::move(textureImage);
            mMemory = std::move(deviceMemory);

            mRenderer.transitionImageLayout(**mImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral);
            mRenderer.copyBufferToImage(**stagingBuffer, **mImage, 0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
//            mRenderer.transitionImageLayout(**mImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

            mView = std::make_unique<vk::raii::ImageView>(mRenderer.createImageView(**mImage, imgFormat));
        }

        const std::unique_ptr<vk::raii::ImageView>& View() const
        {
            return mView;
        }

        void LoadSubImage(u32 x, u32 y, u32 width, u32 height, RGBA32* pPixels)
        {
            vk::DeviceSize imageSize = width * height * (mFormat == Format::Indexed ? 1 : 4);
            auto [stagingBuffer, stagingBufferMemory] = mRenderer.createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            void* data = stagingBufferMemory->mapMemory(0, imageSize);
            memcpy(data, pPixels, static_cast<std::size_t>(imageSize));
            stagingBufferMemory->unmapMemory();

            mRenderer.copyBufferToImage(**stagingBuffer, **mImage, x, y, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
       //     mRenderer.transitionImageLayout(**mImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
        }

    private:
        std::unique_ptr<vk::raii::Image> mImage;
        std::unique_ptr<vk::raii::DeviceMemory> mMemory;
        std::unique_ptr<vk::raii::ImageView> mView;
        VulkanRenderer& mRenderer;
        Format mFormat = Format::Indexed;
    };

    class VulkanTextureCache final : public TextureCache2<std::shared_ptr<Texture>>
    {
    public:
    };
    VulkanTextureCache mTextureCache;

    std::unique_ptr<Texture> mPaletteTexture;

    u32 mTextureArrayIdx = 0;
    std::vector<std::shared_ptr<Texture>> mTexturesForThisFrame;
    struct RenderBatch final
    {
        PipelineIndex mPipeline = PipelineIndex::eAddBlending;
        u32 mNumTrisToDraw = 0;
        u32 mTexturesInBatch = 0;
    };
    RenderBatch mConstructingBatch;
    std::vector<RenderBatch> mBatches;

    // Apparently 1 sampler can do all the textures in the shader
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

    u16 mIndexBufferIndex = 0;
};

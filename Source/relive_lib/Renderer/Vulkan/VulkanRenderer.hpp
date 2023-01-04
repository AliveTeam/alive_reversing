#pragma once
#include "../IRenderer.hpp"
#include "../TextureCache.hpp"
#include "../PaletteCache.hpp"

#include <volk/volk.h>

#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#define VK_NO_PROTOTYPES 
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <optional>
#include <SDL_vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VulkanLib final
{
public:
    VulkanLib()
    {
        TRACE_ENTRYEXIT;

        int ret = SDL_Vulkan_LoadLibrary(nullptr);
        if (ret == 0)
        {
            LOG_INFO("Vulkan lib loaded");
            mLoaded = true;
        }
        else
        {
            std::string path = GetDylibPath();
            LOG_ERROR("Failed to load vulkan lib %s trying %s", SDL_GetError(), path.c_str());
            ret = SDL_Vulkan_LoadLibrary(path.c_str());
            if (ret == 0)
            {
                LOG_INFO("Vulkan lib loaded (2nd attempt)");
                mLoaded = true;
            }
            else
            {
                LOG_ERROR("Failed to load vulkan lib %s (2nd attempt)", SDL_GetError());
            }
        }
    }

    static std::string GetDylibPath()
    {
        char* tmp = SDL_GetBasePath();
        std::string path = tmp;
        path = path.substr(0, path.length() - 10);
        SDL_free(tmp);
        path += "Frameworks/libMoltenVK.dylib";
        LOG_INFO("dylib path = %s", path.c_str());
        return path;
    }

    ~VulkanLib()
    {
        TRACE_ENTRYEXIT;

        if (mLoaded)
        {
            SDL_Vulkan_UnloadLibrary();
        }
    }

private:
    bool mLoaded = false;
};

class ScopedDynamicLib final
{
public:
    explicit ScopedDynamicLib(const char_type* pLib)
    {
        mLibHandle = SDL_LoadObject(pLib);
    }

    ~ScopedDynamicLib()
    {
        if (Loaded())
        {
            SDL_UnloadObject(mLibHandle);
        }
    }

    template<typename T>
    T GetFn(const char_type* pFnName) const
    {
        return reinterpret_cast<T>(SDL_LoadFunction(mLibHandle, pFnName));
    }

    bool Loaded() const
    {
        return mLibHandle != nullptr;
    }

private:
    void* mLibHandle = nullptr;
};

static constexpr int MAX_FRAMES_IN_FLIGHT = 1; // TODO: FIX ME, >= 2 seems to result in the wrong descriptors being bound

template <typename TextureType, typename RenderBatchType, typename VertexType>
class Batcher final
{
public:
    std::unique_ptr<TextureType> mPaletteTexture;
    std::shared_ptr<TextureType> mCamTexture;
    std::vector<std::shared_ptr<TextureType>> mBatchTextures;

    struct RenderBatch final : public RenderBatchType
    {
        u32 mNumTrisToDraw = 0;
        u32 mTexturesInBatch = 0;
        u32 mTextureIds[14] = {};

        bool AddTexture(u32 id)
        {
            for (u32 i = 0; i < mTexturesInBatch; i++)
            {
                if (mTextureIds[i] == id)
                {
                    // Already have it
                    return false;
                }
            }
            mTextureIds[mTexturesInBatch] = id;
            mTexturesInBatch++;
            return true;
        }

        u32 TextureIdxForId(u32 id) const
        {
            for (u32 i = 0; i < mTexturesInBatch; i++)
            {
                if (mTextureIds[i] == id)
                {
                    return i;
                }
            }
            return mTexturesInBatch;
        }
    };

    void NewBatch()
    {
        mBatches.push_back(mConstructingBatch);
        mConstructingBatch = {};
        mBatchInProgress = false;
    }

    RenderBatch mConstructingBatch;
    std::vector<RenderBatch> mBatches;
    bool mBatchInProgress = false;
    u16 mIndexBufferIndex = 0;

    std::vector<VertexType> mVertices;
    std::vector<uint16_t> mIndices;
};

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

    vk::raii::CommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(vk::raii::CommandBuffer& commandBuffer);

private:
#ifdef __APPLE__
    std::unique_ptr<ScopedDynamicLib> mMoltenVkLib;
#endif

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

    PaletteCache mPaletteCache[MAX_FRAMES_IN_FLIGHT];


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
        eNone = -1,
        eReverseBlending = 0,
        eAddBlending = 1,
        eFBOPipeline = 2,
    };
    void createGraphicsPipeline(PipelineIndex idx);
    void createFramebuffers();
    void createOffScreenPass(u32 idx);
    void createCommandPool();
    void createTextureSampler();
    vk::raii::ImageView createImageView(vk::Image image, vk::Format format);
    std::pair<std::unique_ptr<vk::raii::Image>, std::unique_ptr<vk::raii::DeviceMemory>> createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);
    void transitionImageLayout(vk::raii::CommandBuffer& commandBuffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
    void copyBufferToImage(vk::raii::CommandBuffer& commandBuffer, vk::Buffer buffer, vk::Image image, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void createVertexBuffer(u32 idx);
    void createIndexBuffer(u32 idx);
    void createUniformBuffers(u32 idx);
    void createDescriptorPool();
    void createDescriptorSets();
    void updateDescriptorSets();
    std::pair<std::unique_ptr<vk::raii::Buffer>, std::unique_ptr<vk::raii::DeviceMemory>> createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    void createCommandBuffers();
    void recordCommandBuffer(vk::raii::CommandBuffer& commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void updateUniformBuffer();
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
            vk::Format imgFormat = format == Format::Indexed ? vk::Format::eR8Unorm : vk::Format::eR8G8B8A8Unorm;
            auto [stagingBuffer, stagingBufferMemory] = mRenderer.createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
            mStagingBuffer = std::move(stagingBuffer);
            mStagingBufferMemory = std::move(stagingBufferMemory);

            void* data = mStagingBufferMemory->mapMemory(0, imageSize);
            memcpy(data, pPixels, static_cast<std::size_t>(imageSize));
            mStagingBufferMemory->unmapMemory();

            auto [textureImage, deviceMemory] = mRenderer.createImage(width, height, imgFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
            mImage = std::move(textureImage);
            mMemory = std::move(deviceMemory);

            auto commandBuffer = mRenderer.beginSingleTimeCommands();
            mRenderer.transitionImageLayout(commandBuffer, **mImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
            mRenderer.copyBufferToImage(commandBuffer, **mStagingBuffer, **mImage, 0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            mRenderer.transitionImageLayout(commandBuffer, **mImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
            mRenderer.endSingleTimeCommands(commandBuffer);

            mView = std::make_unique<vk::raii::ImageView>(mRenderer.createImageView(**mImage, imgFormat));
        }

        const std::unique_ptr<vk::raii::ImageView>& View() const
        {
            return mView;
        }

        void LoadSubImage(u32 x, u32 y, u32 width, u32 height, RGBA32* pPixels)
        {
            vk::DeviceSize imageSize = width * height * (mFormat == Format::Indexed ? 1 : 4);

            void* data = mStagingBufferMemory->mapMemory(0, imageSize);
            memcpy(data, pPixels, static_cast<std::size_t>(imageSize));
            mStagingBufferMemory->unmapMemory();

            auto commandBuffer = mRenderer.beginSingleTimeCommands();
            mRenderer.transitionImageLayout(commandBuffer, **mImage, vk::ImageLayout::eShaderReadOnlyOptimal, vk::ImageLayout::eTransferDstOptimal);
            mRenderer.copyBufferToImage(commandBuffer, **mStagingBuffer, **mImage, x, y, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            mRenderer.transitionImageLayout(commandBuffer, **mImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
            mRenderer.endSingleTimeCommands(commandBuffer);
        }

    private:
        std::unique_ptr<vk::raii::Buffer> mStagingBuffer;
        std::unique_ptr<vk::raii::DeviceMemory> mStagingBufferMemory;
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
    VulkanTextureCache mTextureCache[MAX_FRAMES_IN_FLIGHT];

    struct Vertex final
    {
        glm::vec2 pos;
        u8 color[3];
        glm::vec2 texCoord;
        u32 mSamplerIdx;
        u32 mPalIdx;
        IRenderer::PsxDrawMode mDrawType;
        u32 mIsShaded;
        u32 mBlendMode;
        u32 mIsSemiTrans;

        static vk::VertexInputBindingDescription getBindingDescription()
        {
            vk::VertexInputBindingDescription bindingDescription;
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = vk::VertexInputRate::eVertex;

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 9> getAttributeDescriptions()
        {
            std::array<vk::VertexInputAttributeDescription, 9> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = vk::Format::eR8G8B8Uscaled;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            attributeDescriptions[3].binding = 0;
            attributeDescriptions[3].location = 3;
            attributeDescriptions[3].format = vk::Format::eR32Uint;
            attributeDescriptions[3].offset = offsetof(Vertex, mSamplerIdx);

            attributeDescriptions[4].binding = 0;
            attributeDescriptions[4].location = 4;
            attributeDescriptions[4].format = vk::Format::eR32Uint;
            attributeDescriptions[4].offset = offsetof(Vertex, mPalIdx);

            attributeDescriptions[5].binding = 0;
            attributeDescriptions[5].location = 5;
            attributeDescriptions[5].format = vk::Format::eR32Uint;
            attributeDescriptions[5].offset = offsetof(Vertex, mDrawType);

            attributeDescriptions[6].binding = 0;
            attributeDescriptions[6].location = 6;
            attributeDescriptions[6].format = vk::Format::eR32Uint;
            attributeDescriptions[6].offset = offsetof(Vertex, mIsShaded);

            attributeDescriptions[7].binding = 0;
            attributeDescriptions[7].location = 7;
            attributeDescriptions[7].format = vk::Format::eR32Uint;
            attributeDescriptions[7].offset = offsetof(Vertex, mBlendMode);

            attributeDescriptions[8].binding = 0;
            attributeDescriptions[8].location = 8;
            attributeDescriptions[8].format = vk::Format::eR32Uint;
            attributeDescriptions[8].offset = offsetof(Vertex, mIsSemiTrans);

            return attributeDescriptions;
        }
    };

    struct BatchData
    {
        PipelineIndex mPipeline = PipelineIndex::eNone;
    };
    Batcher<Texture, BatchData, Vertex> mBatcher[MAX_FRAMES_IN_FLIGHT];
  
    struct FrameBufferAttachment final
    {
        std::unique_ptr<vk::raii::Image> image;
        std::unique_ptr<vk::raii::DeviceMemory> mem;
        std::unique_ptr<vk::raii::ImageView> view;
    };

    struct OffscreenPass final
    {
        int32_t width;
        int32_t height;
        std::unique_ptr<vk::raii::Framebuffer> frameBuffer;
        FrameBufferAttachment color;
        std::unique_ptr<vk::raii::RenderPass> renderPass;
        std::unique_ptr<vk::raii::Sampler> sampler;
        vk::DescriptorImageInfo descriptor;
    };
    OffscreenPass mOffScreenPass[MAX_FRAMES_IN_FLIGHT] = {};

    // Apparently 1 sampler can do all the textures in the shader
    std::unique_ptr<vk::raii::Sampler> mTextureSampler[MAX_FRAMES_IN_FLIGHT];

    struct MappedBuffer final
    {
        void clear()
        {
            mBuffer->clear();
            mBufferMemory->clear();
            mBufferSize = 0;
            mMappedBuffferMemory = nullptr;
        }

        template<typename T>
        void makeBuffer(VulkanRenderer& renderer, std::unique_ptr<vk::raii::Device>& device, vk::BufferUsageFlags usageFlags, vk::MemoryPropertyFlags memoryFlags, const T* pData, size_t dataElementCount)
        {
            const vk::DeviceSize bufferSize = sizeof(T) * dataElementCount;

            // Create if we don't have a buffer or its too small
            if (!mBuffer || !mBufferMemory || mBufferSize < bufferSize)
            {
                mBufferSize = bufferSize;
                if (bufferSize < sizeof(T) * 1024)
                {
                    mBufferSize = sizeof(T) * 1024;
                }

                // The buffer is host and device visible and we keep it mapped for fast updating
                auto [tmpBuffer, tmpMemory] = renderer.createBuffer(mBufferSize, usageFlags, memoryFlags);
                mBuffer = std::move(tmpBuffer);
                mBufferMemory = std::move(tmpMemory);

                mMappedBuffferMemory = mBufferMemory->mapMemory(0, mBufferSize);
            }

            memcpy(mMappedBuffferMemory, pData, (size_t) bufferSize);

            vk::MappedMemoryRange mappedRange;
            mappedRange.memory = **mBufferMemory;
            mappedRange.size = VK_WHOLE_SIZE; // TODO: Limit size to corrrect multiple
            device->flushMappedMemoryRanges(mappedRange);
        }

        std::unique_ptr<vk::raii::Buffer> mBuffer;
        std::unique_ptr<vk::raii::DeviceMemory> mBufferMemory;
        vk::DeviceSize mBufferSize;
        void* mMappedBuffferMemory = nullptr;
    };

    MappedBuffer mVertexBuffer[MAX_FRAMES_IN_FLIGHT];
    MappedBuffer mIndexBuffer[MAX_FRAMES_IN_FLIGHT];
    MappedBuffer mUniformBuffer[MAX_FRAMES_IN_FLIGHT];

    std::unique_ptr<vk::raii::DescriptorPool> mDescriptorPool;
    std::vector<vk::raii::DescriptorSet> mDescriptorSets;

    std::vector<std::unique_ptr<vk::raii::CommandBuffer>> mCommandBuffers;

    std::vector<vk::raii::Semaphore> mImageAvailableSemaphores;
    std::vector<vk::raii::Semaphore> mRenderFinishedSemaphores;
    std::vector<vk::raii::Fence> mInFlightFences;
    uint32_t mCurrentFrame = 0;
};

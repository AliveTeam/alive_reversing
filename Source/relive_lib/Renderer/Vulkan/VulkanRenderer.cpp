#include "VulkanRenderer.hpp"

#include "../../../relive_lib/FatalError.hpp"
#include "../../../relive_lib/ResourceManagerWrapper.hpp"
#include "../../../relive_lib/Animation.hpp"
#include "../../../AliveLibAE/Font.hpp"
#include <SDL_syswm.h>
#include <cmath>

#include <SDL.h>
#include <SDL_vulkan.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <set>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "fragment_shader.h"
#include "vertex_shader.h"

static constexpr int MAX_FRAMES_IN_FLIGHT = 1; // TODO: FIX ME, >= 2 seems to result in the wrong descriptors being bound


const std::vector<const char*> kValidationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char*> kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

static const vk::Format kFramebufferFormat = vk::Format::eR8G8B8A8Unorm;

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

struct UniformBufferObject final
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

// TODO: Make memebers
static std::vector<Vertex> vertices;
static std::vector<uint16_t> gIndices;

void VulkanRenderer::initVulkan()
{
    TRACE_ENTRYEXIT;

    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();

    createCommandPool();

    std::vector<RGBA32> blackPixels(256 * 256);
    mPaletteTexture = std::make_unique<Texture>(*this, 256, 256, blackPixels.data(), Texture::Format::RGBA);

    createDescriptorSetLayout();

    createOffScreenPass();

    createGraphicsPipeline(PipelineIndex::eReverseBlending);
    createGraphicsPipeline(PipelineIndex::eAddBlending);
    createGraphicsPipeline(PipelineIndex::eFBOPipeline);

    createFramebuffers();

    createTextureSampler();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    updateDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
}

void VulkanRenderer::cleanupSwapChain()
{
    mSwapChainFramebuffers.clear();
    mSwapChainImageViews.clear();
    mSwapChain->clear();
}

void VulkanRenderer::cleanup()
{
    cleanupSwapChain();

    for (auto& pipeline : mGraphicsPipelines)
    {
        pipeline->clear();
    }

    for (auto& pipelineLayout : mPipelineLayouts)
    {
        pipelineLayout->clear();
    }

    mRenderPass->clear();

    mUniformBuffers.clear();
    mUniformBuffersMemory.clear();

    mDescriptorPool->clear();

    mTextureSampler->clear();

    mTextureCache.Clear();

    mPaletteTexture.reset();
    mTexturesForThisFrame.clear();

    mDescriptorSetLayout->clear();

    mIndexBuffer->clear();
    mIndexBufferMemory->clear();

    mVertexBuffer->clear();
    mVertexBufferMemory->clear();

    mRenderFinishedSemaphores.clear();
    mImageAvailableSemaphores.clear();
    mInFlightFences.clear();

    mCommandPool->clear();
    mDevice->clear();

    if (enableValidationLayers)
    {
        mDebugMessenger->clear();
    }

    mSurface->clear();
    mInstance->clear();
}

void VulkanRenderer::recreateSwapChain()
{
    int width = 0, height = 0;
    SDL_Vulkan_GetDrawableSize(mWindow, &width, &height);
    while (width == 0 || height == 0)
    {
        SDL_Vulkan_GetDrawableSize(mWindow, &width, &height);

        // TODO: Seems like el hacko ??
        // glfwWaitEvents();
        SDL_Event event;
        SDL_WaitEvent(&event);
    }

    mDevice->waitIdle();

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createFramebuffers();
}

void VulkanRenderer::createInstance()
{
    TRACE_ENTRYEXIT;

    bool haveValidationLayers = true;
    if constexpr (enableValidationLayers)
    {
        if (!checkValidationLayerSupport())
        {
            LOG_ERROR("validation layers requested, but not available!");
            haveValidationLayers = false;
        }
    }

    vk::ApplicationInfo appInfo{};
    appInfo.pApplicationName = "RELIVE";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const std::vector<const char*> extensions = getRequiredExtensions();
    std::vector<const char*> validationLayers;
    if constexpr (enableValidationLayers)
    {
        if (haveValidationLayers)
        {
            for (auto& layer : kValidationLayers)
            {
                validationLayers.push_back(layer);
            }
        }
    }

    vk::InstanceCreateFlags flags = {};
#ifdef __APPLE__
    flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
#endif

    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
        flags,
        &appInfo,
        static_cast<uint32_t>(validationLayers.size()), validationLayers.data(), // enabled layers
        static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
    );


    LOG_INFO("Make context");
    mContext = std::make_unique<vk::raii::Context>(vkGetInstanceProcAddr);

    LOG_INFO("Make instance");
    mInstance = std::make_unique<vk::raii::Instance>(*mContext, createInfo);

    volkLoadInstance(**mInstance);
}


void VulkanRenderer::setupDebugMessenger()
{
    if (!enableValidationLayers)
    {
        return;
    }

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.setMessageSeverity(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
    createInfo.setMessageType(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);

    createInfo.pfnUserCallback = debugCallback;

    mDebugMessenger = std::make_unique<vk::raii::DebugUtilsMessengerEXT>(mInstance->createDebugUtilsMessengerEXT(createInfo));
}

void VulkanRenderer::createSurface()
{
    VkSurfaceKHR tmpSurace{};
    if (!SDL_Vulkan_CreateSurface(mWindow, **mInstance, &tmpSurace))
    {
        throw std::runtime_error("failed to create window surface!");
    }
    LOG_INFO("Make surface");
    mSurface = std::make_unique<vk::raii::SurfaceKHR>(*mInstance, tmpSurace);
}

void VulkanRenderer::pickPhysicalDevice()
{
    vk::raii::PhysicalDevices physicalDevices(*mInstance);
    LOG_INFO("Got %d physicalDevices", physicalDevices.size());

    if (physicalDevices.empty())
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    for (std::size_t i = 0; i < physicalDevices.size(); i++)
    {
        if (isDeviceSuitable(physicalDevices[i]))
        {
            mPhysicalDevice = std::make_unique<vk::raii::PhysicalDevice>(std::move(physicalDevices[i]));
            return;
        }
    }

    throw std::runtime_error("failed to find a suitable GPU!");
}

void VulkanRenderer::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(*mPhysicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    vk::DeviceCreateInfo createInfo;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(kDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = kDeviceExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
        createInfo.ppEnabledLayerNames = kValidationLayers.data();
    }

    mDevice = std::make_unique<vk::raii::Device>(mPhysicalDevice->createDevice(createInfo));

    mGraphicsQueue = std::make_unique<vk::raii::Queue>(mDevice->getQueue(indices.graphicsFamily.value(), 0));
    mPresentQueue = std::make_unique<vk::raii::Queue>(mDevice->getQueue(indices.presentFamily.value(), 0));
}

void VulkanRenderer::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*mPhysicalDevice);

    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface = **mSurface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    QueueFamilyIndices indices = findQueueFamilies(*mPhysicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    mSwapChain = std::make_unique<vk::raii::SwapchainKHR>(mDevice->createSwapchainKHR(createInfo));

    mSwapChainImages = mSwapChain->getImages(); // TODO: non raii ??

    mSwapChainImageFormat = surfaceFormat.format;
    mSwapChainExtent = extent;
}

void VulkanRenderer::createImageViews()
{
    mSwapChainImageViews.clear();
    for (size_t i = 0; i < mSwapChainImages.size(); i++)
    {
        mSwapChainImageViews.emplace_back(createImageView(mSwapChainImages[i], mSwapChainImageFormat));
    }
}

void VulkanRenderer::createRenderPass()
{
    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = mSwapChainImageFormat;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    vk::SubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    //dependency.srcAccessMask = 0; // TODO ??
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    mRenderPass = std::make_unique<vk::raii::RenderPass>(mDevice->createRenderPass(renderPassInfo));
}

void VulkanRenderer::createGraphicsPipeline(VulkanRenderer::PipelineIndex idx)
{
    // TODO: probably don't need to create all the shaders and other bits more than once
    auto fragShaderCode = std::vector<char>(std::begin(fragment_shader), std::end(fragment_shader));
    auto vertShaderCode = std::vector<char>(std::begin(vertex_shader), std::end(vertex_shader));

    vk::raii::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    vk::raii::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = *vertShaderModule;
    vertShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = *fragShaderModule;
    fragShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eCounterClockwise; 
    rasterizer.depthBiasEnable = VK_FALSE;

    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_TRUE;
    if (idx == PipelineIndex::eAddBlending)
    {
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;
        colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    }
    else
    {
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOne;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.colorBlendOp = vk::BlendOp::eReverseSubtract;
    }

    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eZero;
    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eOne;
    colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 1.0f;
    colorBlending.blendConstants[1] = 1.0f;
    colorBlending.blendConstants[2] = 1.0f;
    colorBlending.blendConstants[3] = 1.0f;

    if (idx == PipelineIndex::eFBOPipeline)
    {
        colorBlendAttachment.blendEnable = VK_FALSE;
    }

    const std::vector<vk::DynamicState> dynamicStates = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor};

    vk::PipelineDynamicStateCreateInfo dynamicState;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &**mDescriptorSetLayout;

    mPipelineLayouts.emplace_back(std::make_unique<vk::raii::PipelineLayout>(mDevice->createPipelineLayout(pipelineLayoutInfo)));

    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = **mPipelineLayouts.back();
    // The final FBO pass needs to use the render pass we create in here to do the present
    pipelineInfo.renderPass = idx == PipelineIndex::eFBOPipeline ? **mRenderPass : **mOffScreenPass.renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    vk::Optional<const vk::raii::PipelineCache> cache(nullptr);
    auto pipelines = mDevice->createGraphicsPipelines(cache, vk::ArrayProxy(pipelineInfo));

    mGraphicsPipelines.emplace_back(std::make_unique<vk::raii::Pipeline>(std::move(pipelines[0])));
}

void VulkanRenderer::createFramebuffers()
{
    for (size_t i = 0; i < mSwapChainImageViews.size(); i++)
    {
        vk::ImageView attachments[] = {*mSwapChainImageViews[i]};

        vk::FramebufferCreateInfo framebufferInfo;
        framebufferInfo.renderPass = **mRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = mSwapChainExtent.width;
        framebufferInfo.height = mSwapChainExtent.height;
        framebufferInfo.layers = 1;

        mSwapChainFramebuffers.emplace_back(mDevice->createFramebuffer(framebufferInfo));
    }
}

void VulkanRenderer::createOffScreenPass()
{
    mOffScreenPass.width = 640;
    mOffScreenPass.height = 240;

    // Color attachment
    auto [image, memory] = createImage(mOffScreenPass.width, mOffScreenPass.height, kFramebufferFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
    mOffScreenPass.color.image = std::move(image);
    mOffScreenPass.color.mem = std::move(memory);

    vk::ImageViewCreateInfo createInfo;
    createInfo.viewType = vk::ImageViewType::e2D;
    createInfo.format = kFramebufferFormat;
    createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.layerCount = 1;
    createInfo.image = **mOffScreenPass.color.image;
    mOffScreenPass.color.view = std::make_unique<vk::raii::ImageView>(mDevice->createImageView(createInfo));

    // Create sampler to sample from the attachment in the fragment shader
    vk::PhysicalDeviceProperties properties = mPhysicalDevice->getProperties();

    vk::SamplerCreateInfo samplerInfo;
    samplerInfo.magFilter = vk::Filter::eNearest;
    samplerInfo.minFilter = vk::Filter::eNearest;
    samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
    samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
    samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = vk::CompareOp::eAlways;
    samplerInfo.mipmapMode = vk::SamplerMipmapMode::eNearest;

    mOffScreenPass.sampler = std::make_unique<vk::raii::Sampler>(mDevice->createSampler(samplerInfo));

    // Create a separate render pass for the offscreen rendering as it may differ from the one used for scene rendering

    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = kFramebufferFormat;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal; // Only expect the shader to read this FB

    vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpassDescription;
    subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentRef;

    // Use subpass dependencies for layout transitions
    std::array<vk::SubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eFragmentShader;
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eShaderRead;
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eShaderRead;
    dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    // Create the actual renderpass
    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    mOffScreenPass.renderPass = std::make_unique<vk::raii::RenderPass>(mDevice->createRenderPass(renderPassInfo));

    vk::ImageView attachments[] = {**mOffScreenPass.color.view};

    vk::FramebufferCreateInfo fbufCreateInfo;
    fbufCreateInfo.renderPass = **mOffScreenPass.renderPass;
    fbufCreateInfo.attachmentCount = 1;
    fbufCreateInfo.pAttachments = attachments;
    fbufCreateInfo.width = mOffScreenPass.width;
    fbufCreateInfo.height = mOffScreenPass.height;
    fbufCreateInfo.layers = 1;

    mOffScreenPass.frameBuffer = std::make_unique<vk::raii::Framebuffer>(mDevice->createFramebuffer(fbufCreateInfo));

    // Fill a descriptor for later use in a descriptor set
    mOffScreenPass.descriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    mOffScreenPass.descriptor.imageView = **mOffScreenPass.color.view;
    mOffScreenPass.descriptor.sampler = **mOffScreenPass.sampler;
}

void VulkanRenderer::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(*mPhysicalDevice);

    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    mCommandPool = std::make_unique<vk::raii::CommandPool>(mDevice->createCommandPool(poolInfo));
}

void VulkanRenderer::createTextureSampler()
{
    vk::PhysicalDeviceProperties properties = mPhysicalDevice->getProperties();

    vk::SamplerCreateInfo samplerInfo;
    samplerInfo.magFilter = vk::Filter::eNearest;
    samplerInfo.minFilter = vk::Filter::eNearest;
    samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
    samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
    samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = vk::CompareOp::eAlways;
    samplerInfo.mipmapMode = vk::SamplerMipmapMode::eNearest;

    mTextureSampler = std::make_unique<vk::raii::Sampler>(mDevice->createSampler(samplerInfo));
}

vk::raii::ImageView VulkanRenderer::createImageView(vk::Image image, vk::Format format)
{
    vk::ImageViewCreateInfo viewInfo;
    viewInfo.image = image;
    viewInfo.viewType = vk::ImageViewType::e2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    return mDevice->createImageView(viewInfo);
}

std::pair<std::unique_ptr<vk::raii::Image>, std::unique_ptr<vk::raii::DeviceMemory>> VulkanRenderer::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties)
{
    vk::ImageCreateInfo imageInfo;
    imageInfo.imageType = vk::ImageType::e2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = vk::ImageLayout::eUndefined;
    imageInfo.usage = usage;
    imageInfo.samples = vk::SampleCountFlagBits::e1;
    imageInfo.sharingMode = vk::SharingMode::eExclusive;

    vk::raii::Image image = mDevice->createImage(imageInfo);

    vk::MemoryRequirements memRequirements = image.getMemoryRequirements();

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    vk::raii::DeviceMemory imageMemory = mDevice->allocateMemory(allocInfo);
    image.bindMemory(*imageMemory, 0);

    return std::make_pair(std::make_unique<vk::raii::Image>(std::move(image)), std::make_unique<vk::raii::DeviceMemory>(std::move(imageMemory)));
}

void VulkanRenderer::transitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
    vk::raii::CommandBuffer commandBuffer = beginSingleTimeCommands();

    vk::ImageMemoryBarrier barrier;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vk::PipelineStageFlags sourceStage;
    vk::PipelineStageFlags destinationStage;

    if ((oldLayout == vk::ImageLayout::eUndefined || oldLayout == vk::ImageLayout::eShaderReadOnlyOptimal) && newLayout == vk::ImageLayout::eTransferDstOptimal)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eTransfer;
    }
    else if ((oldLayout == vk::ImageLayout::eTransferDstOptimal) && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        sourceStage = vk::PipelineStageFlagBits::eTransfer;
        destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    commandBuffer.pipelineBarrier(sourceStage, destinationStage,
                                  vk::DependencyFlags{},
                                  nullptr,
                                  nullptr,
                                  vk::ArrayProxy(barrier));



    endSingleTimeCommands(commandBuffer);
}

void VulkanRenderer::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    vk::raii::CommandBuffer commandBuffer = beginSingleTimeCommands();

    vk::BufferImageCopy region;
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = vk::Offset3D{static_cast<int32_t>(x), static_cast < int32_t>(y), 0};
    region.imageExtent = vk::Extent3D {
        width,
        height,
        1};

    commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

    endSingleTimeCommands(commandBuffer);
}

void VulkanRenderer::createVertexBuffer()
{
    const vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // Create if we don't have a buffer or its too small
    if (!mVertexBuffer || !mVertexBufferMemory || mVertexBufferSize < bufferSize)
    {
        mVertexBufferSize = bufferSize;

        // The buffer is host and device visible and we keep it mapped for fast updating
        auto [tmpBuffer, tmpMemory] = createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eDeviceLocal);
        mVertexBuffer = std::move(tmpBuffer);
        mVertexBufferMemory = std::move(tmpMemory);

        mMappedVertexBuffferMemory = mVertexBufferMemory->mapMemory(0, bufferSize);
    }

    memcpy(mMappedVertexBuffferMemory, vertices.data(), (size_t) bufferSize);

    vk::MappedMemoryRange mappedRange;
    mappedRange.memory = **mVertexBufferMemory;
    mappedRange.size = VK_WHOLE_SIZE; // TODO: Limit size to corrrect multiple
    mDevice->flushMappedMemoryRanges(mappedRange);
}

void VulkanRenderer::createIndexBuffer()
{
    const vk::DeviceSize bufferSize = sizeof(gIndices[0]) * gIndices.size();
    if (!mIndexBuffer || !mIndexBufferMemory || mIndexBufferSize < bufferSize)
    {
        mIndexBufferSize = bufferSize;

        auto [stagingBuffer2, stagingBufferMemory2] = createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eDeviceLocal);
        mIndexBuffer = std::move(stagingBuffer2);
        mIndexBufferMemory = std::move(stagingBufferMemory2);

        mMappedIndexBuffferMemory  = mIndexBufferMemory->mapMemory(0, bufferSize);
    }

    memcpy(mMappedIndexBuffferMemory, gIndices.data(), (size_t) bufferSize);

    vk::MappedMemoryRange mappedRange;
    mappedRange.memory = **mIndexBufferMemory;
    mappedRange.size = VK_WHOLE_SIZE; // TODO: Limit size to corrrect multiple
    mDevice->flushMappedMemoryRanges(mappedRange);
}

void VulkanRenderer::createUniformBuffers()
{
    vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

    mUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    mUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    mUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        auto [buffer, memory] = createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        mUniformBuffers[i] = std::move(buffer);
        mUniformBuffersMemory[i] = std::move(memory);
        mUniformBuffersMapped[i] = mUniformBuffersMemory[i]->mapMemory(0, bufferSize);
    }
}


void VulkanRenderer::createDescriptorSetLayout()
{
    vk::DescriptorSetLayoutBinding uboLayoutBinding;
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vk::DescriptorSetLayoutBinding texPalette;
    texPalette.binding = 1;
    texPalette.descriptorCount = 1;
    texPalette.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    texPalette.pImmutableSamplers = nullptr;
    texPalette.stageFlags = vk::ShaderStageFlagBits::eFragment;

    vk::DescriptorSetLayoutBinding texCamera;
    texCamera.binding = 2;
    texCamera.descriptorCount = 1;
    texCamera.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    texCamera.pImmutableSamplers = nullptr;
    texCamera.stageFlags = vk::ShaderStageFlagBits::eFragment;

    vk::DescriptorSetLayoutBinding texSpriteSheets;
    texSpriteSheets.binding = 3;
    texSpriteSheets.descriptorCount = 14; // texture array size
    texSpriteSheets.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    texSpriteSheets.pImmutableSamplers = nullptr;
    texSpriteSheets.stageFlags = vk::ShaderStageFlagBits::eFragment;

    vk::DescriptorSetLayoutBinding bindings[] = {uboLayoutBinding, texPalette, texCamera, texSpriteSheets};
    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = ALIVE_COUNTOF(bindings);
    layoutInfo.pBindings = bindings;

    mDescriptorSetLayout = std::make_unique<vk::raii::DescriptorSetLayout>(mDevice->createDescriptorSetLayout(layoutInfo));
}
constexpr u32 kMaxBatches = 200;

void VulkanRenderer::createDescriptorPool()
{
    vk::DescriptorPoolSize poolSizes[] = 
    {
        // ubo
        {vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches},
        // pal
        {vk::DescriptorType::eCombinedImageSampler, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches},
        // cam
        {vk::DescriptorType::eCombinedImageSampler, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches},
        // sprites
        {vk::DescriptorType::eCombinedImageSampler, 14 * static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches},
    };

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.poolSizeCount = ALIVE_COUNTOF(poolSizes);
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches;

    mDescriptorPool = std::make_unique<vk::raii::DescriptorPool>(mDevice->createDescriptorPool(poolInfo));
}

void VulkanRenderer::createDescriptorSets()
{
    std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT * kMaxBatches, **mDescriptorSetLayout);
    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = **mDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches;
    allocInfo.pSetLayouts = layouts.data();

    mDescriptorSets = mDevice->allocateDescriptorSets(allocInfo);
}

static u32 To1dIdx(u32 row, u32 col)
{
    return (row * MAX_FRAMES_IN_FLIGHT) + col;
}

void VulkanRenderer::updateDescriptorSets()
{
    const u32 batchCount = static_cast<u32>(std::max(1u, static_cast<u32>(mBatches.size())));

    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        u32 textureIdxOff = 0;
        for (u32 j = 0; j < batchCount; j++)
        {
            std::array<vk::WriteDescriptorSet, 4> descriptorWrites{};

            // ubo
            vk::DescriptorBufferInfo bufferInfo;
            bufferInfo.buffer = **mUniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            descriptorWrites[0].dstSet = *mDescriptorSets[To1dIdx(i, j)];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            // Pal texture
            vk::DescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            imageInfo.imageView = **mPaletteTexture->View();
            imageInfo.sampler = **mTextureSampler;

            descriptorWrites[1].dstSet = *mDescriptorSets[To1dIdx(i, j)];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            // Camera texture or FBO
            vk::DescriptorImageInfo imageInfo4{};
            descriptorWrites[2].dstSet = *mDescriptorSets[To1dIdx(i, j)];
            descriptorWrites[2].dstBinding = 2;
            descriptorWrites[2].dstArrayElement = 0;
            descriptorWrites[2].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[2].descriptorCount = 1;
            if (j < mBatches.size() && mBatches[j].mPipeline == PipelineIndex::eFBOPipeline)
            {
                descriptorWrites[2].pImageInfo = &mOffScreenPass.descriptor;
            }
            else
            {
                imageInfo4.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
                if (mCamTexture)
                {
                    imageInfo4.imageView = **mCamTexture->View();
                }
                else
                {
                    imageInfo4.imageView = **mPaletteTexture->View();
                }
                imageInfo4.sampler = **mTextureSampler;
                descriptorWrites[2].pImageInfo = &imageInfo4;
            }

            // Sprite sheets
            vk::DescriptorImageInfo imageInfo2[14];
            for (u32 k = 0; k < 14; k++)
            {
                imageInfo2[k].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
                if (!mBatches.empty() && k < mBatches[j].mTexturesInBatch)
                {
                    if (textureIdxOff + k < mTexturesForThisFrame.size())
                    {
                        imageInfo2[k].imageView = **mTexturesForThisFrame[textureIdxOff + k]->View();
                    }
                }

                if (!imageInfo2[k].imageView)
                {
                    imageInfo2[k].imageView = **mPaletteTexture->View();
                }

                imageInfo2[k].sampler = **mTextureSampler;
            }

            descriptorWrites[3].dstSet = *mDescriptorSets[To1dIdx(i, j)];
            descriptorWrites[3].dstBinding = 3;
            descriptorWrites[3].dstArrayElement = 0;
            descriptorWrites[3].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[3].descriptorCount = 14; // texture array size
            descriptorWrites[3].pImageInfo = imageInfo2;

            mDevice->updateDescriptorSets(descriptorWrites, {});

            if (j < mBatches.size())
            {
                textureIdxOff += static_cast<u32>(mBatches[j].mTexturesInBatch);
            }
        }
    }
}

std::pair<std::unique_ptr<vk::raii::Buffer>, std::unique_ptr<vk::raii::DeviceMemory>> VulkanRenderer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties)
{
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    vk::raii::Buffer buffer = mDevice->createBuffer(bufferInfo);

    vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();


    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    vk::raii::DeviceMemory bufferMemory = mDevice->allocateMemory(allocInfo);

    buffer.bindMemory(*bufferMemory, 0);

    return std::make_pair(std::make_unique<vk::raii::Buffer>(std::move(buffer)), std::make_unique<vk::raii::DeviceMemory>(std::move(bufferMemory)));
}

vk::raii::CommandBuffer VulkanRenderer::beginSingleTimeCommands()
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = **mCommandPool;
    allocInfo.commandBufferCount = 1;

    std::vector<vk::raii::CommandBuffer> commandBuffer = mDevice->allocateCommandBuffers(allocInfo);

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    commandBuffer[0].begin(beginInfo);

    return std::move(commandBuffer[0]);
}

void VulkanRenderer::endSingleTimeCommands(vk::raii::CommandBuffer& commandBuffer)
{
    commandBuffer.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &*commandBuffer;

    mGraphicsQueue->submit(submitInfo);
    mGraphicsQueue->waitIdle();
}

void VulkanRenderer::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
{
    vk::raii::CommandBuffer commandBuffer = beginSingleTimeCommands();

    vk::BufferCopy copyRegion;
    copyRegion.size = size;
    commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);

    endSingleTimeCommands(commandBuffer);
}

uint32_t VulkanRenderer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProperties = mPhysicalDevice->getMemoryProperties();
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanRenderer::createCommandBuffers()
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = **mCommandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    auto tmp = mDevice->allocateCommandBuffers(allocInfo);
    for (std::size_t i = 0; i < tmp.size(); i++)
    {
        mCommandBuffers.emplace_back(std::make_unique<vk::raii::CommandBuffer>(std::move(tmp[i])));
    }
}

void VulkanRenderer::recordCommandBuffer(vk::raii::CommandBuffer& commandBuffer, uint32_t imageIndex)
{
    updateDescriptorSets();

    vk::CommandBufferBeginInfo beginInfo;
    commandBuffer.begin(beginInfo);

    // First render pass : Offscreen rendering
    vk::RenderPassBeginInfo offScreenPassInfo;
    offScreenPassInfo.renderPass = **mOffScreenPass.renderPass;
    offScreenPassInfo.framebuffer = **mOffScreenPass.frameBuffer;
    offScreenPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    offScreenPassInfo.renderArea.extent.width = mOffScreenPass.width;
    offScreenPassInfo.renderArea.extent.height = mOffScreenPass.height;


    vk::ClearValue clearColor(vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f));
    offScreenPassInfo.clearValueCount = 1;
    offScreenPassInfo.pClearValues = &clearColor;

    commandBuffer.beginRenderPass(offScreenPassInfo, vk::SubpassContents::eInline);

    u32 batchIdx = 0;
    u32 idxOffset = 0;
    PipelineIndex lastPipeLine = PipelineIndex::eNone;

    for (std::size_t i = 0; i < mBatches.size(); i++)
    {
        if (mBatches[i].mPipeline != PipelineIndex::eFBOPipeline)
        {
            if (lastPipeLine != mBatches[i].mPipeline)
            {
                commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, **mGraphicsPipelines[mBatches[i].mPipeline]);

                vk::Viewport viewport;
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = (float) mOffScreenPass.width;
                viewport.height = (float) mOffScreenPass.height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;
                commandBuffer.setViewport(0, viewport);

                vk::Rect2D scissor{0, 0};
                scissor.extent.width = mOffScreenPass.width;
                scissor.extent.height = mOffScreenPass.height;
                commandBuffer.setScissor(0, scissor);


                lastPipeLine = mBatches[i].mPipeline;

                vk::Buffer vertexBuffers[] = {**mVertexBuffer};
                vk::DeviceSize offsets[] = {0};
                commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

                commandBuffer.bindIndexBuffer(**mIndexBuffer, 0, vk::IndexType::eUint16);
            }

            if (!vertices.empty() && mVertexBuffer && mIndexBuffer)
            {
                commandBuffer.bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics,
                    **mPipelineLayouts[mBatches[i].mPipeline],
                    0,
                    *mDescriptorSets[To1dIdx(mCurrentFrame, batchIdx)],
                    {});

                commandBuffer.drawIndexed(
                    (mBatches[i].mNumTrisToDraw) * 3,
                    1,
                    idxOffset,
                    0,
                    0);
            }
        }
        idxOffset += (mBatches[i].mNumTrisToDraw) * 3;
        batchIdx++;
    }

    commandBuffer.endRenderPass();

    // Note: Explicit synchronization is not required between the render pass, as this is done implicit via sub pass dependencies
    // Copy frame buffer to swap chain pass
    vk::RenderPassBeginInfo renderPassInfo;
    renderPassInfo.renderPass = **mRenderPass;
    renderPassInfo.framebuffer = *mSwapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    renderPassInfo.renderArea.extent = mSwapChainExtent;
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    batchIdx = 0;
    idxOffset = 0;
    lastPipeLine = PipelineIndex::eNone;
    for (std::size_t i = 0; i < mBatches.size(); i++)
    {
        if (mBatches[i].mPipeline == PipelineIndex::eFBOPipeline)
        {
            if (lastPipeLine != mBatches[i].mPipeline)
            {
                commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, **mGraphicsPipelines[mBatches[i].mPipeline]);
                lastPipeLine = mBatches[i].mPipeline;

                vk::Viewport viewport2;
                viewport2.x = 0.0f;
                viewport2.y = 0.0f;
                viewport2.width = (float) mSwapChainExtent.width;
                viewport2.height = (float) mSwapChainExtent.height;
                viewport2.minDepth = 0.0f;
                viewport2.maxDepth = 1.0f;
                commandBuffer.setViewport(0, viewport2);

                vk::Rect2D scissor2{0, 0};
                scissor2.extent = mSwapChainExtent;
                commandBuffer.setScissor(0, scissor2);

                vk::Buffer vertexBuffers[] = {**mVertexBuffer};
                vk::DeviceSize offsets[] = {0};
                commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

                commandBuffer.bindIndexBuffer(**mIndexBuffer, 0, vk::IndexType::eUint16);
            }

            if (!vertices.empty() && mVertexBuffer && mIndexBuffer)
            {
                commandBuffer.bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics,
                    **mPipelineLayouts[mBatches[i].mPipeline],
                    0,
                    *mDescriptorSets[To1dIdx(mCurrentFrame, batchIdx)],
                    {});

                commandBuffer.drawIndexed(
                    (mBatches[i].mNumTrisToDraw) * 3,
                    1,
                    idxOffset,
                    0,
                    0);
            }
        }
        idxOffset += (mBatches[i].mNumTrisToDraw) * 3;
        batchIdx++;
    }

    commandBuffer.endRenderPass();

    commandBuffer.end();
}

void VulkanRenderer::createSyncObjects()
{
    vk::SemaphoreCreateInfo semaphoreInfo;

    vk::FenceCreateInfo fenceInfo;
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        mImageAvailableSemaphores.emplace_back(mDevice->createSemaphore(semaphoreInfo));
        mRenderFinishedSemaphores.emplace_back(mDevice->createSemaphore(semaphoreInfo));
        mInFlightFences.emplace_back(mDevice->createFence(fenceInfo));
    }
}

void VulkanRenderer::updateUniformBuffer(uint32_t currentImage)
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::ortho(0.0F, 640.0f, 0.0f, 240.0f); // TODO double height

    memcpy(mUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void VulkanRenderer::drawFrame()
{
    auto x = mDevice->waitForFences(*mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);
    if (x != vk::Result::eSuccess)
    {

    }

    auto [result, imageIndex] = mSwapChain->acquireNextImage(UINT64_MAX, *mImageAvailableSemaphores[mCurrentFrame]);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        recreateSwapChain();
        return;
    }
    else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(mCurrentFrame);

    mDevice->resetFences(*mInFlightFences[mCurrentFrame]);

    mCommandBuffers[mCurrentFrame]->reset();
    recordCommandBuffer(*mCommandBuffers[mCurrentFrame], imageIndex);

    vk::SubmitInfo submitInfo;

    vk::Semaphore waitSemaphores[] = {*mImageAvailableSemaphores[mCurrentFrame]};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &**mCommandBuffers[mCurrentFrame];

    vk::Semaphore signalSemaphores[] = {*mRenderFinishedSemaphores[mCurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    mGraphicsQueue->submit(submitInfo, *mInFlightFences[mCurrentFrame]);
    mGraphicsQueue->waitIdle(); // TODO: crashes uploading the index buffer without this wait, not sure if this is just masking a bug ??

    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = {**mSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = mPresentQueue->presentKHR(presentInfo);
    mPresentQueue->waitIdle(); // todo
    mDevice->waitIdle(); // todo

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
    {
        recreateSwapChain();
    }
    else if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

vk::raii::ShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
    vk::ShaderModuleCreateInfo createInfo;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    vk::raii::ShaderModule shaderModule(*mDevice, createInfo);
    return shaderModule;
}

vk::SurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }

    // eFifo
    return vk::PresentModeKHR::eImmediate; // use eImmediate to disable vsync, not all modes are supported on all devices
}

VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        SDL_Vulkan_GetDrawableSize(mWindow, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

VulkanRenderer::SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(vk::raii::PhysicalDevice& vkDevice)
{
    SwapChainSupportDetails details;
    details.capabilities = vkDevice.getSurfaceCapabilitiesKHR(**mSurface);
    details.formats = vkDevice.getSurfaceFormatsKHR(**mSurface);
    details.presentModes = vkDevice.getSurfacePresentModesKHR(**mSurface);
    return details;
}

bool VulkanRenderer::isDeviceSuitable(vk::raii::PhysicalDevice& vkDevice)
{
    QueueFamilyIndices indices = findQueueFamilies(vkDevice);

    bool extensionsSupported = checkDeviceExtensionSupport(vkDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vkDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    vk::PhysicalDeviceFeatures supportedFeatures = vkDevice.getFeatures();

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool VulkanRenderer::checkDeviceExtensionSupport(vk::raii::PhysicalDevice& vkDevice)
{
    std::vector<vk::ExtensionProperties> availableExtensions = vkDevice.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(kDeviceExtensions.begin(), kDeviceExtensions.end());
    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VulkanRenderer::QueueFamilyIndices VulkanRenderer::findQueueFamilies(vk::raii::PhysicalDevice& vkDevice)
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = vkDevice.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = vkDevice.getSurfaceSupportKHR(i, **mSurface); // TODO: leak ?
        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

std::vector<const char*> VulkanRenderer::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    if (!SDL_Vulkan_GetInstanceExtensions(mWindow, &glfwExtensionCount, nullptr))
    {
        throw RendererException(SDL_GetError());
    }

    std::vector<const char*> extensions;
    extensions.resize(glfwExtensionCount);
    if (!SDL_Vulkan_GetInstanceExtensions(mWindow, &glfwExtensionCount, extensions.data()))
    {
        throw RendererException(SDL_GetError());
    }

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

bool VulkanRenderer::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto& layer : availableLayers)
    {
        LOG_INFO("Layer: %s", layer.layerName);
    }

    for (const char* layerName : kValidationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

/*static*/ VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT /*messageSeverity*/, VkDebugUtilsMessageTypeFlagsEXT /*messageType*/, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* /*pUserData*/)
{
    LOG_ERROR("%s", pCallbackData->pMessage);

    return VK_FALSE;
}


VulkanRenderer::VulkanRenderer(TWindowHandleType window)
    : IRenderer(window)
    , mPaletteCache(256)
{
    TRACE_ENTRYEXIT;

#ifdef __APPLE__
    mMoltenVkLib = std::make_unique<ScopedDynamicLib>(VulkanLib::GetDylibPath().c_str());
    auto fnTmp = mMoltenVkLib->GetFn<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    if (!fnTmp)
    {
        throw RendererException("vkGetInstanceProcAddr not found in dylib");
    }
    volkInitializeCustom(fnTmp);
#else
    const VkResult result = volkInitialize();
    if (result != VK_SUCCESS)
    {
        throw RendererException("volkInitialize failed");
    }
#endif
    initVulkan();
}

VulkanRenderer::~VulkanRenderer()
{
    TRACE_ENTRYEXIT;

    // TODO: Fix me, dtor can't call clear else pure call boom
    //mTextureCache.Clear();
}

void VulkanRenderer::Clear(u8 /*r*/, u8 /*g*/, u8 /*b*/)
{

}

void VulkanRenderer::StartFrame()
{
    IRenderer::StartFrame();

    if (!mFrameStarted)
    {
        mFrameStarted = true;

        // Set offsets for the screen (this is for the screen shake effect)
        mOffsetX = 0;
        mOffsetY = 0;

    }
}

void VulkanRenderer::EndFrame()
{
    if (mFrameStarted)
    {
        // Add the in flight batch
        if (mBatchInProgress && mConstructingBatch.mNumTrisToDraw > 0)
        {
            mBatches.push_back(mConstructingBatch);
            mBatchInProgress = false;
        }

        // TODO: HACK, refactor this so the full screen tris are done in their own func without copy pasta
        {
            {
                float u0 = 0.0f;
                float v0 = 0.0f;

                float u1 = 1.0f;
                float v1 = 1.0f;

                mTextureArrayIdx = 0;

                vertices.push_back({{static_cast<f32>(0), static_cast<f32>(0)}, {255, 255, 255}, {u0, v0}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});
                vertices.push_back({{static_cast<f32>(640), static_cast<f32>(0)}, {255, 255, 255}, {u1, v0}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});
                vertices.push_back({{static_cast<f32>(0), static_cast<f32>(240)}, {255, 255, 255}, {u0, v1}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});
                vertices.push_back({{static_cast<f32>(640), static_cast<f32>(240)}, {255, 255, 255}, {u1, v1}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});

                gIndices.emplace_back((u16) (mIndexBufferIndex + 1));
                gIndices.emplace_back((u16) (mIndexBufferIndex + 0));
                gIndices.emplace_back((u16) (mIndexBufferIndex + 3));

                gIndices.emplace_back((u16) (mIndexBufferIndex + 3));
                gIndices.emplace_back((u16) (mIndexBufferIndex + 0));
                gIndices.emplace_back((u16) (mIndexBufferIndex + 2));

                mIndexBufferIndex += 4;

                mConstructingBatch.mPipeline = PipelineIndex::eFBOPipeline;
                mConstructingBatch.mTexturesInBatch = 1;
                mConstructingBatch.mNumTrisToDraw = 2;
                
                NewBatch();
            }
        }

        if (!vertices.empty() && !gIndices.empty())
        {
            this->createVertexBuffer();
            this->createIndexBuffer();
        }

        this->drawFrame();



        mFrameStarted = false;

        vertices.clear();
        gIndices.clear();
        mIndexBufferIndex = 0;

        //this->mVertexBuffer.reset();
        //this->mVertexBufferMemory.reset();
        //this->mIndexBuffer.reset();
        //this->mIndexBufferMemory.reset();

        mTexturesForThisFrame.clear();

        mCamTexture = nullptr;
        mTextureArrayIdx = 0;
        mConstructingBatch = {};
        mBatches.clear();
    }

    // Always decrease resource lifetimes regardless of drawing to prevent
    // memory leaks
    DecreaseResourceLifetimes();
}

void VulkanRenderer::SetTPage(u16 tPage)
{
    mGlobalTPage = tPage;
}

void VulkanRenderer::SetClip(Prim_PrimClipper&)
{
   
}

void VulkanRenderer::ToggleFilterScreen()
{
    // TODO
}

void VulkanRenderer::Draw(Prim_GasEffect& /*gasEffect*/)
{
    // TODO
}

void VulkanRenderer::Draw(Line_G2& /*line*/)
{
    // TODO
}

void VulkanRenderer::Draw(Line_G4& /*line*/)
{
    // TODO
}


void VulkanRenderer::Draw(Poly_G3&)
{
  
}

/*
void VulkanRenderer::SetupBlendMode(u16 blendMode)
{
    if ((TPageAbr) blendMode == TPageAbr::eBlend_2)
    {
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT));
    }
    else
    {
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD));
    }
}*/

u32 VulkanRenderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache.Add(pCache);

    if (addRet.mAllocated)
    {
        // Write palette data
        mPaletteTexture->LoadSubImage(0, addRet.mIndex, 256, 1, pCache.mPal);

        //mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

// TODO: Copy pasted from GL renderer
inline u16 GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

void VulkanRenderer::NewBatch()
{
    mBatches.push_back(mConstructingBatch);
    mConstructingBatch = {};
    mTextureArrayIdx = 0;
    mBatchInProgress = false;
}

void VulkanRenderer::Draw(Poly_FT4& poly)
{
    constexpr u32 kTextureBatchSize = 12;

    if (poly.mCam && !poly.mFg1)
    {
        CamResource* pRes = poly.mCam;
        std::shared_ptr<Texture> texture = mTextureCache.GetCachedTexture(poly.mCam->mUniqueId.Id(), 1); // TODO: temp, kill texture ASAP
        if (!texture)
        {
            auto newTex = std::make_unique<Texture>(*this, pRes->mData.mWidth, pRes->mData.mHeight, pRes->mData.mPixels->data(), Texture::Format::RGBA);

            texture = mTextureCache.Add(poly.mCam->mUniqueId.Id(), 300, std::move(newTex));

        }
        mCamTexture = texture;

        // TODO: temp haxo
        {
            float u0 = 0.0f;
            float v0 = 0.0f;

            float u1 = 1.0f;
            float v1 = 1.0f;

            vertices.push_back({{static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y)}, {255, 255, 255}, {u0, v0}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});
            vertices.push_back({{static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y)}, {255, 255, 255}, {u1, v0}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});
            vertices.push_back({{static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y)}, {255, 255, 255}, {u0, v1}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});
            vertices.push_back({{static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y)}, {255, 255, 255}, {u1, v1}, mTextureArrayIdx, 0, PsxDrawMode::Camera, 0, 0, 0});

            gIndices.emplace_back((u16) (mIndexBufferIndex + 1));
            gIndices.emplace_back((u16) (mIndexBufferIndex + 0));
            gIndices.emplace_back((u16) (mIndexBufferIndex + 3));

            gIndices.emplace_back((u16) (mIndexBufferIndex + 3));
            gIndices.emplace_back((u16) (mIndexBufferIndex + 0));
            gIndices.emplace_back((u16) (mIndexBufferIndex + 2));

            mIndexBufferIndex += 4;

            // TODO: We are duplicating textures here
            //mTexturesForThisFrame.emplace_back(texture);

           // mTextureArrayIdx++;
            mConstructingBatch.mTexturesInBatch = mTextureArrayIdx;
            mConstructingBatch.mNumTrisToDraw += 2;

            // Over the texture limit or changed to/from subtractive blending
            const bool bNewBatch = (mTextureArrayIdx == kTextureBatchSize);
            if (bNewBatch)
            {
                NewBatch();
            }
            else
            {
                mConstructingBatch.mPipeline = PipelineIndex::eAddBlending;
                mBatchInProgress = true;
            }
        }
    }
    else if (poly.mAnim)
    {
        AnimResource& animRes = poly.mAnim->mAnimRes;

        const u32 palIndex = PreparePalette(*animRes.mCurPal);

        auto pTga = animRes.mTgaPtr;
        std::shared_ptr<Texture> texture = mTextureCache.GetCachedTexture(animRes.mUniqueId.Id(), 1); // TODO: temp, kill texture ASAP
        if (!texture)
        {
            auto newTex = std::make_unique<Texture>(*this, pTga->mWidth, pTga->mHeight, pTga->mPixels.data(), Texture::Format::Indexed);

            texture = mTextureCache.Add(animRes.mUniqueId.Id(), 300, std::move(newTex));

            //mStats.mCamUploadCount++;
        }

        u8 r = poly.mBase.header.rgb_code.r;
        u8 g = poly.mBase.header.rgb_code.g;
        u8 b = poly.mBase.header.rgb_code.b;

        u32 isSemiTrans = GetPolyIsSemiTrans(&poly) ? 1 : 0;
        u32 isShaded = GetPolyIsShaded(&poly) ? 1 : 0;
        u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

        PipelineIndex pipeLine = blendMode == 2 ? PipelineIndex::eReverseBlending : PipelineIndex::eAddBlending;
        if (mConstructingBatch.mPipeline != pipeLine && mConstructingBatch.mPipeline != PipelineIndex::eNone)
        {
            NewBatch();
        }

        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);

        float u0 = (static_cast<float>(pHeader->mSpriteSheetX) / pTga->mWidth);
        float v0 = (static_cast<float>(pHeader->mSpriteSheetY) / pTga->mHeight);

        float u1 = u0 + ((float) pHeader->mWidth / (float) pTga->mWidth);
        float v1 = v0 + ((float) pHeader->mHeight / (float) pTga->mHeight);

        if (poly.mFlipX)
        {
            std::swap(u0, u1);
        }

        if (poly.mFlipY)
        {
            std::swap(v1, v0);
        }

       
        vertices.push_back({{static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y)}, {r, g, b}, {u0, v0}, mTextureArrayIdx, palIndex, PsxDrawMode::DefaultFT4, isShaded, blendMode, isSemiTrans});
        vertices.push_back({{static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y)}, {r, g, b}, {u1, v0}, mTextureArrayIdx, palIndex, PsxDrawMode::DefaultFT4, isShaded, blendMode, isSemiTrans});
        vertices.push_back({{static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y)}, {r, g, b}, {u0, v1}, mTextureArrayIdx, palIndex, PsxDrawMode::DefaultFT4, isShaded, blendMode, isSemiTrans});
        vertices.push_back({{static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y)}, {r, g, b}, {u1, v1}, mTextureArrayIdx, palIndex, PsxDrawMode::DefaultFT4, isShaded, blendMode, isSemiTrans});
    
        gIndices.emplace_back((u16) (mIndexBufferIndex + 1));
        gIndices.emplace_back((u16) (mIndexBufferIndex + 0));
        gIndices.emplace_back((u16) (mIndexBufferIndex + 3));

        gIndices.emplace_back((u16) (mIndexBufferIndex + 3));
        gIndices.emplace_back((u16) (mIndexBufferIndex + 0));
        gIndices.emplace_back((u16) (mIndexBufferIndex + 2));

        mIndexBufferIndex += 4;

        // TODO: We are duplicating textures here
        mTexturesForThisFrame.emplace_back(texture);

        mTextureArrayIdx++;
        mConstructingBatch.mTexturesInBatch = mTextureArrayIdx;
        mConstructingBatch.mNumTrisToDraw += 2;

        // Over the texture limit or changed to/from subtractive blending
        const bool bNewBatch = (mTextureArrayIdx == kTextureBatchSize);
        if (bNewBatch)
        {
            NewBatch();
        }
        else
        {
            mConstructingBatch.mPipeline = pipeLine;
            mBatchInProgress = true;
        }


    }
}

void VulkanRenderer::Draw(Poly_G4&)
{

}


void VulkanRenderer::DecreaseResourceLifetimes()
{
    mTextureCache.DecreaseResourceLifetimes();

    mPaletteCache.ResetUseFlags();
}

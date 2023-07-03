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


#include "fragment_shader.h"
#include "vertex_shader.h"

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

struct UniformBufferObject final
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


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
    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        mPaletteTexture[mCurrentFrame] = std::make_shared<Texture>(*this, 256, 256, blackPixels.data(), Texture::Format::RGBA);
    }
    createDescriptorSetLayout();

    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        createOffScreenPass(i);
    }

    createGraphicsPipeline(PipelineIndex::eReverseBlending);
    createGraphicsPipeline(PipelineIndex::eAddBlending);
    createGraphicsPipeline(PipelineIndex::eFBOPipeline);

    createFramebuffers();

    createTextureSampler();
    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        createUniformBuffers(i);
    }
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

  
    mDescriptorPool->clear();



    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        mIndexBuffer[i].clear();
        mVertexBuffer[i].clear();
        mUniformBuffer[i].clear();
        mTextureSampler[i]->clear();
        mTextureCache[i].Clear();
        mPaletteTexture[mCurrentFrame].reset();
    }

    mBatcher[mCurrentFrame].mBatchTextures.clear();

    mDescriptorSetLayout->clear();


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
            const auto deviceName = physicalDevices[i].getProperties().deviceName.data();
            LOG_INFO("using suitable vulkan GPU: %s", deviceName);
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
    rasterizer.cullMode = vk::CullModeFlagBits::eNone; // Culling off like GL and DX
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
    pipelineInfo.renderPass = idx == PipelineIndex::eFBOPipeline ? **mRenderPass : **mOffScreenPass[0].renderPass; // TODO: Double buffer 
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

void VulkanRenderer::createOffScreenPass(u32 idx)
{
    mOffScreenPass[idx].width = 640;
    mOffScreenPass[idx].height = 240;

    // Color attachment
    auto [image, memory] = createImage(mOffScreenPass[idx].width, mOffScreenPass[idx].height, kFramebufferFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
    mOffScreenPass[idx].color.image = std::move(image);
    mOffScreenPass[idx].color.mem = std::move(memory);

    vk::ImageViewCreateInfo createInfo;
    createInfo.viewType = vk::ImageViewType::e2D;
    createInfo.format = kFramebufferFormat;
    createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.layerCount = 1;
    createInfo.image = **mOffScreenPass[idx].color.image;
    mOffScreenPass[idx].color.view = std::make_unique<vk::raii::ImageView>(mDevice->createImageView(createInfo));

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

    mOffScreenPass[idx].sampler = std::make_unique<vk::raii::Sampler>(mDevice->createSampler(samplerInfo));

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

    mOffScreenPass[idx].renderPass = std::make_unique<vk::raii::RenderPass>(mDevice->createRenderPass(renderPassInfo));

    vk::ImageView attachments[] = {**mOffScreenPass[idx].color.view};

    vk::FramebufferCreateInfo fbufCreateInfo;
    fbufCreateInfo.renderPass = **mOffScreenPass[idx].renderPass;
    fbufCreateInfo.attachmentCount = 1;
    fbufCreateInfo.pAttachments = attachments;
    fbufCreateInfo.width = mOffScreenPass[idx].width;
    fbufCreateInfo.height = mOffScreenPass[idx].height;
    fbufCreateInfo.layers = 1;

    mOffScreenPass[idx].frameBuffer = std::make_unique<vk::raii::Framebuffer>(mDevice->createFramebuffer(fbufCreateInfo));

    // Fill a descriptor for later use in a descriptor set
    mOffScreenPass[idx].descriptor.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    mOffScreenPass[idx].descriptor.imageView = **mOffScreenPass[idx].color.view;
    mOffScreenPass[idx].descriptor.sampler = **mOffScreenPass[idx].sampler;
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

    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        mTextureSampler[i] = std::make_unique<vk::raii::Sampler>(mDevice->createSampler(samplerInfo));
    }
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

void VulkanRenderer::transitionImageLayout(vk::raii::CommandBuffer& commandBuffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
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
}

void VulkanRenderer::copyBufferToImage(vk::raii::CommandBuffer& commandBuffer, vk::Buffer buffer, vk::Image image, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
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
}

void VulkanRenderer::createVertexBuffer(u32 idx)
{
    auto usageFlags = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
    auto memoryFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eDeviceLocal;

    std::vector<Vertex> vertices;
    vertices.resize(mBatcher[idx].mVertices.size());
    for (std::size_t i = 0; i < vertices.size(); i++)
    {
        const auto& src = mBatcher[idx].mVertices[i];
        auto& dst = vertices[i];

        dst.pos.x = src.x;
        dst.pos.y = src.y;
        dst.color[0] = static_cast<u8>(src.r);
        dst.color[1] = static_cast<u8>(src.g);
        dst.color[2] = static_cast<u8>(src.b);
        dst.texCoord.x = src.u;
        dst.texCoord.y = src.v;
        dst.mSamplerIdx = src.textureUnitIndex;
        dst.mPalIdx = src.paletteIndex;
        dst.mDrawType = src.drawMode;
        dst.mIsShaded = src.isShaded;
        dst.mBlendMode = src.blendMode;
        dst.mIsSemiTrans = src.isSemiTrans;
    }

    mVertexBuffer[idx].makeBuffer(*this, mDevice, usageFlags, memoryFlags, vertices.data(), vertices.size());
}

void VulkanRenderer::createIndexBuffer(u32 idx)
{
    auto usageFlags = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;
    auto memoryFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eDeviceLocal;
    mIndexBuffer[idx].makeBuffer(*this, mDevice, usageFlags, memoryFlags, mBatcher[idx].mIndices.data(), mBatcher[idx].mIndices.size());
}

void VulkanRenderer::createUniformBuffers(u32 idx)
{
    auto usageFlags = vk::BufferUsageFlagBits::eUniformBuffer;
    auto memoryFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    UniformBufferObject uboTemp = {};
    mUniformBuffer[idx].makeBuffer(*this, mDevice, usageFlags, memoryFlags, &uboTemp, 1);
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

    vk::DescriptorSetLayoutBinding texGas;
    texGas.binding = 3;
    texGas.descriptorCount = 1;
    texGas.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    texGas.pImmutableSamplers = nullptr;
    texGas.stageFlags = vk::ShaderStageFlagBits::eFragment;

    vk::DescriptorSetLayoutBinding texSpriteSheets;
    texSpriteSheets.binding = 4;
    texSpriteSheets.descriptorCount = 13; // texture array size
    texSpriteSheets.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    texSpriteSheets.pImmutableSamplers = nullptr;
    texSpriteSheets.stageFlags = vk::ShaderStageFlagBits::eFragment;

    vk::DescriptorSetLayoutBinding bindings[] = {uboLayoutBinding, texPalette, texCamera, texGas, texSpriteSheets};
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
        // gas
        {vk::DescriptorType::eCombinedImageSampler, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches},
        // sprites
        {vk::DescriptorType::eCombinedImageSampler, 13 * static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * kMaxBatches},
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

// TODO: Arg names should be swapped + call site changed
static u32 To1dIdx(u32 col, u32 row)
{
    return (row * MAX_FRAMES_IN_FLIGHT) + col;
}

void VulkanRenderer::updateDescriptorSets()
{
    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        const u32 batchCount = static_cast<u32>(std::max(1u, static_cast<u32>(mBatcher[i].mBatches.size())));
        u32 textureIdxOff = 0;
        for (u32 j = 0; j < batchCount; j++)
        {
            std::array<vk::WriteDescriptorSet, 5> descriptorWrites{};

            // ubo
            vk::DescriptorBufferInfo bufferInfo;
            bufferInfo.buffer = **mUniformBuffer[i].mBuffer;
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
            imageInfo.imageView = **mPaletteTexture[i]->View();
            imageInfo.sampler = **mTextureSampler[i];

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

            // Last item is the full screen quad to copy the FB to the screen
            if (j == batchCount - 1)
            {
                descriptorWrites[2].pImageInfo = &mOffScreenPass[i].descriptor;
            }
            else
            {
                imageInfo4.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
                if (mBatcher[i].mCamTexture)
                {
                    imageInfo4.imageView = **mBatcher[i].mCamTexture->View();
                }
                else
                {
                    imageInfo4.imageView = **mPaletteTexture[i]->View();
                }
                imageInfo4.sampler = **mTextureSampler[i];
                descriptorWrites[2].pImageInfo = &imageInfo4;
            }

            // Gas texture
            vk::DescriptorImageInfo gasImage{};
            descriptorWrites[3].dstSet = *mDescriptorSets[To1dIdx(i, j)];
            descriptorWrites[3].dstBinding = 3;
            descriptorWrites[3].dstArrayElement = 0;
            descriptorWrites[3].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[3].descriptorCount = 1;
            if (mGasTexture[mCurrentFrame])
            {
                gasImage.imageView = **mGasTexture[mCurrentFrame]->View();
            }
            else
            {
                gasImage.imageView = **mPaletteTexture[i]->View();
            }
            gasImage.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            gasImage.sampler = **mTextureSampler[i];
            descriptorWrites[3].pImageInfo = &gasImage;

            // Sprite sheets
            vk::DescriptorImageInfo imageInfo2[14];
            for (u32 k = 0; k < 14; k++)
            {
                imageInfo2[k].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
                if (!mBatcher[i].mBatches.empty() && k < mBatcher[i].mBatches[j].mTexturesInBatch)
                {
                    if (textureIdxOff + k < mBatcher[i].mBatchTextures.size())
                    {
                        imageInfo2[k].imageView = **mBatcher[i].mBatchTextures[textureIdxOff + k]->View();
                    }
                }

                if (!imageInfo2[k].imageView)
                {
                    imageInfo2[k].imageView = **mPaletteTexture[i]->View();
                }

                imageInfo2[k].sampler = **mTextureSampler[i];
            }

            descriptorWrites[4].dstSet = *mDescriptorSets[To1dIdx(i, j)];
            descriptorWrites[4].dstBinding = 4;
            descriptorWrites[4].dstArrayElement = 0;
            descriptorWrites[4].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[4].descriptorCount = 13; // texture array size
            descriptorWrites[4].pImageInfo = imageInfo2;

            mDevice->updateDescriptorSets(descriptorWrites, {});

            if (j < mBatcher[i].mBatches.size())
            {
                textureIdxOff += static_cast<u32>(mBatcher[i].mBatches[j].mTexturesInBatch);
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
    offScreenPassInfo.renderPass = **mOffScreenPass[0].renderPass; // TODO
    offScreenPassInfo.framebuffer = **mOffScreenPass[mCurrentFrame].frameBuffer;
    offScreenPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    offScreenPassInfo.renderArea.extent.width = mOffScreenPass[mCurrentFrame].width;
    offScreenPassInfo.renderArea.extent.height = mOffScreenPass[mCurrentFrame].height;


    vk::ClearValue clearColor(vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f));
    offScreenPassInfo.clearValueCount = 1;
    offScreenPassInfo.pClearValues = &clearColor;

    commandBuffer.beginRenderPass(offScreenPassInfo, vk::SubpassContents::eInline);

    u32 batchIdx = 0;
    u32 idxOffset = 0;
    PipelineIndex lastPipeLine = PipelineIndex::eNone;

    for (std::size_t i = 0; i < mBatcher[mCurrentFrame].mBatches.size() - 1; i++)
    {
        if (mBatcher[mCurrentFrame].mBatches[i].mNumTrisToDraw > 0)
        {
            const auto pipeLineForThisBatch = mBatcher[mCurrentFrame].mBatches[i].mBlendMode == 2 ? PipelineIndex::eReverseBlending : PipelineIndex::eAddBlending;
            if (lastPipeLine != pipeLineForThisBatch)
            {
                commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, **mGraphicsPipelines[pipeLineForThisBatch]);

                vk::Viewport viewport;
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = (float) mOffScreenPass[mCurrentFrame].width;
                viewport.height = (float) mOffScreenPass[mCurrentFrame].height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;
                commandBuffer.setViewport(0, viewport);

                lastPipeLine = pipeLineForThisBatch;

                vk::Buffer vertexBuffers[] = {**mVertexBuffer[mCurrentFrame].mBuffer};
                vk::DeviceSize offsets[] = {0};
                commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

                commandBuffer.bindIndexBuffer(**mIndexBuffer[mCurrentFrame].mBuffer, 0, vk::IndexType::eUint32);
            }

            SDL_Rect scissorRect = mBatcher[mCurrentFrame].mBatches[i].mScissor;
            if (scissorRect.x == 0 && scissorRect.y == 0 && scissorRect.w == 0 && scissorRect.h == 0)
            {
                scissorRect.x = 0;
                scissorRect.y = 0;
                scissorRect.w = mOffScreenPass[mCurrentFrame].width;
                scissorRect.h = mOffScreenPass[mCurrentFrame].height;
            }

            vk::Rect2D scissor;
            scissor.offset.x = scissorRect.x;
            scissor.offset.y = scissorRect.y;
            scissor.extent.width = scissorRect.w;
            scissor.extent.height = scissorRect.h;
            commandBuffer.setScissor(0, scissor);

            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                **mPipelineLayouts[pipeLineForThisBatch],
                0,
                *mDescriptorSets[To1dIdx(mCurrentFrame, batchIdx)],
                {});

            commandBuffer.drawIndexed(
                (mBatcher[mCurrentFrame].mBatches[i].mNumTrisToDraw) * 3,
                1,
                idxOffset,
                0,
                0);
        }
        idxOffset += (mBatcher[mCurrentFrame].mBatches[i].mNumTrisToDraw) * 3;
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
    for (std::size_t i = 0; i < mBatcher[mCurrentFrame].mBatches.size(); i++)
    {
        if (i == mBatcher[mCurrentFrame].mBatches.size() - 1)
        {
            if (mBatcher[mCurrentFrame].mBatches[i].mNumTrisToDraw > 0)
            {
                // It can only be the FBO pipeline here
                const auto pipeLineForThisBatch = PipelineIndex::eFBOPipeline;
                if (lastPipeLine != pipeLineForThisBatch)
                {
                    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, **mGraphicsPipelines[pipeLineForThisBatch]);
                    lastPipeLine = pipeLineForThisBatch;

                    SDL_Rect viewPortRect = GetTargetDrawRect();

                    vk::Viewport viewport2;
                    viewport2.x = (float) viewPortRect.x;
                    viewport2.y = (float) viewPortRect.y;
                    viewport2.width = (float) viewPortRect.w;
                    viewport2.height = (float) viewPortRect.h;
                    viewport2.minDepth = 0.0f;
                    viewport2.maxDepth = 1.0f;
                    commandBuffer.setViewport(0, viewport2);

                    vk::Rect2D scissor2{0, 0};
                    scissor2.extent = mSwapChainExtent;
                    commandBuffer.setScissor(0, scissor2);

                    vk::Buffer vertexBuffers[] = {**mVertexBuffer[mCurrentFrame].mBuffer};
                    vk::DeviceSize offsets[] = {0};
                    commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

                    commandBuffer.bindIndexBuffer(**mIndexBuffer[mCurrentFrame].mBuffer, 0, vk::IndexType::eUint32);
                }

                commandBuffer.bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics,
                    **mPipelineLayouts[pipeLineForThisBatch],
                    0,
                    *mDescriptorSets[To1dIdx(mCurrentFrame, batchIdx)],
                    {});

                commandBuffer.drawIndexed(
                    (mBatcher[mCurrentFrame].mBatches[i].mNumTrisToDraw) * 3,
                    1,
                    idxOffset,
                    0,
                    0);
            }
        }
        idxOffset += (mBatcher[mCurrentFrame].mBatches[i].mNumTrisToDraw) * 3;
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

void VulkanRenderer::updateUniformBuffer()
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::ortho(0.0F, 640.0f, 0.0f, 240.0f); // TODO double height

    memcpy(mUniformBuffer[mCurrentFrame].mMappedBuffferMemory, &ubo, sizeof(ubo));
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

    updateUniformBuffer();

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

    try
    {
        result = mPresentQueue->presentKHR(presentInfo);
        // mPresentQueue->waitIdle(); // todo
        // mDevice->waitIdle(); // todo
    }
    catch (vk::OutOfDateKHRError const& /*err*/)
    {
        result = vk::Result::eErrorOutOfDateKHR;
    }
    if (result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR)
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
        if ((availableFormat.format == vk::Format::eR8G8B8A8Unorm || availableFormat.format == vk::Format::eB8G8R8A8Unorm) && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormat;
        }
    }

    throw RendererException("vk::Format::eR8G8B8A8Unorm or vk::Format::eB8G8R8A8Unorm is not a supported surface format");
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

        mBatcher[mCurrentFrame].StartFrame();

        // Set offsets for the screen (this is for the screen shake effect)
        mOffsetX = 0;
        mOffsetY = 0;

    }
}

void VulkanRenderer::EndFrame()
{
    if (mFrameStarted)
    {
        mBatcher[mCurrentFrame].NewBatch();

        Poly_FT4 fullScreenPoly;
        PolyFT4_Init(&fullScreenPoly);
        SetRGB0(&fullScreenPoly, 255, 255, 255);
        SetXYWH(&fullScreenPoly, 0, 0, 640, 240);
        // NOTE: This texture in the last batch is always used as the FB source
        std::shared_ptr<Texture> nullTex;
        mBatcher[mCurrentFrame].PushCAM(fullScreenPoly, nullTex);

        // Add the in flight batch
        mBatcher[mCurrentFrame].EndFrame();

        if (!mBatcher[mCurrentFrame].mVertices.empty() && !mBatcher[mCurrentFrame].mIndices.empty())
        {
            createVertexBuffer(mCurrentFrame);
            createIndexBuffer(mCurrentFrame);
        }

        drawFrame();

        mFrameStarted = false;
    }

    // Always decrease resource lifetimes regardless of drawing to prevent
    // memory leaks
    DecreaseResourceLifetimes();
}

void VulkanRenderer::SetTPage(u16 tPage)
{
    mGlobalTPage = tPage;
}

void VulkanRenderer::SetClip(const Prim_PrimClipper& clipper)
{
    SDL_Rect rect;
    rect.x = clipper.field_C_x;
    rect.y = clipper.field_E_y;
    rect.w = clipper.mBase.header.mRect.w;
    rect.h = clipper.mBase.header.mRect.h;

    mBatcher[mCurrentFrame].NewBatch();

    if (rect.x == 0 && rect.y == 0 && rect.w == 1 && rect.h == 1)
    {
        // No scissor
        rect = {};
    }

    mBatcher[mCurrentFrame].SetScissor(rect);
}

void VulkanRenderer::Draw(const Prim_GasEffect& gasEffect)
{
    const u32 gasWidth = static_cast<u32>(std::floor((gasEffect.w - gasEffect.x) / 4));
    const u32 gasHeight = static_cast<u32>(std::floor((gasEffect.h - gasEffect.y) / 2));

    mGasTexture[mCurrentFrame] = std::make_shared<Texture>(*this, gasWidth, gasHeight, gasEffect.pData, Texture::Format::RGBA16);

    // TODO: If there is more than 1 gas in a frame break the batch ?
    mBatcher[mCurrentFrame].PushGas(gasEffect);
}

void VulkanRenderer::Draw(const Line_G2& line)
{
    mBatcher[mCurrentFrame].PushLine(line, GetTPageBlendMode(mGlobalTPage));
}

void VulkanRenderer::Draw(const Line_G4& line)
{
    mBatcher[mCurrentFrame].PushLine(line, GetTPageBlendMode(mGlobalTPage));
}

void VulkanRenderer::Draw(const Poly_G3& poly)
{
    mBatcher[mCurrentFrame].PushPolyG3(poly, GetTPageBlendMode(mGlobalTPage));
}

u32 VulkanRenderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache[mCurrentFrame].Add(pCache);

    if (addRet.mAllocated)
    {
        // Write palette data
        mPaletteTexture[mCurrentFrame]->LoadSubImage(0, addRet.mIndex, 256, 1, pCache.mPal);

        //mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

void VulkanRenderer::Draw(const Poly_FT4& poly)
{
    if (poly.mCam && !poly.mFg1)
    {
        CamResource* pRes = poly.mCam;
        std::shared_ptr<Texture> texture = mTextureCache[mCurrentFrame].GetCachedTexture(poly.mCam->mUniqueId.Id(), 300);
        if (!texture)
        {
            auto newTex = std::make_unique<Texture>(*this, pRes->mData.mWidth, pRes->mData.mHeight, pRes->mData.mPixels->data(), Texture::Format::RGBA);
            texture = mTextureCache[mCurrentFrame].Add(poly.mCam->mUniqueId.Id(), 300, std::move(newTex));
        }

        mBatcher[mCurrentFrame].PushCAM(poly, texture);
    }
    else if (poly.mCam && poly.mFg1)
    {
        Fg1Layer* pRes = poly.mFg1;
        std::shared_ptr<Texture> texture = mTextureCache[mCurrentFrame].GetCachedTexture(poly.mFg1->mUniqueId.Id(), 300);
        if (!texture)
        {
            texture = std::make_shared<Texture>(*this, pRes->mImage.mWidth, pRes->mImage.mHeight, pRes->mImage.mPixels->data(), Texture::Format::RGBA);
            mTextureCache[mCurrentFrame].Add(pRes->mUniqueId.Id(), 300, texture);
        }
        mBatcher[mCurrentFrame].PushFG1(poly, texture);
    }
    else if (poly.mAnim)
    {
        AnimResource& animRes = poly.mAnim->mAnimRes;

        const u32 palIndex = PreparePalette(*animRes.mCurPal);

        auto pPng = animRes.mPngPtr;
        std::shared_ptr<Texture> texture = mTextureCache[mCurrentFrame].GetCachedTexture(animRes.mUniqueId.Id(), 300);
        if (!texture)
        {
            texture = std::make_shared<Texture>(*this, pPng->mWidth, pPng->mHeight, pPng->mPixels.data(), Texture::Format::Indexed);
            mTextureCache[mCurrentFrame].Add(animRes.mUniqueId.Id(), 300, texture);
        }

        mBatcher[mCurrentFrame].PushAnim(poly, palIndex, texture);
    }
    else if (poly.mFont)
    {
        FontResource& fontRes = poly.mFont->mFntResource;
        std::shared_ptr<PngData> pPng = fontRes.mPngPtr;

        const u32 palIndex = PreparePalette(*fontRes.mCurPal);

        std::shared_ptr<Texture> texture = mTextureCache[mCurrentFrame].GetCachedTexture(poly.mFont->mFntResource.mUniqueId.Id(), 300);
        if (!texture)
        {
            texture = std::make_shared<Texture>(*this, pPng->mWidth, pPng->mHeight, pPng->mPixels.data(), Texture::Format::Indexed);
            mTextureCache[mCurrentFrame].Add(poly.mFont->mFntResource.mUniqueId.Id(), 300, texture);
        }

        mBatcher[mCurrentFrame].PushFont(poly, palIndex, texture);
    }
}

void VulkanRenderer::Draw(const Poly_G4& poly)
{
    mBatcher[mCurrentFrame].PushPolyG4(poly, GetTPageBlendMode(mGlobalTPage));
}

void VulkanRenderer::DecreaseResourceLifetimes()
{
    mTextureCache[mCurrentFrame].DecreaseResourceLifetimes();

    mPaletteCache[mCurrentFrame].ResetUseFlags();
}

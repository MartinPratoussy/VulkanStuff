#include "ImageViews.hpp"

#include <stdexcept>

namespace ImageViews
{

    VkImageView createImageView(VkDevice &device, VkImage &textureImage, VkFormat format)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = textureImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }

    void createImageViews(
        VkDevice &device,
        std::vector<VkImage> &swapChainImages,
        VkFormat swapChainImageFormat,
        std::vector<VkImageView> &swapChainImageViews
    )
    {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++)
        {
            swapChainImageViews
                [i] = createImageView(device, swapChainImages[i], swapChainImageFormat);
        }
    }

    void
    createTextureImageView(VkDevice &device, VkImage &textureImage, VkImageView &textureImageView)
    {
        textureImageView = createImageView(device, textureImage, VK_FORMAT_R8G8B8A8_SRGB);
    }

} // namespace ImageViews
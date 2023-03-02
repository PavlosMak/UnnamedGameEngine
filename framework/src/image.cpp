#include "image.h"
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
DISABLE_WARNINGS_POP()
#include <cassert>
#include <exception>
#include <iostream>
#include <string>

Image::Image(const std::filesystem::path& filePath)
{
	if (!std::filesystem::exists(filePath)) {
		std::cerr << "Texture file " << filePath << " does not exists!" << std::endl;
		throw std::exception();
	}

	const auto filePathStr = filePath.string(); // Create l-value so c_str() is safe.
	[[maybe_unused]] int numChannelsInSourceImage;
	stbi_uc* stbPixels = stbi_load(filePathStr.c_str(), &width, &height, &numChannelsInSourceImage, STBI_rgb);

	if (!stbPixels) {
		std::cerr << "Failed to read texture " << filePath << " using stb_image.h" << std::endl;
		throw std::exception();
	}

	constexpr size_t numChannels = 3; // STBI_rgb == 3 channels
	for (size_t i = 0; i < width * height * numChannels; i += numChannels) {
            pixels.emplace_back(stbPixels[i + 0] / 255.0f, stbPixels[i + 1] / 255.0f, stbPixels[i + 2] / 255.0f);
	}

	stbi_image_free(stbPixels);
}

glm::vec3 Image::getTexel(const glm::vec2& textureCoordinates) const
{
#ifdef HIDE_SOLUTION
	// TODO: read the correct pixel from m_pixels.
	// The pixels are stored in row major order.
	return glm::vec3(0.0f);
#else
	const glm::ivec2 pixel = glm::ivec2(textureCoordinates * glm::vec2(width, height) + 0.5f);
	return pixels[pixel.y * width + pixel.x];
#endif
}

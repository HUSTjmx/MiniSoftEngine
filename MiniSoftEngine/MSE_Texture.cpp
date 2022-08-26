#include "MSE_Texture.h"
namespace MSE {
	Texture::Texture()
	{
		width = 800;
		height = 600;
		type = TextureType::Color;
		contents = std::vector<std::vector<Vec4>>(height, std::vector<Vec4>(width, Vec4()));
	}

	Texture::Texture(int width, int height)
	{
		width = width;
		height = height;
		type = TextureType::Color;
		contents = std::vector<std::vector<Vec4>>(height, std::vector<Vec4>(width, Vec4()));
	}

	Texture::Texture(const Texture& other)
	{
		width = other.width;
		height = other.height;
		type = other.type;
		contents = other.contents;
	}

	Texture& Texture::operator=(const Texture& other)
	{
		width = other.width;
		height = other.height;
		type = other.type;
		contents = other.contents;
		return *this;
	}
}
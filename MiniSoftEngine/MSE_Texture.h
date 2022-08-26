#pragma once
#include <vector>
#include "MSE_Math.h"

namespace MSE
{
	class Vec4;

	enum class TextureType {
		Color,
		Depth,
	};

	class Texture {
	public:

		Texture();

		Texture(int width, int height);

		Texture(const Texture& other);

		Texture& operator=(const Texture& other);

	public:
		int width;
		int height;
		TextureType type;
		std::vector<std::vector<Vec4>> contents;
	
	};

}
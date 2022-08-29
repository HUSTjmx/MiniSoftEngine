#pragma once
#include <vector>
#include <memory>
#include "MSE_Math.h"

namespace MSE
{
	class Vec4;

	enum class TextureType {
		Color,
		Depth,
	};

	enum class SampleType
	{
		Point,
		Box,
		Bilinear,
		Max,
		Min,
		Near
	};

	enum class BoundType
	{
		Repeat,
		Clamp
	};

	class Texture {
	public:

		Texture();

		Texture(int width, int height);

		Texture(const Texture& other);

		Texture& operator=(const Texture& other);

		Vec4 Sample(float u, float v, SampleType sType = SampleType::Point, BoundType bType = BoundType::Clamp);

		Vec4 Sample(int u, int v, BoundType bType = BoundType::Clamp);

	public:
		int width;
		int height;
		TextureType type;
		std::shared_ptr<std::vector<std::vector<Vec4>>> contents;
	
	};

	Vec4 Sample(std::shared_ptr<Texture> tex, float u, float v, SampleType sType = SampleType::Point, BoundType bType = BoundType::Clamp);

	Vec4 Sample(std::shared_ptr<Texture> tex, int u, int v, SampleType sType = SampleType::Point, BoundType bType = BoundType::Clamp);

}
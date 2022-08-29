#include "MSE_Texture.h"
namespace MSE {
	Texture::Texture()
	{
		width = 800;
		height = 600;
		type = TextureType::Color;
		contents = std::make_shared<std::vector<std::vector<Vec4>>>(std::vector<std::vector<Vec4>>(height, std::vector<Vec4>(width, Vec4())));
	}

	Texture::Texture(int width, int height)
	{
		this->width = width;
		this->height = height;
		type = TextureType::Color;
		contents = std::make_shared<std::vector<std::vector<Vec4>>>(std::vector<std::vector<Vec4>>(height, std::vector<Vec4>(width, Vec4())));
	}

	Texture::Texture(const Texture& other)
	{
		this->width = other.width;
		this->height = other.height;
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

	Vec4 Texture::Sample(float u, float v, SampleType sType, BoundType bType)
	{
		if (sType == SampleType::Point)
		{
			int u_ = (int)(u * (float)width);
			int v_ = (int)(v * (float)height);
			return Sample(u_, v_, sType, bType);
		}
		else if (sType == SampleType::Bilinear)
		{
			// ToDo
			int u_ = (int)(u * (float)width);
			int v_ = (int)(v * (float)height);
			return Sample(u_, v_, sType, bType);
		}
		// .......
		else
		{
			int u_ = (int)(u * (float)width);
			int v_ = (int)(v * (float)height);
			return Sample(u_, v_, sType, bType);
		}
	}

	void SolveBoundValue(int& u, int& v, int width, int height, BoundType btype)
	{
		if (u >= 0 && u <= width - 1 && v >= 0 && v <= height- 1)return;
		if (btype == BoundType::Clamp)
		{
			if (u < 0)u = 0;
			if (v < 0)v = 0;
			if (u >= width) u = width - 1;
			if (v >= height) v = height - 1;
		}
		else if (btype == BoundType::Repeat)
		{
			return; // ToDo
		}
	}

	Vec4 Texture::Sample(int u, int v, BoundType bType)
	{
		SolveBoundValue(u, v, width, height, bType);
		return (*contents)[v][u];
	}


	Vec4 Sample(std::shared_ptr<Texture> tex, float u, float v, SampleType sType, BoundType bType)
	{
		if (sType == SampleType::Point)
		{
			int u_ = (int)(u * (float)tex->width);
			int v_ = (int)(v * (float)tex->height);
			return Sample(tex, u_, v_, sType, bType);
		}
		else
		{
			// ToDO
			int u_ = (int)(u * (float)tex->width);
			int v_ = (int)(v * (float)tex->height);
			return Sample(tex, u_, v_, sType, bType);
		}
	}


	Vec4 Sample(std::shared_ptr<Texture> tex, int u, int v, SampleType sType, BoundType bType)
	{
		SolveBoundValue(u, v, tex->width, tex->height, bType);
		return (*tex->contents)[v][u];
	}
}
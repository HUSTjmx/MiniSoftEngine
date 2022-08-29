#include "MSE_FrameBuffer.h"
#include "MSE_Texture.h"
#include "MSE_Config.h"

namespace MSE
{
	void FrameBuffer::Init(int width, int height, int ColorAttachNum, int DepthAttachNum)
	{
		this->width = width;
		this->height = height;

		for (int i = 0; i < ColorAttachNum; i++)
		{
			std::shared_ptr<MSE::Texture> colorAttach = std::make_shared<MSE::Texture>(MSE::Texture(width, height));
			colorAttach->type = MSE::TextureType::Color;
			colorAttachments.push_back(colorAttach);
		}

		for (int i = 0; i < DepthAttachNum; i++)
		{
			std::shared_ptr<MSE::Texture> depthAttach = std::make_shared<MSE::Texture>(MSE::Texture(width, height));
			depthAttach->type = MSE::TextureType::Depth;
			depthAttachments.push_back(depthAttach);
		}
	}

	void FrameBuffer::DrawColorAttach(int x, int y, const Vec4& col, unsigned int index /* = 0 */)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)return;
		if (colorAttachments.size() <= index)return;
		(*colorAttachments[index]->contents)[y][x] = col;
	}

	Vec4 FrameBuffer::GetColorAttachElem(int x, int y, unsigned int index)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)return Vec4();
		if (colorAttachments.size() <= index)return Vec4();
		return (*colorAttachments[index]->contents)[y][x];
	}

	std::shared_ptr<Texture> FrameBuffer::GetColorAttach(unsigned int index)
	{
		return colorAttachments[index];
	}

	void FrameBuffer::DrawDepthAttach(int x, int y, float dep, unsigned int index /* = 0 */)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)return;
		if (depthAttachments.size() <= index)return;
		(*depthAttachments[index]->contents)[y][x].x = dep;
	}

	float FrameBuffer::GetDepthAttachElem(int x, int y, unsigned int index /* = 0 */)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)return 1.0;
		if (depthAttachments.size() <= index)return 1.0;
		return (*depthAttachments[index]->contents)[y][x].x;
	}

	std::shared_ptr<Texture> FrameBuffer::GetDepthAttach(unsigned int index)
	{
		return depthAttachments[index];
	}

	void FrameBuffer::DrawStencilAttach(int x, int y, float st, unsigned int index /* = 0 */)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)return;
		if (depthAttachments.size() <= index)return;
		(*depthAttachments[index]->contents)[y][x].y = st;
	}

	float FrameBuffer::GetStencilValue(int x, int y, unsigned int index)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)return 0.0;
		if (depthAttachments.size() <= index)return 0.0;
		return (*depthAttachments[index]->contents)[y][x].y;
	}

	void FrameBuffer::TranslateCol(unsigned int* g_frameBuff, unsigned int index)
	{
		for(int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				int m = j * width + i;
				g_frameBuff[m] = VecCol2IntCol((*colorAttachments[index]->contents)[j][i]);
			}
	}

	void FrameBuffer::TranslateDepth(unsigned int* g_frameBuff, unsigned int index /* = 0 */)
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				int m = j * width + i;
				g_frameBuff[m] = FloatCol2IntCol((*depthAttachments[index]->contents)[j][i].x);
			}
	}

	void FrameBuffer::ResetCol(Vec4 col, unsigned int index /* = 0 */)
	{
		if (index >= colorAttachments.size())return;
		for(int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				(*colorAttachments[index]->contents)[j][i] = col;
			}
	}

	void FrameBuffer::ResetDepth(float dp, unsigned int index /* = 0 */)
	{
		if (index >= depthAttachments.size())return;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				(*depthAttachments[index]->contents)[j][i].x = dp;
			}
	}

	void FrameBuffer::ResetStencil(float dp, unsigned int index /* = 0 */)
	{
		if (index >= depthAttachments.size())return;
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				(*depthAttachments[index]->contents)[j][i].y = dp;
			}
	}

	void FrameBuffer::Reset(Vec4 col, float dp, float st)
	{
		ResetCol(col);
		ResetDepth(dp);
		ResetStencil(st);
	}

	void FrameBuffer::Flip_Y_Color(unsigned int index)
	{
		auto temp = *colorAttachments[index]->contents.get();
		for(int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				(*colorAttachments[index]->contents)[height - j - 1][i] = temp[j][i];
			}
	}

	// 注意：调用此函数前需要进行色调映射
	void FrameBuffer::ChangeColRange(unsigned int index /* = 0 */)
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				(*colorAttachments[index]->contents)[j][i] = (*colorAttachments[index]->contents)[j][i] * 255;
			}
	}

	void FrameBuffer::ChangeDepthRange(unsigned int index)
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				(*depthAttachments[index]->contents)[j][i].x = (*depthAttachments[index]->contents)[j][i].x * 255;
			}
	}

}
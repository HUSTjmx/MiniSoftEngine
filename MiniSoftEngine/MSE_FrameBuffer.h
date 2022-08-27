#pragma once
#include <vector>
#include <memory>
#include "MSE_Math.h"

namespace MSE {

	class Texture;

	class FrameBuffer {
	public:
		void Init(int width, int height, int ColorAttachNum = 1, int DepthAttachNum = 1);

		void DrawColorAttach(int x, int y, const Vec4& col, unsigned int index = 0);

		Vec4 GetColorAttachElem(int x, int y, unsigned int index = 0);

		void DrawDepthAttach(int x, int y, float dep, unsigned int index = 0);

		float GetDepthAttachElem(int x, int y, unsigned int index = 0);

		void DrawStencilAttach(int x, int y, float st, unsigned int index = 0);

		float GetStencilValue(int x, int y, unsigned int index = 0);

		void TranslateCol(unsigned int* g_frameBuff, unsigned int index = 0);

		void TranslateDepth(unsigned int* g_frameBuff, unsigned int index = 0);

		void ResetCol(Vec4 col, unsigned int index = 0);

		void ResetDepth(float dp, unsigned int index = 0);

		void ResetStencil(float dp, unsigned int index = 0);

		void Flip_Y_Color(unsigned int index = 0);

		void ChangeColRange(unsigned int index = 0);

		void ChangeDepthRange(unsigned int index = 0);

	public:
		int width;
		int height;

		std::vector<std::shared_ptr<Texture>> colorAttachments;
		std::vector<std::shared_ptr<Texture>> depthAttachments;
	};
}
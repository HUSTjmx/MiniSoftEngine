#pragma once
#include <memory>
#include <vector>
#include "MSE_Object.h"
#include "MSE_Camera.h"
#include "MSE_FrameBuffer.h"

namespace MSE {

	class PostPass;
	class vertShader;
	class fragShader;

	enum class StencilOp
	{
		None,
		Equal,
		Leq,
		Leq_Write,
		Req,
		Req_Write,
	};

	class Pipeline {
	public:
		void TransformScreen(Vec4& p, int width, int height);

		bool ViewClip(const Vec4& v);

		void Rasterize(Facet& facet, FrameBuffer& fbo, std::vector<Fragment>& re, Vec4 param1 = Vec4(), Vec4 param2 = Vec4());

		bool EarlyZTest(FrameBuffer& fbo, int x, int y, float depth);

		bool EarlyStencilTest(FrameBuffer& fbo, int x, int y);

		void ToneMapping(FrameBuffer& fbo, unsigned int index = 0);

		void GammaCorrection(FrameBuffer& fbo, unsigned int index = 0);

		void PostProcessBeforeToneMap(FrameBuffer& fbo, std::shared_ptr<FrameBuffer> texes, unsigned int index = 0);

		void PostProcessAfterToneMap(FrameBuffer& fbo, std::shared_ptr<FrameBuffer> texes, unsigned int index = 0);

	public: 
		std::shared_ptr<vertShader> vert;
		std::shared_ptr<fragShader> frag;
		std::shared_ptr<PostPass> postPassesBefore; /* 色调映射之前的后处理 */
		std::shared_ptr<PostPass> postPassesAfter; /* 色调映射之后的后处理 */
		

		bool UseStencil;
		StencilOp stOp;
		float stencilValue;

		bool UseMSAA;
		int MSAA_Level;
		//int stencil;

		bool NeedFlipY;

		// 固定后处理
		// 全局色调映射
		int ToneMap; /* 0代表不使用Tone Map,1, 2, 3分别使用不同的色调函数 */
		float exposure;

		// 伽马校正
		bool UseGamma;
		float gammaBase;
	};
}
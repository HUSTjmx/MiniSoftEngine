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
		std::shared_ptr<PostPass> postPassesBefore; /* ɫ��ӳ��֮ǰ�ĺ��� */
		std::shared_ptr<PostPass> postPassesAfter; /* ɫ��ӳ��֮��ĺ��� */
		

		bool UseStencil;
		StencilOp stOp;
		float stencilValue;

		bool UseMSAA;
		int MSAA_Level;
		//int stencil;

		bool NeedFlipY;

		// �̶�����
		// ȫ��ɫ��ӳ��
		int ToneMap; /* 0����ʹ��Tone Map,1, 2, 3�ֱ�ʹ�ò�ͬ��ɫ������ */
		float exposure;

		// ٤��У��
		bool UseGamma;
		float gammaBase;
	};
}
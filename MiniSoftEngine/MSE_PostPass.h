#pragma once



namespace MSE
{
	class FrameBuffer;

	class PostPass {
	public:
		virtual void Process(FrameBuffer& fbo) {}
		virtual void Process(FrameBuffer& fbo, const FrameBuffer& tex1) {}
		virtual void Process(FrameBuffer& fbo, const FrameBuffer& tex1, const FrameBuffer& tex2) {}

	public:
		int kind; /* 1,2,3*/
	};



}
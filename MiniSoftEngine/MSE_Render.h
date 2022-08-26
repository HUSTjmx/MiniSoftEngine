#pragma once
#include <memory>
#include <windows.h>
#include "MSE_Camera.h"
#include "MSE_Pipeline.h"

namespace MSE {


	enum class RenderType
	{
		WireFrame,
		Normal
	};

	class Render {
	public:

		Render();

		void InitRender(int w, int h, HWND hWnd);

		void update(HWND hWnd);

		void ClearBuffer();

		void ShutDown();

		void onMouseMessage(UINT message, WPARAM wParam, LPARAM lParam);

		void Draw(Scene& scene, Pipeline& pipe, FrameBuffer& fbo);

		void Draw(Scene& scene, Pipeline& pipe, FrameBuffer& fbo, std::shared_ptr<Camera> camera);

		void DrawLine(FrameBuffer& fbo, int x1, int y1, int x2, int y2, Vec4 color);

	public:
		int width;
		int height;

		HDC g_tempDC;
		HBITMAP g_tempBm;
		HBITMAP g_oldBm;
		unsigned int* g_frameBuff;
		std::shared_ptr<float[]> g_depthBuff;
		std::shared_ptr<Camera> mainCamera;

		RenderType renderType;
	};
}
#include "MSE_Render.h"
#include "MSE_Shader.h"

namespace MSE {

	Render::Render()
	{
		g_tempDC = nullptr;
		g_tempBm = nullptr;
		g_oldBm = nullptr;
		g_frameBuff = nullptr;
		g_depthBuff = nullptr;
		renderType = RenderType::Normal;
	}

	void Render::InitRender(int w, int h, HWND hWnd)
	{
		width = w;
		height = h;

		// 1. 创建一个屏幕缓冲
		// 1.1 创建一个与当前设备兼容的DC
		HDC hDC = GetDC(hWnd);
		g_tempDC = CreateCompatibleDC(hDC);
		ReleaseDC(hWnd, hDC);

		// 1.2 创建该DC的bitmap缓冲  32位色
		BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
			(DWORD)w * h * 4, 0, 0, 0, 0 } };
		g_tempBm = CreateDIBSection(g_tempDC, &bi, DIB_RGB_COLORS, (void**)&g_frameBuff, 0, 0);
		// 1.3 选择该bitmap到dc中
		g_oldBm = (HBITMAP)SelectObject(g_tempDC, g_tempBm);
		// 1.4 创建深度缓冲区
		g_depthBuff.reset(new float[w * h]);

		// 清理屏幕缓冲
		ClearBuffer();

	}

	void Render::Draw(Scene& scene, Pipeline& pipe, FrameBuffer& fbo)
	{
		Draw(scene, pipe, fbo, mainCamera);
	}

	void Render::Draw(Scene& scene, Pipeline& pipe, FrameBuffer& fbo, std::shared_ptr<Camera> camera)
	{
		camera->GetViewMat();
		if(camera->projectionType == CameraType::Perspective)camera->GetProjMat(fbo.width / fbo.height);
		else camera->GetOrthoMat(10, 10);


		// 渲染不透明材质
		for (int i = 0; i < scene.OpaqueObjects.size(); i++)
		{
			// 提前计算模型矩阵
			scene.OpaqueObjects[i].CalModelMat();

			// 
			if (pipe.UseStencil)
			{
				pipe.stOp = (StencilOp)scene.OpaqueObjects[i].material->stencilOp;
				pipe.stencilValue = scene.OpaqueObjects[i].material->stencilValue;
			}

			for (int j = 0; j < scene.OpaqueObjects[i].facets->size(); j++)
			{
				auto facet = (*scene.OpaqueObjects[i].facets)[j];

				// 顶点着色器
				pipe.vert->Process(facet.p1, facet.p1_R, scene.OpaqueObjects[i].modelMat, camera, scene.lights);
				pipe.vert->Process(facet.p2, facet.p2_R, scene.OpaqueObjects[i].modelMat, camera, scene.lights);
				pipe.vert->Process(facet.p3, facet.p3_R, scene.OpaqueObjects[i].modelMat, camera, scene.lights);
				// 齐次除法
				facet.p1_R.pos.HomogenousDivide();
				facet.p2_R.pos.HomogenousDivide();
				facet.p3_R.pos.HomogenousDivide();
				Vertex& p1 = facet.p1_R;
				Vertex& p2 = facet.p2_R;
				Vertex& p3 = facet.p3_R;
				// 视锥体剔除
				if (pipe.ViewClip(p1.pos))continue;
				if (pipe.ViewClip(p2.pos))continue;
				if (pipe.ViewClip(p3.pos))continue;
				// 重映射到屏幕空间
				pipe.TransformScreen(p1.pos, fbo.width, fbo.height);
				pipe.TransformScreen(p2.pos, fbo.width, fbo.height);
				pipe.TransformScreen(p3.pos, fbo.width, fbo.height);

				if (renderType == RenderType::WireFrame)
				{
					int x1 = (int)(p1.pos.x + 0.5f), x2 = (int)(p2.pos.x + 0.5f), x3 = (int)(p3.pos.x + 0.5f);
					int y1 = (int)(p1.pos.y + 0.5f), y2 = (int)(p2.pos.y + 0.5f), y3 = (int)(p3.pos.y + 0.5f);
					DrawLine(fbo, x1, y1, x2, y2, COLOR::Red);
					DrawLine(fbo, x2, y2, x3, y3, COLOR::Red);
					DrawLine(fbo, x1, y1, x3, y3, COLOR::Red);
				}
				else
				{
					// 光栅化
					std::vector<Fragment> frags;
					pipe.Rasterize(facet, fbo, frags, camera);

					// 像素着色器
					pipe.frag->Process(frags, fbo, scene.lights, camera, scene.OpaqueObjects[i].material);
				}


			}
		}
		
		
	}

	void Render::update(HWND hWnd)
	{
		// 1. clear frameBuffer
		//ClearBuffer();

		// present frameBuffer to screen
		HDC hDC = GetDC(hWnd);
		BitBlt(hDC, 0, 0, width, height, g_tempDC, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hDC);

	}

	void Render::ClearBuffer()
	{
		Vec4 bgColor = const_cast<Vec4&>(COLOR::BgColor);
		for (int row = 0; row < height; ++row)
		{
			for (int col = 0; col < width; ++col)
			{
				int idx = row * width + col;
				// 默认背景色浅蓝 R123 G195 B221
				g_frameBuff[idx] = VecCol2IntCol(bgColor * 255.0f);
				// 深度缓冲区 1.0f
				g_depthBuff[idx] = 1.0f;
			}
		}

	}


	void Render::ShutDown()
	{
		if (g_tempDC)
		{
			if (g_oldBm)
			{
				SelectObject(g_tempDC, g_oldBm);
				g_oldBm = nullptr;
			}
			DeleteDC(g_tempDC);
			g_tempDC = nullptr;
		}

		if (g_tempBm)
		{
			DeleteObject(g_tempBm);
			g_tempBm = nullptr;
		}

	}

	void Render::onMouseMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		static bool isPressed = false;
		static LPARAM lpCur = 0;
		switch (message)
		{
		case WM_MOUSEWHEEL:
			mainCamera->ProcessMouseScroll(GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		case WM_LBUTTONDOWN:
			isPressed = true;
			lpCur = lParam;
			break;
		case WM_LBUTTONUP:
			isPressed = false;
			break;
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON)
			{
				short xMove = LOWORD(lParam) - LOWORD(lpCur);
				short yMove = HIWORD(lParam) - HIWORD(lpCur);
				lpCur = lParam;
				mainCamera->ProcessMouseMovement(xMove, yMove);
			}
			break;
		case WM_KEYDOWN:
			if (wParam == VK_SPACE)
				mainCamera->Reset();
			else if (wParam == VK_SHIFT)
			{
				if (renderType == RenderType::WireFrame) renderType = RenderType::Normal;
				else renderType = RenderType::WireFrame;
			}
			break;
		default:
			break;
		}
	}

	void Render::DrawLine(FrameBuffer& fbo, int x1, int y1, int x2, int y2, Vec4 color)
	{
		if (x1 == x2 && y1 == y2)
		{
			fbo.DrawColorAttach(x1, y1, color);
			//drawPixel(x1, y1, color);
		}
		else if (x1 == x2)
		{
			if (y1 > y2) std::swap(y1, y2);
			for (int y = y1; y <= y2; ++y)
				fbo.DrawColorAttach(x1, y, color); //drawPixel(x1, y, color);
		}
		else if (y1 == y2)
		{
			if (x1 > x2) std::swap(x1, x2);
			for (int x = x1; x <= x2; ++x)
				fbo.DrawColorAttach(x, y1, color); //drawPixel(x, y1, color);
		}
		else
		{
			// Bresenham
			int diff = 0;
			int dx = std::abs(x1 - x2);
			int dy = std::abs(y1 - y2);
			if (dx > dy)
			{
				if (x1 > x2) std::swap(x1, x2), std::swap(y1, y2);
				for (int x = x1, y = y1; x < x2; ++x)
				{
					fbo.DrawColorAttach(x, y, color); //drawPixel(x, y, color);
					diff += dy;
					if (diff >= dx)
					{
						diff -= dx;
						y += (y1 < y2) ? 1 : -1;
					}
				}
				fbo.DrawColorAttach(x2, y2, color); //drawPixel(x2, y2, color);
			}
			else
			{
				if (y1 > y2) std::swap(x1, x2), std::swap(y1, y2);
				for (int y = y1, x = x1; y < y2; ++y)
				{
					fbo.DrawColorAttach(x, y, color); //drawPixel(x, y, color);
					diff += dx;
					if (diff >= dy)
					{
						diff -= dy;
						x += (x1 < x2) ? 1 : -1;
					}
				}
				fbo.DrawColorAttach(x2, y2, color); //drawPixel(x2, y2, color);
			}
		}

	}
}
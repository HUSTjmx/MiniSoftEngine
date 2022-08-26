#include <windows.h>
#include <memory>
#include "MSE_Render.h"
#include "MSE_FrameBuffer.h"
#include "MSE_Config.h"
#include "MSE_Texture.h"
#include "MSE_Object.h"
#include "MSE_Pipeline.h"

// 全局变量
static MSE::Render render;

void InitData()
{
	MSE::DATA::SetupMaterialList();
}

void SetUpPipeLine(MSE::Pipeline& pipe)
{
	pipe.UseStencil = false;
	pipe.UseMSAA = false;
	pipe.MSAA_Level = 2;
	pipe.NeedFlipY = true;
	pipe.ToneMap = 0;
	pipe.UseGamma = true;
	pipe.gammaBase = 2.2;
	pipe.vert = std::make_shared<MSE::vertShader>();
	pipe.frag = std::make_shared<MSE::fragShader>();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// 1 创建win32程序
	// "https://docs.microsoft.com/zh-cn/previous-versions/visualstudio/visual-studio-2008/bb384843(v%3dvs.90)"
	// 1.1 注册类
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"MiniSoftRender";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE((WORD)IDI_APPLICATION));
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			L"Call to RegisterClassEx failed!",
			L"Win32 Guided Tour",
			NULL);

		return 1;
	}

	// 1.2 创建窗口
	HWND hWnd = CreateWindow(
		L"MiniSoftRender",
		L"MiniSoftRender",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL,
			L"Call to CreateWindow failed!",
			L"Win32 Guided Tour",
			NULL);

		return 1;
	}

	// 1.3 展示窗口
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	// 初始化一些测试数据
	InitData();

	// 渲染器初始化
	render.InitRender(MSE::SCR_WIDTH, MSE::SCR_HEIGHT, hWnd);
	render.renderType = MSE::RenderType::Normal; /* 测试 */
	render.mainCamera = std::make_shared<MSE::Camera>(MSE::Vec4(0.0f, 0.0f, -10.0f, 1.0f), MSE::Vec4(0.0f, 1.0f, 0.0f, 0.0f));

	// 主FBO初始化
	MSE::FrameBuffer mainPresentFBO;
	mainPresentFBO.width = MSE::SCR_WIDTH;
	mainPresentFBO.height = MSE::SCR_HEIGHT;
	std::shared_ptr<MSE::Texture> colorAttach = std::make_shared<MSE::Texture>(MSE::Texture(mainPresentFBO.width, mainPresentFBO.height));
	colorAttach->type = MSE::TextureType::Color;
	std::shared_ptr<MSE::Texture> depthAttach = std::make_shared<MSE::Texture>(MSE::Texture(mainPresentFBO.width, mainPresentFBO.height));
	depthAttach->type = MSE::TextureType::Depth;
	mainPresentFBO.colorAttachments.push_back(colorAttach);
	mainPresentFBO.depthAttachments.push_back(depthAttach);

	// 场景初始化
	MSE::Scene scene;
	MSE::Light sun(MSE::LIGHT::colors[0], MSE::LIGHT::dirs[0], MSE::LIGHT::powers[0]);
	scene.lights.push_back(sun);
	MSE::Object cube_01(MSE::GEOMETRY::cube_01_Vertices, MSE::GEOMETRY::cube_01_Layouts);
	cube_01.material = MSE::DATA::GetStaticMaterial(0);
	cube_01.position = MSE::Vec4(0.0, 0.0, 0.0, 1.0);
	//cube_01.scale = MSE::Vec4(1.0, 0.5, 2.0, 1.0);
	cube_01.rotate = MSE::Vec4(10.0, 10.0, 0.0, 1.0);
	scene.OpaqueObjects.push_back(cube_01);

	// 管道初始化
	MSE::Pipeline pipe;
	SetUpPipeLine(pipe);


	// 1.4 消息循环
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Clear
			render.ClearBuffer();
			mainPresentFBO.ResetCol(MSE::COLOR::BgColor);
			mainPresentFBO.ResetDepth(1.0);
			mainPresentFBO.ResetStencil(0.0);


			render.Draw(scene, pipe, mainPresentFBO);


			// Final Post Processing
			pipe.ToneMapping(mainPresentFBO);
			pipe.GammaCorrection(mainPresentFBO);

			// Final
			if (pipe.NeedFlipY) mainPresentFBO.Flip_Y_Color();
			mainPresentFBO.ChangeColRange();
			mainPresentFBO.TranslateCol(render.g_frameBuff);
			render.update(hWnd);
		}
	}
	render.ShutDown();
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_KEYDOWN:
		render.onMouseMessage(message, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
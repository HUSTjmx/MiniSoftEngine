#include <windows.h>
#include <memory>
#include "MSE_Render.h"
#include "MSE_FrameBuffer.h"
#include "MSE_Config.h"
#include "MSE_Texture.h"
#include "MSE_Object.h"
#include "MSE_Pipeline.h"
#include "MSE_Shader.h"

// 全局变量
static MSE::Render render;

void InitData()
{
	MSE::DATA::SetupMaterialList();
}

void InitScene(MSE::Scene& scene)
{
	MSE::Light sun(MSE::LIGHT::colors[0], MSE::LIGHT::dirs[0], MSE::LIGHT::powers[0]);
	sun.CanCastShadow = true;
	sun.fbo->Init(MSE::SCR_WIDTH, MSE::SCR_HEIGHT, 1, 1);
	sun.camera->DirectUseFront = true;
	scene.lights.push_back(sun);

	MSE::Object cube_01(MSE::GEOMETRY::GetCubeObj());
	cube_01.material = MSE::DATA::GetStaticMaterial(0);
	cube_01.position = MSE::Vec4(0.0, 0.7, 0.0, 1.0);
	cube_01.scale = MSE::Vec4(1.0, 1.0, 1.0, 1.0);
	cube_01.rotate = MSE::Vec4(0.0, 45.0, 0.0, 1.0);
	scene.OpaqueObjects.push_back(cube_01);

	MSE::Object plane_Ground(MSE::GEOMETRY::GetPlaneObj());
	plane_Ground.material = MSE::DATA::GetStaticMaterial(1);
	plane_Ground.position = MSE::Vec4(0.0, 0.0, 0.0, 1.0);
	plane_Ground.scale = MSE::Vec4(1.0, 1.0, 1.0, 1.0);
	plane_Ground.rotate = MSE::Vec4(0.0, 0.0, 0.0, 1.0);
	scene.OpaqueObjects.push_back(plane_Ground);
}

void SetUpPipeLine(MSE::Pipeline& pipe)
{
	pipe.UseStencil = false;
	pipe.UseMSAA = false;
	pipe.MSAA_Level = 2;
	pipe.NeedFlipY = true;
	pipe.ToneMap = 0;
	pipe.UseGamma = false;
	pipe.gammaBase = 2.2;
	pipe.vert = std::make_shared<MSE::vertShader>(MSE::vertShader());
	//pipe.frag = std::make_shared<MSE::fragShader>(MSE::fragShader::Instance());
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
	render.mainCamera = std::make_shared<MSE::Camera>(MSE::Vec4(0.0f, 8.3f, -5.0f, 1.0f), MSE::Vec4(0.0f, 1.0f, 0.0f, 0.0f), MSE::YAW_C);
	render.mainCamera->projectionType = MSE::CameraType::Perspective;
	render.mainCamera->DirectUseFront = false;

	// 主FBO初始化
	MSE::FrameBuffer mainPresentFBO;
	mainPresentFBO.Init(MSE::SCR_WIDTH, MSE::SCR_HEIGHT);

	// 场景初始化
	MSE::Scene scene;
	InitScene(scene);

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
			mainPresentFBO.Reset(MSE::COLOR::BgColor, 0.0, 0.0);
			for (int i = 0; i < scene.lights.size(); i++)
				scene.lights[i].fbo->ResetDepth(1.0);

			// 绘制shadowMap
			pipe.frag = std::make_shared<MSE::fragShader_DepthDraw>(MSE::fragShader_DepthDraw());
			pipe.vert = std::make_shared<MSE::vertShader>(MSE::vertShader());

			//scene.lights[0].camera->projectionType = MSE::CameraType::Ortho;

			for(int i = 0; i < scene.lights.size(); i++)
				if(scene.lights[i].CanCastShadow)render.Draw(scene, pipe, *scene.lights[i].fbo, scene.lights[i].camera);
			
			// 正式
			//scene.lights[0].camera->projectionType = MSE::CameraType::Perspective;
			pipe.frag = std::make_shared<MSE::fragShader>(MSE::fragShader());
			pipe.vert = std::make_shared<MSE::vertShaderWithPosInLightSpace>(MSE::vertShaderWithPosInLightSpace());
			render.Draw(scene, pipe, mainPresentFBO);

			// Final Post Processing
			pipe.ToneMapping(mainPresentFBO);
			pipe.GammaCorrection(mainPresentFBO);

			// Final FBO Op
			if (pipe.NeedFlipY) mainPresentFBO.Flip_Y_Color();
			mainPresentFBO.ChangeColRange();
			mainPresentFBO.TranslateCol(render.g_frameBuff);
			//scene.lights[0].fbo->ChangeDepthRange();
			//scene.lights[0].fbo->TranslateDepth(render.g_frameBuff);
			//mainPresentFBO = *scene.lights[0].fbo;
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
#include "Dx11Form.h"

namespace catool
{
	namespace dx11
	{
		HRESULT FormDx11::InitDevice()
		{
			HRESULT hResult = S_OK;//返回结果

			RECT rc;
			GetClientRect(raw_handle, &rc);//获取窗口客户区大小
			UINT width = rc.right - rc.left;
			UINT height = rc.bottom - rc.top;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			//驱动类型数组
			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			//特征级别数组
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			//交换链
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));//填充
			sd.BufferCount = 1;                              //我们只创建一个后缓冲（双缓冲）因此为1
			sd.BufferDesc.Width = width;
			sd.BufferDesc.Height = height;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = raw_handle;
			sd.SampleDesc.Count = 1;                      //1重采样
			sd.SampleDesc.Quality = 0;                      //采样等级
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;      //常用参数
			sd.Windowed = TRUE;                              //是否全屏

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
			{
				g_driverType = driverTypes[driverTypeIndex];
				hResult = D3D11CreateDeviceAndSwapChain(
					NULL,                                //默认图形适配器
					g_driverType,                        //驱动类型
					NULL,                                //实现软件渲染设备的动态库句柄，如果使用的驱动设备类型是软件设备则不能为NULL
					createDeviceFlags,                    //创建标志，0用于游戏发布，一般D3D11_CREATE_DEVICE_DEBUG允许我们创建可供调试的设备，在开发中比较有用
					featureLevels,                        //特征等级
					numFeatureLevels,                    //特征等级数量
					D3D11_SDK_VERSION,                    //sdk版本号
					&sd,
					&g_pSwapChain,
					&g_pd3dDevice,
					&g_featureLevel,
					&g_pImmediateContext
				);
				if (SUCCEEDED(hResult))
					break;
			}
			if (FAILED(hResult))
				return hResult;

			//创建渲染目标视图
			ID3D11Texture2D *pBackBuffer = NULL;
			//获取后缓冲区地址
			hResult = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (FAILED(hResult))
				return hResult;

			//创建目标视图
			hResult = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
			//释放后缓冲
			pBackBuffer->Release();
			if (FAILED(hResult))
				return hResult;

			//绑定到渲染管线
			g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

			//设置viewport
			D3D11_VIEWPORT vp;
			vp.Height = (FLOAT)height;
			vp.Width = (FLOAT)width;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			g_pImmediateContext->RSSetViewports(1, &vp);

			return S_OK;

		}

		void FormDx11::CleanupDevice()
		{
			if (g_pImmediateContext)
				g_pImmediateContext->ClearState();
			if (g_pSwapChain)
				g_pSwapChain->Release();
			if (g_pRenderTargetView)
				g_pRenderTargetView->Release();
			if (g_pImmediateContext)
				g_pImmediateContext->Release();
			if (g_pd3dDevice)
				g_pd3dDevice->Release();
		}

		HRESULT FormDx11::create_window()
		{
			FormWin32::create_window();
			if (FAILED(InitDevice()))
			{
				CleanupDevice();
				return S_FALSE;
			}
			
			return S_OK;
		}

		void FormDx11::destory_window()
		{
			CleanupDevice();
			FormWin32::destory_window();
		}

		void FormDx11::message_process()
		{
			//FormWin32::message_process();
			MSG msg;
			ZeroMemory(&msg, sizeof(MSG));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					float ClearColor[4] = { 0.5f, 0.1f, 0.2f, 1.0f }; //red,green,blue,alpha
					g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
					g_pSwapChain->Present(0, 0);
				}
			}
		}

	}
}

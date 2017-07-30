#include "Dx11Form.h"

namespace catool
{
	namespace dx11
	{
		HRESULT FormDx11::InitDevice()
		{
			HRESULT hResult = S_OK;//���ؽ��

			RECT rc;
			GetClientRect(raw_handle, &rc);//��ȡ���ڿͻ�����С
			UINT width = rc.right - rc.left;
			UINT height = rc.bottom - rc.top;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			//������������
			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			//������������
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			//������
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));//���
			sd.BufferCount = 1;                              //����ֻ����һ���󻺳壨˫���壩���Ϊ1
			sd.BufferDesc.Width = width;
			sd.BufferDesc.Height = height;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = raw_handle;
			sd.SampleDesc.Count = 1;                      //1�ز���
			sd.SampleDesc.Quality = 0;                      //�����ȼ�
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;      //���ò���
			sd.Windowed = TRUE;                              //�Ƿ�ȫ��

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
			{
				g_driverType = driverTypes[driverTypeIndex];
				hResult = D3D11CreateDeviceAndSwapChain(
					NULL,                                //Ĭ��ͼ��������
					g_driverType,                        //��������
					NULL,                                //ʵ�������Ⱦ�豸�Ķ�̬���������ʹ�õ������豸����������豸����ΪNULL
					createDeviceFlags,                    //������־��0������Ϸ������һ��D3D11_CREATE_DEVICE_DEBUG�������Ǵ����ɹ����Ե��豸���ڿ����бȽ�����
					featureLevels,                        //�����ȼ�
					numFeatureLevels,                    //�����ȼ�����
					D3D11_SDK_VERSION,                    //sdk�汾��
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

			//������ȾĿ����ͼ
			ID3D11Texture2D *pBackBuffer = NULL;
			//��ȡ�󻺳�����ַ
			hResult = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (FAILED(hResult))
				return hResult;

			//����Ŀ����ͼ
			hResult = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
			//�ͷź󻺳�
			pBackBuffer->Release();
			if (FAILED(hResult))
				return hResult;

			//�󶨵���Ⱦ����
			g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

			//����viewport
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

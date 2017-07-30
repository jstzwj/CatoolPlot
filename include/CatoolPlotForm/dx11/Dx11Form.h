#pragma once

#include"..\win32\Win32Form.h"
#include<Windows.h>
#include<d3d11.h>
#include<D3DX11.h>
#include<DxErr.h>

namespace catool
{
	namespace dx11
	{
		struct FormDx11Initial
		{

		};
		class FormDx11 :public win32::FormWin32
		{
			ID3D11Device *g_pd3dDevice;                                    //�豸
			ID3D11DeviceContext *g_pImmediateContext;                    //�豸������
			IDXGISwapChain *g_pSwapChain;                                //������

			D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;                //��������
			D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;            //�����ȼ�    
			ID3D11RenderTargetView *g_pRenderTargetView = NULL;                    //Ҫ��������ͼ

		public:
			FormDx11(const FormDx11Initial& init= FormDx11Initial())
				:FormWin32()
			{
			}
			decltype(auto) get_Dx11_device() { return g_pd3dDevice; }
			decltype(auto) get_Dx11_context() { return g_pImmediateContext; }
			decltype(auto) get_swap_chain() { return g_pSwapChain; }

			HRESULT InitDevice();
			void CleanupDevice();
			virtual HRESULT create_window();
			virtual void destory_window();
			virtual void message_process();
			void Render()
			{
				float ClearColor[4] = { 0.5f, 0.1f, 0.2f, 1.0f }; //red,green,blue,alpha
				g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
				g_pSwapChain->Present(0, 0);
			}
		};
	}


}

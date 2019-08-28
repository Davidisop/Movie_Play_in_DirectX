#pragma once
#include <d3d11.h>

namespace David_DX
{

	class David_DX_State
	{
	public:
		static ID3D11DepthStencilState*   David_DSVStateEnableLessEqual;
		static ID3D11DepthStencilState*   David_DSVStateEnableGreater;
		static ID3D11DepthStencilState*   David_DSVStateDetphWriteDisable;

	public:


		static ID3D11RasterizerState*	  David_RSSolid;
		static ID3D11RasterizerState*	  David_RSBackCull_Solid;
		static ID3D11RasterizerState*	  David_RSFrontCull_Solid;
		static ID3D11RasterizerState*	  David_RSNoneCull_Solid;

		static ID3D11RasterizerState*	  David_RSWireFrame;
		static ID3D11RasterizerState*	  David_RSFrontCull_WireFrame;
		static ID3D11RasterizerState*	  David_RSBackCull_WireFrame;
		static ID3D11RasterizerState*	  David_RSNoneCull_WireFrame;

	public:


		static ID3D11BlendState*		  David_BSNoBlend;
		static ID3D11BlendState*		  David_BSAlphaBlend;

	public:
		static ID3D11SamplerState*		  David_SSWrapLinear;
		static ID3D11SamplerState*		  David_SSWrapPoint;

	public:

		static HRESULT Create_States(ID3D11Device* pd3dDevice);
		static bool Release();

	public:
		David_DX_State();
		~David_DX_State();
	};
	

	static void Apply_DS_State(ID3D11DeviceContext* pContext, ID3D11DepthStencilState* pState, UINT iRef = 0x01)
	{
		pContext->OMSetDepthStencilState(pState, iRef);
		//iRef = Reference value to perform against when doing a depth-stencil test. 
	}

	static void Apply_Raterizer_State(ID3D11DeviceContext* pContext, ID3D11RasterizerState* pState)
	{
		pContext->RSSetState(pState);
	}

	static void Apply_Blend_State(ID3D11DeviceContext* pContext, ID3D11BlendState* pState, const float fBlendFactor[] = 0, UINT iMask = 0xffffffff)
	{
		pContext->OMSetBlendState(pState, fBlendFactor, iMask);
		//Array of blend factors, one for each RGBA component.
		//The blend factors modulate values for the pixel shader, render target, or both. 
		//If you pass NULL, the runtime uses or stores a blend factor equal to { 1, 1, 1, 1 }.
	}

	static void Apply_Sampler_State(ID3D11DeviceContext*   pContext, ID3D11SamplerState*     pSamplerState, UINT iSlot = 0, UINT iArray = 1)
	{
		pContext->PSSetSamplers(iSlot, iArray, &pSamplerState);
		//UINT     StartSlot,
		//Number of samplers in the array. Each pipeline stage has a total of 16 sampler slots available (ranges from 0 )
	}
}

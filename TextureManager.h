#pragma once
#include "xStd.h"

class David_Texture
{
public:

	T_STR Da_szName;
	T_STR Da_szPath;
	T_STR Da_merged_path_name;

public: //텍스쳐 로딩으로 리소스뷰 생성

	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11SamplerState*		  m_pSamplerState;
	D3D11_SAMPLER_DESC		  m_SampleDesc;
public:

	HRESULT					Load_Get_SRV(ID3D11Device* pDevice, const TCHAR* str_FilePath);
	bool					Apply_thisSRV_to_PipeLine(ID3D11DeviceContext* pImmedateContext);
	bool				    Release();
	bool					Set_to_MyPath(const TCHAR* pPath);

public:

	David_Texture(void);
	virtual ~David_Texture(void);
};



class David_TextureManager
{
private:
	static David_TextureManager* uniqueInstance_ptr;
public:
	static David_TextureManager* get_Instance(void)
	{
		if (uniqueInstance_ptr == NULL)
		{
			uniqueInstance_ptr = new David_TextureManager;
		}
		return uniqueInstance_ptr;
	}

private:
	ID3D11Device*			m_pd3Device;
	ID3D11DeviceContext*    m_pImmediateContext;

public:

	typedef map<int, David_Texture*>	 Texture_Map;
	typedef Texture_Map::iterator		 Texture_Map_Itor;

	Texture_Map											TMap;
	Texture_Map_Itor									TItor;
	int													m_iCurIndex;

public:
	
	int	ADD(ID3D11Device* pDedvice, const TCHAR* pFileName);
	int ADD(ID3D11Device* pDevice, const TCHAR* pFileName, const TCHAR* szpath);

	David_Texture* const Getptr(int iIndex);
	David_Texture* const Getptr(T_STR strFindName);
	
public:

	bool	Release();
	David_TextureManager();
	virtual ~David_TextureManager();
}; 
#define TextureManager David_TextureManager::get_Instance()

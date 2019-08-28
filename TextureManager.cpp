#include "TextureManager.h"

David_TextureManager* David_TextureManager::uniqueInstance_ptr = nullptr;


HRESULT	David_Texture::Load_Get_SRV(ID3D11Device* pDevice, const TCHAR* str_FileName)//strFilePath에서  str_FileName로 제가 수정했습니다.
{ 
	//파일 경로가 외부에서 주어지면,

	if (str_FileName == NULL) return S_OK;
	HRESULT hr = S_OK;
	TCHAR sz_merged_FileName[MAX_PATH] = { 0, };
	_stprintf_s(sz_merged_FileName, _T("%s%s"), Da_szPath.c_str(), str_FileName);
	// 버퍼에 합쳐 저장하기._stprintf_s(m_csBuffer, L"FPS:%d TIME:%10.4f SPF:%10.4f", m_dwFPS, m_fGameTimer, m_fSecPerFrame);
	
	Da_szName = str_FileName; // 왜 같은 Path를 놔두고?
	Da_merged_path_name = sz_merged_FileName;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));

	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;//좋다.

	hr = D3DX11CreateShaderResourceViewFromFile(pDevice, sz_merged_FileName, &loadInfo, NULL, &m_pTextureSRV, NULL);

	if (FAILED(hr))//hr이 실패하면,
	{
		return hr;
	}
	return hr;
}


bool David_Texture::Apply_thisSRV_to_PipeLine(ID3D11DeviceContext* pImmedateContext)
{
	pImmedateContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	//pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);
	return true;
}

bool David_Texture::Release()
{
	m_pTextureSRV->Release(); m_pTextureSRV = nullptr;
	m_pSamplerState->Release(); m_pSamplerState = nullptr;
	return true;
}

bool David_Texture::Set_to_MyPath(const TCHAR* pPath)
{
	Da_szPath = pPath;
	return true;
}

David_Texture::David_Texture(void)
{
	m_pTextureSRV = nullptr;
	m_pSamplerState = nullptr;
	Da_szPath = _T("");
	Da_merged_path_name = _T("");
}


David_Texture::~David_Texture(void)
{

}


int	David_TextureManager::ADD(ID3D11Device* pDedvice, const TCHAR* pFileName) //외부에서 가져온 파일 이름.
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];



	if (pFileName) 
	{//기존 것과 같은 게 있으면, 그 인덱스 반환.
		_tsplitpath(pFileName, Drive, Dir, FName, Ext); // 이름을 가지고 쪼갠다.
		Ext[4] = 0; //5번째 원소를 0으로 한다.	문자열 버퍼의 비밀(\0 = NULL 문자 삽입)
		
		memset(szFileName, 0, sizeof(TCHAR)*MAX_PATH); //szFileName 메모리바를 0으로 채워넣는다.

		if (!_tcsicmp(Ext, _T(".tga"))) //같으면 0반환.
		{
			_tcscpy(Ext, _T(".bmp"));//문자열덮어쓰기. 그럼 bmp가 들어갈 것이다.
		}
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);
		//버퍼에 합쳐 저장하기.

		for (Texture_Map_Itor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			//TemplateMap  map<int, David_TextureManager*> TMap
			David_Texture *pPoint = (David_Texture*)(*itor).second;//itor에서 second복원했다.

			if (!_tcsicmp(pPoint->Da_szName.c_str(), szFileName)) ////외부에서 가져온 파일 이름. 기존의 이름이 일치하는 지 확인한다.
			{   ////같으면 0반환.
				return (*itor).first; // map 저장되있는 기존의 인덱스 번호 반환한다. 기존에 저장해뒀으니 몇번 사용해라. 이런 것인듯.
			}
		}
	}

	//기존 것과 같은 게 없으면, 새로 만들어 인덱스 반환.

	David_Texture *pdPoint;
	pdPoint = new David_Texture;

	TCHAR szPath[MAX_PATH] = { 0, };
	
	_stprintf_s(szPath, _T("%s%s"), Drive, Dir);// 앞에서 쪼개져 저장되었던 것들을, 버퍼에 합쳐 저장하기.
	pdPoint->Set_to_MyPath(szPath);

	if(FAILED(pdPoint->Load_Get_SRV(pDedvice, szFileName)))
	{
		return 0;//SRV 만드는 거 실패하면, 0반환.
	}
	//SRV만드는 거 성공했으면, MAP에 저장하고, 그 인덱스를 반환합니다.

	TMap[++m_iCurIndex]= pdPoint;

	return m_iCurIndex;
}

int David_TextureManager::ADD(ID3D11Device* pDevice, const TCHAR* pFileName, const TCHAR* szpath)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	if (pFileName)
	{

		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;//다섯번칸에는 문자열 끝 Null문자
		memset(szFileName, 0, sizeof(TCHAR)*MAX_PATH);// szFileName을 일단 다 Null문자로 채웠다

		if (_tcsicmp(Ext, _T(".tga")) == 0)//같으면, 바꿔라.
		{
			_tcscpy(Ext, _T(".dds"));
		}
		_stprintf_s(szFileName, _T("%s,%s"), FName, Ext);// 버퍼에 합쳐 저장하기.

		for (Texture_Map_Itor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			David_Texture* pPoint = (David_Texture*)(*itor).second;
		
			if (!_tcsicmp(pPoint->Da_szName.c_str(), szFileName))
			{
				return (*itor).first;//기존 저장된 것을 검색해서, 같으면, 추가 필요 없으니, 그냥 그 텍스쳐 인덱스를 반환하고, 이 함수 끝!
			}
		}
	}

	David_Texture* pPoint = NULL;
	pPoint = new David_Texture;
	pPoint->Set_to_MyPath(szpath);

	if(FAILED(pPoint->Load_Get_SRV(pDevice, szFileName)))
	{
		return 0; // 
	}
	
	TMap[++m_iCurIndex] = pPoint;//걍 1번부터 텍스쳐 저장된다.
	return m_iCurIndex;
}

David_Texture* const David_TextureManager::Getptr(int iIndex)
{
	Texture_Map_Itor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return nullptr; //원래 end까지 쓰지 않습니다.
	else
	{
		David_Texture* pPoint = (*itor).second;
		return pPoint;
	}
}

David_Texture* const David_TextureManager::Getptr(T_STR strFindName)
{
	for (Texture_Map_Itor itor = TMap.begin(); itor != TMap.end(); itor++)//한바퀴 쫙 돈다
	{
		David_Texture* pPoint = (David_Texture*)(*itor).second;
		if (pPoint->Da_szName == strFindName)
		{
			return(*itor).second;
		}
	}
	//이름 같은 거 못 찾으면,
	return nullptr;

}



bool David_TextureManager::Release()
{
	David_Texture* pPoint;

	for (Texture_Map_Itor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (David_Texture*)(*itor).second;
		pPoint->Release();
	}
	TMap.clear();
	return true;
}


David_TextureManager::David_TextureManager()
{

	m_pd3Device = nullptr;
	m_iCurIndex = 0;
	TMap.clear();

}

David_TextureManager::~David_TextureManager()
{
	David_Texture* pPoint;

	for (Texture_Map_Itor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (David_Texture*)(*itor).second;
		pPoint->Release();
	}
	TMap.clear();
	m_iCurIndex = 0;
}

#include "Sprite_Manager.h"

Sprite_Manager* Sprite_Manager::uniqueInstance_ptr = nullptr;

void	Sprite_Manager::SetDevice(ID3D11Device* pDevice)
{
	m_pd3Device = pDevice;
}

int		Sprite_Manager::Add(ID3D11Device* pDevice,
	TCHAR* pTextureFileName,
	TCHAR* pShaderFileName,
	ID3D11BlendState* m_pBlendState ,
	bool bInstancing)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	if (pTextureFileName) // �̰� �ؽ��ķ� �Ѵٰ�?? ��������Ʈ �̸��� �ƴ϶�?? �� �׷���?
	{
		_tsplitpath_s(pTextureFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR)*MAX_PATH); //�ϴ� szFileName�� 0���� ä���.
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);//// ���ۿ� ���� �����ϱ�

		for (Sprite_TemplateMap_Itor itor = Sprite_Map.begin(); itor != Sprite_Map.end(); itor++)
		{
			Sprite *pPoint = (*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))//���ٸ�,
			{
				return (*itor).first;//�ε��� ��ȯ.
			}
		}
	}

	// �ؽ��� �̸��� �Ѿ�Դµ�, ������ �Ͱ� ���� �� ������

	Sprite *pPoint = nullptr;
	pPoint = new Sprite;
	pPoint->m_szPath = Dir;

	// Sprite Create �� Load���.
	if (FAILED(pPoint->Load(pDevice, pShaderFileName, pTextureFileName, bInstancing, m_pBlendState)))
	{
		//�����ϸ�
		pPoint->Release();
		delete pPoint;
		pPoint = nullptr;
		return false;
	}

	else // Sprite Create �� Load���. �����ϸ�,
	{
		Sprite_Map.insert(make_pair(++Current_Index, pPoint)); //��� ���� ��������Ʈ
		pPoint->m_iIndex = Current_Index;
		return pPoint->m_iIndex;
	}
}
Sprite* const Sprite_Manager::GetPtr(int iIndex)
{
	Sprite_TemplateMap_Itor itor = Sprite_Map.find(iIndex);
	if (itor == Sprite_Map.end())return nullptr;// ��ã����
	else //ã����,
	{
		Sprite* pPoint = (*itor).second;
		return pPoint;
	}
}

bool	Sprite_Manager::Release()
{
	Sprite* pPoint;
	for (Sprite_TemplateMap_Itor itor = Sprite_Map.begin(); itor != Sprite_Map.end(); itor++)
	{
		pPoint = (*itor).second; //��������Ʈ �� ���� ��� ��������Ʈ Release()�Ѵ�.
		pPoint->Release();  // �ٵ� ������ ���� David_Shape���� �������ٴ°�.. ���� ��������Ʈ Ŭ������ �ش��Լ��� ����.
		delete pPoint;
		pPoint = nullptr;
	}
	Sprite_Map.clear();//MAP��ü�� ����.
	Current_Index = 0;
	return true;
}



bool Sprite_Manager::Delete(int iIndex)
{
	Sprite* const pPoint = GetPtr(iIndex);
	if (pPoint != nullptr)
	{
		pPoint->Release();
		Sprite_Map.erase(iIndex);
	}

	return true;
};

Sprite_Manager::Sprite_Manager()
{
	m_pd3Device = nullptr;
	Current_Index = 0;
	Sprite_Map.clear();
}


Sprite_Manager::~Sprite_Manager()
{

	Sprite* pPoint;
	for (Sprite_TemplateMap_Itor itor = Sprite_Map.begin(); itor != Sprite_Map.end(); itor++)
	{
		pPoint = (*itor).second; //��������Ʈ �� ���� ��� ��������Ʈ Release()�Ѵ�.
		pPoint->Release();  // �ٵ� ������ ���� David_Shape���� �������ٴ°�.. ���� ��������Ʈ Ŭ������ �ش��Լ��� ����.
		delete pPoint;
		pPoint = nullptr;
	}
	Sprite_Map.clear();//MAP��ü�� ����.
	Current_Index = 0;

}

#include "Sprite.h"

//#define Sprite_TemplateMap  map<int, Sprite*>
//#define Sprite_TemplateMap_Itor Sprite_TemplateMap::iterator

class Sprite_Manager
{
private:
	static Sprite_Manager* uniqueInstance_ptr;
public:
	static Sprite_Manager* get_Instance(void)
	{
		if (uniqueInstance_ptr == NULL)
		{
			uniqueInstance_ptr = new Sprite_Manager;
		}
		return uniqueInstance_ptr;
	}

private:

	ID3D11Device* m_pd3Device;

public:
	
	typedef map<int, Sprite*> Sprite_TemplateMap;
	typedef Sprite_TemplateMap::iterator Sprite_TemplateMap_Itor;

	Sprite_TemplateMap        Sprite_Map;
	Sprite_TemplateMap_Itor	  Sprite_Map_Itor;

	int	Current_Index;

public:
	void		SetDevice(ID3D11Device* pDevice);
	int			Add(ID3D11Device* pDevice,
					TCHAR* pTextureFileName,
					TCHAR* pShaderFileName,
					ID3D11BlendState* m_pBlendState = nullptr,
					bool bInstancing = false);
	Sprite* const GetPtr(int iIndex);
	bool	Release();
	bool	Delete(int iIndex);
public:
	Sprite_Manager();
	virtual ~Sprite_Manager();
};
#define SpriteManager Sprite_Manager::get_Instance()


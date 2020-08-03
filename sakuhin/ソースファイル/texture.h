#pragma once

#include "main.h"
#include "renderer.h"
#include <vector>

#define BIT32_R8G8B8A8	(32)

struct aiTexel;

class CTexture
{

public:

	void Load( const char *FileName );
	void Unload();

	void LoadFromMemory(aiTexel* data, unsigned int width);

	std::vector < ID3D11ShaderResourceView*> GetShaderResourceView(){ return m_ShaderResourceView; }

private:

	ID3D11Texture2D*	m_Texture;



	std::vector<ID3D11ShaderResourceView*>	m_ShaderResourceView;

	

	

	
};
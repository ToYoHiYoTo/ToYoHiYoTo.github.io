
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "main.h"
#include "renderer.h"
#include "texture.h"



void CTexture::Load(const char *FileName)
{

	unsigned char	header[18];
	unsigned char	*image;
	unsigned int	width_, height_;
	unsigned char	depth;
	unsigned int	bpp;
	unsigned int	size;

	ID3D11ShaderResourceView* ShaderResourceView;


	// ヘッダ読み込み
	FILE* file;
	file = fopen(FileName, "rb");
	assert(file);

	fread(header, sizeof(header), 1, file);


	// 画像サイズ取得
	width_ = header[13] * 256 + header[12];
	height_ = header[15] * 256 + header[14];
	depth = header[16];

	if (depth == 32)
		bpp = 4;
	else if (depth == 24)
		bpp = 3;
	else
		bpp = 0;

	if (bpp != 4) {
		assert(false);
	}

	size = width_ * height_ * bpp;

	// メモリ確保
	image = (unsigned char*)new unsigned char[size];

	// 画像読み込み
	fread(image, size, 1, file);

	// R<->B
	for (unsigned int y = 0; y < height_; y++)
	{
		for (unsigned int x = 0; x < width_; x++)
		{
			unsigned char c;
			c = image[(y * width_ + x) * bpp + 0];
			image[(y * width_ + x) * bpp + 0] = image[(y * width_ + x) * bpp + 2];
			image[(y * width_ + x) * bpp + 2] = c;
		}
	}

	fclose(file);


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width_;
	desc.Height = height_;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image;
	initData.SysMemPitch = width_ * 4;
	initData.SysMemSlicePitch = size;

	auto hr = CRenderer::GetDevice()->CreateTexture2D(&desc, &initData, &m_Texture);
	if (FAILED(hr)) {
		assert(false);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = CRenderer::GetDevice()->CreateShaderResourceView(m_Texture, &SRVDesc, &ShaderResourceView);
	if (FAILED(hr))
	{
		assert(false);
	}

	m_ShaderResourceView.push_back(ShaderResourceView);

	delete image;
}


void CTexture::Unload()
{
	m_Texture->Release();
	for (auto tex : m_ShaderResourceView)
	{
		tex->Release();
	}

}

/************************************/
// メモリにある圧縮データの読み込み //
/************************************/
void CTexture::LoadFromMemory(aiTexel* data, unsigned int width)
{
	/*
	unsigned char* pixel;
	unsigned int	size;
	int	bpp;

	int		width_;
	int		height_;
	ID3D11Texture2D* image_;
	ID3D11ShaderResourceView* ShaderResourceView_;


	pixel = stbi_load_from_memory((unsigned char*)data, width, &width_, &height_, &bpp, 0);

	size = width_ * height_ * BIT32_R8G8B8A8;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width_;
	desc.Height = height_;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pixel;
	initData.SysMemPitch = width_ * BIT32_R8G8B8A8;
	initData.SysMemSlicePitch = size;

	auto hr = CRenderer::GetDevice()->CreateTexture2D(&desc, &initData, &image_);
	if (FAILED(hr))	assert(false);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = CRenderer::GetDevice()->CreateShaderResourceView(image_, &SRVDesc, &ShaderResourceView_);
	if (FAILED(hr))	assert(false);

	m_ShaderResourceView.push_back(ShaderResourceView_);

	stbi_image_free(pixel);*/


	unsigned char* pixel;
	unsigned int size;
	int width_, height_;
	int bpp;

	ID3D11ShaderResourceView* shaderResourceView_;

	pixel = stbi_load_from_memory((unsigned char*)data, width, &width_, &height_, &bpp, 0);

	size = width_ * height_ * BIT32_R8G8B8A8;

	pixel = (unsigned char*) new unsigned char[size];

	assert(pixel);

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width_;
	desc.Height = height_;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pixel;
	initData.SysMemPitch = width_ * BIT32_R8G8B8A8;
	initData.SysMemSlicePitch = size;

	auto hr = CRenderer::GetDevice()->CreateTexture2D(&desc, &initData, &m_Texture);
	if (FAILED(hr)) assert(false);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = CRenderer::GetDevice()->CreateShaderResourceView(m_Texture, &SRVDesc, &shaderResourceView_);
	if (FAILED(hr)) assert(false);

	m_ShaderResourceView.push_back(shaderResourceView_);

	stbi_image_free(pixel);
}

#include "pch.h"
#include "ComputeImage.h"
#include "Utility.h"

Aether::ComputeImage::ComputeImage(const std::wstring& filePath, const ComputeDevice& device)
{
	View = nullptr;
	Type = ComputeImageType::Input;

	auto image = Utility::LoadImageFile(filePath);

	Metadata = image->GetMetadata();

	auto result = DirectX::CreateShaderResourceView(
		device.Device,
		image->GetImages(),
		image->GetImageCount(),
		Metadata,
		(ID3D11ShaderResourceView**)&View);

	if (FAILED(result))
	{
		View = nullptr;
		// TODO: Logging
		throw std::exception("Failed to create shader resource view.");
	}
}

Aether::ComputeImage::ComputeImage(UINT width, UINT height, UINT arraySize, DXGI_FORMAT format, const ComputeDevice& device)
{
	// Create output image
	DirectX::TexMetadata metadata 
	{
		.width = width,
		.height = height,
		.depth = 1,
		.arraySize = arraySize,
		.mipLevels = 1,
		.miscFlags = 0,
		.miscFlags2 = 0,
		.format = format,
		.dimension = DirectX::TEX_DIMENSION_TEXTURE2D
	};

	Create(metadata, device);
}

Aether::ComputeImage::ComputeImage(const DirectX::TexMetadata& metadata, const ComputeDevice& device)
{
	Create(metadata, device);
}

void Aether::ComputeImage::Create(const DirectX::TexMetadata& metadata, const ComputeDevice& device)
{
	View = nullptr;
	Type = ComputeImageType::Output;
	Metadata = metadata;

	D3D11_TEXTURE2D_DESC desc{};

	desc.Width = static_cast<UINT>(metadata.width);
	desc.Height = static_cast<UINT>(metadata.height);
	desc.MipLevels = static_cast<UINT>(metadata.mipLevels);
	desc.ArraySize = static_cast<UINT>(metadata.arraySize);
	desc.Format = metadata.format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = 0;

	ComPtr<ID3D11Resource> resource;
	auto result = device.Device->CreateTexture2D(&desc, NULL, (ID3D11Texture2D**)resource.GetAddressOf());

	if (FAILED(result))
	{
		return;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc{};
	viewDesc.Format = Metadata.format;
	viewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;
	// If we're making an output image from this, we'll assume that
	// we're
	Utility::ValidateHResult(
		device.Device->CreateUnorderedAccessView(
			resource.Get(),
			&viewDesc,
			(ID3D11UnorderedAccessView**)&View),
		"Failed to create UAV.");
}

void Aether::ComputeImage::Load(const std::wstring& filePath, const ComputeDevice& device)
{
	if (View != nullptr)
	{
		View->Release();
		View = nullptr;
	}

	Type = ComputeImageType::Input;

	auto image = Utility::LoadImageFile(filePath);

	Metadata = image->GetMetadata();

	auto result = DirectX::CreateShaderResourceView(
		device.Device,
		image->GetImages(),
		image->GetImageCount(),
		Metadata,
		(ID3D11ShaderResourceView**)&View);

	if (FAILED(result))
	{
		View = nullptr;
		// TODO: Logging
		throw std::exception("Failed to create shader resource view.");
	}
}

void Aether::ComputeImage::Save(const std::wstring& filePath, const ComputeDevice& device)
{
	auto result = std::make_unique<DirectX::ScratchImage>();

	ComPtr<ID3D11Resource> resource;
	View->GetResource(resource.GetAddressOf());

	Utility::ValidateHResult(
		DirectX::CaptureTexture(device.Device, device.Context, resource.Get(), *result),
		"Failed to capture texture.");

	Utility::SaveImageFile(filePath, result.get());
}

void Aether::ComputeImage::Bind(const std::string& name, const ComputeDevice& device, const ComputeShader& shader)
{
	if (Type == ComputeImageType::Input)
	{
		device.Context->CSSetShaderResources(
			shader.GetInputTextureIndex(name),
			1,
			(ID3D11ShaderResourceView**)&View);
	}
	else
	{
		device.Context->CSSetUnorderedAccessViews(
			shader.GetOutputTextureIndex(name),
			1,
			(ID3D11UnorderedAccessView**)&View,
			nullptr);
	}
}

Aether::ComputeImage::~ComputeImage()
{
	if (View != nullptr)
	{
		
		View->Release();
	}
}

#pragma once
#include "ComputeShader.h"
#include "ComputeDevice.h"

namespace Aether
{
	enum class ComputeImageType
	{
		Input,

		Output,
	};

	class ComputeImage
	{
	public:
		// The view stored on the device.
		ID3D11View* View;
		// The type, defining if it is a UAV or input shader resource.
		ComputeImageType Type{};
		// The metadata of the image stored on the device.
		DirectX::TexMetadata Metadata{};

		// Creates an input compute image from the provided file.
		ComputeImage(const std::wstring& filePath, const ComputeDevice& device);
		// Creates an output compute image with the provided information.
		ComputeImage(UINT width, UINT height, UINT arraySize, DXGI_FORMAT format, const ComputeDevice& device);
		// Creates an output compute image with the provided information.
		ComputeImage(const DirectX::TexMetadata& metadata, const ComputeDevice& device);

		// Creates from the provided metadata.
		void Create(const DirectX::TexMetadata& metadata, const ComputeDevice& device);
		// Loads a compute image from the provided file.
		void Load(const std::wstring& filePath, const ComputeDevice& device);
		// Saves the compute image to the provided file.
		void Save(const std::wstring& filePath, const ComputeDevice& device);
		// Binds the texture to the provided shader.
		void Bind(const std::string& name, const ComputeDevice& device, const ComputeShader& shader);

		~ComputeImage();
	};
}


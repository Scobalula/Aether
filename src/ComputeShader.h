#pragma once

namespace Aether
{
	class ComputeShader
	{
	public:
		// The actual shader on the GPU.
		ID3D11ComputeShader* Shader;
		// Shader input textures
		std::map<std::string, UINT> InputTextures;
		// Shader output textures
		std::map<std::string, UINT> OutputTextures;
		// Shader samplers
		std::map<std::string, UINT> Samplers;
		// X Threads
		UINT XThreads;
		// Y Threads
		UINT YThreads;
		// Z Threads
		UINT ZThreads;

		ComputeShader() :
			Shader(nullptr),
			XThreads(0),
			YThreads(0),
			ZThreads(0) {}

		// Gets the index of the resource with the given name.
		UINT GetInputTextureIndex(const std::string& name) const;
		// Gets the index of the resource with the given name.
		UINT GetOutputTextureIndex(const std::string& name) const;
		// Gets the index of the resource with the given name.
		UINT GetSamplerIndex(const std::string& name) const;

		~ComputeShader();
	};
}

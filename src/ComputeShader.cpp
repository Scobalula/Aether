#include "pch.h"
#include "ComputeShader.h"

UINT Aether::ComputeShader::GetInputTextureIndex(const std::string& name) const
{
	auto v = InputTextures.find(name);

	if (v == InputTextures.end())
	{
		std::string msg = "Failed to find texture in shader: " + name;
		throw std::exception(msg.c_str());
	}

	return v->second;
}

UINT Aether::ComputeShader::GetOutputTextureIndex(const std::string& name) const
{
	auto v = OutputTextures.find(name);

	if (v == OutputTextures.end())
	{
		std::string msg = "Failed to find rw texture in shader: " + name;
		throw std::exception(msg.c_str());
	}

	return v->second;
}

UINT Aether::ComputeShader::GetSamplerIndex(const std::string& name) const
{
	auto v = Samplers.find(name);

	if (v == Samplers.end())
	{
		std::string msg = "Failed to find sampler in shader: " + name;
		throw std::exception(msg.c_str());
	}

	return v->second;
}

Aether::ComputeShader::~ComputeShader()
{
	if (Shader != nullptr)
	{
		Shader->Release();
		InputTextures.clear();
		OutputTextures.clear();
		Samplers.clear();
	}
}

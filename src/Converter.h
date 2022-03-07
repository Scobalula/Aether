#pragma once
#include "ComputeDevice.h"
#include "ComputeShader.h"

namespace Aether
{
	class Converter
	{
	public:
		// The shader that performs the conversion.
		ComputeShader Shader;

		virtual const char* GetShorthandName() = 0;

		virtual void Run(const argh::parser& cmdl, const ComputeDevice& device) = 0;

		virtual std::filesystem::path GetShaderFilePath();

		virtual void Initialize(const ComputeDevice& device);
	};
}


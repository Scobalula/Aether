#pragma once
#include "ComputeShader.h"
#include "Converter.h"

namespace Aether
{
	class CubeArray2Equ : public Converter
	{
	public:
		const char* GetShorthandName();

		void Run(const argh::parser& cmdl, const ComputeDevice& device);
	};
}

#pragma once
#include "Converter.h"

namespace Aether
{
	class ConverterFactory
	{
	public:
		std::vector<std::unique_ptr<Converter>> Converters;

		Converter* FindConverter(const char* name);
	};
}


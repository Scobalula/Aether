#include "pch.h"
#include "ConverterFactory.h"

Aether::Converter* Aether::ConverterFactory::FindConverter(const char* name)
{
	for (auto& converter : Converters)
	{
		if (_stricmp(converter->GetShorthandName(), name) == 0)
		{
			return converter.get();
		}
	}

	return nullptr;
}

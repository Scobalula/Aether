#pragma once

#define VERBOSE(v) if(verbose) std::cout << v << std::endl;

namespace Aether
{
	namespace Utility
	{
		// Loads the provided image.
		const std::unique_ptr<DirectX::ScratchImage> LoadImageFile(const std::wstring& filePath);
		// Saves the provided image.
		void SaveImageFile(const std::wstring& filePath, const DirectX::ScratchImage* image);

		// Validates the HResult and dumps to the global log file
		// if provided, an exception will be thrown with the given message
		void ValidateHResult(HRESULT result, const char* exceptionMessage);

		// Validates the HResult and dumps to the global log file
		// if provided, an exception will be thrown with the given message
		// The param preException will be called before the exception is thrown
		void ValidateHResult(HRESULT result, const char* exceptionMessage, std::function<void(HRESULT)> preException);

		// Gets the name of the provided adapter.
		const std::wstring GetAdapterName(IDXGIAdapter* adapter);

		// Gets the arg with the given name, if not found throws an exception.
		const std::string GetArg(const argh::parser& cmdl, const std::string& name);

		// Gets the arg with the given name, if not found throws an exception.
		const std::wstring GetArgW(const argh::parser& cmdl, const std::string& name);

		// Converts the provided string to a wide string.
		const std::wstring ToWideString(const std::string& value);

		const std::filesystem::path GetExecutablePath();
	}
}
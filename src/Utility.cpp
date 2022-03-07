#include "pch.h"
#include "Utility.h"

// EXR Support
#include "DirectXTexEXR.h"

const std::unique_ptr<DirectX::ScratchImage> Aether::Utility::LoadImageFile(const std::wstring& filePath)
{
    HRESULT hr{};
    DirectX::TexMetadata meta{};

    auto result = std::make_unique<DirectX::ScratchImage>();
    auto asPath = std::filesystem::path(filePath);
    auto ext = asPath.extension();

    if (_wcsicmp(ext.c_str(), L".dds") == 0)
    {
        auto hr = DirectX::LoadFromDDSFile(
            filePath.c_str(),
            DirectX::DDS_FLAGS_ALLOW_LARGE_FILES,
            &meta,
            *result);
    }
    else if (_wcsicmp(ext.c_str(), L".tga") == 0)
    {
        hr = DirectX::LoadFromTGAFile(
            filePath.c_str(),
            &meta,
            *result);
    }
    else if (_wcsicmp(ext.c_str(), L".hdr") == 0)
    {
        hr = DirectX::LoadFromHDRFile(
            filePath.c_str(),
            &meta,
            *result);
    }
    else if (_wcsicmp(ext.c_str(), L".exr") == 0)
    {
        hr = DirectX::LoadFromEXRFile(
            filePath.c_str(),
            &meta,
            *result);
    }
    else 
    {
        hr = DirectX::LoadFromWICFile(
            filePath.c_str(),
            DirectX::WIC_FLAGS_NONE,
            &meta,
            *result);
    }

    if (FAILED(hr))
    {
        // TODO: Logging
        throw std::exception("Failed to load image.");
    }

    return result;
}

void Aether::Utility::SaveImageFile(const std::wstring& filePath, const DirectX::ScratchImage* image)
{
    auto asPath = std::filesystem::path(filePath);
    auto ext = asPath.extension();

    if (_wcsicmp(ext.c_str(), L".dds") == 0)
    {
        auto hr = DirectX::SaveToDDSFile(
            image->GetImages(),
            image->GetImageCount(),
            image->GetMetadata(),
            DirectX::DDS_FLAGS_ALLOW_LARGE_FILES,
            filePath.c_str());
    }
    else if (_wcsicmp(ext.c_str(), L".exr") == 0)
    {
        auto hr = DirectX::SaveToEXRFile(
            *image->GetImage(0, 0, 0),
            filePath.c_str());
    }
}

void Aether::Utility::ValidateHResult(HRESULT result, const char* exceptionMessage)
{
}

void Aether::Utility::ValidateHResult(HRESULT result, const char* exceptionMessage, std::function<void(HRESULT)> preException)
{
    if (FAILED(result))
    {
        // TODO: Logging
        preException(result);
        throw std::exception(exceptionMessage);
    }
}

const std::wstring Aether::Utility::GetAdapterName(IDXGIAdapter* adapter)
{
    DXGI_ADAPTER_DESC desc{};

    if (FAILED(adapter->GetDesc(&desc)))
    {
        // TODO: Logging
        throw std::exception("Failed to get adapter description.");
    }

    return std::wstring(desc.Description);
}

const std::string Aether::Utility::GetArg(const argh::parser& cmdl, const std::string& name)
{
    auto potentialVal = cmdl(name);

    if (!potentialVal)
    {
        std::string msg = "Failed to get argument: " + name;
        throw std::exception(msg.c_str());
    }

    return potentialVal.str();
}

const std::wstring Aether::Utility::GetArgW(const argh::parser& cmdl, const std::string& name)
{
    auto potentialVal = cmdl(name);

    if (!potentialVal)
    {
        std::string msg = "Failed to get argument: " + name;
        throw std::exception(msg.c_str());
    }

    return ToWideString(potentialVal.str());
}

const std::wstring Aether::Utility::ToWideString(const std::string& value)
{
    int sizeOf = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, NULL, 0);
    auto wideBuf = std::make_unique<wchar_t[]>(sizeOf);
    MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, wideBuf.get(), sizeOf);

    return wideBuf.get();
}

const std::filesystem::path Aether::Utility::GetExecutablePath()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    // Check if our file path is too small
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        return "";

    return path;
}

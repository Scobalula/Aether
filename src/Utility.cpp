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
        hr = DirectX::LoadFromDDSFile(
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

    ValidateHResult(hr, "Failed to load image.");

    return result;
}

void Aether::Utility::SaveImageFile(const std::wstring& filePath, const DirectX::ScratchImage* image)
{
    auto asPath = std::filesystem::path(filePath);
    auto directory = asPath.parent_path();
    auto ext = asPath.extension();
    HRESULT hr{};

    std::filesystem::create_directories(directory);

    if (_wcsicmp(ext.c_str(), L".dds") == 0)
    {
        hr = DirectX::SaveToDDSFile(
            image->GetImages(),
            image->GetImageCount(),
            image->GetMetadata(),
            DirectX::DDS_FLAGS_ALLOW_LARGE_FILES,
            filePath.c_str());
    }
    else if (_wcsicmp(ext.c_str(), L".tga") == 0)
    {
        hr = DirectX::SaveToTGAFile(
            *image->GetImage(0, 0, 0),
            DirectX::TGA_FLAGS_NONE,
            filePath.c_str());
    }
    else if (_wcsicmp(ext.c_str(), L".hdr") == 0)
    {
        hr = DirectX::SaveToHDRFile(
            *image->GetImage(0, 0, 0),
            filePath.c_str());
    }
    else if (_wcsicmp(ext.c_str(), L".exr") == 0)
    {
        auto hr = DirectX::SaveToEXRFile(
            *image->GetImage(0, 0, 0),
            filePath.c_str());
    }
    else if (_wcsicmp(ext.c_str(), L".png") == 0)
    {
        hr = DirectX::SaveToWICFile(
            image->GetImages(),
            image->GetImageCount(),
            DirectX::WIC_FLAGS::WIC_FLAGS_NONE,
            DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG),
            filePath.c_str());
    }
    else if (_wcsicmp(ext.c_str(), L".jpg") == 0)
    {
        hr = DirectX::SaveToWICFile(
            image->GetImages(),
            image->GetImageCount(),
            DirectX::WIC_FLAGS::WIC_FLAGS_NONE,
            DirectX::GetWICCodec(DirectX::WIC_CODEC_JPEG),
            filePath.c_str(), nullptr,
            [&](IPropertyBag2* props)
            {
                PROPBAG2 options = {};
                VARIANT varValues = {};
                options.pstrName = const_cast<wchar_t*>(L"ImageQuality");
                varValues.vt = VT_R4;
                varValues.fltVal = 1.f;
                std::ignore = props->Write(1, &options, &varValues);
            });
    }
    else if (_wcsicmp(ext.c_str(), L".tiff") == 0 || _wcsicmp(ext.c_str(), L".tif") == 0)
    {
        hr = DirectX::SaveToWICFile(
            image->GetImages(),
            image->GetImageCount(),
            DirectX::WIC_FLAGS::WIC_FLAGS_NONE,
            DirectX::GetWICCodec(DirectX::WIC_CODEC_TIFF),
            filePath.c_str(), nullptr,
            [&](IPropertyBag2* props)
            {
                PROPBAG2 options = {};
                VARIANT varValues = {};
                options.pstrName = const_cast<wchar_t*>(L"TiffCompressionMethod");
                varValues.vt = VT_UI1;
                varValues.bVal = 0x00000001; // WICTiffCompressionNone
                (void)props->Write(1, &options, &varValues);
            });
    }

    ValidateHResult(hr, "Failed to save image.");
}

void Aether::Utility::ValidateHResult(HRESULT result, const char* exceptionMessage)
{
    if (FAILED(result))
    {
        throw std::exception(exceptionMessage);
    }
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

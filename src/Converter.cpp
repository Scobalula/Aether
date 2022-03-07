#include "pch.h"
#include "Utility.h"
#include "Converter.h"

std::filesystem::path Aether::Converter::GetShaderFilePath()
{
    auto filePath = Utility::GetExecutablePath().parent_path();

    filePath /= L"Shaders";
    filePath /= GetShorthandName();
    filePath.replace_extension(".hlsl");

    return filePath;
}

void Aether::Converter::Initialize(const ComputeDevice& device)
{
    ComPtr<ID3DBlob> CSBlob;
    ComPtr<ID3DBlob> ErrBlob;

    auto filePath = GetShaderFilePath();

    D3DCompileFromFile(
        filePath.c_str(),
        NULL,
        NULL,
        "ConverterMain",
        "cs_5_0",
        0,
        0,
        CSBlob.GetAddressOf(),
        ErrBlob.GetAddressOf());


    Utility::ValidateHResult(D3DCompileFromFile(
        filePath.c_str(),
        NULL,
        NULL,
        "ConverterMain",
        "cs_5_0",
        0,
        0,
        CSBlob.GetAddressOf(),
        ErrBlob.GetAddressOf()),
        "Failed to compile compute shader.", [&ErrBlob]()
        {
            std::cout << (char*)ErrBlob->GetBufferPointer() << "\n";
        });

    // We need reflection to dynamically aquire inputs/outputs
    ComPtr<ID3D11ShaderReflection> reflection;
    Utility::ValidateHResult(D3DReflect(
        CSBlob->GetBufferPointer(),
        CSBlob->GetBufferSize(),
        __uuidof(ID3D11ShaderReflection),
        reinterpret_cast<void**>(reflection.GetAddressOf())), 
        "Failed to aquire shader reflection.");

    reflection->GetThreadGroupSize(
        &Shader.XThreads,
        &Shader.YThreads,
        &Shader.ZThreads);

    D3D11_SHADER_DESC rDesc{};
    D3D11_SHADER_INPUT_BIND_DESC bDesc{};

    Utility::ValidateHResult(
        reflection->GetDesc(&rDesc),
        "Failed to get shader reflection description.");

    for (UINT i = 0; i < rDesc.BoundResources; i++)
    {
        Utility::ValidateHResult(
            reflection->GetResourceBindingDesc(i, &bDesc),
            "Failed to get resource binding description.");

        switch (bDesc.Type)
        {
        case D3D_SIT_TEXTURE:
            Shader.InputTextures[bDesc.Name] = bDesc.BindPoint;
            break;
        case D3D_SIT_SAMPLER:
            Shader.Samplers[bDesc.Name] = bDesc.BindPoint;
            break;
        case D3D_SIT_UAV_RWTYPED:
            Shader.OutputTextures[bDesc.Name] = bDesc.BindPoint;
            break;
        }
    }

    // Finally let's create the compute shader.
    Utility::ValidateHResult(device.Device->CreateComputeShader(
        CSBlob->GetBufferPointer(),
        CSBlob->GetBufferSize(),
        0,
        &Shader.Shader),
        "Failed to create compute shader.");
}

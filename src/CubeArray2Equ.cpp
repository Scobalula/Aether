#include "pch.h"
#include "CubeArray2Equ.h"
#include "Utility.h"
#include "ComputeImage.h"

const char* Aether::CubeArray2Equ::GetShorthandName()
{
    return "cubearray2equ";
}

void Aether::CubeArray2Equ::Run(const argh::parser& cmdl, const ComputeDevice& device)
{
    device.Context->CSSetShader(Shader.Shader, nullptr, 0);

    auto inputPath = Utility::GetArgW(cmdl, "input");
    auto outputPath = Utility::GetArgW(cmdl, "output");

    ComputeImage inputImage(inputPath, device);
    ComputeImage outputImage(6144, 3072, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, device);

    inputImage.Bind("input_texture", device, Shader);
    outputImage.Bind("output_texture", device, Shader);

    // TODO: Move to a sampler class
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ComPtr<ID3D11SamplerState> samplerState;
    Utility::ValidateHResult(
        device.Device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf()),
        "Failed to create sampler state.");

    device.Context->CSSetSamplers(Shader.GetSamplerIndex("input_sampler"), 1, samplerState.GetAddressOf());
    device.Context->Dispatch(
        (UINT)outputImage.Metadata.width,
        (UINT)outputImage.Metadata.height, 1);

    outputImage.Save(outputPath, device);
}

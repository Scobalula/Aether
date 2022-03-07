// Utility
#include "pch.h"
#include "Utility.h"
// Compute classes
#include "ComputeDeviceFactory.h"
#include "ComputeDevice.h"
#include "ComputeShader.h"
#include "ComputeImage.h"
// Convert classes
#include "Converter.h"
#include "ConverterFactory.h"
// Converters
#include "CubeArray2Equ.h"
#include "HemiOcta2Equ.h"

void Run(const argh::parser& cmdl)
{
    // Start with the factory for adapters, etc.
    // as if this fails nothing else can work,
    // kinda like Harry's fx pack.
    Aether::ComputeDeviceFactory factory(true);
    Aether::ConverterFactory convertFactory;

    // Add supported modes
    convertFactory.Converters.push_back(std::make_unique<Aether::CubeArray2Equ>());

    // List the adapters available. If the user wants to select
    // their own. If they want to select their own, this will show
    // them what's avaiable.
    if (cmdl["listadapter"])
    {
        DXGI_ADAPTER_DESC desc{};

        std::cout << "| Listing adapters: " << std::endl;

        for (auto& adapter : factory.Adapters)
        {
            if (FAILED(adapter->GetDesc(&desc)))
                continue;
            std::wcout << L"| Found adapter: " << desc.Description << std::endl;
        }
    }

    Aether::ComputeDevice device;

    // If we have an adapter name, use that instead.
    auto potentialAdapterName = cmdl("adapter");

    if (potentialAdapterName)
    {
        auto name = potentialAdapterName.str();
        factory.InitializeDevice(device, name.c_str());
    }
    else
    {
        factory.InitializeDevice(device, (wchar_t*)nullptr);
    }

    auto modeName = Aether::Utility::GetArg(cmdl, "mode");

    std::wcout << L"| Using adapter: " << device.GetAdapterName() << L"\n";
    std::cout << "| Using converter: " << modeName << "\n";

    auto converter = convertFactory.FindConverter(modeName.c_str());

    std::cout << "| Executing converter..." << std::endl;

    converter->Initialize(device);
    converter->Run(cmdl, device);

    std::cout << "| Successfully ran converter." << std::endl;
}

int main(int, char* argv[])
{
    std::cout << "| -----------------------------------" << std::endl;
    std::cout << "| Aether - Environment Map Converter" << std::endl;
    std::cout << "| Developed by Scobalula" << std::endl;
    std::cout << "| Version 1.0.0.0" << std::endl;
    std::cout << "| -----------------------------------" << std::endl;

    argh::parser cmdl(argv);

    try
    {
        Run(cmdl);
    }
    catch (const std::exception& ex)
    {
        std::cout << "| ERROR: " << ex.what() << std::endl;
        std::cout << "| Check Github for usage info and help." << std::endl;
    }

    if (!cmdl["close"])
    {
        std::cout << "| Execution complete, press Enter to exit." << std::endl;
        std::cin.get();
    }
}
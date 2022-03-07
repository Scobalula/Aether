// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#pragma warning(push)
#pragma warning(disable : 4244 4996 26812 26451 26495)
// DirectX 11
#include <d3d11_1.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

// OpenEXR
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfIO.h>

// Argh
#include  "../external/argh/argh.h"

#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <string>
#include <filesystem>
#include <memory>
#include <iostream>

// WRL
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#pragma warning(pop)
#endif //PCH_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <iostream>

Microsoft::WRL::ComPtr<ID3DBlob> CompileShaderFromFile(const std::wstring& path, LPCSTR entryPoint, LPCSTR target)
{
    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    Microsoft::WRL::ComPtr<ID3DBlob> err;
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                    entryPoint, target, flags, 0, &blob, &err);
    if (FAILED(hr))
    {
        if (err)
        {
            std::cerr << "Shader compile error (" << std::string(entryPoint) << "):\n"
                      << static_cast<const char*>(err->GetBufferPointer()) << std::endl;
        }
        else
        {
            std::cerr << "D3DCompileFromFile failed, hr=0x" << std::hex << hr << std::dec << std::endl;
        }
        return nullptr;
    }
    return blob;
}
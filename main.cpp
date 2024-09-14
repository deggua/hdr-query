#include <cstdio>

#include <dxgi1_6.h>
#include <windows.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "user32.lib")

static void DumpOutputInfo(const DXGI_OUTPUT_DESC1& desc, UINT index)
{
    MONITORINFOEX monitor_info;
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfoA(desc.Monitor, &monitor_info);

    printf("Output Index: %u\n", index);
    printf("    Device Name: %S\n", desc.DeviceName);
    printf("    Monitor Name: %s\n", monitor_info.szDevice);
    printf(
        "    Resolution: %lu,%lu\n",
        monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
        monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top);
    printf("    Bits per color: %u\n", desc.BitsPerColor);
    printf(
        "    Color Space: %s\n",
        desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 ? "HDR" : "SDR");
    printf("    Min Luminance: %f\n", desc.MinLuminance);
    printf("    Max Luminance: %f\n", desc.MaxLuminance);
    printf("    Max Fullframe Luminance: %f\n", desc.MaxFullFrameLuminance);
    printf("\n");
}

int main(int argc, char** argv)
{
    IDXGIFactory2* factory2;
    CreateDXGIFactory2(0, IID_PPV_ARGS(&factory2));

    IDXGIFactory6* factory6;
    factory2->QueryInterface(IID_PPV_ARGS(&factory6));

    IDXGIAdapter1* adapter1;
    for (UINT ii = 0; factory6->EnumAdapters1(ii, &adapter1) != DXGI_ERROR_NOT_FOUND; ii++) {
        IDXGIOutput* output;
        for (UINT jj = 0; adapter1->EnumOutputs(jj, &output) != DXGI_ERROR_NOT_FOUND; jj++) {
            IDXGIOutput6* output6;
            output->QueryInterface(IID_PPV_ARGS(&output6));

            DXGI_OUTPUT_DESC1 desc1;
            output6->GetDesc1(&desc1);

            DumpOutputInfo(desc1, jj);
        }
    }

    printf("\nPress any key to exit...\n");
    getchar();
}

#pragma once

#ifndef basewind
#define basewind

#include "EpicEngine.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void FatalError(const char* msg, ...);

class MainWindow
{
public:

    MainWindow() : g_pd3dDevice(NULL), g_pd3dDeviceContext(NULL), g_pSwapChain(NULL), g_mainRenderTargetView(NULL), hwnd(NULL), initialized(FALSE), wc() {}
    ~MainWindow() {}
  //  WNDPROC wndProc;

    bool InitializeWindow(const char* p_szTitle, HINSTANCE hInst, WNDPROC _wndProc);
    void ExitApplication();
    void KillWindow();
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void CleanupRenderTarget();
    void CreateRenderTarget();
    void CleanupDeviceD3D();
    HWND GetWindowHandle() { return hwnd; }
    void AddRenderFunction(std::function<void()> fnc);

    void SetMenuStyle();

    AppWindow window;
    
    bool Render(ImGuiIO& io, MSG& msg);
    WNDCLASSEXA wc;

//private:
     ID3D11Device* g_pd3dDevice = NULL;
     ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
     IDXGISwapChain* g_pSwapChain = NULL;
     ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
    
//protected:
     HWND hwnd;
     std::string szTitle;
     std::vector<std::function<void()>> RenderFunctions{};

     bool initialized;
     //ImVec2 Size;
     //ImVec2 Pos;
     //bool active; //if false the application quits
    
};

inline MainWindow hWnd_main;

#endif
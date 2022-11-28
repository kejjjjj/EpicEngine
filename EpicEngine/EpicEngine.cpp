// EpicEngine.cpp : Defines the entry point for the application.
//


#include "EpicEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    FILE* _con;
    AllocConsole();
    freopen_s(&_con, "CONOUT$", "w", stdout);

    std::cout << "requesting to inject\n";

#if _WIN64 
    const char* szTitle = "Epic Engine 1.0 (x64)";
#else
    const char* szTitle = "Epic Engine 1.0 (x86)";
#endif


    hWnd_main.initialized = hWnd_main.InitializeWindow(szTitle, hInstance, hWnd_main.WndProc);

    if (!hWnd_main.initialized) {
        FatalError("hWnd_main.InitializeWindow(\"%s\", hInstance, hWnd_main.WndProc) == false", szTitle);
        return 0;
    }
    hWnd_main.SetMenuStyle();

    hWnd_main.AddRenderFunction(MW::RenderTabBar);
    hWnd_main.AddRenderFunction(ProcList::ProcWindow.Render);
    hWnd_main.AddRenderFunction(CurrentProcess.MonitorActiveProcess);
    hWnd_main.AddRenderFunction(MemoryView.Render);
    
    MW::MainWndRendingFunc.push_back(MW::Render);
    MW::MainWndRendingFunc.push_back(mem_scan.Render);

    MSG msg{};
    ImGuiIO& io = ImGui::GetIO();

    SetWindowPos(hWnd_main.GetWindowHandle(), NULL, 300, 300, 300, 300, SWP_NOACTIVATE | SWP_HIDEWINDOW);


    while (hWnd_main.window.open) {

        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                hWnd_main.window.open = false;
        }
        
        if ((hWnd_main.Render(io, msg) == false && hWnd_main.window.open == false)) {
            hWnd_main.ExitApplication();
            break;
        }

       // SetWindowPos(hWnd_main.GetWindowHandle(), NULL, hWnd_main.window.Pos.x, hWnd_main.window.Pos.y, hWnd_main.window.Size.x + 15, hWnd_main.window.Size.y + 38, SWP_NOACTIVATE | SWP_HIDEWINDOW);
    }

    //hWnd_main.KillWindow();

    return (int) msg.wParam;
}
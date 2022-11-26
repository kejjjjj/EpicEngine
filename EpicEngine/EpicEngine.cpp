// EpicEngine.cpp : Defines the entry point for the application.
//


#include "EpicEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    FILE* _con;
    AllocConsole();
    freopen_s(&_con, "CONOUT$", "w", stdout);

    std::cout << "requesting to inject\n";


    hWnd_main.initialized = hWnd_main.InitializeWindow("Epic Engine 1.0", hInstance, hWnd_main.WndProc);

    if (!hWnd_main.initialized) {
        FatalError("hWnd_main.InitializeWindow(\"Epic Engine 1.0\", hInstance, hWnd_main.WndProc) == false");
        return 0;
    }
    hWnd_main.SetMenuStyle();

    hWnd_main.AddRenderFunction(MW::RenderTabBar);
    hWnd_main.AddRenderFunction(MW::Render);
    hWnd_main.AddRenderFunction(ProcList::ProcWindow.Render);

    MSG msg{};
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    while (hWnd_main.window.open) {

        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                hWnd_main.window.open = false;
        }
        
        if ((hWnd_main.Render(io, msg) == false && hWnd_main.window.open == false)) {
            hWnd_main.KillWindow();
            break;
        }

        SetWindowPos(hWnd_main.GetWindowHandle(), NULL, hWnd_main.window.Pos.x, hWnd_main.window.Pos.y, hWnd_main.window.Size.x + 15, hWnd_main.window.Size.y + 38, SWP_NOACTIVATE | SWP_HIDEWINDOW);
    }

    //hWnd_main.KillWindow();

    return (int) msg.wParam;
}
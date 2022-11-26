// EpicEngine.cpp : Defines the entry point for the application.
//


#include "EpicEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    hWnd_main = (MainWindow*)malloc(sizeof(MainWindow));
    if (!hWnd_main)
        return false;


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hWnd_main->initialized = hWnd_main->InitializeWindow("Epic Engine 1.0", hInstance, hWnd_main->WndProc);

    if (!hWnd_main->initialized) {
        MessageBoxA(NULL, "hWnd_main->InitializeWindow(\"Epic Engine 1.0\", hInstance, hWnd_main->WndProc) == false", "InitializeWindow failed", MB_ICONERROR);
        return 0;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EPICENGINE));
    MSG msg;
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    bool open = true;
    bool done = false;
    while (!done) {

        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                done = true;
        }
        
        if (done || !hWnd_main->Render(io, msg, open)) {
            hWnd_main->KillWindow();
            break;
        }

        SetWindowPos(hWnd_main->GetWindowHandle(), NULL, hWnd_main->Pos.x, hWnd_main->Pos.y, hWnd_main->Size.x + 15, hWnd_main->Size.y + 38, SWP_NOACTIVATE | SWP_HIDEWINDOW);
    }

    //hWnd_main->KillWindow();

    return (int) msg.wParam;
}
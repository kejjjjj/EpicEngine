#include "EpicEngine.h"

LRESULT WINAPI _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void FatalError(const char* msg, ...)
{
    char v2[4096];



    va_list va;

    va_start(va, msg);
    _vsnprintf_s(v2, 0x1000u, msg, va);
    v2[4095] = 0;


    MessageBoxA(NULL, v2, "Fatal Error!", MB_ICONERROR);
    hWnd_main.ExitApplication();
}

bool MainWindow::InitializeWindow(const char* p_szTitle, HINSTANCE hInst, WNDPROC _wndProc)
{
  //  wndProc = _wndProc;
   // WNDCLASSEXA wc;

    wc.cbSize = (sizeof(WNDCLASSEXA));
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = _wndProc;
    wc.cbClsExtra = 0l;
    wc.cbWndExtra = 0l;
    wc.hInstance = hInst;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = p_szTitle;
    wc.hIconSm = 0;

    RenderFunctions.clear();
    window.open = true;
   // szTitle = std::string(p_szTitle);


    ::RegisterClassExA(&wc);

    hwnd = ::CreateWindowA(wc.lpszClassName, p_szTitle, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    if (!hwnd) {
        
        FatalError("::CreateWindowA(wc.lpszClassName, p_szTitle, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL)\nreturned NULL");
        return false;
    }

	const auto CreateDevice = [this]()-> bool {
		
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK) {
            FatalError("(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)");
            return false;
        }

        CreateRenderTarget();
        return true;
	
	};
    if (!CreateDevice()) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(hwnd)) {
        FatalError("!ImGui_ImplWin32_Init(hwnd)");
        return false;
    }

    if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext)) {
        FatalError("!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext)");
        return false;
    }

    sys_baseTime = timeGetTime();
    root_directory = fs::GetRootDirectory();

    rsc.LoadAllResources();
   // MessageBoxA(NULL, fs::GetRootDirectory().c_str(), "Root Directory", MB_ICONEXCLAMATION);

    return true;

}
void MainWindow::KillWindow()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}
void MainWindow::ExitApplication()
{
    rsc.Font.~Font_s();
    ProcList::ProcWindow.~ProcessWindow();
    KillWindow();

    exit(-1);

}
void MainWindow::CleanupRenderTarget()
{
    if (g_mainRenderTargetView != nullptr) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = NULL; 
    }
}
void MainWindow::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}
void MainWindow::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}
void MainWindow::AddRenderFunction(std::function<void()> fnc)
{
    if (fnc == nullptr) {
        FatalError("AddRenderFunction(std::function<void()> fnc): fnc == nullptr");
        return;
    }
    //char buff[42];
    //sprintf_s(buff, "fncptr is at 0x%p\n", fnc);

    //MessageBoxA(NULL, "fnc", buff, MB_OK);

    RenderFunctions.push_back(fnc);
}
LRESULT WINAPI MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* wnd = &hWnd_main;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:

        if (wnd->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            if (wnd->initialized == 1) {
                wnd->CleanupRenderTarget();
                wnd->g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                wnd->CreateRenderTarget();
            }
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
       // wnd->KillWindow();
        ::PostQuitMessage(0);
        //exit(-1);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
bool MainWindow::Render(ImGuiIO& io, MSG& msg)
{

    if (!ImGui::GetCurrentContext())
        return false;

    static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static ImGuiStyle* style = &ImGui::GetStyle();

    ImGui::PushFont(rsc.Font.fonts[SegoeUI_SemiBoldSM].first);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style->FramePadding.x, style->FramePadding.y*3));

    ImGui::Begin(wc.lpszClassName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
    ImGui::SetWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);

    

    for (auto& renderbit : RenderFunctions)
        renderbit();

    hWnd_main.window.Pos = ImGui::GetWindowPos();
    hWnd_main.window.Size = ImGui::GetWindowSize();
    window.CornerActionButtons();


    const float frametime = 1000.f / io.Framerate;

    if (frametime < 16.7)
        Sleep((int)(25.7f - frametime));

    ImGui::End();

    ImGui::PopStyleVar();

    ImGui::PopFont();

    // Rendering
    ImGui::Render();
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    g_pSwapChain->Present(1, 0); // Present with vsync
    //g_pSwapChain->Present(0, 0); // Present without vsync

    return window.open;

}
void MainWindow::SetMenuStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();

   // style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    //style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;
    style->WindowTitleAlign = ImVec2(0.07f, style->WindowTitleAlign.y);

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = style->Colors[ImGuiCol_FrameBg];
    style->Colors[ImGuiCol_FrameBgActive] = style->Colors[ImGuiCol_FrameBg];
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    //style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    //style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    //style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    //style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    //style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}
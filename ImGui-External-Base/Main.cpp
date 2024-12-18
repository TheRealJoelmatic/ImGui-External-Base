﻿//
//  My personal external cheat base Ive tried to make the code clean and easy
//  to understand.
// 
//  CREDITS
//
//  I used these github repo for the Navida Overlay hijack
//  https://github.com/es3n1n/nvidia-overlay-renderer/
//  https://github.com/Autarch-s/external-warzone-cheat/
//      
//  I used this github repo for the base of this menu:
//  https://github.com/Guardln/ImGui-External-Base
// 
// 



#include <fcntl.h>
#include <thread>

#include "header Files/globals.hh"
#include "header Files/main.h"
#include "header Files/menu.hh"

#include "header Files/font/tahoma.h"
#include "header Files/font/icons.h"

#include "header Files/functions/memory.h"
#include "header Files/functions/configManger.h"
#include "header Files/functions/utils.h"
#include "header Files/modules/teleport.h"
#include "header Files/modules/esp.h"

#include "header Files/functions/colored_cout.h"
#include "overlay/renderer/renderer.h"

// Main code
int main()
{
    hideConsole();

    std::cout << clr::blue << " (                               \n";
    std::cout << " )\\ )         (          (       \n";
    std::cout << "(()/(   (   ( )\\     (   )\\ )    \n";
    std::cout << " /(_))  )\\  )((_)    )\\ (()/     \n";
    std::cout << "(_))_  ((_)((_)_  _ ((_) /(_))_  \n";
    std::cout << " |   \\ | __|| _ )| | | |(_)) __| \n";
    std::cout << " | |) || _| | _ \\| |_| |  | (_ | \n";
    std::cout << " |___/ |___||___/ \\___/    \\___| \n\n-----------------------------------------------------------\n\n";


    std::cout << clr::yellow << "Waiting for " + globals.processName + " \n";

    if (!(FindProcess(globals.processName)))
    {
        ShowDebugInfo("Failed to find the game " + globals.processName);
        exit(0);
    }

    std::cout << clr::green << "Found: " + globals.processName + " \n";

    globals.screenX = GetSystemMetrics(SM_CXSCREEN);
    globals.screenY = GetSystemMetrics(SM_CYSCREEN);

    sdk.processHandle = GetProcessHandleByName(globals.processName);
    sdk.baseAddress = getBaseAddress(sdk.processHandle, globals.processName);
    sdk.process_id = GetPIDByModuleName(globals.processName.c_str());
    EnumWindows(enum_windows, (LPARAM)sdk.process_id);

    // Hook Navida Overlay
    if (!hijack::init()) {
        ShowDebugInfo("failed to hijack nvidia overlay");
    }
    if (!renderer::init()) {
        ShowDebugInfo("failed to initlize nvidia overlay  renderer");
    }

    std::cout << clr::yellow << "\n-----------------------------------------------------------\n";
    std::cout << clr::green << "[-] PROCESS INFO" << std::endl;
    std::cout << clr::green << " [] pid:  0x" << std::hex << sdk.process_id << std::endl;
    std::cout << clr::green << " [] base address: 0x" << std::hex << sdk.baseAddress << std::endl;
    std::cout << clr::green << " [] process handle:  0x" << std::hex << sdk.processHandle << std::endl;
    std::cout << clr::green << " [] hwnd: 0x" << std::hex << sdk.hwnd << std::dec << std::endl;
    std::cout << clr::green << "[-] SCREEN INFO" << std::endl;
    std::cout << clr::green << " [] screen width: " << globals.screenX << std::endl;
    std::cout << clr::green << " [] screen height: " << globals.screenY << std::endl;
    std::cout << clr::yellow << "-----------------------------------------------------------\n\n";

    //std::thread _overlayThread(renderer::scene::renderThread);
    std::thread _espThread(espThread);
    
    setupImgui();
    exit(0);
    return 0;
}

//
// This is were the main loop is.
//
void setupImgui() {
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, ui::window_title, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, ui::window_title, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    // Setup Dear ImGui context
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; //crutial for not leaving the imgui.ini file
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;

    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;

    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;

    //io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 15, &CustomFont);
    io.Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &CustomFont);
    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 32.5f, &icons_config, icons_ranges);


    io.Fonts->AddFontDefault();

    //e
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            static int init = false;
            if (!init) {
                ui::init(g_pd3dDevice);
                init = true;
            }
            else {
                ui::renderMenu();
            }
        }
        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!globals.active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();


    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
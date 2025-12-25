// Dear ImGui: Snake Game - DirectX 9
// Enhanced UI Design Version - AUTO START

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "SnakeGame.h"
#include <d3d9.h>
#include <tchar.h>

static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static bool g_DeviceLost = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS g_d3dpp = {};

static SnakeGame* g_game = nullptr;
static int g_gameSpeed = 2;
static bool g_showHelp = false;
static bool g_showGameWindow = true;  // Always show game window
static int g_highScore = 0;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void SetupImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    
    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]      = ImVec4(0.06f, 0.06f, 0.11f, 0.94f);
    colors[ImGuiCol_TitleBg]       = ImVec4(0.00f, 0.70f, 0.00f, 0.80f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_Button]        = ImVec4(0.00f, 0.60f, 0.00f, 0.80f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.80f, 0.00f, 1.00f);
    colors[ImGuiCol_ButtonActive]  = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
    colors[ImGuiCol_Text]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]  = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_FrameBg]       = ImVec4(0.10f, 0.10f, 0.15f, 0.80f);
    colors[ImGuiCol_FrameBgHovered]= ImVec4(0.15f, 0.15f, 0.20f, 0.90f);
    colors[ImGuiCol_Border]        = ImVec4(0.00f, 0.70f, 0.00f, 0.50f);
    
    // Sizing
    style.WindowPadding = ImVec2(20, 20);
    style.WindowRounding = 5.0f;
    style.FramePadding = ImVec2(10, 8);
    style.FrameRounding = 3.0f;
    style.ItemSpacing = ImVec2(10, 10);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
}

int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Snake Game", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Snake Game - ImGui Edition", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SetupImGuiStyle();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    g_game = new SnakeGame(20, 20);

    ImVec4 clear_color = ImVec4(0.05f, 0.05f, 0.1f, 1.0f);
    bool done = false;

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_DeviceLost)
        {
            HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            g_DeviceLost = false;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();

        // Process game controls
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_W))
            g_game->SetDirection(Direction::UP);
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) || ImGui::IsKeyPressed(ImGuiKey_S))
            g_game->SetDirection(Direction::DOWN);
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_A))
            g_game->SetDirection(Direction::LEFT);
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow) || ImGui::IsKeyPressed(ImGuiKey_D))
            g_game->SetDirection(Direction::RIGHT);
        if (ImGui::IsKeyPressed(ImGuiKey_Space))
            g_game->SetPaused(!g_game->IsGamePaused());
        if (ImGui::IsKeyPressed(ImGuiKey_R))
            g_game->Reset();

        float deltaTime = io.DeltaTime;
        float gameSpeed = (g_gameSpeed == 1) ? 1.5f : (g_gameSpeed == 2) ? 1.0f : 0.5f;
        g_game->Update(deltaTime / gameSpeed);

        if (g_game->GetScore() > g_highScore)
            g_highScore = g_game->GetScore();

        // Control Panel - Left Side
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(280, 760), ImGuiCond_FirstUseEver);
        ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoMove);

        // Title
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "SNAKE GAME");
        ImGui::Separator();

        // Score Display
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SCORE");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%d", g_game->GetScore());
        
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "HIGH SCORE");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%d", g_highScore);

        // Game State
        ImGui::Separator();
        ImGui::Text("STATE");
        if (g_game->IsWaitingForStart())
        {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[WAITING FOR INPUT]");
        }
        else if (g_game->IsGameOver())
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[GAME OVER]");
        }
        else if (g_game->IsGamePaused())
        {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[PAUSED]");
        }
        else
        {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[PLAYING]");
        }

        // Speed Control
        ImGui::Separator();
        ImGui::Text("SPEED");
        ImGui::RadioButton("Slow", &g_gameSpeed, 1);
        ImGui::RadioButton("Normal", &g_gameSpeed, 2);
        ImGui::RadioButton("Fast", &g_gameSpeed, 3);

        ImGui::Separator();

        // Reset Button (replaces NEW GAME)
        if (ImGui::Button("[RESET GAME]", ImVec2(-1, 50)))
        {
            g_game->Reset();
        }

        if (ImGui::Button("[HELP]", ImVec2(-1, 50)))
            g_showHelp = !g_showHelp;

        ImGui::Separator();
        ImGui::TextDisabled("Controls:");
        ImGui::TextDisabled("Arrows/WASD");
        ImGui::TextDisabled("SPACE: Pause");
        ImGui::TextDisabled("R: Reset");

        ImGui::End();

        // Game Window - Always visible
        {
            ImGui::SetNextWindowPos(ImVec2(310, 10), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(550, 500), ImGuiCond_FirstUseEver);
            ImGui::Begin("SNAKE GAME - Play Area", nullptr);

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 canvasPos = ImGui::GetCursorScreenPos();
            float cellSize = 20.0f;
            ImVec2 canvasSize(400.0f, 400.0f);

            ImGui::InvisibleButton("##canvas", canvasSize);
            g_game->Render(drawList, canvasPos, cellSize);

            ImGui::Separator();
            ImGui::Text("SCORE: %d", g_game->GetScore());
            ImGui::Text("HIGH SCORE: %d", g_highScore);
            
            if (g_game->IsWaitingForStart())
            {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[PRESS ARROW TO START]");
            }
            else
            {
                ImGui::Text("STATE: %s", g_game->IsGameOver() ? "[GAME OVER]" : (g_game->IsGamePaused() ? "[PAUSED]" : "[PLAYING]"));
            }

            ImGui::End();
        }

        // Help Window
        if (g_showHelp)
        {
            ImGui::SetNextWindowPos(ImVec2(310, 520), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(550, 250), ImGuiCond_FirstUseEver);
            ImGui::Begin("HELP", &g_showHelp);
            
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "HOW TO PLAY");
            ImGui::Separator();
            ImGui::TextWrapped(
                "OBJECTIVE:\n"
                "Guide your snake to eat the red food and grow longer!\n\n"
                "RULES:\n"
                "- Avoid hitting walls\n"
                "- Avoid the moving ORANGE obstacle block\n"
                "- Don't collide with your own body\n"
                "- Each food = 10 points\n\n"
                "GAME ELEMENTS:\n"
                "GREEN: Your Snake\n"
                "RED: Food pellets (+10 points)\n"
                "ORANGE: Moving obstacle (causes game over)\n\n"
                "CONTROLS:\n"
                "UP/DOWN/LEFT/RIGHT : Move Snake\n"
                "WASD               : Alternative Movement\n"
                "SPACE              : Pause/Resume Game\n"
                "R                  : Reset Game\n\n"
                "STRATEGY TIP:\n"
                "Plan your movements carefully to avoid the moving obstacle!\n"
                "Adjust speed to match your reflexes."
            );
            ImGui::End();
        }

        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255), (int)(clear_color.y * 255), (int)(clear_color.z * 255), (int)(clear_color.w * 255));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            g_DeviceLost = true;
    }

    delete g_game;
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

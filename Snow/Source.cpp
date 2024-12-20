#include <Windows.h>
#include <cmath>
#include <string>
#include <iostream>
#include <SFML/Audio.hpp>

#define SNOW_FLAKES_COUNT 1200
#define SPEED_OF_FLAKES 1.0f // 1.0 is standart
#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 550
#define FPS 10.0f // 100 fps


typedef struct Snow {

    float x, y;
    float speed = SPEED_OF_FLAKES + ((float)rand() / RAND_MAX) * 2.0f;
    float drift = ((float)rand() / RAND_MAX) * 2.0f * 3.1f;
    float windowOfset = ((float)rand() / RAND_MAX) - 0.5f;

    inline void Draw(const HDC& hdc) const noexcept {
        Ellipse(hdc, x, y, x + 6, y + 6);
    }
    inline void UpdatePosition() noexcept {
        y += speed;
        drift += 01.f;
        x += -sin(drift) * 0.5f + windowOfset;
    }

} SnowParticle;

std::unique_ptr<SnowParticle[]> flakes = std::make_unique<SnowParticle[]>(SNOW_FLAKES_COUNT);


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
   

    switch (uMsg) {

    case WM_CREATE:
        SetTimer(hwnd, 10, FPS, NULL);
        for (int i = 0; i < SNOW_FLAKES_COUNT; ++i) {
            flakes[i].x = rand() % WINDOW_WIDTH;
            flakes[i].y = rand() % WINDOW_HEIGHT;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

       
        HDC hdcBuffer = CreateCompatibleDC(hdc);
        HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

   
        HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        FillRect(hdcBuffer, &rect, hBrush);

     
        SetBkColor(hdcBuffer, RGB(0, 0, 0));
        SetTextColor(hdcBuffer, RGB(255, 255, 255));
        TextOut(hdcBuffer, 5, 5, L"Snow Flakes!", 15);

      
        for (int i = 0; i < SNOW_FLAKES_COUNT; ++i) {
            flakes[i].Draw(hdcBuffer);
        }

        
        BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);

        
        SelectObject(hdcBuffer, hbmOld);
        DeleteObject(hbmBuffer);
        DeleteDC(hdcBuffer);

        EndPaint(hwnd, &ps);
    }
    break;


    case WM_TIMER:

        

        for (int i = 0; i < SNOW_FLAKES_COUNT; ++i) {
           
            
            flakes[i].UpdatePosition();
          
            if (flakes[i].y > WINDOW_HEIGHT || flakes[i].y < 0) {
                flakes[i].y = 0;
                flakes[i].x = rand() % WINDOW_WIDTH; 
            }
        }

       
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    sf::Music player;
    player.setVolume(100);

    if (!player.openFromFile("C:\\Users\\user\\Downloads\\p1.ogg")) {
        MessageBox(NULL, L"Audio file error", L":(", MB_ICONERROR);
    }
    player.play();
    WNDCLASSEX WNDclass = { 0 };
    HWND MainWND;

    WNDclass.cbSize = sizeof(WNDCLASSEX);
    WNDclass.style = CS_HREDRAW | CS_VREDRAW;
    WNDclass.lpfnWndProc = WindowProc;
    WNDclass.cbClsExtra = 0;
    WNDclass.cbWndExtra = 0;
    WNDclass.hInstance = hInstance;
    WNDclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WNDclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WNDclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WNDclass.lpszMenuName = NULL;
    WNDclass.lpszClassName = L"Snowww Falllll";
    WNDclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&WNDclass)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_ICONERROR);
        return 0;
    }

    MainWND = CreateWindowEx(
        0,
        WNDclass.lpszClassName,
        L"SNOOOOOW",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    if (!MainWND) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONERROR);
        return 0;
    }

    

    ShowWindow(MainWND, nCmdShow);
    UpdateWindow(MainWND);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

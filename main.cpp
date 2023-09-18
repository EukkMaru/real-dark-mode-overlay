#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get the current mouse position
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);

            // Define flashlight circle radius
            const int radius = 100;

            // Create a black brush to darken the screen
            HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
            RECT rect;
            GetClientRect(hwnd, &rect);

            // Fill the entire window with black
            FillRect(hdc, &rect, hBlackBrush);

            // Clear a circle around the mouse cursor to simulate the flashlight effect
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Ellipse(hdc, pt.x - radius, pt.y - radius, pt.x + radius, pt.y + radius);

            // Cleanup
            DeleteObject(hBlackBrush);
            EndPaint(hwnd, &ps);
        }
        return 0;

    case WM_HOTKEY:
        if (wParam == 1) {  // If our registered hotkey was pressed (Ctrl + Shift + X)
            PostQuitMessage(0);
            return 0;
        }
        break;

    case WM_DESTROY:
        UnregisterHotKey(NULL, 1);
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"DarkModeWindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        CLASS_NAME,
        L"Real Dark Mode",
        WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYFULLSCREEN),
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Set the transparency level of the window (200 out of 255, adjust as needed)
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA); 

    // Register the global hotkey (Ctrl + Shift + X)
    if (!RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT, 'X')) {
        MessageBoxW(hwnd, L"Failed to register exit hotkey!", L"Error", MB_ICONERROR);
    }

    ShowWindow(hwnd, nCmdShow);

    // Main message loop:
    MSG msg;
    msg.hwnd = NULL;
    msg.message = 0;
    msg.wParam = 0;
    msg.lParam = 0;
    msg.time = 0;
    msg.pt.x = 0;
    msg.pt.y = 0;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


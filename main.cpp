#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);
            const int radius = 100; // flashlight init
            HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));

            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hBlackBrush);
            HRGN hRgnFull = CreateRectRgn(0, 0, rect.right, rect.bottom);
            HRGN hRgnCircle = CreateEllipticRgn(pt.x - radius, pt.y - radius, pt.x + radius, pt.y + radius);
            CombineRgn(hRgnFull, hRgnFull, hRgnCircle, RGN_DIFF);
            SetWindowRgn(hwnd, hRgnFull, TRUE);

            // Create a semi-transparent brush with an alpha value (127 for half transparency in this case)
            COLORREF semiTransparentBlack = RGB(0, 0, 0) | (127 << 24); // Alpha value is added to the top 8 bits
            HBRUSH hSemiTransBrush = CreateSolidBrush(semiTransparentBlack);

            // Fill the ellipse centered on the cursor using the semi-transparent brush
            Ellipse(hdc, pt.x - radius, pt.y - radius, pt.x + radius, pt.y + radius);

            DeleteObject(hBlackBrush);
            DeleteObject(hRgnCircle);
            DeleteObject(hSemiTransBrush); // delete the new brush

        }
        return 0;
    case WM_HOTKEY:
        if (wParam == 1) {  // If our registered hotkey was pressed
            PostQuitMessage(0);
            return 0;
        }
        break;
    case WM_DESTROY:
        UnregisterHotKey(NULL, 1);
        PostQuitMessage(0);
        return 0;
    case WM_MOUSEMOVE:
        InvalidateRect(hwnd, NULL, FALSE);
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
    // Set the transparency level of the window (x out of 255)
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA); 
    
    if (!RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT, 'X')) {
        MessageBoxW(hwnd, L"Failed to register exit hotkey!", L"Error", MB_ICONERROR);
    }
    ShowWindow(hwnd, nCmdShow);

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


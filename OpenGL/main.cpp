// =============================================================================
// Creates the window widget to host the GL viewport
// =============================================================================

#include <windows.h>
#include "draw.h"

// ================ Global Variables ======================

HWND         hWnd=NULL;             // Holds Our Window Handle
HINSTANCE    hInstance=NULL;        // Holds The Instance Of The Application

// =================== Windows callback function =========================

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND ewin;

    switch (message)
    {
    case WM_CREATE:
        {
            return 0;
        }

    case WM_COMMAND:
        switch (wParam)
        {
        case 2:
            PostQuitMessage (0);
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage (0);
        return 0; 

    case WM_SIZE:
        {
            // Resize the OpenGL Window here
            return 0;   // Continue message loop
        }
    }

    return DefWindowProc (hwnd, message, wParam, lParam);
}

// =================== Create a basic MFC based window =========================

void CreateMFCWindow()
{
    static wchar_t szAppName[] = L"OpenGL Shading Language (GLSL) Example";
    WNDCLASS    wndclass ;

    wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc   = WndProc ;
    wndclass.cbClsExtra    = 0 ;
    wndclass.cbWndExtra    = 0 ;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclass.lpszMenuName  = NULL;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = szAppName;
    RegisterClass (&wndclass);

    hWnd = CreateWindow(szAppName,
                        L"OpenGL Shading Language (GLSL) Example",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        640,
                        480,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    ShowWindow (hWnd, true);
    UpdateWindow (hWnd);
}

// =================== Release all resources and kill the window =========================

void KillWindow()
{
    StopGL();

    if (hWnd)
        DestroyWindow(hWnd);

    UnregisterClass(L"OpenGL",hInstance);
}

// =================== Entry point for the application =========================

int WINAPI WinMain (HINSTANCE hInstance, 
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int nCmdShow)
{
    CreateMFCWindow();

    StartGL();

    MSG        msg;                                 // Windows Message Structure
    BOOL    done=FALSE;                             // Bool Variable To Exit Loop

    while(!done)                                    // Loop That Runs While done=FALSE
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))   // Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)               // Have We Received A Quit Message?
            {
                KillWindow();
                done=TRUE;                          // If So done=TRUE
            }
            else                                    // If Not, eeal With Window Messages
            {
                TranslateMessage(&msg);             // Translate The Message
                DispatchMessage(&msg);              // Dispatch The Message
            }
        }
        else                                        // If There Are No Messages
        {
            DrawGL();
        }
    }

    return 0;
}

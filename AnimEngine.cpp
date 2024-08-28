//-----------------------------------------------------------------
// AnimEngine Object
//-----------------------------------------------------------------
#include "AnimEngine.h"

//-----------------------------------------------------------------
// Variable Initialization
//-----------------------------------------------------------------
AnimEngine* AnimEngine::m_pAnimEngine = nullptr;

//-----------------------------------------------------------------
// AnimEngine Constructor/Destructor
//-----------------------------------------------------------------
AnimEngine::AnimEngine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight) {
    // Initialize AnimEngine variables
    m_pAnimEngine = this;
    m_hInstance = hInstance;
    m_hWindow = nullptr;

    if (lstrlen(szWindowClass) > 0)
        lstrcpy(m_szWindowClass, szWindowClass);

    if (lstrlen(szTitle) > 0)
        lstrcpy(m_szTitle, szTitle);

    m_wIcon = wIcon;
    m_wSmallIcon = wSmallIcon;
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_bSleep = TRUE;
    m_vSprites.reserve(50);
}

AnimEngine::~AnimEngine() {
    CleanupSprites();
}

//-----------------------------------------------------------------
// AnimEngine Methods
//-----------------------------------------------------------------
BOOL AnimEngine::Initialize(int iCmdShow) {
    WNDCLASSEX    wndclass;

    // Create the window class for the main window
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = m_hInstance;
    wndclass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(GetIcon()));
    wndclass.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(GetSmallIcon()));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = m_szWindowClass;

    // Register the window class
    if (!RegisterClassEx(&wndclass))
        return FALSE;

    // Calculate window size and position
    int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
    int iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

    if (wndclass.lpszMenuName != nullptr)
        iWindowHeight += GetSystemMetrics(SM_CYMENU);

    int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2;
    int iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;

    // Create the window
    m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
        iXWindowPos, iYWindowPos, iWindowWidth, iWindowHeight,
        nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWindow)
        return FALSE;

    // Show and update the window
    ShowWindow(m_hWindow, iCmdShow);
    UpdateWindow(m_hWindow);

    return TRUE;
}

LRESULT AnimEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC = GetDC(hWindow);

    switch (msg) {
    case WM_CREATE:
        // Create the window and start the animation
        SetWindow(hWindow);
        AnimStart(hWindow, 0);

        // Create buttons
        CreateButton(hWindow, (HMENU)IDB_BUTTON_1, 40, 455, _T("40 kW/h"));
        CreateButton(hWindow, (HMENU)IDB_BUTTON_2, 40, 500, _T("80 kW/h"));
        CreateButton(hWindow, (HMENU)IDB_BUTTON_3, 40, 545, _T("120 kW/h"));
        CreateButton(hWindow, (HMENU)IDB_BUTTON_4, 325, 455, _T("160 kW/h"));
        CreateButton(hWindow, (HMENU)IDB_BUTTON_5, 325, 500, _T("200 kW/h"));
        CreateButton(hWindow, (HMENU)IDB_BUTTON_6, 325, 545, _T("240 kW/h"));
        CreateButton(hWindow, (HMENU)IDB_BUTTON_7, 610, 455, _T("280 kW/h"));
        CreateButton(hWindow, (HMENU)IDB_BUTTON_8, 610, 500, _T("Restart"));
        return 0;

    case WM_ACTIVATE:
        // Handle sleep mode
        if (wParam != WA_INACTIVE) {
            SetSleep(FALSE);
        }
        else {
            SetSleep(TRUE);
        }
        return 0;
    
    case WM_COMMAND:
        // Handle button clicks
        switch (LOWORD(wParam)) {
        case IDB_BUTTON_1:
            AnimStart(hWindow, 1);
            break;
        case IDB_BUTTON_2:
            AnimStart(hWindow, 2);
            break;
        case IDB_BUTTON_3:
            AnimStart(hWindow, 3);
            break;
        case IDB_BUTTON_4:
            AnimStart(hWindow, 4);
            break;
        case IDB_BUTTON_5:
            AnimStart(hWindow, 5);
            break;
        case IDB_BUTTON_6:
            AnimStart(hWindow, 6);
            break;
        case IDB_BUTTON_7:
            AnimStart(hWindow, 7);
            break;
        case IDB_BUTTON_8:
            AnimStart(hWindow, 0);
            break;
        }
        return 0;
      
    case WM_PAINT:
        BeginPaint(hWindow, &ps);
        // Paint the animation
        AnimPaint(hDC);
        EndPaint(hWindow, &ps);
        return 0;

    case WM_DESTROY:
        // End the animation and close the window
        AnimEnd();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWindow, msg, wParam, lParam);
}

void AnimEngine::AddSprite(Sprite* pSprite) {
    // Add a sprite to the vector
    if (pSprite != nullptr) {
        // Check if the sprite is already in the vector
        if (!m_vSprites.empty()) {
            // Find the correct position to insert the sprite based on ZOrder
            auto it = m_vSprites.begin();
            while (it != m_vSprites.end() && pSprite->GetZOrder() >= (*it)->GetZOrder()) {
                ++it;
            }
            m_vSprites.insert(it, pSprite);
        }
        else {
            // If the vector is empty, just add the sprite
            m_vSprites.push_back(pSprite);
        }
    }
}

void AnimEngine::DrawSprites(HDC hDC) {
    // Draw each sprite in the vector
    for (auto& sprite : m_vSprites) {
        sprite->Draw(hDC);
    }
}

void AnimEngine::UpdateSprites() {
    // Update each sprite
    auto it = m_vSprites.begin();
    while (it != m_vSprites.end()) {
        SPRITEACTION saSpriteAction = (*it)->Update();

        // If the sprite should be killed, remove it from the vector
        if (saSpriteAction & SA_KILL) {
            delete* it;
            it = m_vSprites.erase(it);
        }
        else {
            ++it;
        }
    }
}

void AnimEngine::CleanupSprites() {
    // Remove and delete all sprites
    for (auto& sprite : m_vSprites) {
        delete sprite;
    }
    m_vSprites.clear();
}

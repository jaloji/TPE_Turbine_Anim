//-----------------------------------------------------------------
// Wind Turbine Animation
//-----------------------------------------------------------------
#include "AnimTurbine.h"

//-----------------------------------------------------------------
// Windows Functions
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    MSG msg;
    static int iTickTrigger = 0;
    int iTickCount;

  if (AnimInitialize(hInstance)) {
      // Initialize AnimEngine
      if (!_pAnim->Initialize(iCmdShow))
          return FALSE;

      // Main message loop
      while (TRUE) {
          if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
              // Message processing
              if (msg.message == WM_QUIT)
                  break;

              TranslateMessage(&msg);
              DispatchMessage(&msg);
          }
          else {
              // Check if AnimEngine is not asleep
              if (!_pAnim->IsSleeping()) {
                  // Check elapsed time
                  iTickCount = GetTickCount64();
                  if (iTickCount > iTickTrigger) {
                      iTickTrigger = iTickCount + _pAnim->GetFrameDelay();
                      AnimCycle();
                  }
              }
          }
      }
    return (int)msg.wParam;
  }
  // End of animation
  AnimEnd();

  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Send all window messages to AnimEngine
    return _pAnim->HandleEvent(hWindow, msg, wParam, lParam);
}

//-----------------------------------------------------------------
// AnimEngine Functions
//-----------------------------------------------------------------
HWND CreateButton(HWND hwnd, HMENU code, long x, long y, const wchar_t* text) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    return CreateWindow(_T("BUTTON"), text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, 150, 30, hwnd, code, hInstance, NULL);
}

BOOL AnimInitialize(HINSTANCE hInstance) {
    // Create AnimEngine
    _pAnim = new AnimEngine(hInstance, _T("TPE - Turbine : Animation"), _T("TPE - Turbine : Animation"), IDI_TURBINE_32, IDI_TURBINE_16, 800, 600);
    if (_pAnim == NULL)
        return FALSE;

    // Set frames per second
    _pAnim->SetFrameRate(30);

    // Store instance
    _hInstance = hInstance;
    return TRUE;
}

void AnimStart(HWND hWindow, int Light) {
    // Create offscreen area
    _hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
    _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), _pAnim->GetWidth(), _pAnim->GetHeight());
    SelectObject(_hOffscreenDC, _hOffscreenBitmap);

    // Load bitmaps
    HDC hDC = GetDC(hWindow);

    // Background and button bitmaps
    _pBackgroundTopBitmap = new Bitmap(hDC, IDB_BACKGROUND_TOP, _hInstance);
    _pBackgroundMidBitmap = new Bitmap(hDC, IDB_BACKGROUND_MID, _hInstance);
    _pButton1Bitmap = new Bitmap(hDC, IDB_BUTTON_1, _hInstance);
    _pButton2Bitmap = new Bitmap(hDC, IDB_BUTTON_2, _hInstance);
    _pButton3Bitmap = new Bitmap(hDC, IDB_BUTTON_3, _hInstance);
    _pButton4Bitmap = new Bitmap(hDC, IDB_BUTTON_4, _hInstance);
    _pButton5Bitmap = new Bitmap(hDC, IDB_BUTTON_5, _hInstance);
    _pButton6Bitmap = new Bitmap(hDC, IDB_BUTTON_6, _hInstance);
    _pButton7Bitmap = new Bitmap(hDC, IDB_BUTTON_7, _hInstance);
    _pButton8Bitmap = new Bitmap(hDC, IDB_BUTTON_8, _hInstance);

    // Turbine and house bitmaps
    _pTurbineBitmapOff = new Bitmap(hDC, IDB_TURBINE, _hInstance);
    _pHouseBitmap = new Bitmap(hDC, IDB_HOUSE, _hInstance);
    _pLampOff1Bitmap = new Bitmap(hDC, IDB_LAMP_OFF, _hInstance);
    _pLampOff2Bitmap = new Bitmap(hDC, IDB_LAMP_OFF, _hInstance);
    _pLampOff3Bitmap = new Bitmap(hDC, IDB_LAMP_OFF, _hInstance);
    _pLampOff4Bitmap = new Bitmap(hDC, IDB_LAMP_OFF, _hInstance);
    _pLampOff5Bitmap = new Bitmap(hDC, IDB_LAMP_OFF, _hInstance);
    _pLampOff6Bitmap = new Bitmap(hDC, IDB_LAMP_OFF, _hInstance);
    _pLampOff7Bitmap = new Bitmap(hDC, IDB_LAMP_OFF, _hInstance);

    // Setup background sprites
    RECT rcBoundsBackGrndTop = { 0, 0, 800, 600 };
    Sprite* pSpriteBackGrndTop = new Sprite(_pBackgroundTopBitmap, rcBoundsBackGrndTop);
    pSpriteBackGrndTop->SetNumFrames(1);
    pSpriteBackGrndTop->SetFrameDelay(1);
    pSpriteBackGrndTop->SetPosition(0, 0);
    _pAnim->AddSprite(pSpriteBackGrndTop);

    RECT rcBoundsBackGrndMid = { 0, 0, 800, 600 };
    Sprite* pSpriteBackGrndMid = new Sprite(_pBackgroundMidBitmap, rcBoundsBackGrndMid);
    pSpriteBackGrndMid->SetNumFrames(1);
    pSpriteBackGrndMid->SetFrameDelay(1);
    pSpriteBackGrndMid->SetPosition(0, 411);
    _pAnim->AddSprite(pSpriteBackGrndMid);

    // Setup button sprites
    RECT rcBoundsButton = { 0, 0, 800, 600 };
    AddButtonSprite(_pButton1Bitmap, rcBoundsButton, 40, 455);
    AddButtonSprite(_pButton2Bitmap, rcBoundsButton, 40, 500);
    AddButtonSprite(_pButton3Bitmap, rcBoundsButton, 40, 545);
    AddButtonSprite(_pButton4Bitmap, rcBoundsButton, 325, 455);
    AddButtonSprite(_pButton5Bitmap, rcBoundsButton, 325, 500);
    AddButtonSprite(_pButton6Bitmap, rcBoundsButton, 325, 545);
    AddButtonSprite(_pButton7Bitmap, rcBoundsButton, 610, 455);
    AddButtonSprite(_pButton8Bitmap, rcBoundsButton, 610, 500);

    // Setup turbine and lamp sprites
    // Create turbine sprites based on the light state
    RECT rcBoundsEolienne_off = { 0, 0, 800, 600 };
    Sprite* pSpriteEolienne_off = new Sprite(_pTurbineBitmapOff, rcBoundsEolienne_off);
    pSpriteEolienne_off->SetNumFrames(12);
    pSpriteEolienne_off->SetFrameDelay(-1);
    pSpriteEolienne_off->SetPosition(580, 150);
    _pAnim->AddSprite(pSpriteEolienne_off);

    if (Light != 0) {
        RECT rcBoundsEolienne_on = { 0, 0, 800, 600 };
        _pTurbineBitmapOn = new Bitmap(hDC, IDB_TURBINE, _hInstance);
        Sprite* pSpriteEolienne_on = new Sprite(_pTurbineBitmapOn, rcBoundsEolienne_on);
        pSpriteEolienne_on->SetNumFrames(12);
        pSpriteEolienne_on->SetFrameDelay(7 - Light); // Delay decreases as light level increases
        pSpriteEolienne_on->SetPosition(580, 150);
        _pAnim->AddSprite(pSpriteEolienne_on);
    }

    // Create lamp sprites based on the light state
    RECT rcBoundsMaison = { 0, 0, 800, 600 };
    Sprite* pSpriteMaison = new Sprite(_pHouseBitmap, rcBoundsMaison);
    pSpriteMaison->SetNumFrames(1);
    pSpriteMaison->SetFrameDelay(1);
    pSpriteMaison->SetPosition(101, 92);
    _pAnim->AddSprite(pSpriteMaison);

    // Add lamp sprites depending on the light level
    RECT rcBoundsLampe = { 0, 0, 800, 600 };
    std::vector<Bitmap*> lampBitmaps = {
        _pLampOff1Bitmap, _pLampOff2Bitmap, _pLampOff3Bitmap,
        _pLampOff4Bitmap, _pLampOff5Bitmap, _pLampOff6Bitmap,
        _pLampOff7Bitmap
    };

    std::vector<int> positionsX = { 245, 350, 440, 300, 394, 470, 375 };
    std::vector<int> positionsY = { 330, 330, 330, 240, 240, 240, 148 };

    for (int i = 0; i < 7; ++i) {
        Sprite* pSprite = new Sprite(lampBitmaps[i], rcBoundsLampe);
        pSprite->SetNumFrames(1);
        pSprite->SetFrameDelay(1);
        pSprite->SetPosition(positionsX[i], positionsY[i]);
        _pAnim->AddSprite(pSprite);
    }

    // Update lamps based on light level
    std::vector<Bitmap*> lampOnBitmaps = {
        _pLampOn1Bitmap, _pLampOn2Bitmap, _pLampOn3Bitmap,
        _pLampOn4Bitmap, _pLampOn5Bitmap, _pLampOn6Bitmap,
        _pLampOn7Bitmap
    };

    for (int i = 0; i < Light; ++i) {
        RECT rcBoundsLampe = { 0, 0, 800, 600 };
        lampOnBitmaps[i] = new Bitmap(hDC, IDB_LAMP_ON, _hInstance);
        Sprite* pSprite = new Sprite(lampOnBitmaps[i], rcBoundsLampe);
        pSprite->SetNumFrames(1);
        pSprite->SetFrameDelay(1);
        pSprite->SetPosition(positionsX[i], positionsY[i]);
        _pAnim->AddSprite(pSprite);
    }
}

void AddButtonSprite(Bitmap* bitmap, RECT rcBounds, int x, int y) {
    Sprite* pSprite = new Sprite(bitmap, rcBounds);
    pSprite->SetNumFrames(1);
    pSprite->SetFrameDelay(1);
    pSprite->SetPosition(x, y);
    _pAnim->AddSprite(pSprite);
}

void AnimCycle() {
    // Mets à jour les sprites
    _pAnim->UpdateSprites();

    // On obtient le context de hors-champs pour repaindre l'animation
    HWND  hWindow = _pAnim->GetWindow();
    HDC   hDC = GetDC(hWindow);

    // Repaint le hors-champs
    AnimPaint(_hOffscreenDC);

    // Blit les images du hors-champs de l'animation 
    BitBlt(hDC, 0, 0, _pAnim->GetWidth(), _pAnim->GetHeight(), _hOffscreenDC, 0, 0, SRCCOPY);

    // Nettoyage
    ReleaseDC(hWindow, hDC);
}

void AnimEnd() {
    // Clean up resources
    DeleteObject(_hOffscreenBitmap);
    DeleteDC(_hOffscreenDC);
    delete _pAnim;
}

void AnimPaint(HDC hDC) {
    // Dessine les sprites
    _pAnim->DrawSprites(hDC);
}

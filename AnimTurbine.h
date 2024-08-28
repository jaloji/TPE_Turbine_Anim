//-----------------------------------------------------------------
// Wind Turbine Animation
//-----------------------------------------------------------------
#ifndef EOLIENNE_H
#define EOLIENNE_H

#include <windows.h>
#include <tchar.h>
#include "Ressource.h"
#include "AnimEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE _hInstance;
AnimEngine* _pAnim;
HDC _hOffscreenDC;
HBITMAP _hOffscreenBitmap;

// Bitmaps
Bitmap* _pBackgroundTopBitmap;
Bitmap* _pBackgroundMidBitmap;
Bitmap* _pButton1Bitmap;
Bitmap* _pButton2Bitmap;
Bitmap* _pButton3Bitmap;
Bitmap* _pButton4Bitmap;
Bitmap* _pButton5Bitmap;
Bitmap* _pButton6Bitmap;
Bitmap* _pButton7Bitmap;
Bitmap* _pButton8Bitmap;
Bitmap* _pTurbineBitmapOff;
Bitmap* _pTurbineBitmapOn;
Bitmap* _pHouseBitmap;
Bitmap* _pLampOff1Bitmap;
Bitmap* _pLampOff2Bitmap;
Bitmap* _pLampOff3Bitmap;
Bitmap* _pLampOff4Bitmap;
Bitmap* _pLampOff5Bitmap;
Bitmap* _pLampOff6Bitmap;
Bitmap* _pLampOff7Bitmap;
Bitmap* _pLampOn1Bitmap;
Bitmap* _pLampOn2Bitmap;
Bitmap* _pLampOn3Bitmap;
Bitmap* _pLampOn4Bitmap;
Bitmap* _pLampOn5Bitmap;
Bitmap* _pLampOn6Bitmap;
Bitmap* _pLampOn7Bitmap;

// Sprites and their bounds
RECT rcBoundsTurbineOn = { 0, 0, 800, 600 };
Sprite* pSpriteTurbineOn;

RECT rcBoundsLampOn1 = { 0, 0, 800, 600 };
Sprite* pSpriteLampOn1;

RECT rcBoundsLampOn2 = { 0, 0, 800, 600 };
Sprite* pSpriteLampOn2;

RECT rcBoundsLampOn3 = { 0, 0, 800, 600 };
Sprite* pSpriteLampOn3;

RECT rcBoundsLampOn4 = { 0, 0, 800, 600 };
Sprite* pSpriteLampOn4;

RECT rcBoundsLampOn5 = { 0, 0, 800, 600 };
Sprite* pSpriteLampOn5;

RECT rcBoundsLampOn6 = { 0, 0, 800, 600 };
Sprite* pSpriteLampOn6;

RECT rcBoundsLampOn7 = { 0, 0, 800, 600 };
Sprite* pSpriteLampOn7;

void AddButtonSprite(Bitmap* bitmap, RECT rcBounds, int x, int y);
void AnimCycle();

#endif

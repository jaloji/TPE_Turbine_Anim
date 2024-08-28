//-----------------------------------------------------------------
// AnimEngine Object
//-----------------------------------------------------------------
#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include <mmsystem.h>
#include <vector>
#include <tchar.h>
#include "Sprite.h"
#include "Ressource.h"

using namespace std;

//-----------------------------------------------------------------
// Windows Function Declarations
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
LRESULT CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------
// Animation Function Declarations
//-----------------------------------------------------------------
HWND CreateButton(HWND hwnd, HMENU code, long x, long y, const wchar_t* text);
BOOL AnimInitialize(HINSTANCE hInstance);
void AnimStart(HWND hWindow, int Light);
void AnimEnd();
void AnimPaint(HDC hDC);

//-----------------------------------------------------------------
// AnimEngine Class
//-----------------------------------------------------------------
class AnimEngine {
private:
    static AnimEngine* m_pAnimEngine;
    HINSTANCE m_hInstance;
    HWND m_hWindow;
    TCHAR m_szWindowClass[32];
    TCHAR m_szTitle[32];
    WORD m_wIcon;
    WORD m_wSmallIcon;
    int m_iWidth;
    int m_iHeight;
    int m_iFrameDelay;
    BOOL m_bSleep;
    vector<Sprite*> m_vSprites;

public:
    // Constructor/Destructor
    AnimEngine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight);
    virtual ~AnimEngine();

    // Methods
    BOOL Initialize(int iCmdShow);
    LRESULT HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
    void AddSprite(Sprite* pSprite);
    void DrawSprites(HDC hDC);
    void UpdateSprites();
    void CleanupSprites();

    // Accessors
    void SetWindow(HWND hWindow) { m_hWindow = hWindow; }
    void SetFrameRate(int iFrameRate) { m_iFrameDelay = 1000 / iFrameRate; }
    void SetSleep(BOOL bSleep) { m_bSleep = bSleep; }

    HWND GetWindow() const { return m_hWindow; }
    WORD GetIcon() const { return m_wIcon; }
    WORD GetSmallIcon() const { return m_wSmallIcon; }
    int GetWidth() const { return m_iWidth; }
    int GetHeight() const { return m_iHeight; }
    int GetFrameDelay() const { return m_iFrameDelay; }
    BOOL IsSleeping() const { return m_bSleep; }
};

#endif

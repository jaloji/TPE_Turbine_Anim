//-----------------------------------------------------------------
// Bitmap Object
//-----------------------------------------------------------------
#ifndef BITMAP_H
#define BITMAP_H

#include <windows.h>

//-----------------------------------------------------------------
// Data Type
//-----------------------------------------------------------------
struct BitmapInfo256 {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[256];
};

//-----------------------------------------------------------------
// Bitmap Class
//-----------------------------------------------------------------
class Bitmap {
private:
    HBITMAP m_hBitmap; // Handle to the bitmap
    int m_iWidth; // Width of the bitmap
    int m_iHeight; // Height of the bitmap

public:
    // Constructor/Destructor
    Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance);
    virtual ~Bitmap();

    // Methods
    void Free();
    BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);
    void Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE);
    void DrawPart(HDC hDC, int x, int y, int xPart, int yPart, int wPart, int hPart, BOOL bTrans = FALSE);

    // Accessors
    int GetWidth()  const { return m_iWidth; }
    int GetHeight() const { return m_iHeight; }
};

#endif
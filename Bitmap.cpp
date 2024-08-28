//-----------------------------------------------------------------
// Bitmap Object
//-----------------------------------------------------------------
#include "Bitmap.h"

//-----------------------------------------------------------------
// Bitmap Constructor/Destructor
//-----------------------------------------------------------------
Bitmap::Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance) {
    // Initialize members
    m_hBitmap = NULL;
    m_iWidth = 0;
    m_iHeight = 0;
    Create(hDC, uiResID, hInstance);
}

Bitmap::~Bitmap() {
    Free();
}

//-----------------------------------------------------------------
// Bitmap Methods
//-----------------------------------------------------------------
void Bitmap::Free() {
    // Delete the bitmap graphic object
    if (m_hBitmap != NULL) {
        DeleteObject(m_hBitmap);
        m_hBitmap = NULL;
    }
}

BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInstance) {
    // Release any previous DIB information
    Free();

    // Find the bitmap resource
    HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(uiResID), RT_BITMAP);

    if (hResInfo == NULL)
        return FALSE;

    // Load the bitmap resource
    HGLOBAL hMemBitmap = LoadResource(hInstance, hResInfo);

    if (hMemBitmap == NULL)
        return FALSE;

    // Lock the resource and access the bitmap image
    PBYTE pBitmapImage = (BYTE*)LockResource(hMemBitmap);

    if (pBitmapImage == NULL) {
        FreeResource(hMemBitmap);
        return FALSE;
    }

    // Store the width and height of the bitmap
    BITMAPINFO* pBitmapInfo = (BITMAPINFO*)pBitmapImage;
    m_iWidth = (int)pBitmapInfo->bmiHeader.biWidth;
    m_iHeight = (int)pBitmapInfo->bmiHeader.biHeight;

    // Create a handle for the bitmap and copy the bytes
    PBYTE pBitmapBits;
    m_hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS, (PVOID*)&pBitmapBits, NULL, 0);

    if (m_hBitmap != NULL && pBitmapBits != NULL) {
        const PBYTE pTempBits = pBitmapImage + pBitmapInfo->bmiHeader.biSize + pBitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
        CopyMemory(pBitmapBits, pTempBits, pBitmapInfo->bmiHeader.biSizeImage);

        // Unlock and free the bitmap resource
        UnlockResource(hMemBitmap);
        FreeResource(hMemBitmap);
        return TRUE;
    }

    // On error, clean up
    UnlockResource(hMemBitmap);
    FreeResource(hMemBitmap);
    Free();
    return FALSE;
}

void Bitmap::Draw(HDC hDC, int x, int y, BOOL bTrans) {
    DrawPart(hDC, x, y, 0, 0, GetWidth(), GetHeight(), bTrans);
}

void Bitmap::DrawPart(HDC hDC, int x, int y, int xPart, int yPart, int wPart, int hPart, BOOL bTrans) {
    if (m_hBitmap != NULL) {
        // Create a memory device context for the bitmap
        HDC hMemDC = CreateCompatibleDC(hDC);

        // Select the bitmap into the memory DC
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

        // Draw the bitmap at the specified location
        if (bTrans)
            BitBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, SRCCOPY);

        // Restore and delete the memory DC
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}

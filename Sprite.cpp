//-----------------------------------------------------------------
// Sprite Object
//-----------------------------------------------------------------
#include "Sprite.h"

//-----------------------------------------------------------------
// Sprite Constructor/Destructor
//-----------------------------------------------------------------
Sprite::Sprite(Bitmap* pBitmap, RECT& rcBounds) {
    // Calculate a random position within bounds
    int iXPos = rcBounds.right - rcBounds.left;
    int iYPos = rcBounds.bottom - rcBounds.top;

    // Initialize variables
    m_pBitmap = pBitmap;
    m_iNumFrames = 1;
    m_iCurFrame = 0;
    m_iFrameDelay = 0;
    m_iFrameTrigger = 0;
    SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(), iYPos + pBitmap->GetHeight());
    m_iZOrder = 0;
    CopyRect(&m_rcBounds, &rcBounds);
    m_bHidden = FALSE;
    m_vBitmap.clear();
}

Sprite::~Sprite() {
    CleanupBitmap();
}

//-----------------------------------------------------------------
// Sprite Methods
//-----------------------------------------------------------------
void Sprite::UpdateFrame()
{
    if (m_iFrameDelay >= 0 && --m_iFrameTrigger <= 0) {
        // Reset the frame trigger
        m_iFrameTrigger = m_iFrameDelay;

        // Increment the frame
        if (++m_iCurFrame >= m_iNumFrames)
            m_iCurFrame = 0;
    }
}

SPRITEACTION Sprite::Update() {
    // Update the sprite's frame
    UpdateFrame();
    return SA_NONE;
}

void Sprite::Draw(HDC hDC) {
    // Draw the sprite if it is not hidden
    if (m_pBitmap != NULL && !m_bHidden) {
        // Draw the appropriate frame
        if (m_iNumFrames == 1)
            m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, TRUE);
        else
            m_pBitmap->DrawPart(hDC, m_rcPosition.left, m_rcPosition.top, m_iCurFrame * GetWidth(), 0, GetWidth(), GetHeight(), TRUE);
    }
}

void Sprite::CleanupBitmap() {
    // Remove and delete bitmaps
    for (auto siBitmap = m_vBitmap.begin(); siBitmap != m_vBitmap.end();) {
        delete* siBitmap;
        siBitmap = m_vBitmap.erase(siBitmap);
    }
}

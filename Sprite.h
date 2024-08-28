//-----------------------------------------------------------------
// Sprite Object
//-----------------------------------------------------------------
#ifndef SPRITE_H
#define SPRITE_H

#include <windows.h>
#include <vector>
#include "Bitmap.h"

using namespace std;

//-----------------------------------------------------------------
// Data Type
//-----------------------------------------------------------------
using SPRITEACTION = WORD;

const SPRITEACTION SA_NONE = 0x0000L;
const SPRITEACTION SA_KILL = 0x0001L;
                    
//-----------------------------------------------------------------
// Sprite Class
//-----------------------------------------------------------------
class Sprite {
private:
    Bitmap* m_pBitmap; // Pointer to the bitmap
    int m_iNumFrames; // Number of frames in the sprite
    int m_iCurFrame; // Current frame of the sprite
    int m_iFrameDelay; // Delay between frames
    int m_iFrameTrigger; // Frame trigger for animation
    RECT m_rcPosition; // Position of the sprite
    int m_iZOrder; // Z-order of the sprite (for layering)
    RECT m_rcBounds; // Bounds of the sprite
    BOOL m_bHidden; // Visibility flag for the sprite
    vector<Bitmap*> m_vBitmap;

public:
    // Constructor/Destructor
    Sprite(Bitmap* pBitmap, RECT& rcBounds);
    virtual ~Sprite();

    // Methods
    void UpdateFrame(); // Update the sprite's frame
    virtual SPRITEACTION Update(); // Update the sprite's state
    void Draw(HDC hDC); // Draw the sprite
    void CleanupBitmap(); // Cleanup bitmap resources
  
    // Accessors
    void SetNumFrames(int iNumFrames);
    void SetPosition(int x, int y);
    void SetPosition(RECT& rcPosition);

    void SetFrameDelay(int iFrameDelay) { m_iFrameDelay = iFrameDelay; }
    RECT& GetPosition() { return m_rcPosition; }
    int GetZOrder() const { return m_iZOrder; }
    int GetWidth() const { return (m_pBitmap->GetWidth() / m_iNumFrames); }
    int GetHeight() const { return m_pBitmap->GetHeight(); }
};

//-----------------------------------------------------------------
// Sprite Inline Methods
//-----------------------------------------------------------------
/*
inline void Sprite::UpdateFrame()
{
    if (m_iFrameDelay >= 0 && --m_iFrameTrigger <= 0) {
        // Reset the frame trigger
        m_iFrameTrigger = m_iFrameDelay;

        // Increment the frame
        if (++m_iCurFrame >= m_iNumFrames)
            m_iCurFrame = 0;
    }
}
*/
//-----------------------------------------------------------------
// Sprite Inline Accessors
//-----------------------------------------------------------------
inline void Sprite::SetNumFrames(int iNumFrames) {
    // Set the number of frames
    m_iNumFrames = iNumFrames;

    // Recalculate the sprite's position
    RECT rect = GetPosition();
    rect.right = rect.left + ((rect.right - rect.left) / iNumFrames);
    SetPosition(rect);
}

inline void Sprite::SetPosition(int x, int y) {
    OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
}

inline void Sprite::SetPosition(RECT& rcPosition) {
    CopyRect(&m_rcPosition, &rcPosition);
}

#endif

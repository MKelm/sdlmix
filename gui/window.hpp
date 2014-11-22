#ifndef _INCLUDE_GUI_WINDOW_HPP
#define _INCLUDE_GUI_WINDOW_HPP

#include "global.hpp"
#include "element.hpp"

class GuiWindow: public GuiElement {
  protected:
    Uint8 mainFrameBorderWidth;
    int titleFrameIdx;
    SDL_Surface *titleText;
    bool hasTitleText;
    SDL_Surface *closeBtnText;
    bool hasCloseBtnText;
    SDL_Rect closeBtnRect;
    SDL_Rect moveRect;
  public:
    GuiWindow(SDL_Surface *);
    void resetMove();
    void setMove(Uint16, Uint16);
    void setTitle(string, Uint8, string, Uint8, Uint8, Uint8);
    void setCloseBtn(Uint8, string, Uint8, Uint8, Uint8);
    void addWindowFrame(Uint16, Uint16, Uint16, Uint16, Uint8, Uint8, Uint8);
    void setWindowBorder(Uint8, Uint8, Uint8, Uint8);
    void addTitleFrame(int, int, int);
    void unsetTitleText();
    void unsetCloseBtnText();
    virtual void update();
    virtual ~GuiWindow();
};

#endif

#ifndef _INCLUDE_GUI_FRAME_HPP
#define _INCLUDE_GUI_FRAME_HPP

#include "global.hpp"
#include "bgcolor.hpp"

class GuiFrame: public GuiBgColor {
  protected:
    Uint8 bpp;
    SDL_Surface *frame;
    SDL_Rect frameRect;
    SDL_Rect innerRect;
    bool hasFrame;
    bool hasBorder;
    struct stGuiFrameBorderActive {
      bool top;
      bool bottom;
      bool left;
      bool right;
    } borderActive;
    Uint8 borderWidth;
    struct stGuiRgbColor borderColor;
  public:
    GuiFrame(Uint8);
    void set(Uint16, Uint16, Uint16, Uint16);
    void setBorder(Uint8, Uint8, Uint8, Uint8);
    void setBorderActive(bool, bool, bool, bool);
    void move(Uint16, Uint16);
    SDL_Rect *getRect();
    SDL_Rect *getInnerRect();
    SDL_Surface *getSurface();
    void bgFill();
    void drawBorder();
    void unset();
    ~GuiFrame();
};

#endif

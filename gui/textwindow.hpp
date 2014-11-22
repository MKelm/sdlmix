#ifndef _INCLUDE_GUI_TEXT_WINDOW_HPP
#define _INCLUDE_GUI_TEXT_WINDOW_HPP

#include "global.hpp"
#include "window.hpp"

class GuiTextWindow: public GuiWindow {
  protected:
    Uint8 textPadding;
    int textFrameIdx;
    vector<string> textLines;
    Uint8 fontSize;
    string fontFile;
    stGuiRgbColor fontColor;
  public:
    GuiTextWindow(SDL_Surface *);
    void addTextFrame(Uint8, int, int, int);
    vector<string> wrapText(TTF_Font *, const string &, unsigned);
    void setText(string, Uint8, string, Uint8, Uint8, Uint8);
    void unsetText();
    virtual void update();
    virtual ~GuiTextWindow();
};

#endif

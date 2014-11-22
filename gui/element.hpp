#ifndef _INCLUDE_GUI_ELEMENT_HPP
#define _INCLUDE_GUI_ELEMENT_HPP

#include "global.hpp"
#include "eventareas.hpp"
#include "frame.hpp"

class GuiElement {
  protected:
    SDL_Surface *screen;
    int mainFrameIdx;
    vector<GuiFrame *> frames;
  public:
    bool fullUpdate;
    GuiEventAreas eventAreas;
    GuiElement(SDL_Surface *);
    GuiFrame *addFrame(Uint16, Uint16, Uint16, Uint16);
    GuiFrame *addMainFrame(Uint16, Uint16, Uint16, Uint16);
    void setMainFrameBgColor(int, int, int);
    virtual void update();
    virtual ~GuiElement();
};

#endif

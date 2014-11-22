#ifndef _INCLUDE_GUI_TASK_BAR_HPP
#define _INCLUDE_GUI_TASK_BAR_HPP

#include "global.hpp"
#include "element.hpp"

class GuiTaskBar: public GuiElement {
  protected:
    bool isBottom;
    Uint8 height;
  public:
    GuiTaskBar(SDL_Surface *);
    void init(bool, Uint8, Uint8, int, int, int, int, int, int);
    virtual void update();
    virtual ~GuiTaskBar();
};

#endif

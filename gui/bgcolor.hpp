#ifndef _INCLUDE_GUI_BG_COLOR_HPP
#define _INCLUDE_GUI_BG_COLOR_HPP

#include "global.hpp"

class GuiBgColor {
  protected:
    bool hasBgColor;
    struct stGuiRgbColor bgColor;
  public:
    GuiBgColor();
    void setBgColor(int, int, int);
};

#endif

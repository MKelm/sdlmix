#ifndef _INCLUDE_GUI_EVENT_AREAS_HPP
#define _INCLUDE_GUI_EVENT_AREAS_HPP

#include "global.hpp"

class GuiEventAreas {
  protected:
    map<string, SDL_Rect> areas;
  public:
    void add(string, Uint16, Uint16, Uint16, Uint16);
    void update(string, Uint16, Uint16);
    bool isEventInArea(string, Uint16, Uint16);
};

#endif

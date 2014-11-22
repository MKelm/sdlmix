#include "eventareas.hpp"

void GuiEventAreas::add(
         string _name, Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h
       ) {
  SDL_Rect area;
  area.x = _x;
  area.y = _y;
  area.w = _w;
  area.h = _h;
  areas[_name] = area;
}
void GuiEventAreas::update(string _name, Uint16 _x, Uint16 _y) {
  areas[_name].x = _x;
  areas[_name].y = _y;
}
bool GuiEventAreas::isEventInArea(string _name, Uint16 _eventX, Uint16 _eventY) {
  if (_eventX > areas[_name].x &&
      _eventX < areas[_name].x + areas[_name].w &&
      _eventY > areas[_name].y &&
      _eventY < areas[_name].y + areas[_name].h) {
    return true;
  }
  return false;
}

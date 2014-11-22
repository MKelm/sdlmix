#include "bgcolor.hpp"

GuiBgColor::GuiBgColor() {
  hasBgColor = false;
}

void GuiBgColor::setBgColor(int _r, int _g, int _b) {
  if (_r > -1 && _g > -1 && _b > -1) {
    bgColor.r = _r;
    bgColor.g = _g;
    bgColor.b = _b;
    hasBgColor = true;
  }
}

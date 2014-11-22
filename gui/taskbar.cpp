#include "taskbar.hpp"

GuiTaskBar::GuiTaskBar(SDL_Surface *_screen) : GuiElement(_screen) {
}
void GuiTaskBar::init(bool _isBottom, Uint8 _height, Uint8 _borderWidth,
                      int _bgR, int _bgG, int _bgB,
                      int _borderR, int _borderG, int _borderB) {
  isBottom = _isBottom;
  height = _height;
  if (mainFrameIdx == -1) {
    if (isBottom == true)
      addMainFrame(0, screen->h - height, screen->w, screen->h);
    else
      addMainFrame(0, screen->h - height, screen->w, screen->h);
  }
  setMainFrameBgColor(_bgR, _bgG, _bgB);
  if (_borderWidth > 0) {
    frames[mainFrameIdx]->setBorder(_borderWidth, _borderR, _borderG, _borderB);
    frames[mainFrameIdx]->setBorderActive(
      isBottom, !isBottom, false, false
    );
  }
}
void GuiTaskBar::update() {
  GuiElement::update();
}
GuiTaskBar::~GuiTaskBar() {
}

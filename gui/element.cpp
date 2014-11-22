#include "element.hpp"

GuiElement::GuiElement(SDL_Surface *_screen) {
  screen = _screen;
  mainFrameIdx = -1;
  fullUpdate = true;
}

GuiFrame *GuiElement::addFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
  GuiFrame *tempFrame = new GuiFrame(screen->format->BytesPerPixel * 8);
  tempFrame->set(_x, _y, _w, _h);
  frames.push_back(tempFrame);
  return tempFrame;
}

GuiFrame *GuiElement::addMainFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
  GuiFrame *tempFrame = addFrame(_x, _y, _w, _h);
  mainFrameIdx = frames.size() - 1;
  return tempFrame;
}

void GuiElement::setMainFrameBgColor(int _r, int _g, int _b) {
  if (mainFrameIdx > -1) {
    frames[mainFrameIdx]->setBgColor(_r, _g, _b);
  }
}

void GuiElement::update() {
  if (mainFrameIdx > -1) {
    if (fullUpdate == true) {
      frames[mainFrameIdx]->bgFill();
      frames[mainFrameIdx]->drawBorder();
    }
    SDL_Surface *mainFrameSurface = frames[mainFrameIdx]->getSurface();
    if (fullUpdate == true && frames.size() > 0) {
      vector<GuiFrame *>::iterator it;
      int i = 0;
      for (it = frames.begin(); it != frames.end(); it++) {
        if (i != mainFrameIdx) {
          (*it)->bgFill();
          (*it)->drawBorder();
          SDL_BlitSurface((*it)->getSurface(), NULL, mainFrameSurface, (*it)->getRect());
        }
        i++;
      }
    }
    SDL_BlitSurface(mainFrameSurface, NULL, screen, frames[mainFrameIdx]->getRect());
  }
}

GuiElement::~GuiElement() {
  vector<GuiFrame *>::iterator it;
  for (it = frames.begin(); it != frames.end(); it++) {
    delete (*it);
  }
  frames.clear();
}

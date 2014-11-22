#include "window.hpp"

GuiWindow::GuiWindow(SDL_Surface *_screen) : GuiElement(_screen) {
  hasTitleText = false;
  hasCloseBtnText = false;
  titleFrameIdx = -1;
  mainFrameBorderWidth = 0;
  resetMove();
}
void GuiWindow::resetMove() {
  fullUpdate = true;
  moveRect.x = -1;
  moveRect.y = -1;
}
void GuiWindow::setMove(Uint16 _x, Uint16 _y) {
  fullUpdate = false;
  if (moveRect.x > -1 && moveRect.y > -1) {
    SDL_Rect *mainFrameRect = frames[mainFrameIdx]->getRect();
    if (mainFrameRect->x - (moveRect.x - _x) > 0 &&
        mainFrameRect->y - (moveRect.y - _y) > 0 &&
        mainFrameRect->x + mainFrameRect->w - (moveRect.x - _x) < screen->w &&
        mainFrameRect->y + mainFrameRect->h - (moveRect.y - _y) < screen->h) {
      frames[mainFrameIdx]->move(moveRect.x - _x, moveRect.y - _y);
    }
  }
  moveRect.x = _x;
  moveRect.y = _y;
}
void GuiWindow::setTitle(string _title, Uint8 _fontSize, string _fontFile,
                         Uint8 _r, Uint8 _g, Uint8 _b) {
  unsetTitleText();
  TTF_Font *font = TTF_OpenFont(_fontFile.c_str(), _fontSize);
  SDL_Color tmpFontColor = { _r, _g, _b };
  titleText = TTF_RenderText_Blended(font, _title.c_str(), tmpFontColor);
  TTF_CloseFont(font);
  hasTitleText = true;
}
void GuiWindow::setCloseBtn(Uint8 _fontSize, string _fontFile,
                            Uint8 _r, Uint8 _g, Uint8 _b) {
  unsetCloseBtnText();
  TTF_Font *font = TTF_OpenFont(_fontFile.c_str(), _fontSize);
  SDL_Color tmpFontColor = { _r, _g, _b };
  closeBtnText = TTF_RenderText_Blended(font, "X", tmpFontColor);
  TTF_CloseFont(font);
  hasCloseBtnText = true;
}
void GuiWindow::addWindowFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h,
                               Uint8 _r, Uint8 _g, Uint8 _b) {
  addMainFrame(_x, _y, _w, _h);
  setMainFrameBgColor(_r, _g, _b);
}
void GuiWindow::setWindowBorder(Uint8 _width, Uint8 _r, Uint8 _g, Uint8 _b) {
  if (mainFrameIdx > -1) {
    mainFrameBorderWidth = _width;
    frames[mainFrameIdx]->setBorder(mainFrameBorderWidth, _r, _g, _b);
  }
}
void GuiWindow::addTitleFrame(int _r, int _g, int _b) {
  if (mainFrameIdx > -1 && hasTitleText == true) {
    SDL_Rect *innerRect = frames[mainFrameIdx]->getInnerRect();
    addFrame(innerRect->x, innerRect->y, innerRect->w, titleText->h + mainFrameBorderWidth);
    titleFrameIdx = frames.size() - 1;
    frames[titleFrameIdx]->setBgColor(_r, _g, _b);
    innerRect->y += titleText->h + mainFrameBorderWidth;
    innerRect->h -= titleText->h + mainFrameBorderWidth;
    SDL_Rect *titleFrameRect = frames[titleFrameIdx]->getRect();
    SDL_Rect *mainFrameRect = frames[mainFrameIdx]->getRect();
    eventAreas.add(
      "windowMoveBar",
      mainFrameRect->x + titleFrameRect->x,
      mainFrameRect->y + titleFrameRect->y,
      titleFrameRect->w, titleFrameRect->h
    );
    if (hasCloseBtnText == true) {
      SDL_Surface *titleFrameSurface = frames[titleFrameIdx]->getSurface();
      closeBtnRect.x = titleFrameSurface->w - closeBtnText->w;
      closeBtnRect.y = 0;
      closeBtnRect.w = closeBtnText->w;
      closeBtnRect.h = closeBtnText->h;
      eventAreas.add(
        "windowCloseButton",
        mainFrameRect->x + titleFrameRect->x + closeBtnRect.x,
        mainFrameRect->y + titleFrameRect->y + closeBtnRect.y,
        closeBtnRect.w, closeBtnRect.h
      );
    }
  }
}
void GuiWindow::update() {
  GuiElement::update();
  if (titleFrameIdx > -1) {
    SDL_Rect *mainFrameRect = frames[mainFrameIdx]->getRect();
    SDL_Rect *titleFrameRect = frames[titleFrameIdx]->getRect();
    eventAreas.update(
      "windowMoveBar",
      mainFrameRect->x + titleFrameRect->x,
      mainFrameRect->y + titleFrameRect->y
    );
    if (hasCloseBtnText == true) {
      eventAreas.update(
        "windowCloseButton",
        mainFrameRect->x + titleFrameRect->x + closeBtnRect.x,
        mainFrameRect->y + titleFrameRect->y + closeBtnRect.y
      );
    }
    if (fullUpdate == true) {
      SDL_Surface *titleFrameSurface = frames[titleFrameIdx]->getSurface();
      if (hasTitleText == true) {
        SDL_BlitSurface(titleText, NULL, titleFrameSurface, NULL);
      }
      if (hasCloseBtnText == true) {
        SDL_BlitSurface(closeBtnText, NULL, titleFrameSurface, &closeBtnRect);
      }
      SDL_Surface *mainFrameSurface = frames[mainFrameIdx]->getSurface();
      SDL_BlitSurface(
        titleFrameSurface, NULL, mainFrameSurface, titleFrameRect
      );
      SDL_BlitSurface(mainFrameSurface, NULL, screen, frames[mainFrameIdx]->getRect());
    }
  }
}
void GuiWindow::unsetTitleText() {
  if (hasTitleText == true) {
    SDL_FreeSurface(titleText);
    hasTitleText = false;
  }
}
void GuiWindow::unsetCloseBtnText() {
  if (hasCloseBtnText == true) {
    SDL_FreeSurface(closeBtnText);
    hasCloseBtnText = false;
  }
}
GuiWindow::~GuiWindow() {
  unsetTitleText();
  unsetCloseBtnText();
}

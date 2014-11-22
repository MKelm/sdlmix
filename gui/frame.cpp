#include "frame.hpp"

GuiFrame::GuiFrame(Uint8 _bpp) {
  bpp = _bpp;
  hasFrame = false;
  hasBorder = false;
  setBorderActive(true, true, true, true);
}

void GuiFrame::set(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
  unset();
  frameRect.x = _x;
  frameRect.y = _y;
  frameRect.w = _w;
  frameRect.h = _h;
  innerRect.x = 0;
  innerRect.y = 0;
  innerRect.w = _w;
  innerRect.h = _h;
  frame = SDL_CreateRGBSurface(
    0, frameRect.w, frameRect.h, bpp, 0, 0, 0, 0
  );
  hasFrame = true;
}

void GuiFrame::setBorder(Uint8 _width, Uint8 _r, Uint8 _g, Uint8 _b) {
  borderWidth = _width;
  borderColor.r = _r;
  borderColor.g = _g;
  borderColor.b = _b;
  hasBorder = true;
  innerRect.x += _width;
  innerRect.y += _width;
  innerRect.w = frameRect.w - 1 - 2 * _width;
  innerRect.h = frameRect.h - 1 - 2 * _width;
}

void GuiFrame::setBorderActive(bool _top, bool _bottom, bool _left, bool _right) {
  borderActive.top = _top;
  borderActive.bottom = _bottom;
  borderActive.left = _left;
  borderActive.right = _right;
}

void GuiFrame::move(Uint16 _x, Uint16 _y) {
  frameRect.x -= _x;
  frameRect.y -= _y;
}

SDL_Rect *GuiFrame::getRect() {
  return &frameRect;
}

SDL_Rect *GuiFrame::getInnerRect() {
  return &innerRect;
}

SDL_Surface *GuiFrame::getSurface() {
  return frame;
}

void GuiFrame::bgFill() {
  if (hasBgColor == true) {
    SDL_FillRect(
      frame, &frame->clip_rect,
      SDL_MapRGB(frame->format, bgColor.r, bgColor.g, bgColor.b)
    );
  }
}

void GuiFrame::drawBorder() {
  if (hasBorder == true) {
    if (borderActive.top == true)
      boxRGBA(
        frame, 0, 0, frame->clip_rect.w - 1, borderWidth,
        borderColor.r, borderColor.g, borderColor.b, 255
      );
    if (borderActive.left == true)
      boxRGBA(
        frame,
        0, 0,
        borderWidth, frame->clip_rect.h - 1,
        borderColor.r, borderColor.g, borderColor.b, 255
      );
    if (borderActive.bottom == true) {
      boxRGBA(
        frame,
        0, frame->clip_rect.h - 1 - borderWidth,
        frame->clip_rect.w - 1, frame->clip_rect.h - 1,
        borderColor.r, borderColor.g, borderColor.b, 255
      );
    }
    if (borderActive.right == true)
      boxRGBA(
        frame,
        frame->clip_rect.w - 1 - borderWidth, 0,
        frame->clip_rect.w - 1, frame->clip_rect.h - 1,
        borderColor.r, borderColor.g, borderColor.b, 255
      );
  }
}

void GuiFrame::unset() {
  if (hasFrame == true) {
    SDL_FreeSurface(frame);
    hasFrame = false;
  }
}

GuiFrame::~GuiFrame() {
  unset();
}

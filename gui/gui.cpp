#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <ctime>
#include <cmath>
#include <random>
#include <list>
#include <iostream>
using namespace std;

class GuiBase {
  public:
    SDL_Surface *screen;
    bool hasFrame;
    SDL_Surface *frame;
    SDL_Rect frameRect;
    GuiBase(SDL_Surface *);
    virtual void setFrame();
    virtual void bgFillFrame(Uint8, Uint8, Uint8);
    virtual void bgFillScreen(Uint8, Uint8, Uint8);
    virtual void update();
    virtual void unsetFrame();
    virtual ~GuiBase();
};

GuiBase::GuiBase(SDL_Surface *_screen) {
  screen = _screen;
  hasFrame = false;
}
void GuiBase::setFrame() {
  unsetFrame();
  frame = SDL_CreateRGBSurface(
    0, frameRect.w, frameRect.h, screen->format->BytesPerPixel * 8, 0, 0, 0, 0
  );
  hasFrame = true;
}
void GuiBase::bgFillFrame(Uint8 _r, Uint8 _g, Uint8 _b) {
  SDL_FillRect(
    frame, &frame->clip_rect, SDL_MapRGB(frame->format, _r, _g, _b)
  );
}
void GuiBase::bgFillScreen(Uint8 _r, Uint8 _g, Uint8 _b) {
  SDL_FillRect(
    screen, &screen->clip_rect, SDL_MapRGB(screen->format, _r, _g, _b)
  );
}
void GuiBase::update() {
  if (hasFrame == true) {
    SDL_BlitSurface(frame, NULL, screen, &frameRect);
    SDL_Flip(frame);
  }
  SDL_Flip(screen);
}
void GuiBase::unsetFrame() {
  if (hasFrame == true)
    SDL_FreeSurface(frame);
}
GuiBase::~GuiBase() {
  unsetFrame();
}

int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  GuiBase *gui = new GuiBase(screen);
  gui->frameRect.x = 200;
  gui->frameRect.y = 100;
  gui->frameRect.w = 100;
  gui->frameRect.h = 100;
  gui->setFrame();

  gui->bgFillScreen(255, 0, 0);
  gui->bgFillFrame(0, 255, 0);
  gui->update();

  SDL_Event event;
  bool quit = false;
  while (quit == false) {
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = 1;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_r:
              gui->bgFillScreen(255, 0, 0);
              gui->update();
              break;
            case SDLK_ESCAPE:
            case SDLK_q:
              quit = 1;
              break;
            default:
              break;
          }
          break;
      }
    }
  }

  delete gui;
  SDL_Quit();
  return 0;
}

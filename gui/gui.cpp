#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <ctime>
#include <cmath>
#include <random>
#include <list>
#include <iostream>
using namespace std;

class GuiBase {
  protected:
    SDL_Surface *screen;
    SDL_Surface *frame;
    bool hasFrame;
  public:
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
  }
  SDL_Flip(screen);
}
void GuiBase::unsetFrame() {
  if (hasFrame == true) {
    SDL_FreeSurface(frame);
    hasFrame = false;
  }
}
GuiBase::~GuiBase() {
  unsetFrame();
}

class GuiWindow: public GuiBase {
  protected:
    SDL_Surface *titleFrame;
    bool hasTitleFrame;
    SDL_Rect titleFrameRect;
  public:
    string title;
    struct stTitleColor {
      Uint8 r;
      Uint8 g;
      Uint8 b;
    } titleColor;
    GuiWindow(SDL_Surface *);
    void setTitleFrame();
    void bgFillTitleFrame(Uint8, Uint8, Uint8);
    void update();
    void unsetTitleFrame();
    ~GuiWindow();
};
GuiWindow::GuiWindow(SDL_Surface *_screen) : GuiBase(_screen) {
  hasTitleFrame = false;
}
void GuiWindow::setTitleFrame() {
  unsetTitleFrame();
  titleFrameRect.w = (hasFrame == true) ? frameRect.w : screen->w;
  titleFrameRect.h = 20; // todo: use font size / height here
  titleFrameRect.x = 0;
  titleFrameRect.y = 0;
  titleFrame = SDL_CreateRGBSurface(
    0, titleFrameRect.w, titleFrameRect.h,
    screen->format->BytesPerPixel * 8, 0, 0, 0, 0
  );
  hasTitleFrame = true;
}
void GuiWindow::bgFillTitleFrame(Uint8 _r, Uint8 _g, Uint8 _b) {
  SDL_FillRect(
    titleFrame, &titleFrame->clip_rect,
    SDL_MapRGB(titleFrame->format, _r, _g, _b)
  );
}
void GuiWindow::update() {
  if (hasTitleFrame == true) {
    if (hasFrame == true) {
      SDL_BlitSurface(titleFrame, NULL, frame, &titleFrameRect);
    } else {
      SDL_BlitSurface(titleFrame, NULL, screen, &titleFrameRect);
    }
  }
  GuiBase::update();
}
void GuiWindow::unsetTitleFrame() {
  if (hasTitleFrame == true) {
    SDL_FreeSurface(titleFrame);
    hasTitleFrame = false;
  }
}
GuiWindow::~GuiWindow() {
  unsetTitleFrame();
}

int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  GuiWindow *gui = new GuiWindow(screen);
  gui->frameRect.x = screen->w / 2 - 150;
  gui->frameRect.y = screen->h / 2 - 150;
  gui->frameRect.w = 300;
  gui->frameRect.h = 300;
  gui->setFrame();

  gui->title = "Test Window";
  gui->titleColor.r = 0;
  gui->titleColor.g = 0;
  gui->titleColor.b = 0;
  gui->setTitleFrame();

  gui->bgFillScreen(255, 0, 0);
  gui->bgFillFrame(0, 255, 0);
  gui->bgFillTitleFrame(0, 0, 255);
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

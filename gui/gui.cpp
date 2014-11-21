#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <list>
#include <iostream>
using namespace std;

class GuiBase {
  protected:
    SDL_Surface *screen;
    SDL_Surface *frame;
    SDL_Rect frameRect;
    bool hasFrame;
  public:
    GuiBase(SDL_Surface *);
    virtual void setFrame(Uint16, Uint16, Uint16, Uint16);
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
void GuiBase::setFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
  unsetFrame();
  frameRect.x = _x;
  frameRect.y = _y;
  frameRect.w = _w;
  frameRect.h = _h;
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
    SDL_Rect titleFrameRect;
    bool hasTitleFrame;
    SDL_Surface *titleText;
    bool hasTitleText;
  public:
    GuiWindow(SDL_Surface *);
    void setTitle(string, Uint8, Uint8, Uint8, Uint8, string);
    void setTitleFrame();
    void bgFillTitleFrame(Uint8, Uint8, Uint8);
    void update();
    void unsetTitleText();
    void unsetTitleFrame();
    ~GuiWindow();
};
GuiWindow::GuiWindow(SDL_Surface *_screen) : GuiBase(_screen) {
  hasTitleFrame = false;
  hasTitleText = false;
}
void GuiWindow::setTitle(string _title, Uint8 _r, Uint8 _g, Uint8 _b,
                         Uint8 _fontSize, string _fontFile) {
  unsetTitleText();
  TTF_Font *font = TTF_OpenFont(_fontFile.c_str(), _fontSize);
  SDL_Color tmpFontColor = { _r, _g, _b };
  titleText = TTF_RenderText_Solid(font, _title.c_str(), tmpFontColor);
  TTF_CloseFont(font);
  hasTitleText = true;
}
void GuiWindow::setTitleFrame() {
  if (hasTitleText == true) {
    unsetTitleFrame();
    titleFrameRect.w = (hasFrame == true) ? frameRect.w : screen->w;
    titleFrameRect.h = titleText->h;
    titleFrameRect.x = 0;
    titleFrameRect.y = 0;
    titleFrame = SDL_CreateRGBSurface(
      0, titleFrameRect.w, titleFrameRect.h,
      screen->format->BytesPerPixel * 8, 0, 0, 0, 0
    );
    hasTitleFrame = true;
  }
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
  if (hasTitleText == true) {
    if (hasFrame == true) {
      SDL_BlitSurface(titleText, NULL, frame, NULL);
    } else {
      SDL_BlitSurface(titleText, NULL, screen, NULL);
    }
  }
  GuiBase::update();
}
void GuiWindow::unsetTitleText() {
  if (hasTitleText == true) {
    SDL_FreeSurface(titleText);
    hasTitleText = false;
  }
}
void GuiWindow::unsetTitleFrame() {
  if (hasTitleFrame == true) {
    SDL_FreeSurface(titleFrame);
    hasTitleFrame = false;
  }
}
GuiWindow::~GuiWindow() {
  unsetTitleFrame();
  unsetTitleText();
}

int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  GuiWindow *gui = new GuiWindow(screen);
  gui->setFrame(
    screen->w / 2 - 150, screen->h / 2 - 150, 300, 300
  );

  gui->setTitle("Test Window", 0, 0, 0, 24, "mkds.ttf");
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
  TTF_Quit();
  SDL_Quit();
  return 0;
}

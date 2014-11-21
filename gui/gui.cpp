#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <list>
#include <iostream>
using namespace std;

// ----> GUI FRAME
class GuiFrame {
  protected:
    Uint8 bpp;
    SDL_Surface *frame;
    SDL_Rect frameRect;
    bool hasFrame;
  public:
    GuiFrame(Uint8);
    virtual void set(Uint16, Uint16, Uint16, Uint16);
    virtual SDL_Rect *getRect();
    virtual SDL_Surface *getSurface();
    virtual void bgFill(Uint8, Uint8, Uint8);
    virtual void unset();
    virtual ~GuiFrame();
};
GuiFrame::GuiFrame(Uint8 _bpp) {
  bpp = _bpp;
  hasFrame = false;
}
void GuiFrame::set(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
  unset();
  frameRect.x = _x;
  frameRect.y = _y;
  frameRect.w = _w;
  frameRect.h = _h;
  frame = SDL_CreateRGBSurface(
    0, frameRect.w, frameRect.h, bpp, 0, 0, 0, 0
  );
  hasFrame = true;
}
SDL_Rect *GuiFrame::getRect() {
  return &frameRect;
}
SDL_Surface *GuiFrame::getSurface() {
  return frame;
}
void GuiFrame::bgFill(Uint8 _r, Uint8 _g, Uint8 _b) {
  SDL_FillRect(
    frame, &frame->clip_rect, SDL_MapRGB(frame->format, _r, _g, _b)
  );
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

// ----> GUI SCREEN
class GuiScreen {
  protected:
    SDL_Surface *screen;
    list<GuiFrame *> frames;
  public:
    GuiScreen(SDL_Surface *);
    virtual GuiFrame *addFrame(string, Uint16, Uint16, Uint16, Uint16);
    virtual void bgFill(Uint8, Uint8, Uint8);
    virtual void update();
    virtual ~GuiScreen();
};
GuiScreen::GuiScreen(SDL_Surface *_screen) {
  screen = _screen;
}
GuiFrame *GuiScreen::addFrame(string name, Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
  GuiFrame *tempFrame = new GuiFrame(screen->format->BytesPerPixel * 8);
  tempFrame->set(_x, _y, _w, _h);
  frames.push_back(tempFrame);
  return tempFrame;
}
void GuiScreen::bgFill(Uint8 _r, Uint8 _g, Uint8 _b) {
  SDL_FillRect(
    screen, &screen->clip_rect, SDL_MapRGB(screen->format, _r, _g, _b)
  );
}
void GuiScreen::update() {
  if (frames.size() > 0) {
    list<GuiFrame *>::iterator it;
    for (it = frames.begin(); it != frames.end(); it++) {
      SDL_BlitSurface((*it)->getSurface(), NULL, screen, (*it)->getRect());
    }
  }
  SDL_Flip(screen);
}
GuiScreen::~GuiScreen() {
  list<GuiFrame *>::iterator it;
  for (it = frames.begin(); it != frames.end(); it++) {
    delete (*it);
  }
  frames.clear();
}

/* ----> GUI WINDOW
class GuiWindow: public GuiScreen {
  protected:
    bool hasTitleFrame;
    SDL_Surface *titleText;
    bool hasTitleText;
  public:
    GuiWindow(SDL_Surface *);
    void setTitle(string, Uint8, Uint8, Uint8, Uint8, string);
    void setTitleFrame();
    void fillTitleFrame(Uint8, Uint8, Uint8);
    void update();
    void unsetTitleText();
    ~GuiWindow();
};
GuiWindow::GuiWindow(SDL_Surface *_screen) : GuiScreen(_screen) {
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

    addFrame(
      "title", (hasFrame == true) ? frameRect.w : screen->w, Uint16 _y, Uint16 _w, Uint16 _h)

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
}*/

int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  GuiScreen *gui = new GuiScreen(screen);
  GuiFrame *windowFrame = gui->addFrame(
    "window",
    screen->w / 2 - 150, screen->h / 2 - 150,
    300, 300
  );
  GuiFrame *titleFrame = gui->addFrame(
    "title",
    screen->w / 2 - 150, screen->h / 2 - 150,
    300, 20
  );

  gui->bgFill(255, 0, 0);
  windowFrame->bgFill(0, 255, 0);
  titleFrame->bgFill(0, 0, 255);
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
            case SDLK_ESCAPE:
            case SDLK_q:
              quit = true;
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

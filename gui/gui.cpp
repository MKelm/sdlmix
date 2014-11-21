#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <vector>
#include <iostream>
using namespace std;

// ----> GUI FRAME
class GuiFrame {
  protected:
    Uint8 bpp;
    SDL_Surface *frame;
    SDL_Rect frameRect;
    bool hasFrame;
    bool hasBorder;
    Uint8 borderWidth;
    struct stGuiFrameBorderColor {
      Uint8 r;
      Uint8 g;
      Uint8 b;
    } borderColor;
  public:
    GuiFrame(Uint8);
    virtual void set(Uint16, Uint16, Uint16, Uint16);
    virtual void setBorder(Uint8, Uint8, Uint8, Uint8);
    virtual SDL_Rect *getRect();
    virtual SDL_Surface *getSurface();
    virtual void bgFill(Uint8, Uint8, Uint8);
    virtual void drawBorder();
    virtual void unset();
    virtual ~GuiFrame();
};
GuiFrame::GuiFrame(Uint8 _bpp) {
  bpp = _bpp;
  hasFrame = false;
  hasBorder = false;
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
void GuiFrame::setBorder(Uint8 _width, Uint8 _r, Uint8 _g, Uint8 _b) {
  borderWidth = _width;
  borderColor.r = _r;
  borderColor.g = _g;
  borderColor.b = _b;
  hasBorder = true;
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
void GuiFrame::drawBorder() {
  if (hasBorder == true) {
    boxRGBA(
      frame, 0, 0, frame->clip_rect.w - 1, borderWidth,
      borderColor.r, borderColor.g, borderColor.b, 255
    ); // top
    boxRGBA(
      frame,
      0, 0,
      borderWidth, frame->clip_rect.h - 1,
      borderColor.r, borderColor.g, borderColor.b, 255
    ); // left
    boxRGBA(
      frame,
      0, frame->clip_rect.h - 1 - borderWidth,
      frame->clip_rect.w - 1, frame->clip_rect.h - 1,
      borderColor.r, borderColor.g, borderColor.b, 255
    ); // bottom
    boxRGBA(
      frame,
      frame->clip_rect.w - 1 - borderWidth, 0,
      frame->clip_rect.w - 1, frame->clip_rect.h - 1,
      borderColor.r, borderColor.g, borderColor.b, 255
    ); // bottom
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

// ----> GUI SCREEN
class GuiScreen {
  protected:
    SDL_Surface *screen;
    vector<GuiFrame *> frames;
  public:
    GuiScreen(SDL_Surface *);
    virtual GuiFrame *addFrame(Uint16, Uint16, Uint16, Uint16);
    virtual void bgFill(Uint8, Uint8, Uint8);
    virtual void update();
    virtual ~GuiScreen();
};
GuiScreen::GuiScreen(SDL_Surface *_screen) {
  screen = _screen;
}
GuiFrame *GuiScreen::addFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
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
    vector<GuiFrame *>::iterator it;
    for (it = frames.begin(); it != frames.end(); it++) {
      (*it)->drawBorder();
      SDL_BlitSurface((*it)->getSurface(), NULL, screen, (*it)->getRect());
    }
  }
  SDL_Flip(screen);
}
GuiScreen::~GuiScreen() {
  vector<GuiFrame *>::iterator it;
  for (it = frames.begin(); it != frames.end(); it++) {
    delete (*it);
  }
  frames.clear();
}

// ----> GUI WINDOW
class GuiWindow: public GuiScreen {
  protected:
    int windowFrameIdx;
    int titleFrameIdx;
    SDL_Surface *titleText;
    bool hasTitleText;
  public:
    GuiWindow(SDL_Surface *);
    void setTitle(string, Uint8, string, Uint8, Uint8, Uint8);
    void addWindowFrame(Uint16, Uint16, Uint16, Uint16, Uint8, Uint8, Uint8);
    void setWindowBorder(Uint8, Uint8, Uint8, Uint8);
    void addTitleFrame(Uint8, Uint8, Uint8);
    void update();
    void unsetTitleText();
    ~GuiWindow();
};
GuiWindow::GuiWindow(SDL_Surface *_screen) : GuiScreen(_screen) {
  hasTitleText = false;
  windowFrameIdx = -1;
  titleFrameIdx = -1;
}
void GuiWindow::setTitle(string _title, Uint8 _fontSize, string _fontFile,
                         Uint8 _r, Uint8 _g, Uint8 _b) {
  unsetTitleText();
  TTF_Font *font = TTF_OpenFont(_fontFile.c_str(), _fontSize);
  SDL_Color tmpFontColor = { _r, _g, _b };
  titleText = TTF_RenderText_Solid(font, _title.c_str(), tmpFontColor);
  TTF_CloseFont(font);
  hasTitleText = true;
}
void GuiWindow::addWindowFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h,
                               Uint8 _r, Uint8 _g, Uint8 _b) {
  addFrame(_x, _y, _w, _h);
  windowFrameIdx = frames.size() - 1;
  frames[windowFrameIdx]->bgFill(_r, _g, _b);
}
void GuiWindow::setWindowBorder(Uint8 _width, Uint8 _r, Uint8 _g, Uint8 _b) {
  frames[windowFrameIdx]->setBorder(_width, _r, _g, _b);
}
void GuiWindow::addTitleFrame(Uint8 _r, Uint8 _g, Uint8 _b) {
  if (hasTitleText == true) {
    SDL_Rect *windowRect = frames[windowFrameIdx]->getRect();
    addFrame(windowRect->x, windowRect->y, windowRect->w, titleText->h);
    titleFrameIdx = frames.size() - 1;
    frames[titleFrameIdx]->bgFill(_r, _g, _b);
  }
}
void GuiWindow::update() {
  if (hasTitleText == true) {
    SDL_BlitSurface(titleText, NULL, frames[titleFrameIdx]->getSurface(), NULL);
  }
  GuiScreen::update();
}
void GuiWindow::unsetTitleText() {
  if (hasTitleText == true) {
    SDL_FreeSurface(titleText);
    hasTitleText = false;
  }
}
GuiWindow::~GuiWindow() {
  unsetTitleText();
}

// ----> MAIN
int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  GuiWindow *gui = new GuiWindow(screen);
  gui->setTitle("TEST WINDOW", 24, "mkds.ttf", 0, 0, 0);
  gui->addWindowFrame(screen->w / 2 - 150, screen->h / 2 - 150, 300, 300, 0, 0, 0);
  gui->setWindowBorder(5, 255, 255, 255);
  gui->addTitleFrame(255, 255, 255);
  gui->bgFill(120, 120, 120);
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

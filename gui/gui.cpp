#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <vector>
#include <iostream>
using namespace std;

struct stGuiRgbColor {
  Uint8 r;
  Uint8 g;
  Uint8 b;
};

// ----> GUI BGCOLOR
class GuiBgColor {
  protected:
    bool hasBgColor;
    struct stGuiRgbColor bgColor;
  public:
    GuiBgColor();
    void setBgColor(int, int, int);
};
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

// ----> GUI FRAME
class GuiFrame: public GuiBgColor {
  protected:
    Uint8 bpp;
    SDL_Surface *frame;
    SDL_Rect frameRect;
    SDL_Rect innerFrameRect;
    bool hasFrame;
    bool hasBorder;
    Uint8 borderWidth;
    struct stGuiRgbColor borderColor;
  public:
    GuiFrame(Uint8);
    void set(Uint16, Uint16, Uint16, Uint16);
    void setBorder(Uint8, Uint8, Uint8, Uint8);
    SDL_Rect *getRect();
    SDL_Rect *getInnerRect();
    SDL_Surface *getSurface();
    void bgFill();
    void drawBorder();
    void unset();
    ~GuiFrame();
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
  innerFrameRect = frameRect;
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
  innerFrameRect.x += _width;
  innerFrameRect.y += _width;
  innerFrameRect.w = frameRect.w - 1 - 2 * _width;
  innerFrameRect.h = frameRect.h - 1 - 2 * _width;
}
SDL_Rect *GuiFrame::getRect() {
  return &frameRect;
}
SDL_Rect *GuiFrame::getInnerRect() {
  return &innerFrameRect;
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
class GuiScreen: public GuiBgColor {
  protected:
    SDL_Surface *screen;
    vector<GuiFrame *> frames;
  public:
    GuiScreen(SDL_Surface *);
    GuiFrame *addFrame(Uint16, Uint16, Uint16, Uint16);
    void bgFill();
    virtual void update();
    virtual ~GuiScreen();
};
GuiScreen::GuiScreen(SDL_Surface *_screen) {
  screen = _screen;
  hasBgColor = false;
}
GuiFrame *GuiScreen::addFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h) {
  GuiFrame *tempFrame = new GuiFrame(screen->format->BytesPerPixel * 8);
  tempFrame->set(_x, _y, _w, _h);
  frames.push_back(tempFrame);
  return tempFrame;
}
void GuiScreen::bgFill() {
  if (hasBgColor == true) {
    SDL_FillRect(
      screen, &screen->clip_rect,
      SDL_MapRGB(screen->format,  bgColor.r,  bgColor.g, bgColor.b)
    );
  }
}
void GuiScreen::update() {
  bgFill();
  if (frames.size() > 0) {
    vector<GuiFrame *>::iterator it;
    for (it = frames.begin(); it != frames.end(); it++) {
      (*it)->bgFill();
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
    Uint8 windowBorderWidth;
    int titleFrameIdx;
    SDL_Surface *titleText;
    bool hasTitleText;
    SDL_Rect *innerRect;
  public:
    GuiWindow(SDL_Surface *);
    void setTitle(string, Uint8, string, Uint8, Uint8, Uint8);
    void addWindowFrame(Uint16, Uint16, Uint16, Uint16, Uint8, Uint8, Uint8);
    void setWindowBorder(Uint8, Uint8, Uint8, Uint8);
    void addTitleFrame(int, int, int);
    void unsetTitleText();
    virtual void update();
    virtual ~GuiWindow();
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
  titleText = TTF_RenderText_Blended(font, _title.c_str(), tmpFontColor);
  TTF_CloseFont(font);
  hasTitleText = true;
}
void GuiWindow::addWindowFrame(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h,
                               Uint8 _r, Uint8 _g, Uint8 _b) {
  addFrame(_x, _y, _w, _h);
  windowFrameIdx = frames.size() - 1;
  frames[windowFrameIdx]->setBgColor(_r, _g, _b);
  innerRect = frames[windowFrameIdx]->getInnerRect();
}
void GuiWindow::setWindowBorder(Uint8 _width, Uint8 _r, Uint8 _g, Uint8 _b) {
  if (windowFrameIdx > -1) {
    windowBorderWidth = _width;
    frames[windowFrameIdx]->setBorder(_width, _r, _g, _b);
    innerRect = frames[windowFrameIdx]->getInnerRect();
  }
}
void GuiWindow::addTitleFrame(int _r, int _g, int _b) {
  if (hasTitleText == true && windowFrameIdx > -1) {
    addFrame(innerRect->x, innerRect->y, innerRect->w, titleText->h + windowBorderWidth);
    titleFrameIdx = frames.size() - 1;
    frames[titleFrameIdx]->setBgColor(_r, _g, _b);
    innerRect->y += titleText->h + windowBorderWidth;
  }
}
void GuiWindow::update() {
  GuiScreen::update();
  if (hasTitleText == true) {
    SDL_Surface *titleFrameSurface = frames[titleFrameIdx]->getSurface();
    SDL_BlitSurface(titleText, NULL, titleFrameSurface, NULL);
    SDL_BlitSurface(titleFrameSurface, NULL, screen, frames[titleFrameIdx]->getRect());
    SDL_Flip(screen);
  }
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

// ----> GUI TEXT
class GuiTextWindow: public GuiWindow {
  protected:
    Uint8 textPadding;
    SDL_Surface *text;
    bool hasText;
  public:
    GuiTextWindow(SDL_Surface *);
    void setTextPadding(Uint8);
    vector<string> wrapText(TTF_Font *, const string &, unsigned);
    void setText(string, Uint8, string, Uint8, Uint8, Uint8);
    void unsetText();
    virtual void update();
    virtual ~GuiTextWindow();
};
GuiTextWindow::GuiTextWindow(SDL_Surface *_screen) : GuiWindow(_screen) {
  hasText = false;
}
void GuiTextWindow::setTextPadding(Uint8 _padding) {
  innerRect->w -= _padding * 2;
  innerRect->h -= _padding * 2;
  innerRect->x += _padding;
  innerRect->y += _padding;
}
vector<string> GuiTextWindow::wrapText(
                 TTF_Font *_font, const string &_text, unsigned _maxWidth
               ) {
  vector<string> lines;
  string tmpStr;
  int linePos, pos, rfindLength, textWidth, textHeight;
  pos = linePos = 0;
  while (pos <= _text.length()) {
    tmpStr = _text.substr(linePos, pos-linePos);
    if (tmpStr.length() > 0) {
      TTF_SizeText(_font, tmpStr.c_str(), &textWidth, &textHeight);
      if (textWidth > _maxWidth) {
        rfindLength = _text.rfind(' ', pos);
        tmpStr = _text.substr(linePos, rfindLength - linePos);
        lines.push_back(tmpStr);
        linePos = rfindLength + 1;
        pos = linePos;
      } else if (pos == _text.length()) {
        lines.push_back(tmpStr);
      }
    }
    pos++;
  }
  return lines;
}
void GuiTextWindow::setText(string _text, Uint8 _fontSize, string _fontFile,
                            Uint8 _r, Uint8 _g, Uint8 _b) {
  unsetText();
  TTF_Font *font = TTF_OpenFont(_fontFile.c_str(), _fontSize);
  SDL_Color tmpFontColor = { _r, _g, _b };
  SDL_Surface *tempText = SDL_CreateRGBSurface(
    0, innerRect->w, innerRect->h - titleText->h - windowBorderWidth,
    screen->format->BytesPerPixel * 8, 0, 0, 0, 0
  );
  SDL_Rect tempTextRect;
  tempTextRect.x = 0;
  tempTextRect.y = 0;
  vector<string> lines = wrapText(font, _text, innerRect->w);
  for (Uint8 i = 0; i < lines.size(); i++) {
    if (i > 0)
      SDL_FreeSurface(text);
    text = TTF_RenderText_Blended(font, lines[i].c_str(), tmpFontColor);
    SDL_BlitSurface(text, NULL, tempText, &tempTextRect);
    tempTextRect.y += text->h;
  };
  SDL_FreeSurface(text);
  text = tempText;
  TTF_CloseFont(font);
  hasText = true;
}
void GuiTextWindow::unsetText() {
  if (hasText == true) {
    SDL_FreeSurface(text);
    hasText = false;
  }
}
void GuiTextWindow::update() {
  GuiWindow::update();
  if (hasText == true) {
    SDL_BlitSurface(text, NULL, screen, innerRect);
  }
  SDL_Flip(screen);
}
GuiTextWindow::~GuiTextWindow() {
  unsetText();
}

// ----> MAIN
int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  GuiTextWindow *gui = new GuiTextWindow(screen);
  gui->setTitle("TEST WINDOW", 20, "libertysans.ttf", 0, 0, 0);
  gui->addWindowFrame(screen->w / 2 - 150, screen->h / 2 - 150, 300, 300, 0, 0, 0);
  gui->setWindowBorder(5, 255, 255, 255);
  gui->addTitleFrame(255, 255, 255);
  gui->setTextPadding(5);
  gui->setText(
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce maximus, diam eget congue malesuada, eros mi maximus leo, vel ultrices leo turpis tempus ligula. Nunc pharetra commodo lorem, quis pharetra ligula. Aenean vel metus commodo eros convallis euismod.",
    16, "libertysans.ttf", 255, 255, 255
  );
  gui->setBgColor(120, 120, 120);
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
              gui->update();
              break;
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

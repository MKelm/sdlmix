#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <map>
#include <vector>
#include <iostream>
using namespace std;

// ----> GUI BGCOLOR
struct stGuiRgbColor {
  Uint8 r;
  Uint8 g;
  Uint8 b;
};
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

// ----> GUI EVENT AREAS
class GuiEventAreas {
  protected:
    map<string, SDL_Rect> areas;
  public:
    void add(string, Uint16, Uint16, Uint16, Uint16);
    void update(string, Uint16, Uint16);
    bool isEventInArea(string, Uint16, Uint16);
};
void GuiEventAreas::add(
         string _name, Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h
       ) {
  SDL_Rect area;
  area.x = _x;
  area.y = _y;
  area.w = _w;
  area.h = _h;
  areas[_name] = area;
}
void GuiEventAreas::update(string _name, Uint16 _x, Uint16 _y) {
  areas[_name].x = _x;
  areas[_name].y = _y;
}
bool GuiEventAreas::isEventInArea(string _name, Uint16 _eventX, Uint16 _eventY) {
  if (_eventX > areas[_name].x &&
      _eventX < areas[_name].x + areas[_name].w &&
      _eventY > areas[_name].y &&
      _eventY < areas[_name].y + areas[_name].w) {
    return true;
  }
  return false;
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
    void setPosition(Uint16, Uint16);
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
void GuiFrame::setPosition(Uint16 _x, Uint16 _y) {
  frameRect.x = _x;
  frameRect.y = _y;
  innerFrameRect.x = _x;
  innerFrameRect.y = _y;
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
    bool redrawOnUpdate;
    GuiEventAreas eventAreas;
    GuiScreen(SDL_Surface *);
    GuiFrame *addFrame(Uint16, Uint16, Uint16, Uint16);
    void bgFill();
    virtual void update();
    virtual ~GuiScreen();
};
GuiScreen::GuiScreen(SDL_Surface *_screen) {
  screen = _screen;
  hasBgColor = false;
  redrawOnUpdate = true;
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
      if (redrawOnUpdate == true) {
        (*it)->bgFill();
        (*it)->drawBorder();
      }
      SDL_BlitSurface((*it)->getSurface(), NULL, screen, (*it)->getRect());
    }
  }
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
    SDL_Surface *closeBtnText;
    bool hasCloseBtnText;
    SDL_Rect closeBtnRect;
    SDL_Rect moveRect;
    SDL_Rect *innerRect;
  public:
    GuiWindow(SDL_Surface *);
    void setPosition(Uint16, Uint16);
    void setTitle(string, Uint8, string, Uint8, Uint8, Uint8);
    void setCloseBtn(Uint8, string, Uint8, Uint8, Uint8);
    void addWindowFrame(Uint16, Uint16, Uint16, Uint16, Uint8, Uint8, Uint8);
    void setWindowBorder(Uint8, Uint8, Uint8, Uint8);
    void addTitleFrame(int, int, int);
    void unsetTitleText();
    void unsetCloseBtnText();
    virtual void update();
    virtual ~GuiWindow();
};
GuiWindow::GuiWindow(SDL_Surface *_screen) : GuiScreen(_screen) {
  hasTitleText = false;
  hasCloseBtnText = false;
  windowFrameIdx = -1;
  titleFrameIdx = -1;
}
void GuiWindow::setPosition(Uint16 _x, Uint16 _y) {
  redrawOnUpdate = false;
  vector<GuiFrame *>::iterator it;
  for (it = frames.begin(); it != frames.end(); it++) {
    (*it)->setPosition(_x, _y);
  }
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
    innerRect->h -= titleText->h + windowBorderWidth;
    SDL_Rect *titleFrameRect = frames[titleFrameIdx]->getRect();
    eventAreas.add(
      "windowMoveBar", titleFrameRect->x, titleFrameRect->y,
      titleFrameRect->w, titleFrameRect->h
    );
    if (hasCloseBtnText == true) {
      SDL_Surface *frameSurface = frames[titleFrameIdx]->getSurface();
      closeBtnRect.x = frameSurface->w - closeBtnText->w;
      closeBtnRect.y = 0;
      closeBtnRect.w = closeBtnText->w;
      closeBtnRect.h = closeBtnText->h;
      eventAreas.add(
        "windowCloseButton",
        titleFrameRect->x + closeBtnRect.x,
        titleFrameRect->y + closeBtnRect.y,
        closeBtnRect.w, closeBtnRect.h
      );
    }
  }
}
void GuiWindow::update() {
  GuiScreen::update();
  if (titleFrameIdx > -1) {
    SDL_Rect *titleFrameRect = frames[titleFrameIdx]->getRect();
    eventAreas.update(
      "windowMoveBar", titleFrameRect->x, titleFrameRect->y
    );
    if (hasCloseBtnText == true) {
      eventAreas.update(
        "windowCloseButton",
        titleFrameRect->x + closeBtnRect.x,
        titleFrameRect->y + closeBtnRect.y
      );
    }
    if (redrawOnUpdate == true) {
      SDL_Surface *titleFrameSurface = frames[titleFrameIdx]->getSurface();
      if (hasTitleText == true && redrawOnUpdate == true) {
        SDL_BlitSurface(titleText, NULL, titleFrameSurface, NULL);
      }
      if (hasCloseBtnText == true) {
        SDL_BlitSurface(closeBtnText, NULL, titleFrameSurface, &closeBtnRect);
      }
      SDL_BlitSurface(titleFrameSurface, NULL, screen, titleFrameRect);
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

// ----> GUI TEXT WINDOW
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
  unsigned linePos, pos, rfindLength;
  int textWidth, textHeight;
  pos = linePos = 0;
  while (pos <= _text.length()) {
    tmpStr = _text.substr(linePos, pos-linePos);
    if (tmpStr.length() > 0) {
      TTF_SizeText(_font, tmpStr.c_str(), &textWidth, &textHeight);
      if ((unsigned)textWidth > _maxWidth) {
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
    // todo: use frame to handle redraw on update
    SDL_BlitSurface(text, NULL, screen, innerRect);
  }
}
GuiTextWindow::~GuiTextWindow() {
  unsetText();
}

// ----> GUI LIST WINDOW
struct stGuiListEntry {
  SDL_Surface *image;
  SDL_Surface *titleText;
  SDL_Surface *text;
};
class GuiListWindow: public GuiWindow {
  protected:
    Uint8 imageSize;
    vector<stGuiListEntry> entries;
    Uint16 listFrameIdx;
    string fontFile;
    Uint8 fontSizeTitle;
    Uint8 fontSizeText;
    struct stGuiRgbColor fontColor;
  public:
    GuiListWindow(SDL_Surface *);
    void addListFrame(int, int, int);
    void setTextOptions(string, Uint8, Uint8, Uint8, Uint8, Uint8);
    void addEntry(string, string, string);
    virtual void update();
    virtual ~GuiListWindow();
};
GuiListWindow::GuiListWindow(SDL_Surface *_screen): GuiWindow(_screen) {
}
void GuiListWindow::addListFrame(int _r, int _g, int _b) {
  addFrame(innerRect->x, innerRect->y, innerRect->w, innerRect->h);
  listFrameIdx = frames.size() - 1;
  frames[listFrameIdx]->setBgColor(_r, _g, _b);
}
void GuiListWindow::setTextOptions(string _fontFile,
                                   Uint8 _fontSizeTitle, Uint8 _fontSizeText,
                                   Uint8 _r, Uint8 _g, Uint8 _b) {
  fontFile = _fontFile;
  fontSizeTitle = _fontSizeTitle;
  fontSizeText = _fontSizeText;
  fontColor.r = _r;
  fontColor.g = _g;
  fontColor.b = _b;
}
void GuiListWindow::addEntry(string _image, string _title, string _text) {
  stGuiListEntry tmp;
  TTF_Font *titleFont = TTF_OpenFont(fontFile.c_str(), fontSizeTitle);
  TTF_Font *textFont = TTF_OpenFont(fontFile.c_str(), fontSizeText);
  SDL_Color sdlFontColor = { fontColor.r, fontColor.g, fontColor.b };
  tmp.titleText = TTF_RenderText_Blended(titleFont, _title.c_str(), sdlFontColor);
  tmp.text = TTF_RenderText_Blended(textFont, _text.c_str(), sdlFontColor);
  tmp.image = (_image.length() > 0) ? IMG_Load(_image.c_str()) : NULL;
  TTF_CloseFont(titleFont);
  TTF_CloseFont(textFont);
  entries.push_back(tmp);
}
void GuiListWindow::update() {
  GuiWindow::update();
  if (entries.size() > 0 && redrawOnUpdate == true) {
    SDL_Surface *listFrameSurface = frames[listFrameIdx]->getSurface();
    SDL_Rect tmpRect;
    tmpRect.x = 0;
    tmpRect.y = 0;
    for (Uint8 i = 0; i < entries.size(); i++) {
      tmpRect.x = 0;
      if (entries[i].image != NULL) {
        SDL_BlitSurface(entries[i].image, NULL, listFrameSurface, &tmpRect);
        tmpRect.x += entries[i].image->w;
      }
      SDL_BlitSurface(entries[i].titleText, NULL, listFrameSurface, &tmpRect);
      tmpRect.y += entries[i].titleText->h;
      SDL_BlitSurface(entries[i].text, NULL, listFrameSurface, &tmpRect);
      tmpRect.y += entries[i].text->h;
      SDL_BlitSurface(listFrameSurface, NULL, screen, frames[listFrameIdx]->getRect());
    }
  }
}
GuiListWindow::~GuiListWindow() {
}

// ----> MAIN
int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  GuiTextWindow *guiTW = new GuiTextWindow(screen);
  guiTW->setTitle("TEST TEXT WINDOW", 18, "libertysans.ttf", 0, 0, 0);
  guiTW->setCloseBtn(18, "libertysans.ttf", 0, 0, 0);
  guiTW->addWindowFrame(10, 10, 300, 300, 0, 0, 0);
  guiTW->setWindowBorder(5, 255, 255, 255);
  guiTW->addTitleFrame(255, 255, 255);
  guiTW->setTextPadding(5);
  guiTW->setText(
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce maximus, diam eget congue malesuada, eros mi maximus leo, vel ultrices leo turpis tempus ligula. Nunc pharetra commodo lorem, quis pharetra ligula. Aenean vel metus commodo eros convallis euismod.",
    16, "libertysans.ttf", 255, 255, 255
  );
  guiTW->setBgColor(120, 120, 120);
  guiTW->update();

  GuiListWindow *guiLW = new GuiListWindow(screen);
  guiLW->setTitle("TEST LIST WINDOW", 18, "libertysans.ttf", 0, 0, 0);
  guiLW->setCloseBtn(18, "libertysans.ttf", 0, 0, 0);
  guiLW->addWindowFrame(325, 150, 300, 300, 0, 0, 0);
  guiLW->setWindowBorder(5, 255, 255, 255);
  guiLW->addTitleFrame(255, 255, 255);
  guiLW->addListFrame(0, 0, 0);
  guiLW->setTextOptions("libertysans.ttf", 16, 12, 255, 255, 255);
  guiLW->addEntry("listitem.png", "Title 1", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 2", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 3", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 4", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 5", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 6", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 7", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 8", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 9", "Lorem ipsum dolor sit amet.");
  guiLW->addEntry("listitem.png", "Title 10", "Lorem ipsum dolor sit amet.");
  guiLW->update();

  SDL_UpdateRect(screen, 0, 0, 0, 0);

  SDL_Event event;
  bool quit = false;
  bool lMouseBtnDown = false;
  while (quit == false) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_LEFT) {
        lMouseBtnDown = true;
        if (guiTW->eventAreas.isEventInArea(
              "windowCloseButton", event.button.x, event.button.y) == true
           ) {
          cout << "Text Window Close Button Event" << endl;
        } else if (guiLW->eventAreas.isEventInArea(
                     "windowCloseButton", event.button.x, event.button.y) == true
                  ) {
          cout << "List Window Close Button Event" << endl;
        }
      }
      if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        lMouseBtnDown = false;
      }
      switch (event.type) {
        case SDL_QUIT:
          quit = 1;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_r:
              guiTW->update();
              guiLW->update();
              SDL_UpdateRect(screen, 0, 0, 0, 0);
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

  delete guiTW;
  delete guiLW;
  TTF_Quit();
  SDL_Quit();
  return 0;
}

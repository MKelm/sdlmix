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
      _eventY < areas[_name].y + areas[_name].h) {
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
    SDL_Rect innerRect;
    bool hasFrame;
    bool hasBorder;
    Uint8 borderWidth;
    struct stGuiRgbColor borderColor;
  public:
    GuiFrame(Uint8);
    void set(Uint16, Uint16, Uint16, Uint16);
    void setBorder(Uint8, Uint8, Uint8, Uint8);
    void move(Uint16, Uint16);
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

// ----> GUI ELEMENT
class GuiElement {
  protected:
    SDL_Surface *screen;
    int mainFrameIdx;
    vector<GuiFrame *> frames;
  public:
    bool fullUpdate;
    GuiEventAreas eventAreas;
    GuiElement(SDL_Surface *);
    GuiFrame *addFrame(Uint16, Uint16, Uint16, Uint16);
    GuiFrame *addMainFrame(Uint16, Uint16, Uint16, Uint16);
    void setMainFrameBgColor(int, int, int);
    virtual void update();
    virtual ~GuiElement();
};
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

// ----> GUI WINDOW
class GuiWindow: public GuiElement {
  protected:
    Uint8 mainFrameBorderWidth;
    int titleFrameIdx;
    SDL_Surface *titleText;
    bool hasTitleText;
    SDL_Surface *closeBtnText;
    bool hasCloseBtnText;
    SDL_Rect closeBtnRect;
    SDL_Rect moveRect;
  public:
    GuiWindow(SDL_Surface *);
    void resetMove();
    void setMove(Uint16, Uint16);
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

// ----> GUI TEXT WINDOW
class GuiTextWindow: public GuiWindow {
  protected:
    Uint8 textPadding;
    int textFrameIdx;
    vector<string> textLines;
    Uint8 fontSize;
    string fontFile;
    stGuiRgbColor fontColor;
  public:
    GuiTextWindow(SDL_Surface *);
    void addTextFrame(Uint8, int, int, int);
    vector<string> wrapText(TTF_Font *, const string &, unsigned);
    void setText(string, Uint8, string, Uint8, Uint8, Uint8);
    void unsetText();
    virtual void update();
    virtual ~GuiTextWindow();
};
GuiTextWindow::GuiTextWindow(SDL_Surface *_screen) : GuiWindow(_screen) {
  textFrameIdx = -1;
}
void GuiTextWindow::addTextFrame(Uint8 _padding, int _r, int _g, int _b) {
  SDL_Rect *innerRect = frames[mainFrameIdx]->getInnerRect();
  innerRect->w -= _padding * 2;
  innerRect->h -= _padding * 2;
  innerRect->x += _padding;
  innerRect->y += _padding;
  addFrame(innerRect->x, innerRect->y, innerRect->w, innerRect->h);
  textFrameIdx = frames.size() - 1;
  frames[textFrameIdx]->setBgColor(-1, -1, -1);
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
  fontSize = _fontSize;
  fontFile = _fontFile;
  fontColor.r = _r;
  fontColor.g = _g;
  fontColor.b = _b;
  TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
  textLines = wrapText(font, _text, frames[mainFrameIdx]->getInnerRect()->w);
  TTF_CloseFont(font);
}
void GuiTextWindow::update() {
  GuiWindow::update();
  if (fullUpdate == true && textFrameIdx > -1 && textLines.size() > 0) {
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    SDL_Color tmpFontColor = { fontColor.r, fontColor.g, fontColor.b };
    SDL_Surface *textFrameSurface = frames[textFrameIdx]->getSurface();
    SDL_Surface *text;
    SDL_Rect textRect;
    textRect.x = 0;
    textRect.y = 0;
    for (Uint8 i = 0; i < textLines.size(); i++) {
      if (i > 0)
        SDL_FreeSurface(text);
      text = TTF_RenderText_Blended(font, textLines[i].c_str(), tmpFontColor);
      SDL_BlitSurface(text, NULL, textFrameSurface, &textRect);
      textRect.y += text->h;
    };
    SDL_FreeSurface(text);
    TTF_CloseFont(font);
    SDL_BlitSurface(
      textFrameSurface, NULL,
      frames[mainFrameIdx]->getSurface(), frames[textFrameIdx]->getRect()
    );
    SDL_BlitSurface(frames[mainFrameIdx]->getSurface(), NULL,
      screen, frames[mainFrameIdx]->getRect());
  }
}
GuiTextWindow::~GuiTextWindow() {
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
    Uint16 listOffset;
    string fontFile;
    Uint8 fontSizeTitle;
    Uint8 fontSizeText;
    struct stGuiRgbColor fontColor;
  public:
    GuiListWindow(SDL_Surface *);
    void addListFrame(int, int, int);
    void setTextOptions(string, Uint8, Uint8, Uint8, Uint8, Uint8);
    void addEntry(string, string, string);
    void changeListOffset(int);
    virtual void update();
    virtual ~GuiListWindow();
};
GuiListWindow::GuiListWindow(SDL_Surface *_screen): GuiWindow(_screen) {
  listOffset = 0;
}
void GuiListWindow::addListFrame(int _r, int _g, int _b) {
  SDL_Rect *innerRect = frames[mainFrameIdx]->getInnerRect();
  addFrame(innerRect->x, innerRect->y, innerRect->w, innerRect->h);
  listFrameIdx = frames.size() - 1;
  frames[listFrameIdx]->setBgColor(_r, _g, _b);
  SDL_Rect *mainFrameRect = frames[mainFrameIdx]->getRect();
  SDL_Rect *listFrameRect = frames[listFrameIdx]->getRect();
  eventAreas.add(
    "windowListScroll",
    mainFrameRect->x + listFrameRect->x,
    mainFrameRect->y + listFrameRect->y,
    listFrameRect->w, listFrameRect->h
  );
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
void GuiListWindow::changeListOffset(int value) {
  if (listOffset + value >= 0 && listOffset + value < entries.size()) {
    listOffset += value;
  }
}
void GuiListWindow::update() {
  GuiWindow::update();
  SDL_Rect *mainFrameRect = frames[mainFrameIdx]->getRect();
  SDL_Rect *listFrameRect = frames[listFrameIdx]->getRect();
  eventAreas.update(
    "windowListScroll",
    mainFrameRect->x + listFrameRect->x,
    mainFrameRect->y + listFrameRect->y
  );
  if (entries.size() > 0 && fullUpdate == true) {
    SDL_Surface *listFrameSurface = frames[listFrameIdx]->getSurface();
    SDL_Rect tmpRect;
    tmpRect.x = 0;
    tmpRect.y = 0;
    for (Uint8 i = listOffset; i < entries.size(); i++) {
      tmpRect.x = 0;
      if (entries[i].image != NULL) {
        SDL_BlitSurface(entries[i].image, NULL, listFrameSurface, &tmpRect);
        tmpRect.x += entries[i].image->w;
      }
      SDL_BlitSurface(entries[i].titleText, NULL, listFrameSurface, &tmpRect);
      tmpRect.y += entries[i].titleText->h;
      SDL_BlitSurface(entries[i].text, NULL, listFrameSurface, &tmpRect);
      tmpRect.y += entries[i].text->h;
      SDL_BlitSurface(
        listFrameSurface, NULL, frames[mainFrameIdx]->getSurface(), listFrameRect
      );
      if (tmpRect.y > listFrameRect->h)
        break;
    }
    SDL_BlitSurface(
      frames[mainFrameIdx]->getSurface(), NULL, screen,
      frames[mainFrameIdx]->getRect()
    );
  }
}
GuiListWindow::~GuiListWindow() {
}

// ----> MAIN
void screenBgFill(SDL_Surface *screen) {
  SDL_FillRect(
    screen, &screen->clip_rect,
    SDL_MapRGB(screen->format, 120, 120, 120)
  );
}
int main (int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_WM_SetCaption("SDL GUI", NULL);
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  vector<GuiWindow *>windows;
  vector<GuiListWindow *>listWindows;

  GuiTextWindow *guiTW = new GuiTextWindow(screen);
  guiTW->setTitle("TEST TEXT WINDOW", 18, "libertysans.ttf", 0, 0, 0);
  guiTW->setCloseBtn(18, "libertysans.ttf", 0, 0, 0);
  guiTW->addWindowFrame(10, 10, 300, 300, 0, 0, 0);
  guiTW->setWindowBorder(5, 255, 255, 255);
  guiTW->addTitleFrame(255, 255, 255);
  guiTW->addTextFrame(5, 0, 0, 0);
  guiTW->setText(
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce maximus, diam eget congue malesuada, eros mi maximus leo, vel ultrices leo turpis tempus ligula. Nunc pharetra commodo lorem, quis pharetra ligula. Aenean vel metus commodo eros convallis euismod.",
    16, "libertysans.ttf", 255, 255, 255
  );
  windows.push_back(guiTW);

  GuiListWindow *guiLW = new GuiListWindow(screen);
  guiLW->setTitle("TEST LIST WINDOW", 18, "libertysans.ttf", 0, 0, 0);
  guiLW->setCloseBtn(18, "libertysans.ttf", 0, 0, 0);
  guiLW->addWindowFrame(325, 150, 300, 300, 0, 0, 255);
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
  windows.push_back(guiLW);
  listWindows.push_back(guiLW);

  screenBgFill(screen);
  Uint8 windowIdx;
  for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
    windows[windowIdx]->update();
  }
  SDL_UpdateRect(screen, 0, 0, 0, 0);

  SDL_Event event;
  Uint8 maxFPS = 30;
  Uint32 frameStart = 0;
  bool quit = false;
  bool lMouseBtnDown = false;
  Uint8 activeMoveWindow = -1;

  while (quit == false) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        lMouseBtnDown = true;
        bool windowCloseEvent = false;

        for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
          if (windows[windowIdx]->eventAreas.isEventInArea(
              "windowCloseButton", event.button.x, event.button.y) == true
             ) {
            delete windows[windowIdx];
            windows.erase(windows.begin() + windowIdx);
            windowCloseEvent = true;
          } else if (windows[windowIdx]->eventAreas.isEventInArea(
              "windowMoveBar", event.button.x, event.button.y) == true
             ) {
            activeMoveWindow = windowIdx;
          }
        }
        if (windowCloseEvent == true) {
          screenBgFill(screen);
          for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
            windows[windowIdx]->fullUpdate = false;
            windows[windowIdx]->update();
            windows[windowIdx]->fullUpdate = true;
          }
          windowCloseEvent = false;
        }
      } else if (event.type == SDL_MOUSEMOTION && lMouseBtnDown == true) {
        bool windowMoveEvent = false;

        for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
          if (windows[windowIdx]->eventAreas.isEventInArea(
              "windowMoveBar", event.button.x, event.button.y) == true &&
              activeMoveWindow == windowIdx
             ) {
            windowMoveEvent = true;
            windows[windowIdx]->setMove(event.button.x, event.button.y);
          }
        }
        if (windowMoveEvent == true) {
          screenBgFill(screen);
          for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
            windows[windowIdx]->fullUpdate = false;
            windows[windowIdx]->update();
          }
          windowMoveEvent = false;
        }
      } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        lMouseBtnDown = false;
        for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
          windows[windowIdx]->resetMove();
        }
        activeMoveWindow = -1;

      } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_WHEELUP) {
          for (windowIdx = 0; windowIdx < listWindows.size(); windowIdx++) {
            if (listWindows[windowIdx]->eventAreas.isEventInArea(
                "windowListScroll", event.button.x, event.button.y) == true
               ) {
              listWindows[windowIdx]->changeListOffset(-1);
              listWindows[windowIdx]->update();
            }
          }
        } else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
          for (windowIdx = 0; windowIdx < listWindows.size(); windowIdx++) {
            if (listWindows[windowIdx]->eventAreas.isEventInArea(
                "windowListScroll", event.button.x, event.button.y) == true
               ) {
              listWindows[windowIdx]->changeListOffset(1);
              listWindows[windowIdx]->update();
            }
          }
        }
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

    SDL_UpdateRect(screen, 0, 0, 0, 0);

    if ((SDL_GetTicks() - frameStart) < (1000 / maxFPS)) {
      SDL_Delay((1000 / maxFPS) - (SDL_GetTicks() - frameStart));
    }
  }

  for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
    delete windows[windowIdx];
  }
  windows.clear();
  listWindows.clear();

  TTF_Quit();
  SDL_Quit();
  return 0;
}

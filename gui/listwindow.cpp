#include "listwindow.hpp"

GuiListWindow::GuiListWindow(SDL_Surface *_screen): GuiWindow(_screen) {
  listOffset = 0;
  listEntryHeight = 0;
  listEntriesVisibleCount = 0;
  scrollBarWidth = 0;
  selectedEntryIdx = -1;
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
  eventAreas.add(
    "windowListEntrySelect",
    mainFrameRect->x + listFrameRect->x,
    mainFrameRect->y + listFrameRect->y,
    listFrameRect->w - scrollBarWidth, listFrameRect->h
  );
}

void GuiListWindow::setEntrySelectionColor(Uint8 _r, Uint8 _g, Uint8 _b) {
  entrySelectionColor.r = _r;
  entrySelectionColor.g = _g;
  entrySelectionColor.b = _b;
}

void GuiListWindow::setScrollBarOptions(Uint16 _width,
                                        Uint8 _r, Uint8 _g, Uint8 _b) {
  scrollBarWidth = _width;
  scrollBarColor.r = _r;
  scrollBarColor.g = _g;
  scrollBarColor.b = _b;
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

  listEntryHeight = tmp.titleText->h + tmp.text->h;
  if (tmp.image != NULL && tmp.image->h > listEntryHeight)
    listEntryHeight = tmp.image->h;
  listEntriesVisibleCount = frames[listFrameIdx]->getRect()->h / listEntryHeight;
}

void GuiListWindow::changeListOffset(int value) {
  if (listOffset + value >= 0 &&
      listOffset + value < entries.size() + 1 - listEntriesVisibleCount) {
    listOffset += value;
  }
}

void GuiListWindow::selectEntry(Uint16 _y) {
  SDL_Rect *mainFrameRect = frames[mainFrameIdx]->getRect();
  SDL_Rect *listFrameRect = frames[listFrameIdx]->getRect();
  _y = _y - mainFrameRect->y - listFrameRect->y;
  selectedEntryIdx = listOffset + (_y / listEntryHeight);
}

void GuiListWindow::drawScrollBar(Uint16 entryHeight) {
  if (scrollBarWidth > 0) {
    SDL_Surface *listFrameSurface = frames[listFrameIdx]->getSurface();
    SDL_Rect *listFrameRect = frames[listFrameIdx]->getRect();

    Uint16 sliderMargin = scrollBarWidth / 6;
    Uint16 sliderWidth = scrollBarWidth - sliderMargin * 2;
    Uint16 sliderOffsetY = floor(
      (float)listOffset * ((listFrameRect->h - 1. - sliderMargin) /
        (float)(entries.size() + 1 - listEntriesVisibleCount))
    );
    Uint16 sliderHeight = floor(
      (listFrameRect->h - 1. - sliderMargin) /
        (float)(entries.size() + 1 - listEntriesVisibleCount)
    ) - sliderMargin;
    Uint16 barWidth = sliderWidth + 2 * sliderMargin;

    lineRGBA(
      listFrameSurface,
      listFrameRect->w - barWidth, 0,
      listFrameRect->w - barWidth, listFrameRect->h - 1,
      scrollBarColor.r, scrollBarColor.g, scrollBarColor.b, 255
    );
    boxRGBA(
      listFrameSurface,
      listFrameRect->w - barWidth + sliderMargin,
      sliderOffsetY + sliderMargin,
      listFrameRect->w - barWidth + sliderMargin + sliderWidth,
      sliderOffsetY + sliderMargin + sliderHeight,
      scrollBarColor.r, scrollBarColor.g, scrollBarColor.b, 255
    );
    SDL_BlitSurface(
      listFrameSurface, NULL, frames[mainFrameIdx]->getSurface(), listFrameRect
    );
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
  eventAreas.update(
    "windowListEntrySelect",
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
      if (selectedEntryIdx == i) {
        boxRGBA(
          listFrameSurface,
          tmpRect.x, tmpRect.y,
          listFrameRect->w - scrollBarWidth, tmpRect.y + listEntryHeight,
          entrySelectionColor.r, entrySelectionColor.g, entrySelectionColor.b, 255
        );
      }
      if (entries[i].image != NULL) {
        SDL_BlitSurface(entries[i].image, NULL, listFrameSurface, &tmpRect);
        tmpRect.x += entries[i].image->w;
      }
      SDL_BlitSurface(entries[i].titleText, NULL, listFrameSurface, &tmpRect);
      tmpRect.y += entries[i].titleText->h;
      SDL_BlitSurface(entries[i].text, NULL, listFrameSurface, &tmpRect);
      tmpRect.y += entries[i].text->h;
      if (tmpRect.y > listFrameRect->h)
        break;
    }
    SDL_BlitSurface(
      listFrameSurface, NULL, frames[mainFrameIdx]->getSurface(), listFrameRect
    );
    drawScrollBar(tmpRect.y / (entries.size() - listOffset + 1));
    SDL_BlitSurface(
      frames[mainFrameIdx]->getSurface(), NULL, screen,
      frames[mainFrameIdx]->getRect()
    );
  }
}

GuiListWindow::~GuiListWindow() {
}

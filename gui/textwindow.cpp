#include "textwindow.hpp"

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

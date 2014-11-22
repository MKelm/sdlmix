#ifndef _INCLUDE_GUI_LIST_WINDOW_HPP
#define _INCLUDE_GUI_LIST_WINDOW_HPP

#include "global.hpp"
#include "window.hpp"

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
    Uint16 listEntryHeight;
    Uint16 listEntriesVisibleCount;
    int selectedEntryIdx;
    string fontFile;
    Uint8 fontSizeTitle;
    Uint8 fontSizeText;
    struct stGuiRgbColor fontColor;
    Uint16 scrollBarWidth;
    struct stGuiRgbColor scrollBarColor;
    struct stGuiRgbColor entrySelectionColor;
  public:
    GuiListWindow(SDL_Surface *);
    void addListFrame(int, int, int);
    void setEntrySelectionColor(Uint8, Uint8, Uint8);
    void setScrollBarOptions(Uint16, Uint8, Uint8, Uint8);
    void setTextOptions(string, Uint8, Uint8, Uint8, Uint8, Uint8);
    void addEntry(string, string, string);
    void changeListOffset(int);
    void selectEntry(Uint16);
    void drawScrollBar(Uint16);
    virtual void update();
    virtual ~GuiListWindow();
};

#endif

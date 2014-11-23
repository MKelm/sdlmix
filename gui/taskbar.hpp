#ifndef _INCLUDE_GUI_TASK_BAR_HPP
#define _INCLUDE_GUI_TASK_BAR_HPP

#include "global.hpp"
#include "element.hpp"

struct stGuiTask {
  string title;
  bool minimized;
  Uint16 frameIdx;
};

class GuiTaskBar: public GuiElement {
  protected:
    bool isBottom;
    Uint8 height;
    Uint16 taskWidth;
    struct stGuiRgbColor taskBgColor;
    Uint8 taskFontSize;
    string taskFontFile;
    struct stGuiRgbColor taskFontColor;
    Uint8 taskBorderWidth;
    struct stGuiRgbColor taskBorderColor;
    vector<stGuiTask *> tasks;
  public:
    GuiTaskBar(SDL_Surface *);
    void setFrame(bool, Uint8, int, int, int);
    void setBorder(Uint8, int, int, int);
    void setTaskBgColor(int, int, int);
    void setTaskTextOptions(Uint8, string, int, int, int);
    void setTaskBorderOptions(Uint8, int, int, int);
    void addTask(string, bool);
    virtual void update();
    virtual ~GuiTaskBar();
};

#endif

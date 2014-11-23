#include "taskbar.hpp"

GuiTaskBar::GuiTaskBar(SDL_Surface *_screen) : GuiElement(_screen) {
  taskWidth = 0;
}

void GuiTaskBar::setFrame(bool _isBottom, Uint8 _height,
                          int _bgR, int _bgG, int _bgB) {
  isBottom = _isBottom;
  height = _height;
  if (mainFrameIdx == -1) {
    if (isBottom == true)
      addMainFrame(0, screen->h - height, screen->w, screen->h);
    else
      addMainFrame(0, screen->h - height, screen->w, screen->h);
  }
  setMainFrameBgColor(_bgR, _bgG, _bgB);
}

void GuiTaskBar::setBorder(Uint8 _borderWidth, int _bgR, int _bgG, int _bgB) {
  if (mainFrameIdx > -1 && _borderWidth > 0) {
    frames[mainFrameIdx]->setBorderActive(
      isBottom, !isBottom, false, false
    );
    frames[mainFrameIdx]->setBorder(_borderWidth, _bgR, _bgG, _bgB);
  }
}

void GuiTaskBar::setTaskBgColor(int _r, int _g, int _b) {
  taskBgColor.r = _r;
  taskBgColor.g = _g;
  taskBgColor.b = _b;
}

void GuiTaskBar::setTaskTextOptions(Uint8 _fontSize, string _fontFile,
                                    int _fcR, int _fcG, int _fcB) {
  taskFontSize = _fontSize;
  taskFontFile = _fontFile;
  taskFontColor.r = _fcR;
  taskFontColor.g = _fcG;
  taskFontColor.b = _fcB;
}

void GuiTaskBar::setTaskBorderOptions(Uint8 _width, int _bcR, int _bcG, int _bcB) {
  taskBorderWidth = _width;
  taskBorderColor.r = _bcR;
  taskBorderColor.g = _bcG;
  taskBorderColor.b = _bcB;
}

void GuiTaskBar::addTask(string _title, bool _minimized) {
  if (mainFrameIdx > -1) {
    stGuiTask *task = new stGuiTask;
    task->title = _title;
    task->minimized = _minimized;

    SDL_Rect *mainFrameIRect = frames[mainFrameIdx]->getInnerRect();
    Uint16 tasksIdx = frames.size() - 1;
    GuiFrame *taskFrame = addFrame(
      mainFrameIRect->x + tasksIdx * 50,
      mainFrameIRect->y + 1,
      50, mainFrameIRect->h
    );
    if (taskBorderWidth > 0) {
      taskFrame->setBorder(
        taskBorderWidth, taskBorderColor.r, taskBorderColor.g, taskBorderColor.b
      );
      taskFrame->setBorderActive(
        false, false, true, true
      );
    }

    task->frameIdx = frames.size() - 1;
    tasks.push_back(task);
  }
}

void GuiTaskBar::update() {
  GuiElement::update();
  if (fullUpdate == true && mainFrameIdx > -1 && tasks.size() > 0) {
    TTF_Font *textFont = TTF_OpenFont(taskFontFile.c_str(), taskFontSize);
    SDL_Color textFontColor = { taskFontColor.r, taskFontColor.g, taskFontColor.b };
    SDL_Surface *textSurface;
    for (Uint8 i = 0; i < tasks.size(); i++) {
      if (i > 0)
        SDL_FreeSurface(textSurface);

      textSurface = TTF_RenderText_Blended(
        textFont, tasks[i]->title.c_str(), textFontColor
      );

      SDL_BlitSurface(
        textSurface, NULL, frames[tasks[i]->frameIdx]->getSurface(), NULL
      );
      SDL_BlitSurface(
        frames[tasks[i]->frameIdx]->getSurface(), NULL,
        frames[mainFrameIdx]->getSurface(), frames[tasks[i]->frameIdx]->getRect()
      );
    }
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(textFont);
    SDL_BlitSurface(frames[mainFrameIdx]->getSurface(), NULL,
        screen, frames[mainFrameIdx]->getRect());
  }
}

GuiTaskBar::~GuiTaskBar() {
  vector<stGuiTask *>::iterator it;
  for (it = tasks.begin(); it != tasks.end(); it++) {
    delete (*it);
  }
  tasks.clear();
}

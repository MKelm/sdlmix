#include "global.hpp"
#include "bgcolor.hpp"
#include "eventareas.hpp"
#include "frame.hpp"
#include "element.hpp"
#include "taskbar.hpp"
#include "window.hpp"
#include "textwindow.hpp"
#include "listwindow.hpp"

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
  SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);

  GuiTaskBar *guiTB = new GuiTaskBar(screen);
  guiTB->setFrame(true, 20, 0, 0, 0);
  guiTB->setBorder(2, 255, 255, 255);
  guiTB->setTaskBgColor(0, 0, 0);
  guiTB->setTaskTextOptions(12, "libertysans.ttf", 255, 255, 255);
  guiTB->setTaskBorderOptions(2, 255, 255, 255);

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
  guiTB->addTask("TEST TEXT WINDOW", false);

  GuiListWindow *guiLW = new GuiListWindow(screen);
  guiLW->setTitle("TEST LIST WINDOW", 18, "libertysans.ttf", 0, 0, 0);
  guiLW->setCloseBtn(18, "libertysans.ttf", 0, 0, 0);
  guiLW->setTextOptions("libertysans.ttf", 16, 12, 255, 255, 255);
  guiLW->setScrollBarOptions(20, 255, 255, 255);
  guiLW->setEntrySelectionColor(100, 100, 100);
  guiLW->addWindowFrame(325, 150, 300, 300, 0, 0, 0);
  guiLW->setWindowBorder(5, 255, 255, 255);
  guiLW->addTitleFrame(255, 255, 255);
  guiLW->addListFrame(0, 0, 0);
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
  guiTB->addTask("TEST LIST WINDOW", false);

  screenBgFill(screen);
  Uint8 windowIdx;
  for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
    windows[windowIdx]->update();
  }
  guiTB->update();
  SDL_UpdateRect(screen, 0, 0, 0, 0);

  SDL_Event event;
  Uint8 maxFPS = 30;
  Uint32 frameStart = 0;
  bool quit = false;
  bool lMouseBtnDown = false;
  Uint8 focusWindowIdx = -1;

  while (quit == false) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        lMouseBtnDown = true;
        bool windowClickEvent = false;

        for (windowIdx = 0; windowIdx < listWindows.size(); windowIdx++) {
          if (listWindows[windowIdx]->eventAreas.isEventInArea(
              "windowListEntrySelect", event.button.x, event.button.y) == true
             ) {
            listWindows[windowIdx]->selectEntry(event.button.y);
            listWindows[windowIdx]->update();
          }
        }

        for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
          if (windows[windowIdx]->eventAreas.isEventInArea(
              "windowCloseButton", event.button.x, event.button.y) == true
             ) {
            delete windows[windowIdx];
            windows.erase(windows.begin() + windowIdx);
            windowClickEvent = true;
          } else if (windows[windowIdx]->eventAreas.isEventInArea(
              "windowMoveBar", event.button.x, event.button.y) == true
             ) {
            focusWindowIdx = windowIdx;
          }
        }
        if (focusWindowIdx > -1) {
          windows.insert(windows.end(), windows[focusWindowIdx]);
          windows.erase(windows.begin() + focusWindowIdx);
          focusWindowIdx = windows.size() - 1;
          windowClickEvent = true;
        }
        if (windowClickEvent == true) {
          screenBgFill(screen);
          guiTB->fullUpdate = false;
          guiTB->update();
          for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
            windows[windowIdx]->fullUpdate = false;
            windows[windowIdx]->update();
            windows[windowIdx]->fullUpdate = true;
          }
          windowClickEvent = false;
        }

      } else if (event.type == SDL_MOUSEMOTION && lMouseBtnDown == true) {
        bool windowMoveEvent = false;

        for (windowIdx = 0; windowIdx < windows.size(); windowIdx++) {
          if (windows[windowIdx]->eventAreas.isEventInArea(
              "windowMoveBar", event.button.x, event.button.y) == true &&
              focusWindowIdx == windowIdx
             ) {
            windowMoveEvent = true;
            windows[windowIdx]->setMove(event.button.x, event.button.y);
          }
        }
        if (windowMoveEvent == true) {
          screenBgFill(screen);
          guiTB->fullUpdate = false;
          guiTB->update();
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
        focusWindowIdx = -1;

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

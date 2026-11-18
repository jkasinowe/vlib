#ifndef VSYS_H
#define VSYS_H
#include "vmath.h"


struct VSYS_Window;

#define VSYS_WINDOW_FULLSCREEN (1<<0)
#define VSYS_WINDOW_RESIZABLE  (1<<1)

const b8 *VSYS_KeyboardStateGet(struct VSYS_Window *window);

void VSYS_MousePositionGet(f32 mouse_position[2]);

void VSYS_MouseDeltaGet(f32 mouse_delta[2]);

struct VSYS_Window *VSYS_WindowCreate(f32 size[2]);

void VSYS_WindowTitleSet(struct VSYS_Window *window, char *name);

void VSYS_WindowTitleGet(struct VSYS_Window *window, char *name);

void VSYS_WindowFlagsEnable(struct VSYS_Window *window, usize window_flags);
void VSYS_WindowFlagsDisable(struct VSYS_Window *window, usize window_flags);
usize VSYS_WindowFlagsGet(struct VSYS_Window *window);

void VSYS_KeyboardStateUpdate(struct VSYS_Window *window);

typedef void VSYS_WindowResizeCallback_t(struct VSYS_Window *window,
                                         f32 size_new[2]);

void VSYS_WindowResizeCallbackSet(struct VSYS_Window *window, VSYS_WindowResizeCallback_t *callback);

#endif
#define VSYS_IMPLEMENTATION
#ifdef VSYS_IMPLEMENTATION
#include <SDL3/SDL.h>

struct VSYS_Window {
  SDL_Window* sdlWindow;
};


struct VSYS_Window *VSYS_WindowCreate(f32 size[2]) {
  SDL_CreateWindow("VSYS Program", floorf(size[0]), floorf(size[1]), 0);
  
}  


#endif

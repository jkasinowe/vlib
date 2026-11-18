#ifndef VSYS_H
#define VSYS_H
#include "vmath.h"
#include "vmem.h"
#include "vlog.h"

#define VSYS_KEY_A 4
#define VSYS_KEY_B 5
#define VSYS_KEY_C 6
#define VSYS_KEY_D 7
#define VSYS_KEY_E 8
#define VSYS_KEY_F 9
#define VSYS_KEY_G 10
#define VSYS_KEY_H 11
#define VSYS_KEY_I 12
#define VSYS_KEY_J 13
#define VSYS_KEY_K 14
#define VSYS_KEY_L 15
#define VSYS_KEY_M 16
#define VSYS_KEY_N 17
#define VSYS_KEY_O 18
#define VSYS_KEY_P 19
#define VSYS_KEY_Q 20
#define VSYS_KEY_R 21
#define VSYS_KEY_S 22
#define VSYS_KEY_T 23
#define VSYS_KEY_U 24
#define VSYS_KEY_V 25
#define VSYS_KEY_W 26
#define VSYS_KEY_X 27
#define VSYS_KEY_Y 28
#define VSYS_KEY_Z 29

#define VSYS_KEY_1 30
#define VSYS_KEY_2 31
#define VSYS_KEY_3 32
#define VSYS_KEY_4 33
#define VSYS_KEY_5 34
#define VSYS_KEY_6 35
#define VSYS_KEY_7 36
#define VSYS_KEY_8 37
#define VSYS_KEY_9 38
#define VSYS_KEY_0 39

#define VSYS_KEY_RETURN 40
#define VSYS_KEY_ESCAPE 41
#define VSYS_KEY_BACKSPACE 42
#define VSYS_KEY_TAB 43
#define VSYS_KEY_SPACE 44

#define VSYS_KEY_MINUS 45
#define VSYS_KEY_EQUALS 46
#define VSYS_KEY_LEFTBRACKET 47
#define VSYS_KEY_RIGHTBRACKET 48
#define VSYS_KEY_BACKSLASH 49
#define VSYS_KEY_NONUSHASH 50

#define VSYS_KEY_SEMICOLON 51
#define VSYS_KEY_APOSTROPHE 52
#define VSYS_KEY_GRAVE 53
#define VSYS_KEY_COMMA 54
#define VSYS_KEY_PERIOD 55
#define VSYS_KEY_SLASH 56

#define VSYS_KEY_CAPSLOCK 57

#define VSYS_KEY_F1 58
#define VSYS_KEY_F2 59
#define VSYS_KEY_F3 60
#define VSYS_KEY_F4 61
#define VSYS_KEY_F5 62
#define VSYS_KEY_F6 63
#define VSYS_KEY_F7 64
#define VSYS_KEY_F8 65
#define VSYS_KEY_F9 66
#define VSYS_KEY_F10 67
#define VSYS_KEY_F11 68
#define VSYS_KEY_F12 69

#define VSYS_KEY_PRINTSCREEN 70
#define VSYS_KEY_SCROLLLOCK 71
#define VSYS_KEY_PAUSE 72
#define VSYS_KEY_INSERT 73
#define VSYS_KEY_HOME 74
#define VSYS_KEY_PAGEUP 75
#define VSYS_KEY_DELETE 76
#define VSYS_KEY_END 77
#define VSYS_KEY_PAGEDOWN 78
#define VSYS_KEY_RIGHT 79
#define VSYS_KEY_LEFT 80
#define VSYS_KEY_DOWN 81
#define VSYS_KEY_UP 82

#define VSYS_KEY_NUMLOCKCLEAR 83
#define VSYS_KEY_KP_DIVIDE 84
#define VSYS_KEY_KP_MULTIPLY 85
#define VSYS_KEY_KP_MINUS 86
#define VSYS_KEY_KP_PLUS 87
#define VSYS_KEY_KP_ENTER 88
#define VSYS_KEY_KP_1 89
#define VSYS_KEY_KP_2 90
#define VSYS_KEY_KP_3 91
#define VSYS_KEY_KP_4 92
#define VSYS_KEY_KP_5 93
#define VSYS_KEY_KP_6 94
#define VSYS_KEY_KP_7 95
#define VSYS_KEY_KP_8 96
#define VSYS_KEY_KP_9 97
#define VSYS_KEY_KP_0 98
#define VSYS_KEY_KP_PERIOD 99

#define VSYS_KEY_LCTRL 224
#define VSYS_KEY_LSHIFT 225
#define VSYS_KEY_LALT 226
#define VSYS_KEY_LSUPER 227
#define VSYS_KEY_RCTRL 228
#define VSYS_KEY_RSHIFT 229
#define VSYS_KEY_RALT 230
#define VSYS_KEY_RSUPER 231

struct VSYS_Window;

#define VSYS_WINDOW_FLAG_FULLSCREEN (1 << 0)
#define VSYS_WINDOW_FLAG_RESIZABLE (1 << 1)

void VSYS_MousePositionGet(struct VSYS_Window* window, f32 mouse_position[2]);

void VSYS_MouseDeltaGet(struct VSYS_Window* window, f32 mouse_delta[2]);

b8 VSYS_KeyDownGet(struct VSYS_Window* window, usize key);

b8 VSYS_KeyUpGet(struct VSYS_Window* window, usize key);

#define VSYS_WINDOW_CREATION_FLAG_VULKAN (1 << 0)
#define VSYS_WINDOW_CREATION_FLAG_OPENGL (1 << 1)

struct VSYS_Window *VSYS_WindowCreate(f32 size[2], usize creation_flags);

void VSYS_WindowTitleSet(struct VSYS_Window *window, const char *name);

void VSYS_WindowTitleGet(struct VSYS_Window *window, char *name);

void VSYS_WindowFlagsEnable(struct VSYS_Window *window, usize window_flags);
void VSYS_WindowFlagsDisable(struct VSYS_Window *window, usize window_flags);
usize VSYS_WindowFlagsGet(struct VSYS_Window *window);

void VSYS_InputUpdate(struct VSYS_Window *window);

b8 VSYS_WindowShouldCloseGet(struct VSYS_Window *window);

typedef void VSYS_WindowResizeCallback_t(struct VSYS_Window *window,
                                         f32 size_new[2]);

void VSYS_WindowResizeCallbackSet(struct VSYS_Window *window,
                                  VSYS_WindowResizeCallback_t *callback);

void VSYS_WindowSizeGet(struct VSYS_Window *window, f32 size[2]);

void VSYS_Initialize();

void VSYS_WindowDestroy(struct VSYS_Window *window);

void VSYS_Finalize();


void VSYS_WindowFrameTimeTargetSet(struct VSYS_Window *window,
                                   double frame_time);

b8 VSYS_WindowFrameTimeWaitedGet(struct VSYS_Window *window);

// VULKAN
const char* const* VSYS_VulkanInstanceDefaultExtensionsGet(u32 *count);

void *VSYS_VulkanInstanceSurfaceCreate(struct VSYS_Window *window,
                                       void *vulkan_instance,
                                       struct VMEM_Allocator *vmem_allocator,
                                       void *vulkan_allocation_callbacks);

void VSYS_VulkanInstanceSurfaceDestroy(void *vulkan_instance,
                                       void *vulkan_surface,
                                       void *vulkan_allocation_callbacks);



#endif
#define VSYS_IMPLEMENTATION
#ifdef VSYS_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_vulkan.h>

#define VSYS_KEY_MAX 400

struct VSYS_Keyboard {
  b8 *state;
};  

struct VSYS_Window {
  SDL_Window *sdlWindow;
  VSYS_WindowResizeCallback_t *windowResizeCallback;
  struct VSYS_Keyboard keyboard;
  usize flags;
  u64 frameTimeTarget;
  u64 frameLast;
};

struct VMEM_Allocator *VsysAllocator = NULL;
b8 VsysWindowShouldClose = false;

void iVSYS_AllocatorInitialize() {
  if (VsysAllocator == NULL) {
    VsysAllocator = VMEM_AllocatorCreate(0);
  }

}

struct VSYS_Window *VSYS_WindowCreate(f32 size[2], usize creation_flags) {
  struct VSYS_Window *window = (struct VSYS_Window *)VMEM_Allocate(
      sizeof(struct VSYS_Window), VsysAllocator);

  SDL_WindowFlags window_flags = 0;
  if (creation_flags & VSYS_WINDOW_CREATION_FLAG_VULKAN) {    
    window_flags |= SDL_WINDOW_VULKAN;
  }
  if (creation_flags & VSYS_WINDOW_CREATION_FLAG_OPENGL) {
    window_flags |= SDL_WINDOW_OPENGL;
  }
  
  window->sdlWindow =
      SDL_CreateWindow("VSYS Program", floorf(size[0]), floorf(size[1]), window_flags);

  window->keyboard.state =
      (b8 *)VMEM_Allocate(sizeof(b8) * VSYS_KEY_MAX, VsysAllocator);  
  
  return window;
}

void VSYS_WindowTitleSet(struct VSYS_Window *window, const char *name) {
  SDL_SetWindowTitle(window->sdlWindow, name);
}

void VSYS_InputUpdate(struct VSYS_Window *window) {
  VLOG_Wait("Polling input");
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      VsysWindowShouldClose = true;
      VLOG_Info("Got quit event");
      break;
    case SDL_EVENT_KEY_DOWN:
      window->keyboard.state[event.key.scancode] = true;
      break;
    case SDL_EVENT_KEY_UP:
      window->keyboard.state[event.key.scancode] = false;
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      if (window->windowResizeCallback) {
        f32 window_size[2];
        VM2_Set(window_size, event.window.data1, event.window.data2);
        window->windowResizeCallback(window, window_size);
      }
      break;
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
      VSYS_WindowFlagsEnable(window, VSYS_WINDOW_FLAG_FULLSCREEN);
      break;
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
      VSYS_WindowFlagsDisable(window, VSYS_WINDOW_FLAG_FULLSCREEN);
      break;
    }
  }
  VLOG_Ok("Input polled");
}

void VSYS_WindowResizeCallbackSet(struct VSYS_Window *window,
                                  VSYS_WindowResizeCallback_t *callback) {
  window->windowResizeCallback = callback;
}

b8 VSYS_WindowShouldCloseGet(struct VSYS_Window *window) {
  return VsysWindowShouldClose;
}

void VSYS_Initialize() {
  iVSYS_AllocatorInitialize();
  SDL_Init(SDL_INIT_VIDEO);
}


void VSYS_WindowDestroy(struct VSYS_Window *window) {
  SDL_DestroyWindow(window->sdlWindow);
}

void VSYS_Finalize() {
  VMEM_AllocatorDebug(stdout, VsysAllocator);
  VMEM_AllocatorDestroy(VsysAllocator);
}

void iVSYS_WindowFlagsUpdate(struct VSYS_Window *window) {
  SDL_SetWindowFullscreen(window->sdlWindow,
                          window->flags & VSYS_WINDOW_FLAG_FULLSCREEN);
  SDL_SetWindowResizable(window->sdlWindow,
                         window->flags & VSYS_WINDOW_FLAG_RESIZABLE);
}

void VSYS_WindowFlagsEnable(struct VSYS_Window *window, usize window_flags) {
  window->flags |= window_flags;
  iVSYS_WindowFlagsUpdate(window);
}

void VSYS_WindowFlagsDisable(struct VSYS_Window *window, usize window_flags) {
  window->flags &= ~window_flags;
  iVSYS_WindowFlagsUpdate(window);
}

usize VSYS_WindowFlagsGet(struct VSYS_Window *window) { return window->flags; }


b8 VSYS_KeyDownGet(struct VSYS_Window *window, usize key) {
  return window->keyboard.state[key];
}

b8 VSYS_KeyUpGet(struct VSYS_Window *window, usize key) {
  return !window->keyboard.state[key];
}

void VSYS_WindowFrameTimeTargetSet(struct VSYS_Window *window,
                                   double frame_time) {
  window->frameTimeTarget = ((u64)(frame_time * 1000*1000)) * 1000;
  VLOG_Info("Target frame time %lf\n", window->frameTimeTarget/1000000000.0);
}

b8 VSYS_WindowFrameTimeWaitedGet(struct VSYS_Window *window) {
  u64 ticks_current = SDL_GetTicksNS();
  if (ticks_current < window->frameLast + window->frameTimeTarget) {
    return false;
  }
  window->frameLast = ticks_current;
  return true;
}


const char* const* VSYS_VulkanInstanceDefaultExtensionsGet(u32 *count) {  
  return SDL_Vulkan_GetInstanceExtensions(count);
}



void *VSYS_VulkanInstanceSurfaceCreate(struct VSYS_Window *window,
                                       void *vulkan_instance,
                                       struct VMEM_Allocator *vmem_allocator,
                                       void *vulkan_allocation_callbacks) {
  if (vmem_allocator == NULL) {
    vmem_allocator = VsysAllocator;
  }
  VkSurfaceKHR *surface =
      (VkSurfaceKHR *)VMEM_Allocate(sizeof(VkSurfaceKHR), vmem_allocator);

  b8 result = SDL_Vulkan_CreateSurface(
      (SDL_Window *)window->sdlWindow, (VkInstance)vulkan_instance,
      (struct VkAllocationCallbacks *)vulkan_allocation_callbacks,
      (VkSurfaceKHR *)surface);

  if (result) {
    VLOG_Error("SDL: %s", SDL_GetError());
    abort();
  }    

  
  return surface;
}

void VSYS_VulkanInstanceSurfaceDestroy(void *vulkan_instance,
                                       void *vulkan_surface,
                                       void *vulkan_allocation_callbacks) {
  SDL_Vulkan_DestroySurface(
      (VkInstance)vulkan_instance, (VkSurfaceKHR)vulkan_surface,
      (struct VkAllocationCallbacks *)vulkan_allocation_callbacks);
}

void VSYS_WindowSizeGet(struct VSYS_Window *window, f32 size[2]) {
  i32 window_width = 0;
  i32 window_height = 0;
  SDL_GetWindowSize(window->sdlWindow, &window_width, &window_height);
  size[0] = window_width;
  size[1] = window_height;
}  

#endif

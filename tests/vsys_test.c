#define VSYS_IMPLEMENTATION
#include "../include/vsys.h"
#define VMATH_IMPLEMENTATION
#include "../include/vmath.h"

f32 window_size[2];

void window_resize_callback(struct VSYS_Window*window, f32 size[2]) {
  printf("width:  %f\n", size[0]);
  printf("height: %f\n", size[1]);
}  

int main() {
  VSYS_Initialize();  
  VM2_Set(window_size, 800, 600);
  struct VSYS_Window *window = VSYS_WindowCreate(window_size, 0);

  VSYS_WindowFlagsEnable(window, VSYS_WINDOW_FLAG_RESIZABLE);
  VSYS_WindowTitleSet(window, "New name");
  VSYS_WindowResizeCallbackSet(window, window_resize_callback);
  VSYS_WindowFrameTimeTargetSet(window, 1./60);

  usize frame = 0;
  while (!VSYS_WindowShouldCloseGet(window)) {
    while (!VSYS_WindowFrameTimeWaitedGet(window))
      ;    
    
    VSYS_InputUpdate(window);
    if (VSYS_KeyDownGet(window, VSYS_KEY_ESCAPE)) {
      break;
    }
    if (VSYS_KeyUpGet(window, VSYS_KEY_SPACE)) {
      printf("%zu:\tUnspaced\n", frame);
    } else {
      printf("%zu:\tSpaced\n", frame);
    }      
    frame++;
  }

  VSYS_WindowDestroy(window);
  VSYS_Finalize();
  return 0;
}  

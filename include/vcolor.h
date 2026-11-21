#ifndef VCOLOR_H
#define VCOLOR_H
// Colors are arrays of floats
// i.e. RGBA = float[4]
//
// O-functions Output result to the last argument
// other functions either return value or
// output it into the first argument

// Color

void  VRGBA_Copy(float* to, float* from);


void  VRGB_Copy(float* to, float* from);

#define VRGB_BLACK  ((float[3]){0.0f, 0.0f, 0.0f})
#define VRGB_WHITE  ((float[3]){1.0f, 1.0f, 1.0f})
#define VRGB_RED    ((float[3]){1.0f, 0.0f, 0.0f})
#define VRGB_GREEN  ((float[3]){0.0f, 1.0f, 0.0f})
#define VRGB_BLUE   ((float[3]){0.0f, 0.0f, 1.0f})
#define VRGB_PURPLE ((float[3]){1.0f, 0.0f, 1.0f})
#define VRGB_YELLOW ((float[3]){1.0f, 1.0f, 0.0f})
#define VRGB_CYAN   ((float[3]){0.0f, 1.0f, 1.0f})


#define VRGBA_BLACK  ((float[4]){0.0f, 0.0f, 0.0f, 1.0f})
#define VRGBA_WHITE  ((float[4]){1.0f, 1.0f, 1.0f, 1.0f})
#define VRGBA_RED    ((float[4]){1.0f, 0.0f, 0.0f, 1.0f})
#define VRGBA_GREEN  ((float[4]){0.0f, 1.0f, 0.0f, 1.0f})
#define VRGBA_BLUE   ((float[4]){0.0f, 0.0f, 1.0f, 1.0f})
#define VRGBA_PURPLE ((float[4]){1.0f, 0.0f, 1.0f, 1.0f})
#define VRGBA_YELLOW ((float[4]){1.0f, 1.0f, 0.0f, 1.0f})
#define VRGBA_CYAN   ((float[4]){0.0f, 1.0f, 1.0f, 1.0f})


//#endif
#ifdef VLIB_IMPLEMENTATION


void VRGBA_Copy(float* to, float* from) {
    for (int i = 0; i < 4; i++) {
	to[i] = from[i];
    }
}

void VRGB_Copy(float* to, float* from) {
    for (int i = 0; i < 3; i++) {
	to[i] = from[i];
    }
}



#endif
#endif

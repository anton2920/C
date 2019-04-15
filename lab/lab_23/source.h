#ifndef LAB_DEV_SOURCE_H
#define LAB_DEV_SOURCE_H

/* Header inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <math.h>

/* Macros */
#define TITLE ("lab_23")
#define WIDTH (800)
#define HEIGHT (800)
#define BALL_C (15)
#define RND_COST (10)
#define RED_COEFF (2)

#define CLION (0)

#if (CLION == 1)
#define FONT_PATH ("../font/Font.ttf")
#define MUS_PATH ("../wav/Very_bad_piece.wav")
#define SND_PATH ("../wav/snd.wav")
#define PIC_PATH ("../pictures/purepng.png")
#define BLUE_PIC_PATH ("../pictures/purepng_copy.png")
#else
#define FONT_PATH ("Font.ttf")
#define MUS_PATH ("Very_bad_piece.wav")
#define SND_PATH ("snd.wav")
#define PIC_PATH ("purepng.png")
#define BLUE_PIC_PATH ("purepng_copy.png")
#endif

/* Types */
typedef enum {
    false,
    true
} bool;

enum texttype {
    shaded,
    solid
};

enum speed {
    up_speed = 5,
    down_speed = 5,
    left_speed = 5,
    right_speed = 5
};

/* source.c */
bool check_args(int, char *[]);
bool SDL_Init_All(struct SDL_Window **, struct SDL_Renderer **);
struct SDL_Texture *get_text_texture(struct SDL_Renderer *renderer, char *text, struct _TTF_Font *font,
                                     struct SDL_Color *, struct SDL_Color *, enum texttype);
void draw_text(struct SDL_Renderer *, struct SDL_Texture *, struct SDL_Rect *);
void init_balls(struct SDL_Rect *, int, int *);
void draw_balls(struct SDL_Renderer *, struct SDL_Rect *, int, struct SDL_Texture *, struct SDL_Texture *,
                int *, struct _TTF_Font *my_font, struct SDL_Rect *);
bool is_ball_hit(struct SDL_Rect *, int, int);
void loadmusic(Mix_Music *);
void sound(struct Mix_Chunk *, char *);


#endif
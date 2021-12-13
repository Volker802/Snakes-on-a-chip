#ifndef SNAKE_H
#define SNAKE_H

#pragma once

#include <stdint.h>
#include "food.h"
#include "OLEDDisplay.h"

typedef struct
{
    uint32_t x;
    uint32_t y;
} pixel_t;

typedef struct
{
    int32_t x;
    int32_t y;
} vector_t;

typedef enum
{
    eUP,
    eRIGHT,
    eDOWN,
    eLEFT,
} dir_t;

typedef struct snake_s
{
    pixel_t pos[100];
    dir_t dir;
    vector_t vec;
    uint32_t length;
    bool alive;
} snake_t;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

snake_t *snake_init(void);
void snake_move(void);
void snake_update_direction(void);
bool snake_ate_food(void *f);

#endif // SNAKE_H

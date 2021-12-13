#ifndef FOOD_H
#define FOOD_H

#include "snake.h"

typedef struct food_s {
    uint8_t x;
    uint8_t y;
} food_t;

food_t *food_init(void);
void food_generate_new(void *s);

#endif // FOOD_H

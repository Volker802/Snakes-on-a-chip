#include "food.h"
#include "snake.h"
#include "stdlib.h"

static food_t food;

void food_generate_new(void *s)
{
    snake_t *snake = (snake_t *)s;
    bool food_collide;
    do
    {
        food_collide = false;
        food.x = (rand() % (128 + 1 - 0)) + 0;
        food.y = (rand() % (64 + 1 - 0)) + 0;

        /* Does food collide with snake? */
        for (int i = 0; i <= snake->length; i++)
        {
            if (snake->pos[i].x == food.x && snake->pos[i].y == food.y)
            {
                food_collide = true;
                break;
            }
        }
    } while (food_collide);
}

food_t *food_init(void)
{
    food.x = 80;
    food.y = 50;

    return &food;
}


#include "snake.h"
#include "food.h"

#define SNAKE_START_LENGTH 30
#define HEAD 0

static snake_t snake = {0};

snake_t *snake_init(void)
{
  snake.length = SNAKE_START_LENGTH;
  snake.dir = eRIGHT;
  snake.vec = right;
  snake.alive = true;

  for (int i = 0; i < snake.length; i++)
  {
    snake.pos[i].x = snake.length - i;
    snake.pos[i].y = 50;
  }

  return &snake;
}

static pixel_t wrap(pixel_t p)
{
  p.x = p.x % SCREEN_WIDTH;

  if (p.y >= SCREEN_HEIGHT)
  {
    p.y = 16;
  }
  if (p.y <= 15)
  {
    p.y = SCREEN_HEIGHT - 1;
  }

  return p;
}

static void snake_grow(void)
{
  snake.pos[snake.length + 1].x = snake.pos[snake.length].x;
  snake.pos[snake.length + 1].y = snake.pos[snake.length].y;
  snake.length++;
}

static bool snake_collision_with_body(void)
{
  for (int i = 1; i <= snake.length; i++)
  {
    if (snake.pos[0].x == snake.pos[i].x && snake.pos[0].y == snake.pos[i].y)
    {
      return true;
    }
  }
  return false;
}

void snake_move(void)
{
  /* push tail and body forward */
  for (int i = snake.length; i > 0; i--)
  {
    snake.pos[i].x = snake.pos[i - 1].x;
    snake.pos[i].y = snake.pos[i - 1].y;
  }
  /* move head */
  snake.pos[0].x += snake.vec.x;
  snake.pos[0].y += snake.vec.y;

  snake.pos[0] = wrap(snake.pos[0]);

  if (snake_collision_with_body())
  {
    snake.alive = false;
  }
}

bool snake_ate_food(void *f)
{
  food_t *food = (food_t *)f;
  if (snake.pos[0].x == food->x && snake.pos[0].y == food->y)
  {
    snake_grow();
    return true;
  }
  return false;
}

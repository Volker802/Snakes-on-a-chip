#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "OLEDDisplay.h"
#include "food.h"
#include "snake.h"
#include "mpu6050.hpp"
#include "kalmanfilter.hpp"
#include <cmath>

static snake_t *snake;
static food_t *food;
static uint32_t score = 0;
float pitch, roll;
static const char *TAG = "oled-example";
char gyrox[20] = {0};
char gyroy[20] = {0};
char gyroz[20] = {0};

int SnakeRunTime = 10;
#define CONFIG_I2C_MASTER_FREQUENCY = 700000

bool negpitch, negroll, pospitch, posroll;

#define _I2C_NUMBER(num) I2C_NUM_1
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define I2C_MASTER_SCL_IO 4          /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 5          /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(1) /*!< I2C port number for master dev */

OLEDDisplay_t *oled;

MPU6050 *mpu;

static const vector_t right = {
    .x = 1,
    .y = 0,
};

static const vector_t left = {
    .x = -1,
    .y = 0,
};

static const vector_t up = {
    .x = 0,
    .y = -1,
};

static const vector_t down = {
    .x = 0,
    .y = 1,
};

void display_score(uint32_t score)
{
    char str[20] = {0};
    sprintf(str, "Score: %i", score); // Start at top-left corner
    OLEDDisplay_drawString(oled, 64, 25, str);
}

void display_snake(snake_t *snake)
{
    for (int i = 0; i < snake->length; i++)
    {
        OLEDDisplay_setPixel(oled, snake->pos[i].x, snake->pos[i].y);
    }
}

void display_food(food_t *food)
{
    OLEDDisplay_setPixel(oled, food->x, food->y);
}

void snake_direction(void) //better way to implement this.
{
    if ((pitch > 0) && (roll > 0))
    {
        snake->dir = eDOWN;
        return;
    }
    if ((pitch < 0) && (roll < 0))
    {
        snake->dir = eUP;
        return;
    }
    if ((pitch < 0) && (roll > 0))
    {
        snake->dir = eLEFT;
        return;
    }
    if ((pitch > 0) && (roll < 0))
    {
        snake->dir = eRIGHT;
        return;
    }

    switch (snake->dir)
    {
    case eUP:
        snake->dir = eLEFT;
        snake->vec = left;
        break;

    case eRIGHT:
        snake->dir = eUP;
        snake->vec = up;
        break;

    case eDOWN:
        snake->dir = eRIGHT;
        snake->vec = right;
        break;

    case eLEFT:
        snake->dir = eDOWN;
        snake->vec = down;
        break;

    default:;
    }
}

void display_game_over(void)
{
    OLEDDisplay_clear(oled);
    OLEDDisplay_drawString(oled, 10, 26, "GAME OVER");
    OLEDDisplay_display(oled);
}

void checkDirChange()
{
    if (negpitch == true)
    {
        if (pitch > 0)
        {
            snake_direction();
        }
    }
    if (pospitch == true)
    {
        if (pitch < 0)
        {
            snake_direction();
        }
    }
    if (negroll == true)
    {
        if (roll > 0)
        {
            snake_direction();
        }
    }
    if (posroll == true)
    {
        if (roll < 0)
        {
            snake_direction();
        }
    }
}
static void i2c_test_task(void *arg)
{
    portTickType lastWakeTime;
    portTickType loopPeriod = SnakeRunTime;
    lastWakeTime = xTaskGetTickCount();

    OLEDDisplay_clear(oled);
    OLEDDisplay_drawString(oled, 10, 26, "GAME BEGIN");
    OLEDDisplay_display(oled);

    snake = snake_init();
    food = food_init();
    MPU6050 mpu(GPIO_NUM_4, GPIO_NUM_5, I2C_NUM_0);
    if (!mpu.init())
    {
        ESP_LOGE("mpu6050", "init failed!");
    }
    ESP_LOGI("mpu6050", "init success!");

    float ax, ay, az, gx, gy, gz;

    for (;;)
    {
        OLEDDisplay_clear(oled);
        ax = -mpu.getAccX();
        ay = -mpu.getAccY();
        az = -mpu.getAccZ();
        gx = mpu.getGyroX();
        gy = mpu.getGyroY();
        gz = mpu.getGyroZ();
        pitch = (atan(ax / az) * 57.2958);
        roll = (atan(ay / az) * 57.2958);
        ESP_LOGI("mpu6050", "Gyro: ( %.3f, %.3f, %.3f)", gx, gy, gz);
        ESP_LOGI("mpu6050", "Pitch: %.3f", pitch);
        ESP_LOGI("mpu6050", "Roll: %.3f", roll);
        if (snake->alive)
        {
            OLEDDisplay_resetDisplay(oled);
            OLEDDisplay_drawRect(oled, 0, 0, 128, 64);
            //display_score(score);
            display_snake(snake);
            display_food(food);
            OLEDDisplay_display(oled);
            if (pitch < 0)
            {
                negpitch = true;
            }
            if (roll < 0)
            {
                negroll = true;
            }
            if (pitch > 0)
            {
                pospitch = true;
            }
            if (roll > 0)
            {
                pospitch = true;
            }
            checkDirChange();
            snake_move();
            if (snake_ate_food(food))
            {
                score++;
                food_generate_new(snake);
            }
        }
        else
        {
            display_game_over();
        }
        vTaskDelayUntil(&lastWakeTime, loopPeriod);
    }
    vTaskDelete(NULL);
}

void gameSetup(void)
{
    oled = OLEDDisplay_init(I2C_MASTER_NUM, 0x78, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    ESP_LOGI(TAG, "Running");
    xTaskCreate(i2c_test_task, "i2c_test_task_0", 16384, (void *)0, 10, NULL);
}
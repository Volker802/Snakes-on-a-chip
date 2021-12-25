# Snakes-on-a-chip

Snake Game on ESP32-Wemos-Lolin32 (With inbuilt SSD1306 OLED module). Uses MPU6050 to control direction of snake.
Places where I acquired different libraries:

SSD1306 library: https://github.com/ThingPulse/esp8266-oled-ssd1306.

MPU6050 library: https://github.com/imxieyi/esp32-i2c-mpu6050.

Snake game library: https://github.com/carljohanlandin/snake.

To Do:
1. Increase speed/runtime of the game. This is an issue that significantly reduces the run time of the game. Also compounded by the fact that there are so many pixels to cover. Perhaps in future the size of snake and food must be increased.

2. Significant code clean-up. 

3. Change the way it detects directional changes. Right now I am using boolean flags to accomplish this but that is a very bad way to do in this particular scenario. This will have to be replaced with another method.

4. Add claibration to get initial rest direction instead of hard-coding the values. The direction will change depending on how your IMU modules is placed. So hardcoding this direction is also not a practical application.

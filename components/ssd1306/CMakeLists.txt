idf_component_register(SRCS "src/OLEDDisplay.cpp" 
										"src/OLEDDisplayFonts.c"
                    "src/MainGame.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES snake mpu6050)

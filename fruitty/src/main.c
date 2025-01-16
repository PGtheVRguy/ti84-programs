#include <math.h>
#include <graphx.h>
#include <keypadc.h>
#include <tice.h>
#include <stdlib.h>
#include <ti/real.h>

int OneDimDistance(int a, int b) {
    return abs(a - b);
}

bool PositionInRadius(int x1, int y1, int x2, int y2, int radius) {
    if (OneDimDistance(x1, x2) < radius && OneDimDistance(y1, y2) < radius) {
        return true;
    }
    return false;
}

int main(void) {
    int x = 160; // Initial x-coordinate of the box
    int y = 180; // Fixed y-coordinate of the box
    int box_width = 30, box_height = 15; // Box dimensions
    int score = 0;
    int fruitSize = 10;
    int fruitPosY = -fruitSize;
    int fruitPosX = randInt(40, 320-40);
    char buffer[10]; // Buffer for score string
    int mainState = 0;
    int screen_height = 240;
    // Initialize graphics
    gfx_Begin();
    gfx_SetDrawBuffer(); // Use double buffering to avoid flicker
    gfx_SetColor(0);     // Background color (black)

    while (1) {
        // Scan the keypad for input
        kb_Scan();

        // Clear the screen
        gfx_FillScreen(0);


        switch(mainState){
            case 0:
                gfx_SetTextFGColor(255); 
                gfx_SetTextBGColor(0);
                gfx_SetTextTransparentColor(0);
                gfx_SetTextScale(1, 1);
                gfx_PrintStringXY("Fruitty", 120, 100);
                gfx_PrintStringXY("Press ENTER to start", 100, 120);
                if(kb_Data[6] & kb_Enter){
                    mainState = 1;
                }
                break;
            case 2:
                gfx_SetTextScale(2, 2);
                gfx_PrintStringXY("Game Over", 90, 100);
                gfx_PrintStringXY("Score: ", 100, 120);
                real_t score_real_menu = os_Int24ToReal(score);
                os_RealToStr(buffer, &score_real_menu, 0, 0, 5);
                gfx_PrintStringXY(buffer, 200, 120);
                gfx_SetTextScale(1, 1);
                gfx_PrintStringXY("Press ENTER to restart", 80, 140);
                if(kb_Data[6] & kb_Enter){
                    score = 0;
                    mainState = 1;
                    fruitPosY = -fruitSize;
                }
                break;
            case 1:
                // Check for key presses
                if (kb_Data[7] & kb_Left) {
                    x -= 2; // Move left
                }
                if (kb_Data[7] & kb_Right) {
                    x += 2; // Move right
                }

                // Constrain the box within the screen bounds
                if (x < 0) x = 0;
                if (x > 320 - box_width) x = 320 - box_width;

                fruitPosY += 1;
                gfx_SetColor(31);
                gfx_FillRectangle(fruitPosX, fruitPosY, fruitSize, fruitSize);


                // Draw the box
                gfx_SetColor(255); // Box color (white)
                gfx_FillRectangle(x, y, box_width, box_height);



            if((fruitPosX > x && fruitPosX < x + box_width) && (fruitPosY < y+box_height && fruitPosY > y))
            {
                    score += 1;
                    fruitPosY = -fruitSize;
                    fruitPosX = randInt(40, 320-40);
            }

                // Convert score to string
                real_t score_real = os_Int24ToReal(score);
                os_RealToStr(buffer, &score_real, 0, 0, 5);

                int score_width = gfx_GetStringWidth(buffer);

                gfx_SetTextFGColor(255); 
                gfx_SetTextBGColor(0);
                gfx_SetTextTransparentColor(0);

                gfx_SetTextScale(2, 2);
                gfx_PrintStringXY(buffer, (320 - score_width) / 2, 120);
                if(fruitPosY > screen_height){
                    mainState = 2;
                }
                break;

        }
        // Swap buffers to show the changes
        gfx_SwapDraw();



        // Exit condition: Press [CLEAR] to quit
        if (kb_Data[6] & kb_Clear) {
            break;
        }


        // Delay to control frame rate
        delay(1000 / 60);
    }

    gfx_End();
    return 0;
}
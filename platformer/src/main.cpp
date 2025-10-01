#include <math.h>
#include <graphx.h>
#include <ti/getcsc.h>
#include <keypadc.h>
#include <tice.h>
#include <stdlib.h>

#include "gfx/gfx.h"



/* Include the external tilemap data */
extern unsigned char tilemap_map[];

/* Tilemap defines */
#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define TILEMAP_WIDTH       58
#define TILEMAP_HEIGHT      30

#define TILEMAP_DRAW_WIDTH  20
#define TILEMAP_DRAW_HEIGHT 15

#define Y_OFFSET            0
#define X_OFFSET            0

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT  240


int OneDimDistance(int a, int b) {
    return abs(a - b);
}

bool PositionInRadius(int x1, int y1, int x2, int y2, int radius) {
    if (OneDimDistance(x1, x2) < radius && OneDimDistance(y1, y2) < radius) {
        return true;
    }
    return false;
}
int clamp(int x, int minimum, int maximum) {
    if (x < minimum){
        return minimum;}
    if (x > maximum) {
        return maximum;
    }
    return x;
}

void drawTilemap(gfx_tilemap_t tm, int x, int y) {

    y = clamp(y, 0, ((  TILEMAP_HEIGHT*TILE_HEIGHT) - (SCREEN_HEIGHT) ) );
    x = clamp(x, 0, ((  TILEMAP_WIDTH*TILE_WIDTH) - (SCREEN_WIDTH) ) );

    gfx_TransparentTilemap(&tm, x, y);
}




class Player {
    public:

        double move_speed = 2;
        double x = 0;
        double y = 0;

        double hsp = 0;
        double vsp = 0;
        void move_and_collide() {
            x += hsp;
        }

};


int main(void) {

    Player player;
    int box_width = 30, box_height = 15; // Box dimensions
    unsigned int cam_x = 0, cam_y = 0;
    // Initialize graphics


    gfx_tilemap_t tilemap;

    /* Initialize the tilemap structure */
    tilemap.map         = tilemap_map;
    tilemap.tiles       = tileset_tiles;
    tilemap.type_width  = gfx_tile_16_pixel;
    tilemap.type_height = gfx_tile_16_pixel;
    tilemap.tile_height = TILE_HEIGHT;
    tilemap.tile_width  = TILE_WIDTH;
    tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    tilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    tilemap.height      = TILEMAP_HEIGHT;
    tilemap.width       = TILEMAP_WIDTH;
    tilemap.y_loc       = Y_OFFSET;
    tilemap.x_loc       = X_OFFSET;



    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetColor(0);     // Background color (black)
    gfx_SetDrawBuffer(); // Use double buffering to avoid flicker
    gfx_SetMonospaceFont(8);

    while (true) {

        // Scan the keypad for input
        kb_Scan();

        // Clear the screen
        gfx_FillScreen(1);


        drawTilemap(tilemap, cam_x+player.x, cam_y);

        if (kb_Data[7] & kb_Right) {
            player.hsp = player.move_speed;
        }
        else if (kb_Data[7] & kb_Left){
            player.hsp = -player.move_speed;
        }
        else {
            player.hsp = 0;
        }

        //debug camera
        /*if (kb_Data[7] & kb_Right) {
            cam_x += 2;
        }
        if (kb_Data[7] & kb_Left) {
            cam_x -= 2;
        }*/
        if (kb_Data[7] & kb_Up) {
            cam_y -= 2;
        }
        if (kb_Data[7] & kb_Down) {
            cam_y += 2;
        }
        player.move_and_collide();
        gfx_SetColor(31);

        // Draw the box (player)
        gfx_SetColor(255); // Box color (white)
        gfx_FillRectangle(player.x, player.y, box_width, box_height);


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
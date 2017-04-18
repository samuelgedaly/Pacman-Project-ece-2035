/* Gatech ECE2035 2015 SPRING PAC MAN
 * Copyright (c) 2015 Gatech ECE2035
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include "mbed.h"
#include "globals.h"
#include "map_public.h"
#include "pacman.h"

PLAYER pacman;
int score;

void pacman_init(int grid_x, int grid_y){
    pacman.motion = PACMAN_HEADING_RIGHT;
    pacman.status = PACMAN_WAIT_COMMAND;
    pacman.grid_x = grid_x;
    pacman.grid_y = grid_y;
    map_eat_cookie(grid_x,grid_y); //clear the cookie on the grid.
    pacman_draw(PACMAN_COLOR);
    score = 0;
    uLCD.color(WHITE);
    uLCD.printf("score:%d",score);
}

void pacman_reinit(int grid_x, int grid_y){
    pacman.motion = PACMAN_HEADING_RIGHT;
    pacman.status = PACMAN_WAIT_COMMAND;
    pacman.grid_x = grid_x;
    pacman.grid_y = grid_y;
    map_eat_cookie(grid_x,grid_y); //clear the cookie on the grid.
    pacman_draw(PACMAN_COLOR);
}


void pacman_draw(int color){
    int x = pacman.grid_x;
    int y = pacman.grid_y;
    GRID grid_info = map_get_grid_status(x,y);
    int screen_x = grid_info.x + GRID_RADIUS;
    int screen_y = grid_info.y + GRID_RADIUS;
    uLCD.filled_circle(screen_x, screen_y, GRID_RADIUS, color);
    //draw the mouth of Pacman
    switch(pacman.motion){
        case PACMAN_HEADING_RIGHT: 
            uLCD.filled_rectangle(screen_x,screen_y-1,screen_x+GRID_RADIUS,screen_y+1, BACKGROUND_COLOR);
            break;
        case PACMAN_HEADING_LEFT: 
            uLCD.filled_rectangle(screen_x-GRID_RADIUS,screen_y-1,screen_x,screen_y+1, BACKGROUND_COLOR);
            break;
        case PACMAN_HEADING_UP: 
            uLCD.filled_rectangle(screen_x-1,screen_y-GRID_RADIUS,screen_x+1,screen_y, BACKGROUND_COLOR);
            break;
        case PACMAN_HEADING_DOWN: 
            uLCD.filled_rectangle(screen_x-1,screen_y,screen_x+1,screen_y+GRID_RADIUS, BACKGROUND_COLOR);
            break;
        default: //head right
            uLCD.filled_rectangle(screen_x,screen_y-1,screen_x+GRID_RADIUS,screen_y+1, BACKGROUND_COLOR);
            break;
    }
}

void pacman_draw_lives(int x, int y){
    uLCD.filled_circle(x, y, GRID_RADIUS, PACMAN_COLOR);
    //draw the mouth of Pacman
    uLCD.filled_rectangle(x,y-1,x+GRID_RADIUS,y+1, BACKGROUND_COLOR);
}

void pacman_clear(void){
    int x = pacman.grid_x;
    int y = pacman.grid_y;
    GRID grid_info = map_get_grid_status(x,y);
    uLCD.filled_rectangle(grid_info.x, grid_info.y, grid_info.x+GRID_SIZE-1, grid_info.y+GRID_SIZE-1, BACKGROUND_COLOR);
}

void pacman_set_action(PACMAN_MOTION motion){
    if(motion == PACMAN_NO_UPDATE)
        return;
        
    pacman.motion = motion;
    pacman.status = PACMAN_RUNNING;
}

int getX(void){
    return pacman.grid_x;    
}

int getY(void){
    return pacman.grid_y;    
}

int getX_previous(void){
    return pacman.previous_x;    
}

int getY_previous(void){
    return pacman.previous_x;    
}

int get_score(void) {
    return score;    
}

void set_score(int sc){
    score += sc;
    uLCD.locate(0,0);
    uLCD.color(WHITE); 
    uLCD.printf("score:%d",score);
}

void ephemeral_fruit(void){
    int x = 8;
    int y = 7;
    GRID grid_info = map_get_grid_status(x,y);
    int screen_x = grid_info.x + GRID_RADIUS;
    int screen_y = grid_info.y + GRID_RADIUS;
    uLCD.filled_circle(screen_x, screen_y, GRID_RADIUS, BLUE);
    uLCD.filled_circle(screen_x, screen_y, GRID_RADIUS-2, BACKGROUND_COLOR);
}

void delete_fruit(void){
    int x = 8;
    int y = 7;
    GRID grid_info = map_get_grid_status(x,y);
    int screen_x = grid_info.x + GRID_RADIUS;
    int screen_y = grid_info.y + GRID_RADIUS;
    uLCD.filled_circle(screen_x, screen_y, GRID_RADIUS, BACKGROUND_COLOR);
}

GRID_STATUS pacman_update_position(bool invincible){
    GRID next_grid_info;
    int x = pacman.grid_x;
    int y = pacman.grid_y;
    pacman.previous_x = x;
    pacman.previous_y = y;
    if(pacman.status==PACMAN_RUNNING){
        switch(pacman.motion){
            case PACMAN_HEADING_UP:
                if(y==0)
                    y = NUM_GRID_Y-1;
                else
                    y--;
                break;
            case PACMAN_HEADING_DOWN:
                if(y==(NUM_GRID_Y-1))
                    y = 0;
                else
                    y++;
                break;
            case PACMAN_HEADING_RIGHT:
                if(x==NUM_GRID_X-1)
                    x = 0;
                else
                    x++;
                break;
            case PACMAN_HEADING_LEFT:
                if(x==0)
                    x = NUM_GRID_X-1;
                else
                    x--;
                break;
        }
        next_grid_info = map_get_grid_status(x,y);
        if(next_grid_info.status==GRID_WALL){
            pacman.status=PACMAN_WAIT_COMMAND;
        }
        else{
            //clear the picture of previous
            pacman_clear();
            //update pacman position in map
            pacman.grid_x = x;
            pacman.grid_y = y;
            if(!invincible){
                pacman_draw(PACMAN_COLOR);
            } else {
                pacman_draw(RED);
            }
            if(next_grid_info.status>=GRID_COOKIE){
                map_eat_cookie(x,y);
                score++;
                if(next_grid_info.status==GRID_SUPER_COOKIE){ //one super cookie worth 5 points
                    score+=4;
                }
                uLCD.locate(0,0);
                uLCD.color(WHITE);
                uLCD.printf("score:%d",score);
            }
        }
    }
    return next_grid_info.status;
        
}
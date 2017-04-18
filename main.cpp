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
/** @file main.cpp */
// Include header files for platform
#include "mbed.h"
#include "wave_player.h"
#include "SDFileSystem.h"

// Include header files for pacman project
#include "globals.h"
#include "map_public.h"
#include "pacman.h"
#include "ghost.h"
#include "MMA8452.h"
#include "doubly_linked_list.h"
#include "vector.h"

// Platform initialization
DigitalIn left_pb(p21);  // push bottem
DigitalIn right_pb(p22); // push bottem
DigitalIn up_pb(p23);    // push bottem
DigitalIn down_pb(p24);  // push bottem
uLCD_4DGL uLCD(p9,p10,p11); // LCD (serial tx, serial rx, reset pin;)
Serial pc(USBTX,USBRX);     // used by Accelerometer
MMA8452 acc(p28, p27, 100000); // Accelerometer
AnalogOut DACout(p18);      // speaker
wave_player waver(&DACout); // wav player
SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card and filesystem (mosi, miso, sck, cs)
DigitalOut led(LED1);

// Example of the decleration of your implementation
void playSound(char * wav);


/** Main() is where you start your implementation
    @brief The hints of implementation are in the comments. <br>
    @brief You are expected to implement your code in main.cpp and pacman.cpp. But you could modify any code if you want to make the game work better.
*/
int main()
{   
vector v;
vector_init(&v);
// This while loop is to restart the game 
while(1){
    // Initialize the timer
    /// [Example of time control implementation]
        /// Here is a rough example to implement the timer control <br><br>
    int tick, pre_tick, fruit_tick;
    int tick_inv = 0;
    srand (time(NULL));
    Timer timer;
    timer.start();
    tick = timer.read_ms();
    pre_tick = tick;
     bool go_down = false;
     bool go_up = false;
     bool go_left = false;
     bool go_right = false;
     bool invincible = false;
     bool levelUp = false;
     bool fruit = false;
     int lives = 3;
     int level = 1;
     int totalscore = 0;
     int fruit_activate = 120;
     GRID_STATUS next_status;

    // Initialize the buttons        
    left_pb.mode(PullUp);  // The variable left_pb will be zero when the pushbutton for moving the player left is pressed    
    right_pb.mode(PullUp); // The variable rightt_pb will be zero when the pushbutton for moving the player right is pressed        
    up_pb.mode(PullUp);    //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    down_pb.mode(PullUp);  //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    // interactive initial window to select level (EXTRA FEATURE)
    bool start = true;
    while(right_pb.read()){
        if (start){
            uLCD.cls();
            uLCD.color(WHITE);
            uLCD.printf("Welcome Samuel's pacman, please press right to continue.\n\n");
        }
        start = false;
    }
    start = true;
    while(up_pb.read() && left_pb.read() && up_pb.read() && down_pb.read()){
        if(start){
            uLCD.printf("Please select a level\n\nLevel 1(press down)\nLevel 2(press left)\nLevel 3(press up)");
            if (!left_pb.read()){ 
                level = 2;
            }
            else if(!down_pb.read()){
                level = 1;    
            } else if(!up_pb.read()){
                level = 3;
            }
        }
        start = false;
    }
    playSound("/sd/wavfiles/pacman_beginning.wav");  // cool sounds to begin (Extra Feature)
    
    while(1)
    {
        /// [Example of the game control implementation]
        /// Here is the example to initialize the game <br><br>
        levelUp = false;
        bool lost_life = false;
        uLCD.cls();
        map_init();
        pacman_init(8,9); // Center of the map
        uLCD.locate(0,1);
        uLCD.color(WHITE);
        uLCD.printf("LEVEL:%d   ",level);
        //Your code here
        //Initiate & create & show the ghosts  
        //uLCD.printf("\n ghost show1");
        ghost_init();
        ghost_create(1, 4, RED);
        ghost_create(15, 4, PINK);
        ghost_create(1, 11, WHITE);
        ghost_create(15, 11, ORANGE);
        DLinkedList* list = get_ghost_list();
        ghost_show(list);
        //[Demo of play sound file]
        //ives as icons (EXTRA FEATURE)
        int j;
            for (j = 0; j < 3; j++){
               pacman_draw_lives(100 + (j*10), 10);
           }
    
        /// 1. Begin the game loop
        while(lives > 0 && map_remaining_cookie() > 0 && !levelUp){
            tick = timer.read_ms(); // Read current time
            //ephemeral fruit (Extra Feature)
            if (map_remaining_cookie() < fruit_activate && !fruit) {
                fruit_activate  = fruit_activate - 20;
                fruit = true;
                fruit_tick = tick;
                ephemeral_fruit();
            }
            if (tick - fruit_tick > 10000){
                fruit = false;
                delete_fruit();
            }
            if (getX() == 8 && getY() == 7 && fruit){
                map_eat_fruit();
                delete_fruit();
                set_score(20);
                fruit = false;
            }
            //if two push buttons advance level
            if (!left_pb.read() && !right_pb.read()) {
                levelUp = true;    
            }
        
            /// 2. Implement the code to get user input and update the Pacman
            //double x , y , z;
            //acc.readXYZGravity(&x, &y, &z);
            // taking ghost positions
            bool st = true;
            while(!lost_life){
                if(st){
                    int j;
                    for (j = 0; j < 3; j++){
                        uLCD.filled_circle(100 + (j*10), 10, GRID_RADIUS, BACKGROUND_COLOR);
                    }
                    for (j = 0; j < lives; j++){
                        pacman_draw_lives(100 + (j*10), 10);
                    }
                }
                lost_life = true;
                st = false;
            }
            //check for invincibility
            if (level == 1 && invincible && tick - tick_inv > 8000) {
                invincible = false;    
            } else if(level == 2 && invincible && tick - tick_inv > 4000){
                invincible = false;
            } else if (level == 3 && invincible && tick - tick_inv > 2000){
                invincible = false; 
            }
            //Push buttons to control pacman inputs
            if (!down_pb.read()) {
                go_down = true;
                go_up = false;
                go_left = false;
                go_right = false;
            }
            
            else if (!up_pb.read()) {
                go_down = false;
                go_up = true;
                go_left = false;
                go_right = false;
    
            } else if (!left_pb.read()) {
                go_down = false;
                go_up = false;
                go_left = true;
                go_right = false;
        
            } else if (!right_pb.read()) {
               go_down = false;
                go_up = false;
                go_left = false;
                go_right = true;
                
            }
            //updating motion
            if (go_down) {
                pacman_set_action(PACMAN_HEADING_DOWN);
            }
            if (go_up) {
                pacman_set_action(PACMAN_HEADING_UP);
            }
             if (go_left) {
                pacman_set_action(PACMAN_HEADING_LEFT);   
            }
            if (go_right) {
                pacman_set_action(PACMAN_HEADING_RIGHT);
            }
            //accelerometer inputs
            acc.activate(); 
            int x,y,z;
            acc.readXYZCounts(&x, &y, &z);
            
                if (x>200)
                    pacman_set_action(PACMAN_HEADING_LEFT);
                if (x<-200)
                    pacman_set_action(PACMAN_HEADING_RIGHT);
                if (y<-200)
                    pacman_set_action(PACMAN_HEADING_UP);
                if (y>200)
                    pacman_set_action(PACMAN_HEADING_DOWN);
            
            /// -[Hint] Implement the code to move Pacman. You could use either push-button or accelerometer. <br>
            /// The accelerometer's function readXYZGravity() might be useful.
                
            if((tick-pre_tick)>500){ // Time step control
                 pre_tick = tick;
            
            /// 3. Update the Pacman on the screen
            //move pacman and check super cookie
            next_status = pacman_update_position(invincible); // make pacman red when eats super cookie (Extra feature)
                if (next_status == GRID_SUPER_COOKIE) {
                    invincible = true;
                    tick_inv = timer.read_ms();
                }
            //move ghosts
            ghost_random_walk();
            
            /// -[Hint] You could update the position of Pacman here based on the input at step 2. <br>
            
            }
            
            //check collision
            GHOST* newGhost = (GHOST*)getHead(list);
            while(newGhost) {
                if (newGhost->x == getX() && newGhost->y == getY() || ((getX() == newGhost->previous_x && getY() == newGhost->previous_y) && (newGhost->x == getX_previous() && newGhost->y == getY_previous()))) {
                       if (!invincible){
                           pacman_clear();
                           pacman_reinit(8,9);
                           lives--;
                           // updating life icons
                           lost_life = false;
                       } else {
                           playSound("/sd/wavfiles/pacman_eatghost.wav");
                            deleteGhost(newGhost, list);
                            set_score(10); 
                      }
                        break;
                }    
                newGhost = (GHOST*)getNext(list);
            }
             
         
        /// 4. Implement the code to check the end of game.
        /// -[Hint] Check whether the ghost catch the Pacman. Make sure you could always detect that the ghost and Pacman meet on the screen.
        /// One tricky scenario is that: Pacman is at grid (3,3) and is heading to (3,4), while the ghost is at grid (3,4) and is heading to (3,3).
        /// Either at time t or t+1, you will see that the Pacman and the ghost are not on the same grid.
        /// However, the Pacman should be caught by ghost with this movement.
        /// -[Hint] Check whether Pacman win the game <br>
        
        }
        //If player passes all three levels, wins the game.
         if (map_remaining_cookie() == 0 || levelUp) {
            level++;
            if (lives < 3){
                lives++;
            }
            totalscore += get_score();
            if(level > 3) {
            uLCD.cls();
            uLCD.color(WHITE);
            uLCD.printf("CONGRATS YOU WON\n\n");
            // Using a vector to store previous scores and show them when game finishes (EXTRA FEATURE)
            if (vector_total(&v) > 0){
                uLCD.color(WHITE);
                uLCD.printf("Previous scores:\n");
            }
            int i;
            for (i = 0; i < vector_total(&v); i++){
                uLCD.color(BLUE);
                uLCD.printf("%d: %d\n", i+1, (int *) vector_get(&v, i));
            }
            uLCD.color(WHITE);
            uLCD.printf("Total score: %d", totalscore);
            vector_add(&v, totalscore);
            
            start = true;
            while(down_pb.read()){
                if (start){
                uLCD.color(WHITE);
                uLCD.printf("\n\npress down to restart");
                }
                start = false;
            }
            
            break;    
            }
            // if player losses 3 lives, game over.
         } else {
           playSound("/sd/wavfiles/pacman_death.wav");
            totalscore += get_score();
            uLCD.cls();
            uLCD.printf("GAME OVER\n\n");
             
             if (vector_total(&v) > 0){
                uLCD.color(WHITE);
                uLCD.printf("Previous scores:\n");
            }
            int i;
            for (i = 0; i < vector_total(&v); i++){
                uLCD.color(BLUE);
                uLCD.printf("%d: %d\n", i+1, (int *) vector_get(&v, i));
            }
            uLCD.color(WHITE);
            uLCD.printf("Total score: %d", totalscore);
            vector_add(&v, totalscore);
            
            start = true;
            while(down_pb.read()){
                if (start){
                uLCD.color(WHITE);
                uLCD.printf("\n\npress down to restart");
                }
                start = false;
            }
            break;
        }
    }
    }
}



// Example of implementation of your functions
void playSound(char * wav){
    // open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");

    // play wav file
    waver.play(wave_file);

    // close wav file
    fclose(wave_file);
}




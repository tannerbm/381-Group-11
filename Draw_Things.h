/*
 * Draw_Things.h
 *
 *  Created on: 2014-09-30
 *      Author: Tanner
 */

#ifndef DRAW_THINGS_H_
#define DRAW_THINGS_H_

int move( int x[][100], int y[][100], int z, int colour, int laser);  // passes in the x values, y values, z for left and right, and colour, returns z to remember direction
void draw( int x1, int y1, int x2, int y2, int colour);  // just draws the boxes on the screen, could be replaced later be a bit map drawing
void aliens_alive(int x[][100], int y[][100]); //turns aliens on (1) or off (0) --> off being alien draw all black
void draw_alien_row(int start_index, int end_index, int x[][100], int y[][100], int colour); //uses draw function to draw each row
void change_line_xcoord(int start_of_row, int end_of_row, int x[][100], int y[][100]); //changes x coord of each row

#endif /* DRAW_THINGS_H_ */

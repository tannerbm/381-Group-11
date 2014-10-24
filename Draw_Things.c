/*
 *Draw_Things.c
 *Created on: Sep 28, 2014
 * Author: Dylan
 * Purpose: Implements the logic to move the alien rows and redraw them.
 */

#include <system.h>
#include "Draw_Things.h"
#include <time.h>
#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#define drawer_base (volatile int *) 0x4020

int move(int x[][100], int y[][100], int z, int colour, int laser) {
	int i; // variable used in the for loops
	int down;

	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(
			"/dev/video_pixel_buffer_dma_0");

	// Set the 1st buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr1);


	draw_alien_row(11, 19, x, y, 0);
	draw_alien_row(21, 29, x, y, 0);
	draw_alien_row(31, 39, x, y, 0);
	draw_alien_row(41, 49, x, y, 0);
	draw_alien_row(51, 59, x, y, 0);

	/* These embedded if statements allow us to dynamically move the aliens
	 * further across the screen if a row or column is completely dead
	 */

	if (x[11][0] == 0 && x[21][0] == 0 && x[31][0] == 0 && x[41][0] == 0
			&& x[51][0] == 0) { //first column is gone
		if (x[12][0] == 0 && x[22][0] == 0 && x[32][0] == 0 && x[42][0] == 0
				&& x[52][0] == 0) {
			if (x[13][0] == 0 && x[23][0] == 0 && x[33][0] == 0 && x[43][0] == 0
					&& x[53][0] == 0) {
				if (x[14][0] == 0 && x[24][0] == 0 && x[34][0] == 0
						&& x[44][0] == 0 && x[54][0] == 0) {
					if (x[15][0] == 0 && x[25][0] == 0 && x[35][0] == 0
							&& x[45][0] == 0 && x[55][0] == 0) {
						if (x[16][0] == 0 && x[26][0] == 0 && x[36][0] == 0
								&& x[46][0] == 0 && x[56][0] == 0) {
							if (x[17][0] == 0 && x[27][0] == 0 && x[37][0] == 0
									&& x[47][0] == 0 && x[57][0] == 0) {
								if (x[18][1] < 10) {
									z = 1;
									if (x[51][0] == 0 && x[52][0] == 0
											&& x[53][0] == 0 && x[54][0] == 0
											&& x[55][0] == 0 && x[56][0] == 0
											&& x[57][0] == 0 && x[58][0] == 0) { // bottom row is gone
										if (x[41][0] == 0 && x[42][0] == 0
												&& x[43][0] == 0
												&& x[44][0] == 0
												&& x[45][0] == 0
												&& x[46][0] == 0
												&& x[47][0] == 0
												&& x[48][0] == 0) {
											if (x[31][0] == 0 && x[32][0] == 0
													&& x[33][0] == 0
													&& x[34][0] == 0
													&& x[35][0] == 0
													&& x[36][0] == 0
													&& x[37][0] == 0
													&& x[38][0] == 0) {
												if (x[21][0] == 0
														&& x[22][0] == 0
														&& x[23][0] == 0
														&& x[24][0] == 0
														&& x[25][0] == 0
														&& x[26][0] == 0
														&& x[27][0] == 0
														&& x[28][0] == 0) {
													if (y[51][2] < 255)
														down = 1;
												}
												if (y[51][2] < 235)
													down = 1;
											}
											if (y[51][2] < 215)
												down = 1;
										}
										if (y[51][2] < 195)
											down = 1;
									}
								}
							} else if (x[17][1] < 10) {
								z = 1;
								if (x[51][0] == 0 && x[52][0] == 0
										&& x[53][0] == 0 && x[54][0] == 0
										&& x[55][0] == 0 && x[56][0] == 0
										&& x[57][0] == 0 && x[58][0] == 0) { // bottom row is gone
									if (x[41][0] == 0 && x[42][0] == 0
											&& x[43][0] == 0 && x[44][0] == 0
											&& x[45][0] == 0 && x[46][0] == 0
											&& x[47][0] == 0 && x[48][0] == 0) {
										if (x[31][0] == 0 && x[32][0] == 0
												&& x[33][0] == 0
												&& x[34][0] == 0
												&& x[35][0] == 0
												&& x[36][0] == 0
												&& x[37][0] == 0
												&& x[38][0] == 0) {
											if (x[21][0] == 0 && x[22][0] == 0
													&& x[23][0] == 0
													&& x[24][0] == 0
													&& x[25][0] == 0
													&& x[26][0] == 0
													&& x[27][0] == 0
													&& x[28][0] == 0) {
												if (y[51][2] < 255)
													down = 1;
											}
											if (y[51][2] < 235)
												down = 1;
										}
										if (y[51][2] < 215)
											down = 1;
									}
									if (y[51][2] < 195)
										down = 1;
								}
							}
						} else if (x[16][1] < 10) {
							z = 1;
							if (x[51][0] == 0 && x[52][0] == 0 && x[53][0] == 0
									&& x[54][0] == 0 && x[55][0] == 0
									&& x[56][0] == 0 && x[57][0] == 0
									&& x[58][0] == 0) { // bottom row is gone
								if (x[41][0] == 0 && x[42][0] == 0
										&& x[43][0] == 0 && x[44][0] == 0
										&& x[45][0] == 0 && x[46][0] == 0
										&& x[47][0] == 0 && x[48][0] == 0) {
									if (x[31][0] == 0 && x[32][0] == 0
											&& x[33][0] == 0 && x[34][0] == 0
											&& x[35][0] == 0 && x[36][0] == 0
											&& x[37][0] == 0 && x[38][0] == 0) {
										if (x[21][0] == 0 && x[22][0] == 0
												&& x[23][0] == 0
												&& x[24][0] == 0
												&& x[25][0] == 0
												&& x[26][0] == 0
												&& x[27][0] == 0
												&& x[28][0] == 0) {
											if (y[51][2] < 255)
												down = 1;
										}
										if (y[51][2] < 235)
											down = 1;
									}
									if (y[51][2] < 215)
										down = 1;
								}
								if (y[51][2] < 195)
									down = 1;
							}
						}
					} else if (x[15][1] < 10) {
						z = 1;
						if (x[51][0] == 0 && x[52][0] == 0 && x[53][0] == 0
								&& x[54][0] == 0 && x[55][0] == 0
								&& x[56][0] == 0 && x[57][0] == 0
								&& x[58][0] == 0) { // bottom row is gone
							if (x[41][0] == 0 && x[42][0] == 0 && x[43][0] == 0
									&& x[44][0] == 0 && x[45][0] == 0
									&& x[46][0] == 0 && x[47][0] == 0
									&& x[48][0] == 0) {
								if (x[31][0] == 0 && x[32][0] == 0
										&& x[33][0] == 0 && x[34][0] == 0
										&& x[35][0] == 0 && x[36][0] == 0
										&& x[37][0] == 0 && x[38][0] == 0) {
									if (x[21][0] == 0 && x[22][0] == 0
											&& x[23][0] == 0 && x[24][0] == 0
											&& x[25][0] == 0 && x[26][0] == 0
											&& x[27][0] == 0 && x[28][0] == 0) {
										if (y[51][2] < 255)
											down = 1;
									}
									if (y[51][2] < 235)
										down = 1;
								}
								if (y[51][2] < 215)
									down = 1;
							}
							if (y[51][2] < 195)
								down = 1;
						}
					}
				} else if (x[14][1] < 10) {
					z = 1;
					if (x[51][0] == 0 && x[52][0] == 0 && x[53][0] == 0
							&& x[54][0] == 0 && x[55][0] == 0 && x[56][0] == 0
							&& x[57][0] == 0 && x[58][0] == 0) { // bottom row is gone
						if (x[41][0] == 0 && x[42][0] == 0 && x[43][0] == 0
								&& x[44][0] == 0 && x[45][0] == 0
								&& x[46][0] == 0 && x[47][0] == 0
								&& x[48][0] == 0) {
							if (x[31][0] == 0 && x[32][0] == 0 && x[33][0] == 0
									&& x[34][0] == 0 && x[35][0] == 0
									&& x[36][0] == 0 && x[37][0] == 0
									&& x[38][0] == 0) {
								if (x[21][0] == 0 && x[22][0] == 0
										&& x[23][0] == 0 && x[24][0] == 0
										&& x[25][0] == 0 && x[26][0] == 0
										&& x[27][0] == 0 && x[28][0] == 0) {
									if (y[51][2] < 255)
										down = 1;
								}
								if (y[51][2] < 235)
									down = 1;
							}
							if (y[51][2] < 215)
								down = 1;
						}
						if (y[51][2] < 195)
							down = 1;
					}
				}
			} else if (x[13][1] < 10) {
				z = 1;
				if (x[51][0] == 0 && x[52][0] == 0 && x[53][0] == 0
						&& x[54][0] == 0 && x[55][0] == 0 && x[56][0] == 0
						&& x[57][0] == 0 && x[58][0] == 0) { // bottom row is gone
					if (x[41][0] == 0 && x[42][0] == 0 && x[43][0] == 0
							&& x[44][0] == 0 && x[45][0] == 0 && x[46][0] == 0
							&& x[47][0] == 0 && x[48][0] == 0) {
						if (x[31][0] == 0 && x[32][0] == 0 && x[33][0] == 0
								&& x[34][0] == 0 && x[35][0] == 0
								&& x[36][0] == 0 && x[37][0] == 0
								&& x[38][0] == 0) {
							if (x[21][0] == 0 && x[22][0] == 0 && x[23][0] == 0
									&& x[24][0] == 0 && x[25][0] == 0
									&& x[26][0] == 0 && x[27][0] == 0
									&& x[28][0] == 0) {
								if (y[51][2] < 255)
									down = 1;
							}
							if (y[51][2] < 235)
								down = 1;
						}
						if (y[51][2] < 215)
							down = 1;
					}
					if (y[51][2] < 195)
						down = 1;
				}
			}
		} else if (x[12][1] < 10) {
			z = 1;
			if (x[51][0] == 0 && x[52][0] == 0 && x[53][0] == 0 && x[54][0] == 0
					&& x[55][0] == 0 && x[56][0] == 0 && x[57][0] == 0
					&& x[58][0] == 0) { // bottom row is gone
				if (x[41][0] == 0 && x[42][0] == 0 && x[43][0] == 0
						&& x[44][0] == 0 && x[45][0] == 0 && x[46][0] == 0
						&& x[47][0] == 0 && x[48][0] == 0) {
					if (x[31][0] == 0 && x[32][0] == 0 && x[33][0] == 0
							&& x[34][0] == 0 && x[35][0] == 0 && x[36][0] == 0
							&& x[37][0] == 0 && x[38][0] == 0) {
						if (x[21][0] == 0 && x[22][0] == 0 && x[23][0] == 0
								&& x[24][0] == 0 && x[25][0] == 0
								&& x[26][0] == 0 && x[27][0] == 0
								&& x[28][0] == 0) {
							if (y[51][2] < 255)
								down = 1;
						}
						if (y[51][2] < 235)
							down = 1;
					}
					if (y[51][2] < 215)
						down = 1;
				}
				if (y[51][2] < 195)
					down = 1;
			}
		}
	} else if (x[11][1] < 10) {
		z = 1;
		if (x[51][0] == 0 && x[52][0] == 0 && x[53][0] == 0 && x[54][0] == 0
				&& x[55][0] == 0 && x[56][0] == 0 && x[57][0] == 0
				&& x[58][0] == 0) { // bottom row is gone
			if (x[41][0] == 0 && x[42][0] == 0 && x[43][0] == 0 && x[44][0] == 0
					&& x[45][0] == 0 && x[46][0] == 0 && x[47][0] == 0
					&& x[48][0] == 0) {
				if (x[31][0] == 0 && x[32][0] == 0 && x[33][0] == 0
						&& x[34][0] == 0 && x[35][0] == 0 && x[36][0] == 0
						&& x[37][0] == 0 && x[38][0] == 0) {
					if (x[21][0] == 0 && x[22][0] == 0 && x[23][0] == 0
							&& x[24][0] == 0 && x[25][0] == 0 && x[26][0] == 0
							&& x[27][0] == 0 && x[28][0] == 0) {
						if (y[51][2] < 255)
							down = 1;
					}
					if (y[51][2] < 235)
						down = 1;
				}
				if (y[51][2] < 215)
					down = 1;
			}
			if (y[51][2] < 195)
				down = 1;
		}
		if (y[51][2] < 180)
			down = 1;
	}

	if (x[18][0] == 0 && x[28][0] == 0 && x[38][0] == 0 && x[48][0] == 0
			&& x[58][0] == 0) {
		if (x[17][0] == 0 && x[27][0] == 0 && x[37][0] == 0 && x[47][0] == 0
				&& x[57][0] == 0) {
			if (x[16][0] == 0 && x[26][0] == 0 && x[36][0] == 0 && x[46][0] == 0
					&& x[56][0] == 0) {
				if (x[15][0] == 0 && x[25][0] == 0 && x[35][0] == 0
						&& x[45][0] == 0 && x[55][0] == 0) {
					if (x[14][0] == 0 && x[24][0] == 0 && x[34][0] == 0
							&& x[44][0] == 0 && x[54][0] == 0) {
						if (x[13][0] == 0 && x[23][0] == 0 && x[33][0] == 0
								&& x[43][0] == 0 && x[53][0] == 0) {
							if (x[12][0] == 0 && x[22][0] == 0 && x[32][0] == 0
									&& x[42][0] == 0 && x[52][0] == 0) {
								if (x[11][2] > 300) {
									z = 0;
								}
							} else if (x[12][2] > 300) {
								z = 0;
							}
						} else if (x[13][2] > 300) {
							z = 0;
						}
					} else if (x[14][2] > 300) {
						z = 0;
					}
				} else if (x[15][2] > 300) {
					z = 0;
				}
			} else if (x[16][2] > 300) {
				z = 0;
			}
		} else if (x[17][2] > 300) {
			z = 0;
		}
	} else if (x[18][2] > 300) {
		z = 0;
	}

	if (z == 1) {
		x[11][1] += 2;
		x[11][2] += 2;
	}

	if (z == 0) {
		x[11][1] -= 2;
		x[11][2] -= 2;
	}

	if (down == 1) {
		y[11][1] += 10;
		y[11][2] += 10;

		y[21][1] += 10;
		y[21][2] += 10;

		y[31][1] += 10;
		y[31][2] += 10;

		y[41][1] += 10;
		y[41][2] += 10;

		y[51][1] += 10;
		y[51][2] += 10;

		down = 0;
	}

// the first two clusters initiate the y values

	x[21][1] = x[11][1];
	x[21][2] = x[11][2];

	x[31][1] = x[11][1];
	x[31][2] = x[11][2];

	x[41][1] = x[11][1];
	x[41][2] = x[11][2];

	x[51][1] = x[11][1];
	x[51][2] = x[11][2];

	//switches aliens on/off before draw
	//aliens_alive(x, y);

	//changes the values of the x coordinates for the whole line
	change_line_xcoord(12, 19, x, y);
	change_line_xcoord(22, 29, x, y);
	change_line_xcoord(32, 39, x, y);
	change_line_xcoord(42, 49, x, y);
	change_line_xcoord(52, 59, x, y);

	// draws the boxes for the line
	draw_alien_row(11, 19, x, y, colour);
	draw_alien_row(21, 29, x, y, colour);
	draw_alien_row(31, 39, x, y, colour);
	draw_alien_row(41, 49, x, y, colour);
	draw_alien_row(51, 59, x, y, colour);

	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;
	// Set the 2nd buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr2);

	return z; //returns z to remember the direction of boxes
}

void draw(int x1, int y1, int x2, int y2, int colour) {
	IOWR_32DIRECT(drawer_base, 0, x1);
	// Set x1
	IOWR_32DIRECT(drawer_base, 4, y1);
	// Set y1
	IOWR_32DIRECT(drawer_base, 8, x2);
	// Set x2
	IOWR_32DIRECT(drawer_base, 12, y2);
	// Set y2
	IOWR_32DIRECT(drawer_base, 16, colour);
	// Set colour
	IOWR_32DIRECT(drawer_base, 20, 1);
	// Start drawing
	while (IORD_32DIRECT(drawer_base,20) == 0)
		; // wait until done
}

void aliens_alive(int x[][100], int y[][100]) {

	// x[][0] are used to determine if the box should be drawn 1=on, 0=off
	x[11][0] = 1;
	x[12][0] = 1;
	x[13][0] = 1;
	x[14][0] = 1;
	x[15][0] = 1;
	x[16][0] = 1;
	x[17][0] = 1;
	x[18][0] = 1;

	x[21][0] = 1;
	x[22][0] = 1;
	x[23][0] = 1;
	x[24][0] = 1;
	x[25][0] = 1;
	x[26][0] = 1;
	x[27][0] = 1;
	x[28][0] = 1;

	x[31][0] = 1;
	x[32][0] = 1;
	x[33][0] = 1;
	x[34][0] = 1;
	x[35][0] = 1;
	x[36][0] = 1;
	x[37][0] = 1;
	x[38][0] = 1;

	x[41][0] = 1;
	x[42][0] = 1;
	x[42][0] = 1;
	x[43][0] = 1;
	x[44][0] = 1;
	x[45][0] = 1;
	x[46][0] = 1;
	x[47][0] = 1;
	x[48][0] = 1;

	x[51][0] = 1;
	x[52][0] = 1;
	x[53][0] = 1;
	x[54][0] = 1;
	x[55][0] = 1;
	x[56][0] = 1;
	x[57][0] = 1;
	x[58][0] = 1;

}

void draw_alien_row(int start_index, int end_index, int x[][100], int y[][100],
		int colour) {
	int i;
	for (i = start_index; i < end_index; i++) {
		if (x[i][0] == 1)
			draw(x[i][1], y[i][1], x[i][2], y[i][2], colour);
	}
}
void change_line_xcoord(int start_of_row, int end_of_row, int x[][100],
		int y[][100]) {
	int i;
	for (i = start_of_row; i < end_of_row; i++) {
		x[i][1] = (x[i - 1][1] + 30);
		y[i][1] = (y[i - 1][1]);
		x[i][2] = (x[i - 1][2] + 30);
		y[i][2] = (y[i - 1][2]);
	}

}

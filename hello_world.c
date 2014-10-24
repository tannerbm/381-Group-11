/*
 * This is the player portion of the space invaders game
 * Contains logic for player shooting, moving the player, killing aliens and destroying shields
 */
#include <time.h>
#include <system.h>
#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_character_lcd.h"
#include "Draw_Things.h" //added to get rid of implicit function declaration warnings
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include <altera_up_sd_card_avalon_interface.h>
#include "sys/alt_irq.h"

#define drawer_base (volatile int *) 0x4020
#define keys ((volatile long *) 0x4040)
#define leds ((char *) 0x4080)
#define switches ((volatile char *) 0x4070)

#define DELAY_P_BULLET 5
#define DELAY_A_SHOOT 10
#define LEFT_SCREEN 20
#define RIGHT_SCREEN 290
#define TOP_SCRREN 10
#define BOTTOM_SCREEN 236
#define ALIEN_MOVE_DELAY 20

short int handler;
unsigned int * laser;
int laser_index = 0;
int laser_size;
alt_up_audio_dev * audio_dev;

/* This interrupt function will go through the entire .wav file until it hits the end in which
 * case it will disable the interrupt
 */
audio_interupt(void * context, unsigned int irq_id) {
	if (laser_index < (laser_size - 100)) {
		alt_up_audio_write_fifo(audio_dev, &laser[laser_index], 96,
				ALT_UP_AUDIO_RIGHT);
		alt_up_audio_write_fifo(audio_dev, &laser[laser_index], 96,
				ALT_UP_AUDIO_LEFT);
		laser_index += 96;
	} else {
		alt_up_audio_disable_write_interrupt(audio_dev);
	}
}

void initialize_aliens(int x[][100], int y[][100]);
void initialize_shields(int x[3][2], int y[3][2], int z[3], int g[3]);

int main() {
	char game_over[25] = "Oh dear, you're dead!";
	char won[40] = "Congratulations, you killed everyone!!";
	char start[25] = "Press key 1 to play!!";
	char clear[50] = "                                           ";
	char resetst[30] = "Press all 3 keys to reset";
	int ay[6] = { 0, 0, 0, 0, 0, 0 }; //y coordinates of the 3 alien bullets (y[0] would be the top of bullet 1, and y[1] would be the bottom)
	int ax[3] = { 0, 0, 0 }; //x coordinates of the 3 alien bullets
	int abul[3]; // shows which alien bullets are currently fired or not
	int numalienbul = 0;
	int player_1, player_2, y11_1, y11_2, player, lastplayer_1, lastplayer_2,
			lasty11_1, lasty11_2;
	int buly1, buly2, lastbuly1, lastbuly2, bulx, stopshoot;
	int bulletsuccess = 0;
	int delayreset;
	int stop, stopleft, stopright;
	int colourp;
	int keyinput;
	int shoot = 0;
	int count = 0;
	int aliencount = 0;
	int pcount = 0;
	int i;
	int randNum; //random alien to shoot
	int alienshootcount;
	int alienshoot = 0;
	int m;
	int delayalienshot = 0;
	int score = 0;
	int temp_score = 0;
	int lives = 5;
	int z = 1;

	int x[100][100]; // the array that the x values for the boxes will be stored in
	int y[100][100]; // the array that the y values for the boxes will be stored in

	initialize_aliens(x, y);
	//10 = top, 220 = bottom
	//0 = left, 320-360 = right

	char score_title[10] = "Score \0"; // character arrays for score
	char score_display[10] = "0";
	char lives_title[10] = "Lives \0";
	char lives_display[10] = "5";

	//Renee's Code Changes
	int shieldx[3][2]; //3 shields * 2x values each x[][0] = left x, x[][1] = right x
	int shieldy[3][2]; //3 shields * 2y values each y[][0] = top y, y[][1] = bottom y
	int shield_health[3];
	int shield_on[3];
	short sound_byte;
	unsigned char a, b, c, d;
	unsigned int size = 0;

	player_1 = 155; //coordinates of the player
	y11_1 = 225;
	player_2 = 165;
	y11_2 = 235;
	buly1 = 220; //coordinates of the bullet
	buly2 = 225;
	lastbuly1 = 205; //coordinates
	lastbuly2 = 210;
	bulx = 160;
	lastplayer_1 = 0;
	lasty11_1 = 0;
	lastplayer_2 = 0;
	lasty11_2 = 0;
	player = 1; //player alive or not
	stopshoot = 0;

	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev(
			"/dev/audio_and_video_config_0");
	while (!alt_up_av_config_read_ready(av_config)) {
	}

	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev("/dev/SD_KARD");
	char filename[13];

	int e = 0;
	if (device_reference != NULL) {
		if ((connected == 0) && (alt_up_sd_card_is_Present())) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");

				handler = alt_up_sd_card_fopen("boing.wav", false);

				if (handler < 0)
					printf("Problem creating file. Error %i", handler);
				else {
					for (e = 0; e < 40; e++) {
						//the first 40 bytes are the header information we can skip
						sound_byte = alt_up_sd_card_read(handler);
					}
					if (sound_byte < 0)
						printf("Error\n");
					//the bytes 40-44 contain the size
					a = alt_up_sd_card_read(handler);
					b = alt_up_sd_card_read(handler);
					c = alt_up_sd_card_read(handler);
					d = alt_up_sd_card_read(handler);

					//combine these so that we have the size of our .wav file
					size = a | (b << 8) | (c << 16) | (d << 24);
					printf(".wav file size: %d\n", size);
					//since we are putting 2 bytes together we only need half the size
					laser_size = size / 2;
					laser = malloc(laser_size * sizeof(int));

					// put the two bytes together and swap order to account for little endian
					for (e = 0; e < laser_size; e++) {
						sound_byte = alt_up_sd_card_read(handler);
						if (sound_byte == -1)
							break;
						a = sound_byte;

						sound_byte = alt_up_sd_card_read(handler);
						if (sound_byte == -1)
							break;
						b = sound_byte;
						laser[e] = a | (b << 8);
					}

					alt_up_sd_card_fclose(handler);
				}

				audio_dev = alt_up_audio_open_dev("/dev/audio_0");
				if (audio_dev == NULL)
					alt_printf("Error: could not open audio device \n");
				else
					alt_printf("Opened audio device \n");
				/* read and echo audio data */

				//enable interrupts so our audio will work
				alt_irq_register(1, NULL, audio_interupt);
				alt_irq_enable(1);
			}
		}
	}

	initialize_shields(shieldx, shieldy, shield_health, shield_on);

	//used to determine if the box should be drawn 1=on, 0=off
	//all aliens alive/on at this point in code
	aliens_alive(x, y);

	//code to set up pixel buffer
	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(
			"/dev/video_pixel_buffer_dma_0");
	if (pixel_buffer == 0) {
		printf(
				"error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			PIXEL_BUFFER_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	// Set the 1st buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr1);
	// Set the 2nd buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr2);
	// Clear both buffers (this makes all pixels black)
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);
	//code to set up pixel buffer

	//code to set up character buffer
	alt_up_char_buffer_dev * char_buffer_dev;
	char_buffer_dev = alt_up_char_buffer_open_dev(
			"/dev/video_character_buffer_with_dma_0");
	if (char_buffer_dev == NULL)
		alt_printf("Error: could not open character buffer\n");
	else
		alt_printf("Opened character buffer\n");
	// Initialize the character display
	alt_up_char_buffer_init(char_buffer_dev);
	alt_up_char_buffer_string(char_buffer_dev, score_title, 0, 0);
	alt_up_char_buffer_string(char_buffer_dev, lives_title, 73, 0);
	//code to set up character buffer

	int colour = 0x00ff; //colour of aliens
	int colour_shields = 0xf0ff;
	colourp = 0xf00f; //colour of player

	//randomize the amount of time before alien shots
	srand(time(NULL));
	randNum = (rand() % 8) + 1;
	alienshootcount = randNum * 30;

	//start screen
	while (keyinput != 13) {
		alt_up_audio_enable_write_interrupt(audio_dev);
		keyinput = IORD_8DIRECT(0x4040, 0);
		IOWR_8DIRECT(0x4050, 0, keyinput);
		alt_up_char_buffer_string(char_buffer_dev, start, 29, 25);
	}

	alt_up_char_buffer_string(char_buffer_dev, clear, 29, 25);

	//draw the shields
	draw(shieldx[0][0], shieldy[0][0], shieldx[0][1], shieldy[0][1],
			colour_shields);
	draw(shieldx[1][0], shieldy[1][0], shieldx[1][1], shieldy[1][1],
			colour_shields);
	draw(shieldx[2][0], shieldy[2][0], shieldx[2][1], shieldy[2][1],
			colour_shields);

	while (1) {

		keyinput = *keys;

		//if nothing is pushed do nothing
		if (lives != 0) {
			if (keyinput == 15) {
				stop = 0;
				stopshoot = 0;
			}
			//if the right button is pushed and player hasn't reach right edge of screen keep moving right
			else if (keyinput == 13 && pcount > 10 && stopright != 1) {
				lastplayer_1 = player_1;
				lastplayer_2 = player_1;
				player_1 += 1;
				player_2 += 1;
				stop = 1;
				pcount = 0;
			}
			//if the shooting button is pushed and not already shooting and bullet not blocked then shoot
			else if (keyinput == 11 && stopshoot != 1 && shoot != 1) {
				shoot = 1;
				stopshoot = 1;
				bulx = (player_2 + player_1) / 2;
				laser_index = 0;
				alt_up_audio_enable_write_interrupt(audio_dev);
			}
			//if the left button is pushed and player hasn't reach left edge of screen keep moving left
			else if (keyinput == 7 && pcount > 10 && stopleft != 1) {
				lastplayer_1 = player_2;
				lastplayer_2 = player_2;
				player_1 -= 1;
				player_2 -= 1;
				stop = 1;
				pcount = 0;
			}
			//move left plus shoot
			else if (keyinput == 3 && pcount > 10 && stopleft != 1) {
				lastplayer_1 = player_2;
				lastplayer_2 = player_2;
				player_1 -= 1;
				player_2 -= 1;
				stop = 1;
				pcount = 0;
				if (shoot != 1) {
					shoot = 1;
					stopshoot = 1;
					bulx = (player_2 + player_1) / 2;
					laser_index = 0;
					alt_up_audio_enable_write_interrupt(audio_dev);
				}
			}
			//move right plus shoot
			else if (keyinput == 9 && pcount > 10 && stopright != 1) {
				lastplayer_1 = player_1;
				lastplayer_2 = player_1;
				player_1 += 1;
				player_2 += 1;
				stop = 1;
				pcount = 0;
				if (shoot != 1) {
					shoot = 1;
					stopshoot = 1;
					bulx = (player_2 + player_1) / 2;
					laser_index = 0;
					alt_up_audio_enable_write_interrupt(audio_dev);
				}
			} else {
				stop = 1;
				stopshoot = 1;
			}
		}

		//stop the player from going off the screen left
		if (player_1 < LEFT_SCREEN) {
			stopleft = 1;
		}
		//stop the player from going off the screen right
		else if (player_1 > RIGHT_SCREEN) {
			stopright = 1;
		} else {
			stopright = 0;
			stopleft = 0;
		}

		//logic for incrementing the bullet
		if (shoot == 1 && count > DELAY_P_BULLET) {
			lastbuly1 = buly1;
			lastbuly2 = buly2;
			buly1 -= 1;
			buly2 -= 1;
			count = 0;

			int k;
			for (k = 0; k < 3; k++) { // go through each shield
				if (shield_on[k] == 1) {
					//check for hit on shields by player
					if ((shieldy[k][1] == buly1) && bulletsuccess == 0
							&& ((shieldx[k][0] <= bulx)
									&& (bulx <= shieldx[k][1]))) {
						shield_health[k] = shield_health[k] - 1;
						draw(bulx, buly1, bulx, buly2, 0000); //erase bullet if theres a hit
						bulletsuccess = 1;

						if (shield_health[k] == 0) {
							shield_on[k] = 0;
							draw(shieldx[k][0], shieldy[k][0], shieldx[k][1],
									shieldy[k][1], 0); //erase shield if no more health
						}
					}
				}
			}

			//check for hit on alien by player
			if (bulletsuccess == 0) { //first check if bullet already hit a shield or another alien
				for (i = 11; i < 60; i++) { //check for alien kill
					if (((y[i][2] - 10 <= buly1) && ((y[i][2]) >= buly1))
							|| ((y[i][2] - 10 <= buly2) && ((y[i][2]) >= buly2))) {
						if ((x[i][2] >= bulx) && ((x[i][2] - 10) <= bulx)) {
							if (x[i][0] == 0) { //if alien already dead, do nothing
								break;
							}
							x[i][0] = 0;
							draw(x[i][1], y[i][1], x[i][2], y[i][2], 0000);
							bulletsuccess = 1;
							temp_score += 5; //update the score
							//game won
							if (temp_score > 195) {
								alt_up_char_buffer_string(char_buffer_dev, won, 22, 25);
								alt_up_char_buffer_string(char_buffer_dev, resetst, 29, 28);
							}
							sprintf(score_display, "%d", temp_score);
							break;
						}
					}
					score = temp_score;
				}
			}
		} //end of if-logic for incrementing the bullet

		if (buly1 < TOP_SCRREN || bulletsuccess == 1) { //stop the bullet if it has gone too far or success
			shoot = 0;
			stopshoot = 0;
			bulletsuccess = 0;
			//erase the last drawing of the bullet
			draw(bulx, lastbuly1, bulx, lastbuly2, 0);
			buly1 = 220; //reinitialise the starting position of the players bullet
			buly2 = 225;
		}

		//move the aliens across the screen
		if (aliencount == ALIEN_MOVE_DELAY) {
			z = move(x, y, z, colour, buly1);
			aliencount = 0;
		}

		/* Random alien shoots the bullet after a random amount of time,
		 * allows for 3 shots on the screen at once by rotating which alien
		 * bullet will be shot
		 */
		if (alienshootcount == 0) {
			if (numalienbul == 0) {
				for (m = 5; m > 0; m--) { //start at the bottom of the column and move up if the current alien is dead
					if (x[(m * 10 + randNum)][0] == 1) { //this allows us to make sure the bottom most alive alien will shoot
						//get the cordinates for the bullet to begin from the random variable
						ay[0] = y[(m * 10 + randNum)][2];
						ay[1] = (y[(m * 10 + randNum)][2]) + 5;
						ax[0] = (x[(m * 10 + randNum)][1]
								+ x[(m * 10 + randNum)][2]) / 2;
						abul[0] = 1; //turn the bullet on
						break;
					}
				}
				//get next random alien and random time
				randNum = (rand() % 8) + 1;
				alienshootcount = randNum * 700;
				numalienbul = 1;
			} else if (numalienbul == 1) {
				for (m = 5; m > 0; m--) {
					if (x[(m * 10 + randNum)][0] == 1) {
						ay[2] = y[(m * 10 + randNum)][2];
						ay[3] = (y[(m * 10 + randNum)][2]) + 5;
						ax[1] = (x[(m * 10 + randNum)][1]
								+ x[(m * 10 + randNum)][2]) / 2;
						abul[1] = 1;
						break;
					}
				}
				randNum = (rand() % 8) + 1;
				alienshootcount = randNum * 700;
				numalienbul = 2;
			} else if (numalienbul == 2) {
				for (m = 5; m > 0; m--) {
					if (x[(m * 10 + randNum)][0] == 1) {
						ay[4] = y[(m * 10 + randNum)][2];
						ay[5] = (y[(m * 10 + randNum)][2]) + 5;
						ax[2] = (x[(m * 10 + randNum)][1]
								+ x[(m * 10 + randNum)][2]) / 2;
						abul[2] = 1;
						break;
					}
				}
				randNum = (rand() % 8) + 1;
				alienshootcount = randNum * 700;
				numalienbul = 0;
			}
		}

		if (delayalienshot > DELAY_A_SHOOT) {
			//increment all alien bullets if they have been shot
			if (abul[0] == 1) {
				ay[0] += 1;
				ay[1] += 1;
			}
			if (abul[1] == 1) {
				ay[2] += 1;
				ay[3] += 1;
			}
			if (abul[2] == 1) {
				ay[4] += 1;
				ay[5] += 1;
			}
			delayalienshot = 0;
			//check if alien fire killed player
			if (lives != 0) {
				if ((((y11_2 - 10 <= ay[1]) && (y11_2 >= ay[1])) // collision detection alien bullet 1
				|| ((y11_2 - 10 <= ay[0]) && (y11_2 >= ay[0])))
						&& ((player_2 >= ax[0]) && ((player_2 - 10) <= ax[0]))) {
					if (abul[0] == 1) { //ensures bullet was firing and hasn't hit anything yet
						player = 1; //player remains alive
						abul[0] = 0; //turn the bullet off
						draw(ax[0], ay[0], ax[0], ay[1], 0000); //erase the bullet
						lives -= 1;
						sprintf(lives_display, "%d", lives);
					}
					if (lives == 0) { //if the player has no more lives
						player = 0; //turn the player off
						alienshoot = 0;
						draw(player_1, y11_1, player_2, y11_2, 0000); //erase player
						draw(ax[0], ay[0], ax[0], ay[1], 0000); //erase bullet

						sprintf(lives_display, "%d", lives);
						alt_up_char_buffer_string(char_buffer_dev, game_over, //display defeat message
								29, 25);
						alt_up_char_buffer_string(char_buffer_dev, resetst, 29,
								28);

					}
				}
				if (((((y11_2 - 10 <= ay[3]) && (y11_2 >= ay[3])) // collisioin detection alien bullet 2
				|| ((y11_2 - 10 <= ay[2]) && (y11_2 >= ay[2])))
						&& ((player_2 >= ax[1]) && ((player_2 - 10) <= ax[1])))) {
					if (abul[1] == 1) {
						player = 1;
						abul[1] = 0;
						draw(ax[1], ay[2], ax[1], ay[3], 0000);
						lives -= 1;
						sprintf(lives_display, "%d", lives);
					}
					if (lives == 0) {
						player = 0;
						alienshoot = 0;
						draw(player_1, y11_1, player_2, y11_2, 0000);
						draw(ax[1], ay[2], ax[1], ay[3], 0000);

						sprintf(lives_display, "%d", lives);
						alt_up_char_buffer_string(char_buffer_dev, game_over,
								29, 25);
						alt_up_char_buffer_string(char_buffer_dev, resetst, 29,
								28);
					}
				}
				if (((((y11_2 - 10 <= ay[5]) && (y11_2 >= ay[5])) // collision detection alien bullet 3
				|| ((y11_2 - 10 <= ay[4]) && (y11_2 >= ay[4])))
						&& ((player_2 >= ax[2]) && ((player_2 - 10) <= ax[2])))) {
					if (abul[2] == 1) {
						player = 1;
						abul[2] = 0;
						draw(ax[2], ay[4], ax[2], ay[5], 0000);
						lives -= 1;
						sprintf(lives_display, "%d", lives);

					}
					if (lives == 0) {
						player = 0;
						alienshoot = 0;
						draw(player_1, y11_1, player_2, y11_2, 0000);
						draw(ax[2], ay[4], ax[2], ay[5], 0000);

						sprintf(lives_display, "%d", lives);
						alt_up_char_buffer_string(char_buffer_dev, game_over,
								29, 25);
						alt_up_char_buffer_string(char_buffer_dev, resetst, 29,
								28);

					}
				}

			} //end of alien kill player check

			int k;
			int j;
			//check for alien hit on shields
			for (k = 0; k < 3; k++) { //check all shields
				if (shield_on[k] == 1) {
					for (j = 0; j < 3; j++) { //check all alien bullets
						//check for hit on shields by player
						if ((shieldy[k][0] == ay[(2 * j + 1)]) && abul[j] == 1
								&& ((shieldx[k][0] <= ax[j])
										&& (ax[j] <= shieldx[k][1]))) { //collision detection
							shield_health[k] = shield_health[k] - 1;
							draw(ax[j], ay[(2 * j)], ax[j], ay[(2 * j + 1)],
									0000);
							abul[j] = 0;

							if (shield_health[k] == 0) { //check if the shield is destroyed or not
								shield_on[k] = 0;
								draw(shieldx[k][0], shieldy[k][0],
										shieldx[k][1], shieldy[k][1], 0);
							}
						}
					}
				}
			}
		}

		//displays updated score string and player lives
		alt_up_char_buffer_string(char_buffer_dev, score_display, 0, 1);
		alt_up_char_buffer_string(char_buffer_dev, lives_display, 73, 1);


		//increment the coumters
		count = count + 1; //used for slowing down the player bullet speed
		pcount += 1; //used for slowing down the player movement speed
		aliencount += 1; //used for slowing down the alien movement speed
		alienshootcount -= 1; //used to decrement from the random time given to it for the random alien shooting
		delayalienshot += 1; //used for slowing down the alien bullet speed

		if (ay[0] > BOTTOM_SCREEN) { //stop the alien bullet 1 if it has gone too far
			abul[0] = 0;
			draw(ax[0], ay[0], ax[0], ay[1], 0000);
		}
		if (ay[2] > BOTTOM_SCREEN) { //stop the alien bullet 2 if it has gone too far
			abul[1] = 0;
			draw(ax[1], ay[2], ax[1], ay[3], 0000);
		}
		if (ay[4] > BOTTOM_SCREEN) { //stop the alien bullet 3 if it has gone too far
			abul[2] = 0;
			draw(ax[2], ay[4], ax[2], ay[5], 0000);
		}

		//draw the player
		if (player == 1) { //if alive
			draw(player_1, y11_1, player_2, y11_2, colourp);
			//erase the last spot of the player
			draw(lastplayer_1, y11_1, lastplayer_2, y11_2, 0);
		}

		//draw the bullet
		if (shoot == 1) {
			draw(bulx, buly1, bulx, buly2, 9900);
			//erase the last spot of the bullet
			draw(bulx, (buly1 + 5), bulx, (buly2 + 5), 0);
		}

		//draw the alien bullet
		if (abul[0] == 1) {
			draw(ax[0], ay[0], ax[0], ay[1], 1101);
			//erase the last spot of the bullet
			draw(ax[0], ay[0] - 5, ax[0], ay[1] - 5, 0);
		}
		if (abul[1] == 1) {
			draw(ax[1], ay[2], ax[1], ay[3], 1101);
			//erase the last spot of the bullet
			draw(ax[1], ay[2] - 5, ax[1], ay[3] - 5, 0);
		}
		if (abul[2] == 1) {
			draw(ax[2], ay[4], ax[2], ay[5], 1101);
			//erase the last spot of the bullet
			draw(ax[2], ay[4] - 5, ax[2], ay[5] - 5, 0);
		}

		// reinitialise everything after the game is over
		if (keyinput == 1) {
			if (delayreset == 1) {
				if (temp_score > 195 || lives == 0) { //insures that either you won or that are dead
					delayreset = 0;
					draw(0, 0, 320, 240, 0);
					alt_up_char_buffer_string(char_buffer_dev, clear, 22, 25);
					alt_up_char_buffer_string(char_buffer_dev, clear, 22, 28);
					alt_up_char_buffer_string(char_buffer_dev, clear, 0, 1);
					alt_up_char_buffer_string(char_buffer_dev, clear, 73, 1);

					player = 1;
					lives = 5;
					score = 0;
					temp_score = 0;
					aliens_alive(x, y);
					initialize_shields(shieldx, shieldy, shield_health,
							shield_on);
					initialize_aliens(x, y);

					draw(shieldx[0][0], shieldy[0][0], shieldx[0][1],
							shieldy[0][1], colour_shields);
					draw(shieldx[1][0], shieldy[1][0], shieldx[1][1],
							shieldy[1][1], colour_shields);
					draw(shieldx[2][0], shieldy[2][0], shieldx[2][1],
							shieldy[2][1], colour_shields);
					sprintf(score_display, "%d", temp_score);
					sprintf(lives_display, "%d", lives);
				}
			}
		} else {
			delayreset = 1;
		}
	}

	return 0;
}

void initialize_shields(int x[3][2], int y[3][2], int z[3], int g[3]) {
	x[0][0] = 40; //shield 1 top left x coord
	x[0][1] = 80; //shield 1 bottom right x cord
	x[1][0] = 140; //shield 2 top left x coord
	x[1][1] = 180; // shield 2
	x[2][0] = 240; //240;
	x[2][1] = 280; //280; //shield 3

	y[0][0] = 190; //shield 1 top left y coord
	y[0][1] = 200; //shield 1 bottom left y cord
	y[1][0] = 190; //shield 2 top left
	y[1][1] = 200;
	y[2][0] = 190;
	y[2][1] = 200; //shield 3

	z[0] = 10; //each shield can take 3 hits before disappears
	z[1] = 10;
	z[2] = 10;

	g[0] = 1; //each shield is originally ON (1)
	g[1] = 1; //switches to OFF (0) when destroyed
	g[2] = 1; //so bullets can then go through its space

}

void initialize_aliens(int x[][100], int y[][100]) {
	x[11][1] = 10; // the initial starting points for the boxes
	y[11][1] = 30;
	x[11][2] = 20;
	y[11][2] = 40;

	x[21][1] = 10;
	y[21][1] = 50;
	x[21][2] = 20;
	y[21][2] = 60;

	x[31][1] = 10;
	y[31][1] = 70;
	x[31][2] = 20;
	y[31][2] = 80;

	x[41][1] = 10;
	y[41][1] = 90;
	x[41][2] = 20;
	y[41][2] = 100;

	x[51][1] = 10;
	y[51][1] = 110;
	x[51][2] = 20;
	y[51][2] = 120;
}

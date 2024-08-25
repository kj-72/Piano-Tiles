// Piano Tiles By Kshitij Jajorea [2201CS42] IITP

// Funcionalities:
// 1. Interactive UI
// 2. Single input keys
// 3. Increasing tile speed and decreasing delay between each tile with more tiles you press in Arcade Mode
// 4. Custom Option to play on custom speed, difficulties and custom number of lives.
// 5. Popup message telling the player if keys are pressed late or early or on right time.
// 6. Variable score based on the accuracy of the tiles pressed.

// The game have been throughly debugged in order to ensure bug free and stable experience by including single input keys, seamless transition between states, etc.



// Guide:
// Keys:     [O]     [O]     [O]     [O]
//          Key 3   Key 2   Key 1   Key 0         [Left To Right]
// Key 3 to move up and Key 2 to move down.
// Key 1 or 0 to select the option
// Key 1 to reduce speed/difficulty in custom option and Key 0 to increase it
// When in a game, press all 4 keys together to end a game



#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

volatile int pixel_buffer_start;

int lives; // No of lives
int eaze2 = 5000; // game speed (Higher number means slower speed)
int eaze = 100; // Number of tiles oming in game (difficulty), (Higher number means lower difficulty)

typedef enum {
	startlevel,
	title,
	level,
	gameover,
	credit,
	custom
}  gamestate;
gamestate state; // Current state of the game


short int tiles[4][8] ={{-1, -1, -1, -1, -1, -1, -1, -1},{-1, -1, -1, -1, -1, -1, -1, -1},{19, -1, -1, -1, -1, -1, -1, -1},{-1, -1, -1, -1, -1, -1, -1, -1}}; // coordinate of each tile in each lane (dynamic stack)
short int tileclr[4][8] ={{-1, -1, -1, -1, -1, -1, -1, -1},{-1, -1, -1, -1, -1, -1, -1, -1},{5, -1, -1, -1, -1, -1, -1, -1},{-1, -1, -1, -1, -1, -1, -1, -1}}; // color of each tile in each lane (dynamic stack)
short int tileptr[4] = {-1, -1, 0, -1}; // No of tiles n each lane
short int coor[4] = {3, 82, 161, 240}; // x coordinate of each tile
unsigned long long int colors[13] = {0xc0392b, 0x8e44ad, 0x3498db, 0x16a085, 0x28b463, 0xf4d03f, 0xeb984e, 0x95a5a6, 0x95a5a6, 0x641e16, 0x7fb3d5, 0xf5b7b1, 0xff97eb}; // colors

unsigned char pressedKey = 0; // User Input variable
bool key[4]={0, 0, 0, 0}; // User Input variable
bool lastKey[4] = {0}; // User Input variable
int score=0, lives=3;


bool arcade = 1; // Game mode
int last = 2; // Last tile lane
int option = 0; // selected option on the title screen
int customoption = 1; // selected option on the custom screen

long long int tilecount = 1;

volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
volatile int *interrupt = (int *)0xFFFEC60C;

int main()
{
	pixel_buffer_start = *pixel_ctrl_ptr;
    state = title;
    clear_screen();
	clear_all_text();

	the_title(); // Showing title screen
	draw_box(39, 30, 95, 6, 0); // Selecting the first option

	input(&pressedKey); // Taking Input

    while (1)
    {
		// Checking state
		switch (state) {
			case title:
				input(&pressedKey); // Taking Input
				if (key[0]) // Moving down
				{
					option = (option+3)%4;
					draw_box(3, 97, 70, 100, 0xc1c1c1);
					draw_box(39, 30, 95+16*option, 6, 0);
				}
				if (key[1]) // Moving up
				{
					option = (option+1)%4;
					draw_box(3, 97, 70, 100, 0xc1c1c1);
					draw_box(39, 30, 95+16*option, 6, 0);
				}
				if (key[2] || key[3]) // Selecting option
				{
					switch (option)
					{
					case 0:
						arcade = 1;
						eaze = 200;
						eaze2 = 8000;
						lives = 3;
						clear_screen();
						clear_all_text();
						state = startlevel;
						break;
					case 1:
						clear_screen();
						clear_all_text();
						customoption = 1;
						custommenu();
						state = custom;
						break;
					case 2:
						clear_screen();
						clear_all_text();
						draw_box(2, 316, 2, 236, 0xc1c1c1);
						draw_string(10, 20, "This Game Is Developed By Kshitij Jajorea [2201CS42]");
						draw_string(10, 22, "As End Semester Project Of Course Computer Architecture");
						draw_string(10, 24, "[CS210] In Indian Institute Of Technology Patna");
						state = credit;
						break;
					case 3:
						exit(0);
						break;
					default:
						break;
					}
				}

				break;
			case custom:
				input(&pressedKey); // Taking Input
				if (key[0]) // Moving Down
				{
					customoption = (customoption+4)%5;
					draw_box(0, 85, 70, 100, 0xc1c1c1);
					draw_box(39, 41, 95+16*customoption, 6, 0);
					if (customoption == 1)
					{
						draw_box(90, 100, 110, 8, 0);
						draw_box(92, (int)(96*((float)(200 - eaze)/150)), 112, 4, 0xffffff);
						draw_box(90, 100, 125, 8, 0xc1c1c1);
					}
					else if (customoption == 2)
					{
						draw_box(90, 100, 125, 8, 0);
						draw_box(92, (int)(96*((float)(8000 - eaze2)/6000)), 127, 4, 0xffffff);
						draw_box(90, 100, 110, 8, 0xc1c1c1);
					}
					else
					{
						draw_box(90, 100, 100, 50, 0xc1c1c1);
					}

				}
				if (key[1]) // Moving Up
				{
					customoption = (customoption+1)%5;
					draw_box(0, 85, 70, 100, 0xc1c1c1);
					draw_box(39, 41, 95+16*customoption, 6, 0);
					if (customoption == 1)
					{
						draw_box(90, 100, 110, 8, 0);
						draw_box(92, (int)(96*((float)(200 - eaze)/150)), 112, 4, 0xffffff);
						draw_box(90, 100, 125, 8, 0xc1c1c1);
					}
					else if (customoption == 2)
					{
						draw_box(90, 100, 125, 8, 0);
						draw_box(92, (int)(96*((float)(8000 - eaze2)/6000)), 127, 4, 0xffffff);
						draw_box(90, 100, 110, 8, 0xc1c1c1);
					}
					else
					{
						draw_box(90, 100, 100, 50, 0xc1c1c1);
					}
					
				}
				if (key[2] || key[3])
				{
					switch (customoption)
					{
					case 0:
						arcade = 0;
						state = startlevel;
						clear_all_text();
						clear_screen();
						break;
					case 1:
						if (key[3]) // Increase difficulty in custom
						{
							if (eaze-5>=50)
							{
								eaze-=5;
							}
							else
								eaze = 50;
							
							draw_box(90, 100, 110, 8, 0);
							
							draw_box(92, (int)(96*((float)(200 - eaze)/150)), 112, 4, 0xffffff);
						}
						else // Reduce difficulty in custom
						{
							if (eaze+5<=200)
							{
								eaze+=5;
							}
							else
								eaze = 200;
							draw_box(90, 100, 110, 8, 0);
							draw_box(92, (int)(96*((float)(200 - eaze)/150)), 112, 4, 0xffffff);
						}
						break;
					case 2:
						if (key[3]) // Increase speed in custom
						{
							if (eaze2-200>=2000)
							{
								eaze2-=200;
							}
							else
								eaze2 = 2000;
							

							draw_box(90, 100, 125, 8, 0);
							draw_box(92, (int)(96*((float)(8000 - eaze2)/6000)), 127, 4, 0xffffff);
						}
						else // Reduce speed in custom
						{
							if (eaze2+200<=8000)
							{
								eaze2+=200;
							}
							else
								eaze2 = 8000;

							
							draw_box(90, 100, 125, 8, 0);
							draw_box(92, (int)(96*((float)(8000 - eaze2)/6000)), 127, 4, 0xffffff);
						}
						break;
					case 3:
						if (key[3]) // Increase number of lives in custom
						{
							lives++;
							char livesString[2];
							char livesTitle[8] = "LIVES: ";
							itoa(lives, livesString, 10);
							strcat(livesTitle, livesString);
							draw_string(10, 36, "                      ");
							draw_string(10, 36, livesTitle);
						}
						else // Reduce number of lives in custom
						{
							if (--lives <= 0)
							{
								lives++;
							}
							char livesString[2];
							char livesTitle[8] = "LIVES: ";
							itoa(lives, livesString, 10);
							strcat(livesTitle, livesString);
							draw_string(10, 36, "                      ");
							draw_string(10, 36, livesTitle);
						}
						

						break;
					case 4: // Back to title screen
						state = title;
						clear_all_text();
						clear_screen();
						the_title();
						draw_box(39, 30, 95, 6, 0);
						option = 0;
						break;
					default:
						break;
					}
				}
				break;

			case credit:
				input(&pressedKey);
				if (key[2] || key[3]) // Back to title screen
				{
					option = 0;
					state = title;
					clear_all_text();
					clear_screen();
					the_title();
					draw_box(39, 30, 95, 6, 0);
				}

				break;

			case startlevel:
				
				startmap(); // Initialize the piano game

				state = level;
                write_status(score, lives);

				break;

			case level:
				input(&pressedKey); // Taking input in between game
				if (lastKey[0] && lastKey[1] && lastKey[2] && lastKey[3]) // Exit game
				{
					option = 0;
					state = gameover;
					clear_all_text();
					clear_screen();
					gameoverscreen();
					continue;
				}
				// Executing the tile for specific input and lane
				if (key[0])
				{
					execute(0);
				}
				if (key[1])
				{
					execute(1);
				}
				if (key[2])
				{
					execute(2);
				}
				if (key[3])
				{
					execute(3);
				}
				if (tiles[last][tileptr[last]] > eaze) // New tile using rand() func
				{
					
					int temp = rand()%4;
					int temp2 = tileclr[last][tileptr[last]];
					last = (temp==last)?(3-last):temp;
					tiles[last][++tileptr[last]] = 20;
					int temp3 = rand()%13;
					tileclr[last][tileptr[last]] = temp3;
					if (temp3 == temp2)
					{
						tileclr[last][tileptr[last]] = (temp3+1)%13;
					}
					temp = rand()%(eaze/10);
					if (++tilecount%((400-eaze)/50) == 0 && arcade)
					{
						if (50>=eaze-5)
						{
							eaze = 50;
						}
						else
							eaze-=5;

						if (2000 >= eaze2 - 100)
						{
							eaze2 = 2000;
						}
						else
							eaze2 -=100;
						
					}
					
				}

				update(); // Updating the position of tiles (As tiles are constantly moving down)
				break;

			case gameover:
				input(&pressedKey); // Input to go back to title after the gam gets over
				if (key[2] || key[3])
				{
					score = 0;
					lives = 3;
					state = title;
					clear_screen();
					clear_all_text();

					the_title();
					draw_box(39, 30, 95, 6, 0);
					option = 0;
				}
				break;
		}
    }
}

void gameoverscreen() // Game over screen with final score
{
	clear_all_text();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tiles[i][j] = -1;
			tileclr[i][j] = -1;
		}
		
	}
	tiles[1][0] = 19;
	last = 1;
	tileclr[1][0] = 8;
	tileptr[0] = -1;
	tileptr[1] = 0;
	tileptr[2] = -1;
	tileptr[3] = -1;
	
	
	draw_string(2,2,"                                                                            ");
	for (int i = 0; i < 320; i++)
	{
		for (int j = 0; j < 240; j++)
		{
			plot_pixel(i, j, 0);
		}
		
	}
	draw_string(35, 25, "Game Over!");

	char scoreString[6];
	char scoreTitle[8] = "SCORE: ";
	itoa(score, scoreString, 10);
	strcat(scoreTitle, scoreString);
	draw_string(35, 30, scoreTitle);
}

void execute(i) // Request of pressing of tile
{
	int tp = tiles[i][0];
	if (tp > 220 && tileptr[i]!=-1)
	{
		int tp = 240-tiles[i][0];
		bool posv = 1;
		if (tp<0)
		{
			tp*=-1;
			posv = 0;
		}
		tp/=2;
		if (tp<3)
		{
			score+=10;
			draw_string(34, 10, "PERFECT!        "); // Popup message indicitaing the status of the tiles pressed
		}
		else
		{
			score +=11 - tp;
			if (posv)
			{
				if (tp < 7)
				{
					draw_string(34, 10, "A BIT EARLY!   "); // Popup message indicitaing the status of the tiles pressed
				}
				else
					draw_string(34, 10, "VERY EARLY!   "); // Popup message indicitaing the status of the tiles pressed
			}
			else
			{
				if (tp < 7)
				{
					draw_string(34, 10, "A BIT LATE!   "); // Popup message indicitaing the status of the tiles pressed
				}
				else
					draw_string(34, 10, "VERY LATE!   "); // Popup message indicitaing the status of the tiles pressed
			}
		}
		write_status(score, lives); // Update score
		erasetile(i); // Remove tile from the screen
		deletetile(i); // Remove tile from the data
	}
	else
	{
		draw_string(34, 10, "WRONG KEY!   ");
		knock2(i);
		write_status(score, --lives);
		if (lives==0)
		{
			gameoverscreen();
			state = gameover;
		}
		
	}
	
}

void update()  // Updating the position of tiles (As tiles are constantly moving down)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j <= tileptr[i]; j++)
		{
			tiles[i][j]++;
			if (tiles[i][j]<239 && tiles[i][j]!=198)
			{
				for (int k = 1; k < 76; k++)
				{
					plot_pixel(coor[i]+k, tiles[i][j], colors[tileclr[i][j]]);
				}
			}
			if (tiles[i][j]>59 && tiles[i][j]!=238 && tiles[i][j]!=279)
			{
				for (int k = 1; k < 76; k++)
				{
					plot_pixel(coor[i]+k, tiles[i][j]-40, 0x000000);
				}
			}
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		if (tileptr[i]!=-1)
		{
			if (tiles[i][0] > 260)
			{
				draw_string(34, 10, "WAY TOO LATE!     ");
				knock1(i);
				deletetile(i);
				write_status(score, --lives);
				if (lives == 0)
				{
					gameoverscreen();
					state = gameover;
				}
			}
		}
	}
	// Loop for delay between each frame (speed)
	for (int i = 0; i < eaze2; i++)
	{
		plot_pixel(0, 0, 0x696969);
	}
	
}

void deletetile(int i) // Remove the tile from the data
{
	for (int k = 0; k < 3; k++)
	{
		tiles[i][k] = tiles[i][k+1];
		tileclr[i][k] = tileclr[i][k+1];
	}
	tiles[i][3] = -1;
	tileclr[i][3] = -1;
	tileptr[i]--;
}

void erasetile(int idx) // Remove the tile from the screen
{
	if (tileptr[idx]!=-1)
	{
		for (int y = (238<tiles[idx][0])?238:tiles[idx][0], yl = tiles[idx][0]-40; y>=yl; --y)
		{
			if (y==198)
				continue;
			
			for (int k = 1; k < 76; k++)
			{
				plot_pixel(coor[idx]+k, y, 0x000000);
			}
			
		}
		
	}
	
}

void startmap() // Game Map (Piano)
{
	clear_screen();
	for (int i = 0; i < 320; i++)
	{
		plot_pixel(i, 239, 0xdddddd);
		plot_pixel(i, 198, 0xdddddd);
		plot_pixel(i, 0, 0x696969);
		plot_pixel(i, 1, 0x696969);
		plot_pixel(i, 2, 0x696969);
		plot_pixel(i, 3, 0x696969);
		plot_pixel(i, 4, 0x696969);
		plot_pixel(i, 5, 0x696969);
		plot_pixel(i, 6, 0x696969);
		plot_pixel(i, 7, 0x696969);
		plot_pixel(i, 8, 0x696969);
		plot_pixel(i, 9, 0x696969);
		plot_pixel(i, 10, 0x696969);
		plot_pixel(i, 11, 0x696969);
		plot_pixel(i, 12, 0x696969);
		plot_pixel(i, 13, 0x696969);
		plot_pixel(i, 14, 0x696969);
		plot_pixel(i, 15, 0x696969);
		plot_pixel(i, 16, 0x696969);
		plot_pixel(i, 17, 0x696969);
		plot_pixel(i, 18, 0xdddddd);
		plot_pixel(i, 19, 0xdddddd);
	}
	for (int i = 20; i < 239; i++)
	{
		plot_pixel(0, i, 0xdddddd);
		plot_pixel(1, i, 0xdddddd);
		plot_pixel(2, i, 0xdddddd);
		plot_pixel(80, i, 0xdddddd);
		plot_pixel(81, i, 0xdddddd);
		plot_pixel(159, i, 0xdddddd);
		plot_pixel(160, i, 0xdddddd);
		plot_pixel(238, i, 0xdddddd);
		plot_pixel(239, i, 0xdddddd);
		plot_pixel(317, i, 0xdddddd);
		plot_pixel(318, i, 0xdddddd);
		plot_pixel(319, i, 0xdddddd);
	}
}

void draw_char(int x, int y, char letter) // Function to draw a character
{ 
	volatile int charBuffer = 0xc9000000;
	*(char *)(charBuffer + (y << 7) + x) = letter;
}

void clear_screen() // Function to turn the whole screen black
{
	for (int i = 0; i < 320; i++) {
		for (int j = 0; j < 240; j++) {
			plot_pixel(i, j, 0);
		}
	}
}

void plot_pixel(int x, int y,short int line_color) // Function to update a pizel on screen
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void draw_string(int x, int y, char str[]) // Function to draw a string on screen
{
	for (int i = 0; i < strlen(str); i++) {
		draw_char(x+i, y, str[i]);
	}
}

void clear_all_text() // Function to remove all the text from the screen
{
	for (int x = 0; x < 80; x++) {
		for (int y = 0; y < 60; y++) {
			draw_char(x, y, ' ');
		}
	}
}

void write_status(int score, int lives) // Update score and number of lives on screen
{
    char scoreString[6];
    char scoreTitle[8] = "SCORE: ";
    itoa(score, scoreString, 10);
    strcat(scoreTitle, scoreString);
    
    char livesString[2];
    char livesTitle[8] = "LIVES: ";
    itoa(lives, livesString, 10);
    strcat(livesTitle, livesString);
    strcat(livesTitle, "  ");

    draw_string(2,2,"                                                                            ");

    draw_string(2, 2, scoreTitle);
	draw_string(34, 2, "PIANO BY KJ");
	draw_string(68, 2, livesTitle);
}

void the_title() // Title screen
{
	for (int i = 0; i < 240; i++)
	{
		plot_pixel(0, i, 0x696969);
		plot_pixel(1, i, 0x696969);
		plot_pixel(319, i, 0x696969);
		plot_pixel(318, i, 0x696969);
	}
	for (int i = 2; i < 318; i++)
	{
		plot_pixel(i, 0, 0x696969);
		plot_pixel(i, 1, 0x696969);
		plot_pixel(i, 238, 0x696969);
		plot_pixel(i, 239, 0x696969);
	}
	draw_box(2, 316, 2, 236, 0xc1c1c1);
	draw_box(161, 105, 81, 79, 0xffffff);

	for (int i = 0; i < 80; i++)
	{
		plot_pixel(160, 80+i, 0);
		plot_pixel(175, 80+i, 0);
		plot_pixel(190, 80+i, 0);
		plot_pixel(205, 80+i, 0);
		plot_pixel(220, 80+i, 0);
		plot_pixel(235, 80+i, 0);
		plot_pixel(250, 80+i, 0);
		plot_pixel(265, 80+i, 0);
	}
	for (int i = 161; i < 265; i++)
	{
		plot_pixel(i, 80, 0);
		plot_pixel(i, 159, 0);
	}
//	Piano Keys Art
	draw_box(170, 10, 81, 55, 0);
	draw_box(185, 10, 81, 55, 0);
	draw_box(215, 10, 81, 55, 0);
	draw_box(230, 10, 81, 55, 0);
	draw_box(245, 10, 81, 55, 0);
// P
	draw_box(90, 4, 16, 40, 0xff0027);
	draw_box(94, 16, 16, 4, 0xff0027);
	draw_box(94, 16, 30, 4, 0xff0027);
	draw_box(106, 4, 20, 14, 0xff0027);
// I
	draw_box(120, 20, 16, 4, 0xff0027);
	draw_box(120, 20, 52, 4, 0xff0027);
	draw_box(128, 4, 20, 32, 0xff0027);
//	A
	draw_box(150, 4, 16, 40, 0xff0027);
	draw_box(166, 4, 16, 40, 0xff0027);
	draw_box(154, 12, 16, 4, 0xff0027);
	draw_box(154, 12, 30, 4, 0xff0027);
//	N
	draw_box(180, 4, 16, 40, 0xff0027);
	draw_box(196, 4, 16, 40, 0xff0027);
	draw_box(184, 3, 16, 10, 0xff0027);
	draw_box(187, 3, 26, 10, 0xff0027);
	draw_box(190, 3, 36, 10, 0xff0027);
	draw_box(193, 3, 46, 10, 0xff0027);

//	O
	draw_box(210, 4, 16, 40, 0xff0027);
	draw_box(226, 4, 16, 40, 0xff0027);
	draw_box(214, 12, 16, 4, 0xff0027);
	draw_box(214, 12, 52, 4, 0xff0027);
	
	
	
//	Options on the title screen
	draw_string(10, 24, "ARCADE");
	draw_string(10, 28, "CUSTOM");
	draw_string(10, 32, "CREDITS");
	draw_string(10, 36, "EXIT");
	
}

void draw_box(int x, int xlen, int y, int ylen, int color) // Function to draw a filled rectangle on screen
{
	for (int i = 0; i < xlen; i++)
	{
		for (int j = 0; j < ylen; j++)
		{
			plot_pixel(x+i, y+j, color);
		}
		
	}
	
}

void input(unsigned char *pressedKey) // Function to update user input (Implemented with single input keys on array 'key')
{
	volatile int * PS2_ptr = (int *) 0xFF200050;
	int data = *PS2_ptr;
	*pressedKey = data & 0xFF;

	if (*pressedKey & 8)
	{
		if (!lastKey[0])
		{
			key[0]=1;
			lastKey[0]=1;
		}
		else
		{
			key[0] = 0;
		}
	}
	else
	{
		key[0] = 0;
		lastKey[0] = 0;
	}
	if (*pressedKey & 4)
	{
		if (!lastKey[1])
		{
			key[1]=1;
			lastKey[1]=1;
		}
		else
		{
			key[1] = 0;
		}
	}
	else
	{
		key[1] = 0;
		lastKey[1] = 0;
	}
	if (*pressedKey & 2)
	{
		if (!lastKey[2])
		{
			key[2]=1;
			lastKey[2]=1;
		}
		else
		{
			key[2] = 0;
		}
	}
	else
	{
		key[2] = 0;
		lastKey[2] = 0;
	}
	if (*pressedKey & 1)
	{
		if (!lastKey[3])
		{
			key[3]=1;
			lastKey[3]=1;
		}
		else
		{
			key[3] = 0;
		}
	}
	else
	{
		key[3] = 0;
		lastKey[3] = 0;
	}
}

void knock1(int idx) // Blinking of tile on missing it
{
	for (int i = 0; i < 100; i++)
	{
		draw_box(coor[idx]+1, 75, 220, 19, 0);
	}
	for (int i = 0; i < 100; i++)
	{
		draw_box(coor[idx]+1, 75, 220, 19, colors[tileclr[idx][0]]);
	}
	for (int i = 0; i < 100; i++)
	{
		draw_box(coor[idx]+1, 75, 220, 19, 0);
	}
	for (int i = 0; i < 100; i++)
	{
		draw_box(coor[idx]+1, 75, 220, 19, colors[tileclr[idx][0]]);
	}
	for (int i = 0; i < 100; i++)
	{
		draw_box(coor[idx]+1, 75, 220, 19, 0);
	}
	for (int i = 0; i < 100; i++)
	{
		draw_box(coor[idx]+1, 75, 220, 19, colors[tileclr[idx][0]]);
	}
	for (int i = 0; i < 100; i++)
	{
		draw_box(coor[idx]+1, 75, 220, 19, 0);
	}
}

void knock2(int idx) // Pressing a key without a tile (Srong key wrong time)
{
	for (int i = 0; i < 50; i++)
	{
		draw_box(coor[idx]+1, 75, 199, 39, 0xff000);
	}
	for (int i = 0; i < 50; i++)
	{
		draw_box(coor[idx]+1, 75, 199, 39, 0);
	}
	for (int i = 0; i < 50; i++)
	{
		draw_box(coor[idx]+1, 75, 199, 39, 0xff000);
	}
	for (int i = 0; i < 50; i++)
	{
		draw_box(coor[idx]+1, 75, 199, 39, 0);
	}
}

void custommenu() // Draw the custom menu
{
	clear_screen();
	clear_all_text();
	for (int i = 0; i < 240; i++)
	{
		plot_pixel(0, i, 0x696969);
		plot_pixel(1, i, 0x696969);
		plot_pixel(319, i, 0x696969);
		plot_pixel(318, i, 0x696969);
	}
	for (int i = 2; i < 318; i++)
	{
		plot_pixel(i, 0, 0x696969);
		plot_pixel(i, 1, 0x696969);
		plot_pixel(i, 238, 0x696969);
		plot_pixel(i, 239, 0x696969);
	}
	draw_box(2, 316, 2, 236, 0xc1c1c1);
	draw_box(39, 41, 95+16*customoption, 6, 0);
	draw_string(10, 24, "PLAY");
	draw_string(10, 28, "DIFFICULTY:");
	draw_string(10, 32, "SPEED:");
    char livesString[3];
    char livesTitle[8] = "LIVES: ";
    itoa(lives, livesString, 10);
    strcat(livesTitle, livesString); 
	draw_string(10, 36, livesTitle);
	draw_string(10, 40, "BACK");
	draw_box(90, 100, 110, 8, 0);
	draw_box(92, (int)(96*((float)(200 - eaze)/150)), 112, 4, 0xffffff);
}
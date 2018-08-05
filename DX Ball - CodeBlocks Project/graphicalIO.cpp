#include <string.h>

#include "iGraphics.h"

#include "fileIO.h"
//#include "definations.h"

#define DEBUG_PRINT_MOUSE_POS
//#undef DEBUG_PRINT_MOUSE_POS


#pragma region GlobalObjectVariable

struct Point Screen_START, Screen_END; // to fix the starting & ending pos of screen with respect to sidebar thickness & total window screen

struct Rectangle Brick[31];

struct Rectangle Stick, SideBar_Left, SideBar_Right, SideBar_Upper;

struct Circle Ball;

struct User Player;

struct Image PowerUp[7];

#pragma endregion


#pragma region GlobalCurrentStatusHolderVariable

//  hasBrick			[x]					 [y]
int hasBrick[SCALED_X(SCREEN_WIDTH)][SCALED_Y(SCREEN_HEIGHT)];// this array contains this Level's Brick Pos & Colors info as numbers which is obtained from level file

int remainingBricks, totalBricks;

int hasClickedInTextBox, hasObtainedUserName;
int showHighScoreList, showCredits, showInstuctions;
int hasStartedBallMovement; // to prevent stick from moving before starting ball movement by right click of mouse
int hasStartedGame, hasPausedGame, hasFinishedGame;//hasBallDroppedBelow
int hasProceededToSavePlayerInfo;
int hasUpdatedScore;

int exiting;
#pragma endregion





// updates The Player's Date & Time with the current Date & Time so that this player can be highlighted in highscore list
void takeCurrentDateAndTime()
{
	time_t ctime;
	tm *current;
	time(&ctime);
	current = localtime(&ctime);

	Player.playTime.hour = current->tm_hour;
	Player.playTime.min = current->tm_min;
	Player.playTime.sec = current->tm_sec;

	Player.playEvent.day = current->tm_mday;
	Player.playEvent.month = current->tm_mon + 1; // +1 to make month = [1,31]
	Player.playEvent.year = current->tm_year;
}



#pragma region BooleanFunctions

int isSameDate(Date d1, Date d2)
{
	return (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year);
}

int isSameTime(Time t1, Time t2)
{
	return (t1.hour == t2.hour && t1.min == t2.min && t1.sec == t2.sec);
}

int isSameString(const char *str1, const char *str2)
{
	return !strcmp(str1, str2);
}


int hasFinishedThisLevel()
{
	return !remainingBricks; // means a level has completed
}

int hasFailedToPassThisLevel()
{
	return Player.lives == 0;
}

int hasCompletedTheWholeGame()
{
	return hasFinishedGame;
}

int hasEnteredUserName()
{
	return hasObtainedUserName;
}

//int hasStartedBallMovement()
//{
//	return
//}
#pragma endregion


#pragma region SetInitialValuesForEachLevel

void assignColors();
// assign powerUp containing Bricks Color & PowerUp Array's referrence image files
void assignPowerUps()
{
	PowerUp[Life].fileName = FILE_IMG_POWERUP_LIFE;
	PowerUp[Kill].fileName = FILE_IMG_POWERUP_KILL;
	PowerUp[BigBall].fileName = FILE_IMG_POWERUP_BIGBALL;
	PowerUp[BrickBreaker].fileName = FILE_IMG_POWERUP_BREAKBALL;
	PowerUp[StickEnlarger].fileName = FILE_IMG_POWERUP_STICKENLARGE;
	PowerUp[LevelUp].fileName = FILE_IMG_POWERUP_LEVELUP;

	PowerUp[Life].isPresent = PowerUp[Kill].isPresent = PowerUp[BigBall].isPresent = PowerUp[BrickBreaker].isPresent = PowerUp[StickEnlarger].isPresent = PowerUp[LevelUp].isPresent = 0;


//	Brick[Img_Life].colour.r = 232,				Brick[Img_Life].colour.g = 132,			Brick[Img_Life].colour.b = 162;
//	Brick[Img_Kill].colour.r = 255,				Brick[Img_Kill].colour.g = 128,			Brick[Img_Kill].colour.b = 0;
//	Brick[Img_BigBall].colour.r = 174,			Brick[Img_BigBall].colour.g = 0,		Brick[Img_BigBall].colour.b = 0;
//	Brick[Img_BrickBreaker].colour.r = 0,		Brick[Img_BrickBreaker].colour.g = 0,	Brick[Img_BrickBreaker].colour.b = 174;
//	Brick[Img_StickEnlarger].colour.r = 140,	Brick[Img_StickEnlarger].colour.g = 36,	Brick[Img_StickEnlarger].colour.b = 123;
//	Brick[Img_LevelUp].colour.r = 0,			Brick[Img_LevelUp].colour.g = 142,		Brick[Img_LevelUp].colour.b = 222;

	Brick[Img_Life].colour = Brick[Nor_Blue].colour;
	Brick[Img_Kill].colour = Brick[Nor_Green].colour;
	Brick[Img_BigBall].colour = Brick[Nor_Orange].colour;
	Brick[Img_BrickBreaker].colour = Brick[Nor_Blue].colour;
	Brick[Img_StickEnlarger].colour = Brick[Nor_Red].colour;
	Brick[Img_LevelUp].colour = Brick[Nor_Orange].colour;
}


void setProgStarttingValues()
{
	Player.score = 0, Player.lives = TOTAL_LIVES_INITIAL;
	hasStartedGame = 0;

	assignColors();
	assignPowerUps();
}


// Here drawing color of each object is assigned to default value
void assignColors()
{
	SideBar_Left.colour.r = 195, SideBar_Left.colour.g = 195, SideBar_Left.colour.b = 195;

	Ball.colour.r = 255, Ball.colour.g = 128, Ball.colour.b = 50;

	Stick.colour.r = 56, Stick.colour.g = 78, Stick.colour.b = 177;


	Brick[Nor_Red].colour.r = 140,		Brick[Nor_Red].colour.g = 0,		Brick[Nor_Red].colour.b = 0;
	Brick[Nor_Blue].colour.r = 0,		Brick[Nor_Blue].colour.g = 0,		Brick[Nor_Blue].colour.b = 222;
	Brick[Nor_Green].colour.r = 0,		Brick[Nor_Green].colour.g = 120,	Brick[Nor_Green].colour.b = 0;
	Brick[Nor_Orange].colour.r = 174,	Brick[Nor_Orange].colour.g = 128,	Brick[Nor_Orange].colour.b = 0;

	Brick[Esp_Red].colour.r = 210,		Brick[Esp_Red].colour.g = 36,		Brick[Esp_Red].colour.b = 123;
	Brick[Esp_Blue].colour.r = 0,		Brick[Esp_Blue].colour.g = 142,		Brick[Esp_Blue].colour.b = 174;
	Brick[Esp_Green].colour.r = 0,		Brick[Esp_Green].colour.g = 215,	Brick[Esp_Green].colour.b = 41;
	Brick[Esp_Orange].colour.r = 255,	Brick[Esp_Orange].colour.g = 235,	Brick[Esp_Orange].colour.b = 8;

	Player.colour.r = 30, Player.colour.g = 40, Player.colour.b = 250;
}

// Set Side Boundaries Starting Point with respect to screen's height & width
void setSideBoundaries()
{
	SideBar_Left.start.x = 0, SideBar_Left.start.y = 0;

	SideBar_Right.start.x = SCREEN_WIDTH - SIDEBAR_THICKNESS, SideBar_Right.start.y = 0;

	SideBar_Upper.start.x = 0, SideBar_Upper.start.y = SCREEN_HEIGHT - SIDEBAR_THICKNESS;

	Screen_START.x = SIDEBAR_THICKNESS, Screen_START.y = 0;
	Screen_END.x = SCREEN_WIDTH - SIDEBAR_THICKNESS, Screen_END.y = SCREEN_HEIGHT - SIDEBAR_THICKNESS;
}

// Stick , Ball &  Call :: assignColors() , setSideBoundaries()
void assignLevelInitialValues()
{
	assignColors();
	setSideBoundaries();

	Stick.height = STICK_THICKNESS, Stick.width = STICK_WIDTH_NORMAL;

	Stick.start.x = SCREEN_WIDTH / 2 - Stick.width / 2, Stick.start.y = 0;
	Stick.dx = STICK_SPEED, Stick.dy = 0;

	Ball.radius = BALL_RAD_NORMAL;
	Ball.center.x = Stick.start.x + Stick.width / 2, Ball.center.y = Stick.height + Ball.radius;
	Ball.dx = BALL_SPEED_X, Ball.dy = BALL_SPEED_Y;
	Ball.type = 'n';


	remainingBricks = 0, totalBricks = 0;

	//	Player.score=0,Player.scoreRate=0;
	//	Player.name = (char*)malloc(30*sizeof(char));
	//	Player.name="SUBANGKAR";

//	hasBallDroppedBelow = 0;
	hasFinishedGame = 0;
	hasPausedGame = 0;
	hasProceededToSavePlayerInfo = 0;
}

#pragma endregion


// Load the next level each time with assigning initial values
void LoadNextLevel(int shift = 1) // call by 0 to restart the whole game , -1 to restart this level && 1 to next level
{
	assignLevelInitialValues();
	iPauseTimer(0);

	static int levelInitialScore = Player.score, levelInitialLives = Player.lives;

	if (shift == 1) LoadLevel(&totalBricks, hasBrick, &Player.scoreRate);

	else if (shift == -1)
	{
		LoadLevel(&totalBricks, hasBrick, &Player.scoreRate, -1);
		Player.score = levelInitialScore;
		Player.lives = levelInitialLives;
	}

	else
	{
		printf("%d", shift);
		LoadLevel(&totalBricks, hasBrick, &Player.scoreRate, 1);
		Player.score = 0;
		Player.lives = TOTAL_LIVES_INITIAL;//3;
	}


	remainingBricks = totalBricks;
	hasStartedBallMovement = 0;


	if (Player.score) Player.score += 1000; // always add 1000 to current score while passing to a level except the beginning level

	levelInitialScore = Player.score;
	levelInitialLives = Player.lives;

	for (int i = 0; i<7; i++) PowerUp[i].isPresent = 0;
}



#pragma region DrawsNumberOnScreen

// Draws a vertical   ( |^| ) polygon having 6 vertices starts with assuming (X,Y)=peak point of lower arrow and rect = (dx * dy)
void VerticalSegment(int X, int Y, int dx, int dy)
{
	int triangleHeight = TRIANGLE_HEIGHT;
	double  x[6] = { X , X - dx / 2 , X - dx / 2 , X , X + dx / 2 , X + dx / 2 },
		y[6] = { Y , Y + triangleHeight , Y + dy + triangleHeight , Y + dy + 2 * triangleHeight , Y + dy + triangleHeight , Y + triangleHeight };

	iFilledPolygon(x, y, 6);
}

// Draws a horizontal ( <_> ) polygon having 6 vertices starts with assuming (X,Y)=peak point of left arrow and rect = (dx * dy)
void HorizontalSegment(int X, int Y, int dx, int dy)
{
	int triangleHeight = TRIANGLE_HEIGHT;
	double  x[6] = { X , X + triangleHeight , X + dx + triangleHeight , X + dx + 2 * triangleHeight , X + dx + triangleHeight , X + triangleHeight },
		y[6] = { Y , Y + dy / 2 , Y + dy / 2 , Y , Y - dy / 2 , Y - dy / 2 };

	iFilledPolygon(x, y, 6);
}

// draws a digit's segments starting from (x,y) = peak point of arrow of bottom-leftmost segment
void DrawDigit(int x, int y, int digit)
{
	if (digit == 0 || digit == 2 || digit == 6 || digit == 8)
		VerticalSegment(x, y, SEGMENT_RECT_WIDTH, SEGMENT_RECT_HEIGHT);
	if (digit == 0 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9)
		VerticalSegment(x, y + SEGMENT_RECT_HEIGHT + 2 * TRIANGLE_HEIGHT, SEGMENT_RECT_WIDTH, SEGMENT_RECT_HEIGHT);


	if (digit == 0 || digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9)
		HorizontalSegment(x, y + 2 * SEGMENT_RECT_HEIGHT + 4 * TRIANGLE_HEIGHT, SEGMENT_RECT_HEIGHT, SEGMENT_RECT_WIDTH);
	if (digit == 2 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9)
		HorizontalSegment(x, y + SEGMENT_RECT_HEIGHT + 2 * TRIANGLE_HEIGHT, SEGMENT_RECT_HEIGHT, SEGMENT_RECT_WIDTH);
	if (digit == 0 || digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8 || digit == 9)
		HorizontalSegment(x, y, SEGMENT_RECT_HEIGHT, SEGMENT_RECT_WIDTH);


	if (digit == 0 || digit == 1 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9)
		VerticalSegment(x + SEGMENT_RECT_HEIGHT + 2 * TRIANGLE_HEIGHT, y, SEGMENT_RECT_WIDTH, SEGMENT_RECT_HEIGHT);
	if (digit == 0 || digit == 1 || digit == 2 || digit == 3 || digit == 4 || digit == 7 || digit == 8 || digit == 9)
		VerticalSegment(x + SEGMENT_RECT_HEIGHT + 2 * TRIANGLE_HEIGHT, y + SEGMENT_RECT_HEIGHT + 2 * TRIANGLE_HEIGHT, SEGMENT_RECT_WIDTH, SEGMENT_RECT_HEIGHT);

}

// Splits a number into digits and pass them to DrawDigit
void DrawNumber(int num, int x, int y, int digit_dx)
{
	int digits[10];

	int digitCurrentStart_X = x, digitCurrentStart_Y = y;

	int pos = 0;
	for (pos = 0; num; pos++)
	{
		digits[pos] = num % 10;
		num /= 10;
	} // now digits[] contains all digits of num in reverse order

	do
	{
		DrawDigit(digitCurrentStart_X, digitCurrentStart_Y, digits[--pos]);
		digitCurrentStart_X += digit_dx;
	} while (pos);// now digits[] pass digits of num to DrawDigit() in its reverse order which is actually real order of main number

}

#pragma endregion


#pragma region DrawsStartScreen

// Draws The Pause Menu Text Boxes on Drawing Console
void DrawUserOptionTexts()
{
	iClear();

	iShowBMP(0, 0, FILE_IMG_STARTSCREEN);


	iSetColor(180, 100, 210);

	unsigned short pos = 0;

	if (hasPausedGame) {
		iRectangle(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2, TEXT_BOX_START_Y - pos * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);
		iText(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_START_DIFFERENCE_FROM_BOX, TEXT_BOX_START_Y - pos++ * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + 5, "          RESUME  ", TEXT_FONT_LARGE);

		iRectangle(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2, TEXT_BOX_START_Y - pos * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);
		iText(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_START_DIFFERENCE_FROM_BOX, TEXT_BOX_START_Y - pos++ * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + 5, "  RESTART  LEVEL ", TEXT_FONT_LARGE);
	}

	iRectangle(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2, TEXT_BOX_START_Y - pos * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);
	iText(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_START_DIFFERENCE_FROM_BOX, TEXT_BOX_START_Y - pos++ * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + 5, "START NEW GAME", TEXT_FONT_LARGE);

	iRectangle(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2, TEXT_BOX_START_Y - pos * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);
	iText(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_START_DIFFERENCE_FROM_BOX, TEXT_BOX_START_Y - pos++ * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + 5, "    HIGH   SCORES ", TEXT_FONT_LARGE);

	iRectangle(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2, TEXT_BOX_START_Y - pos * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);
	iText(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_START_DIFFERENCE_FROM_BOX, TEXT_BOX_START_Y - pos++ * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + 5, "         CREDITS ", TEXT_FONT_LARGE);

	iRectangle(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2, TEXT_BOX_START_Y - pos * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);
	iText(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_START_DIFFERENCE_FROM_BOX, TEXT_BOX_START_Y - pos++ * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + 5, "   INSTRUCTIONS ", TEXT_FONT_LARGE);

	iRectangle(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2, TEXT_BOX_START_Y - pos * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);
	iText(SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_START_DIFFERENCE_FROM_BOX, TEXT_BOX_START_Y - pos++ * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + 5, "             QUIT ", TEXT_FONT_LARGE);
}

// Draws The option Value BMP's
void DrawOptionBoxes()
{
	iShowBMP(0, 0, FILE_IMG_STARTSCREEN_OPTIONS);
	iSetColor(170, 25, 25);

	for (int i = 0; i < 15; i++) // to make side border thickness 15-times
		iRectangle(START_PLAYBOX_START_X - i, START_PLAYBOX_START_Y - i, START_PLAYBOX_WIDTH + 2 * i, START_PLAYBOX_HEIGHT + 2 * i); // 2*i = 1i for makeup of decreased one and another is for to increase thickness one step
//	iShowBMP(START_PLAYBOX_START_X, START_PLAYBOX_START_Y, FILE_IMG_STARTSCREEN_BOX_PLAY);
	for (int i = 0; i < 15; i++)
		iRectangle(START_HIGHSCORE_START_X - i, START_HIGHSCORE_START_Y - i, START_HIGHSCORE_WIDTH + 2 * i, START_PLAYBOX_HEIGHT + 2 * i);
	//	iShowBMP(START_HIGHSCORE_START_X, START_HIGHSCORE_START_Y, FILE_IMG_STARTSCREEN_BOX_HIGHSCORE);
}

// with click inside the text box it scans name with iKeyBoard() & shows name in textbox while scanning
void DrawTextBox()
{
	iSetColor(170, 150, 210);
	iRectangle(SCAN_NAME_START_X - 5, SCAN_NAME_START_Y - 5, START_TEXT_BOX_WIDTH, START_TEXT_BOX_HEIGHT);

	if (hasClickedInTextBox) iText(SCAN_NAME_START_X, SCAN_NAME_START_Y, Player.name, TEXT_FONT_LARGE);
}

// shows the start screen img & TextBox to prompt for name
void DrawStartScreenAndTakePlayerName()
{
	iShowBMP(0, 0, FILE_IMG_STARTSCREEN);

	iSetColor(170, 130, 190);

	if (!hasEnteredUserName())
	{
		iText(DRAW_START_NAME_PROMPT_X, DRAW_START_NAME_PROMPT_Y, " ENTER  YOUR  NAME", TEXT_FONT_LARGE);

		DrawTextBox();
	}
	else if (hasEnteredUserName())
	{
		//	DrawOptionBoxes();
		DrawUserOptionTexts();
	}
}

#pragma endregion


#pragma region DrawsObjectsWithUpToDateValuesOnScreen

// Draws left,right & upper SideBar Rectangles with its current color
void DrawSideBoundaries()
{
	SetDrawingColor(SideBar_Left.colour);

	iFilledRectangle(SideBar_Left.start.x, SideBar_Left.start.y, SIDEBAR_THICKNESS, SCREEN_HEIGHT); // for left sidebar

	iFilledRectangle(SideBar_Right.start.x, SideBar_Right.start.y, SIDEBAR_THICKNESS, SCREEN_HEIGHT); // for right sidebar

	iFilledRectangle(SideBar_Upper.start.x, SideBar_Upper.start.y, SCREEN_WIDTH, SIDEBAR_THICKNESS); // for upper sidebar
}

// Draws Pause Menu bars on the sidebars during game
void DrawSideBarOptions()
{
	iShowBMP(Screen_END.x, 0, FILE_IMG_PAUSE);
}

// Draws the stick's rectangle at stick's current start pos with stick's current colour which is initially at midScreen
void DrawStick()
{
	SetDrawingColor(Stick.colour);
	iFilledRectangle(Stick.start.x, Stick.start.y, Stick.width, Stick.height);
}

// Draws the ball's circle at ball's current center pos with ball's current colour which is initially at midScreen
void DrawBall()
{
	SetDrawingColor(Ball.colour);
	iFilledCircle(Ball.center.x, Ball.center.y, Ball.radius);
}

// Draws a single Brick Rect which starts @ given Pos
void DrawBrick(int leftBottom_x, int leftBottom_y, struct Rectangle brick)
{
	SetDrawingColor(brick.colour);
	iFilledRectangle(leftBottom_x + 1, leftBottom_y + 1, BRICK_WIDTH - 1, BRICK_HEIGHT - 1);
}

// DrawThisLevelBrickSets
void DrawBrickSets()
{
	int lastX = SCALED_X(SCREEN_WIDTH), lastY = SCALED_Y(SCREEN_HEIGHT);

	for (int X = BRICK_START_GAPS_LEFT; X < lastX; X++)
	{
		for (int Y = BRICK_START_GAPS_DOWN; Y < lastY; Y++)
		{
			if (hasBrick[X][Y])
			{
				DrawBrick(UNSCALED_x(X), UNSCALED_y(Y), Brick[hasBrick[X][Y]]);
			}
		}
	}
}

// Draw current remaining no of lives
void DrawLives()
{
	iSetColor(231, 0, 0);

	for (size_t i = 0; i < Player.lives; i++)
	{
		iShowBMP(LIFE_X, LIFE_Y - i*(LIFE_IMG_WIDTH + LIFE_IMG_GAP), FILE_IMG_LIVES);
	}
}

// Draw The Player's name & current score in upper side bar of screen
void DrawPlayerInfo()
{
	SetDrawingColor(Player.colour);

	iText(DRAW_NAME_X, DRAW_NAME_Y, Player.name, GLUT_BITMAP_TIMES_ROMAN_24);
	//	iText(SCREEN_WIDTH/2,640,numberToString(Player.score),GLUT_BITMAP_TIMES_ROMAN_24);

	if (Player.score) DrawNumber(Player.score, DRAW_SCORE_X, DRAW_SCORE_Y, SEGMENT_PEAK_TO_PEAK_DIFFERENCE + DIGIT_TO_DIGIT_DIFFERENCE);

	DrawLives();
}


int powerHasCollidedWithStick(Point);

// Draw PowerUps Images in Drawing Console
void DrawPowerUps()
{
	if (!hasStartedBallMovement || hasPausedGame) return;
	for (int powerType = Img_Life - 20; powerType <= Img_LevelUp - 20; powerType++) // -20 coz imgtype starts from 21 while powerType from 1
	{
		if (PowerUp[powerType].isPresent)
		{
			iShowBMP(PowerUp[powerType].start.x, PowerUp[powerType].start.y, PowerUp[powerType].fileName);
			PowerUp[powerType].start.y--;
			if (!PowerUp[powerType].start.y) PowerUp[powerType].isPresent = 0;

			else if (powerHasCollidedWithStick(PowerUp[powerType].start))
			{
				PowerUp[powerType].isPresent = 0;

				switch (powerType)
				{
				case BigBall:
					Ball.radius = BALL_RAD_BIG;
					break;

				case BrickBreaker:
					Ball.type = 'b';
					break;

				case Life:
					Player.lives = (Player.lives+1)%6; // max life doesn't cross 5
					if (!Player.lives) Player.lives = 5;
					break;

				case Kill:
					Player.lives -= 1;
					hasStartedBallMovement = 0;
					break;

				case StickEnlarger:
					Stick.width = STICK_WIDTH_ENLARGED;
					break;

				case LevelUp:
					LoadNextLevel();
					break;

				}
			}
		}
	}
}

#pragma endregion


#pragma region DrawsGameFinishingScreen

void DrawInstructionsOnScreen()
{
	iShowBMP(10, 10, FILE_IMG_INSTRUCTIONS);

}

void DrawCreditScreen()
{
	iShowBMP(10, 10, FILE_IMG_CREDITS);
}

void DrawExitScreen()
{
	//	iClear();

	//iShowBMP(10, 10, FILE_IMG_EXITSCREEN);//

	DrawCreditScreen();

	iText(SCREEN_WIDTH / 2 - SIDEBAR_THICKNESS, 20, "Exiting  .....................", TEXT_FONT_LARGE);
}

// show high scores from file in Drawing console
void ShowScores()
{
	iClear(); // clear the screen

	iShowBMP(0, 0, FILE_IMG_HIGHSCORE_UI);

//	iShowBMP(HIGH_SCORE_BACK_START_X, HIGH_SCORE_BACK_START_Y, FILE_IMG_HIGHSCORE_BACK);

	iText(HIGH_SCORE_SHOW_START_X, HIGH_SCORE_SHOW_START_Y, "POS", TEXT_FONT_LARGE);
	iText(HIGH_SCORE_SHOW_START_X + TEXT_FONT_LARGE_WIDTH * 6, HIGH_SCORE_SHOW_START_Y, "   NAME  ", TEXT_FONT_LARGE);
	iText(SCREEN_WIDTH - TEXT_FONT_LARGE_WIDTH * 6, HIGH_SCORE_SHOW_START_Y, "SCORE", TEXT_FONT_LARGE);


	User readPlayer;

	FILE *fp = fopen(FILE_HIGHSCORE, "rb");

	int showText_x = HIGH_SCORE_SHOW_START_X, showText_y = HIGH_SCORE_SHOW_START_Y - 2 * TEXT_FONT_LARGE_HEIGHT;

	int pos = 1;

	while (pos <= HIGHSCORE_MAX_SHOW_LIMIT && !feof(fp) && fread(&readPlayer, sizeof(User), 1, fp))
	{
		if (isSameString(readPlayer.name, Player.name) && isSameDate(readPlayer.playEvent, Player.playEvent) && isSameTime(readPlayer.playTime, Player.playTime))
			iSetColor(210, 100, 174); // if read person is this person

		else iSetColor(255, 255, 255);

		char scorerInfo[220], score[10];

		sprintf(scorerInfo, "%4d%4s%3s%5s%-185s", pos, "", "->", "", readPlayer.name);
		iText(showText_x, showText_y, scorerInfo, TEXT_FONT_LARGE);

		sprintf(score, "%6d", readPlayer.score);
		iText(SCREEN_WIDTH - TEXT_FONT_LARGE_WIDTH * 5, showText_y, score, TEXT_FONT_LARGE);

		showText_y -= TEXT_FONT_LARGE_HEIGHT + TEXT_TO_TEXT_DIFFERENCE_VERTICAL;

		pos++;
	}

	iSetColor(231, 174, 10);
	iText(SCREEN_WIDTH / 2 - 2*SIDEBAR_THICKNESS, showText_y, "Press ENTER To RETURN", TEXT_FONT_LARGE);

	fclose(fp);

	//iSetColor(231, 174, 10);
	//iRectangle(10, 10, TEXT_FONT_LARGE_WIDTH * 5, TEXT_FONT_LARGE_HEIGHT);
	//iText(12, 12, "BACK", TEXT_FONT_LARGE);
}

// Draw Game Over Screen & User Scores
void DrawGameEnds_Incomplete()
{
	iShowBMP(0, 0, FILE_IMG_GAMEOVER);
}

// Draw Game Completed Screen & User Score
void DrawGameEnds_Complete()
{
	iShowBMP(0, 0, FILE_IMG_GAMEFINISHED);
}

// Draw Game Ending Type image & score
int DrawGameEnds()
{
	SetDrawingColor(Player.colour);
	hasStartedBallMovement = 0;

	if (!hasProceededToSavePlayerInfo && !showHighScoreList && hasFailedToPassThisLevel())
	{
		DrawGameEnds_Incomplete();

		DrawNumber(Player.score, DRAW_FINAL_SCORE_INCOMPLETE_X, DRAW_FINAL_SCORE_INCOMPLETE_Y, SEGMENT_PEAK_TO_PEAK_DIFFERENCE + DIGIT_TO_DIGIT_DIFFERENCE);
	}

	else if (!hasProceededToSavePlayerInfo && !showHighScoreList && hasCompletedTheWholeGame())
	{
		DrawGameEnds_Complete();

		DrawNumber(Player.score, DRAW_FINAL_SCORE_COMPLETE_X, DRAW_FINAL_SCORE_COMPLETE_Y, SEGMENT_PEAK_TO_PEAK_DIFFERENCE + DIGIT_TO_DIGIT_DIFFERENCE);
	}

	if (hasProceededToSavePlayerInfo) // && (hasFailedToPassThisLevel() || hasCompletedTheWholeGame())
	{
		takeCurrentDateAndTime();
		upDateHighScore(&Player);
		hasUpdatedScore = 1;

		hasStartedGame = 0;
		hasFinishedGame = 0;

		//		hasBallDroppedBelow = 0;
		Player.lives = 0;
		hasClickedInTextBox = 0;
		hasStartedBallMovement = 0;
		hasProceededToSavePlayerInfo = 0;


		showHighScoreList = 1;
	}

	iSetColor(210, 50, 50);
	iText(SCREEN_WIDTH - 350, 5, "  PRESS ENTER TO CONTINUE ", TEXT_FONT_LARGE); // 350= This Text Length

	return 1;
}


#pragma endregion



void iDraw()
{
	iClear();

	if (showCredits || showInstuctions)
	{
		if (showCredits) DrawCreditScreen();
		else DrawInstructionsOnScreen();

		iSetColor(231, 174, 10);
		iText(SCREEN_WIDTH / 2 - 2 * SIDEBAR_THICKNESS, 15, "Press ENTER To RETURN", TEXT_FONT_LARGE);
		return;
	}

	if (exiting)
	{
		static short exitScreenDrawn = 0;

		if (exitScreenDrawn) Sleep(2000), exit(0);

		DrawExitScreen();
		exitScreenDrawn = !exitScreenDrawn;

		return;
	}

	if (!hasStartedGame) DrawStartScreenAndTakePlayerName();

	else if (hasPausedGame)
	{
		DrawUserOptionTexts();
	}

	else if (hasStartedGame) {

		if (hasFailedToPassThisLevel() || hasCompletedTheWholeGame())
		{
			DrawGameEnds();
			return; // draw the end screen if game has ended
		}

		if (hasFinishedThisLevel()) LoadNextLevel();

		DrawSideBoundaries();

		DrawSideBarOptions();

		DrawBrickSets();

		DrawPowerUps();

		DrawStick();

		DrawBall();

		DrawPlayerInfo();

		iSetColor(120, 255, 255);
		iText(10, 10, "Press p for pause, r for resume, END for exit.");
	}

	if (showHighScoreList)
	{
		ShowScores();
	}
}



#pragma region ControlBallAndStickMovement

// returns 1 if ball's current pos is found inside stick area else 0
int ballHasCollidedWithStick()
{
	/*
	When ball comes from left side ball_x+rad < Stick_left_x
						right side ball_x-rad > Stick_right_x
						upper side ball_y-rad > Stick_upper_y
								for avoiding getting inside Stick
	*/
	return Stick.start.x <= Ball.center.x + Ball.radius && Ball.center.x - Ball.radius <= Stick.start.x + Stick.width && Ball.center.y - Ball.radius <= Stick.start.y + Stick.height;
}

int powerHasCollidedWithStick(Point powerPosStart)
{
	return (powerPosStart.y <= Stick.start.y + Stick.height) && (Stick.start.x <= powerPosStart.x && powerPosStart.x + POWER_IMG_WIDTH <= Stick.start.x + Stick.width);
}


void checkPowerUp(int X, int Y)
{
//	printf(">> %d <<", hasBrick[X][Y]);

	if (Img_Life <= hasBrick[X][Y] && hasBrick[X][Y] <= Img_LevelUp)
	{
		PowerUp[hasBrick[X][Y] - 20].isPresent = 1;
		PowerUp[hasBrick[X][Y] - 20].start.x = UNSCALED_x(X);
		PowerUp[hasBrick[X][Y] - 20].start.y = UNSCALED_y(Y);
	}
}

// deletes the brick @ given ScaledPos(X,Y) , increase score & decrease remainingBricks
// Controls score increament for both normal & special balls
void DeleteBrick(int Ball_Scaled_X, int Ball_Scaled_Y)
{
	Player.score += (1 + (hasBrick[Ball_Scaled_X][Ball_Scaled_Y] / 10)) * Player.scoreRate; // for Especial Bricks hasBrick[Ball_Scaled_X][Ball_Scaled_Y]/11)>=1 so score rate becomes twice (1+1)

	hasBrick[Ball_Scaled_X][Ball_Scaled_Y] = 0;
	remainingBricks--;
}

// Scales(x->X,y->Y) Ball's surface (x,y) & check brickS availability in those Scaled=(X,Y) & calls DeleteBrick() with those (X,Y) to make those (X,Y) 0 & returns 1 to make ballPos changed and in case of 'f'==fireBall &make those Pos 0 but returns 0 to remove the next bricks continuosly
int hasCollidedWithBrick(char ballType = 'n')
{
	if (ballType == 'n') // for nomal type ball
	{
		int hasBrickHere = 0;

		int Ball_Scaled_X = SCALED_X(Ball.center.x + Ball.radius);
		int Ball_Scaled_Y = SCALED_Y(Ball.center.y + Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			hasBrickHere = 1;
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			if (Esp_Red <= hasBrick[Ball_Scaled_X][Ball_Scaled_Y] && hasBrick[Ball_Scaled_X][Ball_Scaled_Y] <= Esp_Orange)
			{
				printf("1>> %d <<", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
				Player.score += 10;
				hasBrick[Ball_Scaled_X][Ball_Scaled_Y] -= 10; // hardBricks turn into normal bricks
				printf(">> %d <<\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
			}
			else
				DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
			printf("*1* %d **\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
		}

		Ball_Scaled_X = SCALED_X(Ball.center.x + Ball.radius);
		Ball_Scaled_Y = SCALED_Y(Ball.center.y - Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			hasBrickHere = 1;
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			if (Esp_Red <= hasBrick[Ball_Scaled_X][Ball_Scaled_Y] && hasBrick[Ball_Scaled_X][Ball_Scaled_Y] <= Esp_Orange)
			{
				printf("2>> %d <<", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
				Player.score += 10;
				hasBrick[Ball_Scaled_X][Ball_Scaled_Y] -= 10; // hardBricks turn into normal bricks
				printf(">> %d <<\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
			}
			else
				DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
			printf("*2* %d **\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
		}

		Ball_Scaled_X = SCALED_X(Ball.center.x - Ball.radius);
		Ball_Scaled_Y = SCALED_Y(Ball.center.y + Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			hasBrickHere = 1;
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			if (Esp_Red <= hasBrick[Ball_Scaled_X][Ball_Scaled_Y] && hasBrick[Ball_Scaled_X][Ball_Scaled_Y] <= Esp_Orange)
			{
				printf("3>> %d <<", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
				Player.score += 10;
				hasBrick[Ball_Scaled_X][Ball_Scaled_Y] -= 10; // hardBricks turn into normal bricks
				printf(">> %d <<\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
			}
			else
				DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
			printf("*3* %d **\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
		}


		Ball_Scaled_X = SCALED_X(Ball.center.x - Ball.radius);
		Ball_Scaled_Y = SCALED_Y(Ball.center.y - Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			hasBrickHere = 1;
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			if (Esp_Red <= hasBrick[Ball_Scaled_X][Ball_Scaled_Y] && hasBrick[Ball_Scaled_X][Ball_Scaled_Y] <= Esp_Orange)
			{
				printf("4>> %d <<", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
				Player.score += 10;
				hasBrick[Ball_Scaled_X][Ball_Scaled_Y] -= 10; // hardBricks turn into normal bricks
				printf(">> %d <<\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
			}
			else
				DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
			printf("*4* %d **\n\n", hasBrick[Ball_Scaled_X][Ball_Scaled_Y]);
		}


		return hasBrickHere;
	}

	else if (ballType == 'b') // for breaker ball
	{
		int Ball_Scaled_X = SCALED_X(Ball.center.x + Ball.radius);
		int Ball_Scaled_Y = SCALED_Y(Ball.center.y + Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
		}


		Ball_Scaled_X = SCALED_X(Ball.center.x + Ball.radius);
		Ball_Scaled_Y = SCALED_Y(Ball.center.y - Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
		}


		Ball_Scaled_X = SCALED_X(Ball.center.x - Ball.radius);
		Ball_Scaled_Y = SCALED_Y(Ball.center.y + Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
		}

		Ball_Scaled_X = SCALED_X(Ball.center.x - Ball.radius);
		Ball_Scaled_Y = SCALED_Y(Ball.center.y - Ball.radius);

		if (hasBrick[Ball_Scaled_X][Ball_Scaled_Y])
		{
			checkPowerUp(Ball_Scaled_X, Ball_Scaled_Y);
			DeleteBrick(Ball_Scaled_X, Ball_Scaled_Y);
		}

	}

	return 0;
}

// controls the ball's movement according to boundaries
void MoveBall()
{
	if (!hasStartedBallMovement) return;

	Ball.center.x += Ball.dx;
	Ball.center.y += Ball.dy;

	// means ball went down of screen then reset ball's direction & position to initial
	if (Ball.center.y - Ball.radius < Screen_START.y)
	{
		Player.lives -= 1;

		Ball.center.x = Stick.start.x + Stick.width / 2;
		Ball.center.y = Stick.start.y + Stick.height + Ball.radius;

		Ball.dx = BALL_SPEED_X;
		Ball.dy = BALL_SPEED_Y;

		hasStartedBallMovement = 0;
		iPauseTimer(0);

		return;
	}


	if (hasCollidedWithBrick(Ball.type))
	{
		double ballfracPart_X = (SCALED_X((double)Ball.center.x + Ball.radius) - SCALED_X(Ball.center.x + Ball.radius)); // is the horizontal length that is passed by ball
		double ballfracPart_Y = (SCALED_Y((double)Ball.center.y + Ball.radius) - SCALED_Y(Ball.center.y + Ball.radius)); // is the vertical length that is passed by ball

		if (ballfracPart_Y && (ballfracPart_Y < 0.5 || ballfracPart_Y > 0.5))// if collision is happened in vertical part then reflect it with respect to x and then ball must have Y part
		{
			Ball.dx = -Ball.dx;
			//			Ball.dy = +Ball.dy;
		}

		else if (ballfracPart_X && (ballfracPart_X < 0.5 || ballfracPart_X > 0.5)) // if collision is happened in horizontal part then reflect it with respect to y and then ball must have X part
		{
			//			Ball.dx = +Ball.dx;
//			Ball.dy = -Ball.dy;

			Ball.dy = -sign(Ball.dy)*BALL_SPEED_Y;
		}

		else  // if collision happens in center or in corner then just return
		{
			Ball.dx = -sign(Ball.dx)*BALL_SPEED_X;
			Ball.dy = -sign(Ball.dy)*BALL_SPEED_Y;

			//Ball.dx = -Ball.dx;
			//Ball.dy = -Ball.dy;

		}
	}


	if (ballHasCollidedWithStick()) // ball will go upper always and if
	{
		int StickRadius = Stick.width / 2;
		if (Ball.center.x > Stick.start.x + StickRadius) // ball drops on right side of stick then move ball rightside
		{
			Ball.dx = BALL_SPEED_X;
//			Ball.dy = BALL_SPEED_Y;
			//			Ball.dy = +abs(Ball.center.x - (Stick.start.x + (StickRadius)))*BALL_SPEED_Y / StickRadius; // adjusts the ratio of speed_Y
		}

		else if (Ball.center.x < Stick.start.x + StickRadius) // ball drops on left side of stick then move ball leftside
		{
			Ball.dx = -BALL_SPEED_X;
//			Ball.dy = BALL_SPEED_Y;
		}

		else // ball drops on center of stick then just reverse the current X-axis direction
		{
			Ball.dx = -BALL_SPEED_X;//-sign(Ball.dx)*BALL_SPEED_X;
//			Ball.dy = BALL_SPEED_Y;
		}
		Ball.dy = BALL_SPEED_Y - abs(((StickRadius) - (Ball.center.x - Stick.start.x ))*BALL_SPEED_Y / StickRadius); // adjusts the ratio of speed_Y

		if (!Ball.dy) Ball.dy = BALL_SPEED_Y; // if collision is just at the corner
	}

	/*
	When ball comes from ball_x+rad < right_sidebar
						 ball_x-rad > left_sidebar
						 ball_y+rad > upper_sidebar
								to remain inside sidebars
	*/
	if (Ball.center.x + Ball.radius >= Screen_END.x || Ball.center.x - Ball.radius <= Screen_START.x)Ball.dx = -Ball.dx;//-sign(Ball.dx)*BALL_SPEED_X;
	if (Ball.center.y + Ball.radius > Screen_END.y)Ball.dy = -sign(Ball.dy)*BALL_SPEED_Y;
}

// ensurely moves the stick with given dir=='l'/'r' so that it remains inside the sidebars
void MoveStick(char dir)
{
	if (!hasStartedBallMovement) return;

	else if (dir == 'l' && Stick.start.x - Stick.dx >= Screen_START.x)
	{
		Stick.start.x -= Stick.dx;
	}
	else if (dir == 'r' && Stick.start.x + Stick.width + Stick.dx <= Screen_END.x)
	{
		Stick.start.x += Stick.dx;
	}
}

#pragma endregion


#pragma region ControlUserInteractions

// Detects the user mouse pointer click inside initial text box which alternate is ENTER key
int clickedTextBoxNo(int mx, int my)
{
//	if (!hasPausedGame && !hasStartedGame) return 0;

	if (SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 < mx && mx < SCREEN_WIDTH / 2 - TEXT_UI_WIDTH / 2 + TEXT_UI_WIDTH)
	{
		for (int box = 0; box < 7; box++)
			if (TEXT_BOX_START_Y - box * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) < my && my < TEXT_BOX_START_Y - box * (TEXT_UI_BOX_DIFFERENCE + TEXT_FONT_LARGE_HEIGHT) + TEXT_UI_HEIGHT) return hasPausedGame ? box + 1: box+3; // +2 coz 2 boxes is missing @ start
		return 0;
	}
	else
		return 0;
}

int hasClickedInPauseBox(int mx,int my)
{
	return Screen_END.x < mx && mx < SCREEN_WIDTH && 0 < my && my < PAUSE_BOX_WIDTH;
}

void PauseGame()
{
	hasStartedBallMovement = 0;

	hasPausedGame = !hasPausedGame;

	hasPausedGame ? iPauseTimer(0) : iResumeTimer(0);
}

// when the user presses and drags the mouse move the stick & save the last pos.x
void iMouseMove(int mx, int my)
{
	return;
	static int lastMx = mx; // static var used so that when next time a new mx comes it can be compared with last time's mx using lastMx

	if (mx < lastMx) //button == GLUT_LEFT_BUTTON &&
	{
		MoveStick('l');
	}
	else if (mx > lastMx) //button == GLUT_LEFT_BUTTON &&
	{
		MoveStick('r');
	}

	lastMx = mx; // current mx is the last mx of next time's mx
}


// when the user hit left / right key move the stick & if pressed END then quit from game
void iSpecialKeyboard(unsigned char key)
{
	// Deals with stick movement
	if (key == GLUT_KEY_LEFT)
	{
		MoveStick('l');
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		MoveStick('r');
	}



	if (key == GLUT_KEY_END)
	{
//	    exiting=1;
	//    hasStartedBallMovement=0;
		exit(0);
	}
}



// when user click left-mouse button start the ball movement if paused by resuming ball movement timer
// also activate variable "hasClickedInTextBox" for taking user name @ start screen when user presses click inside the text box range
void iMouse(int button, int state, int mx, int my)
{
	// For Activating Text initial Text Box
	if (!hasStartedGame && state == GLUT_LEFT)
	{
		if (SCAN_NAME_START_X - 4 <= mx && mx <= SCAN_NAME_START_X - 4 + START_TEXT_BOX_WIDTH - 4 && SCAN_NAME_START_Y - 4 <= my && my <= SCAN_NAME_START_Y - 4 + START_TEXT_BOX_HEIGHT - 4)
		{
			hasClickedInTextBox = 1;
		}
	}

	// Deals with if user clicks pause box on the sidebar during game play
	else if (hasStartedGame && !hasPausedGame && state == GLUT_LEFT && hasClickedInPauseBox(mx,my))
	{
		PauseGame();
	}

	// For Starting Movement of Ball
	else if (!hasStartedBallMovement && (!showHighScoreList && !showCredits && !showInstuctions) && state == GLUT_LEFT)
	{
		hasStartedBallMovement = 1;
		iResumeTimer(0);
	}


	// Deals with START & PAUSE MENU SCREEN
	if ((!hasStartedGame && hasEnteredUserName()) || (hasStartedGame && hasPausedGame && !showHighScoreList))
	{
		switch (clickedTextBoxNo(mx, my))
		{
		case 1: // for resume box
			if (hasPausedGame)
				hasPausedGame = 0,
				hasStartedBallMovement = 0;
			break;

		case 2: // for restart level box
			if (hasPausedGame)
				LoadNextLevel(-1);
			break;

		case 3: // for start new game box
			hasStartedGame = 1;

			LoadNextLevel(0);
			hasStartedBallMovement = 0;

			break;

		case 4: // for highscore box
			showHighScoreList = 1;
			break;
		case 5: // for credit box
			showCredits = 1;
			break;
		case 6: // for instruction box
			showInstuctions = 1;
			break;
		case 7: // for exit box
			if (!hasStartedGame || hasPausedGame) exiting = 1;
			break;

		default:
			break;
		}
	}




#ifdef DEBUG_PRINT_MOUSE_POS
	printf("x=%d y=%d\n", mx, my);
#endif // PrintMousePosition
}


// when the user drags the mouse move the stick & save the last pos.x
void iMouseOver(int mx, int my)
{
	static int lastMx = mx; // static var used so that when next time a new mx comes it can be compared with last time's mx using lastMx

	if (mx < lastMx)
	{
		MoveStick('l');
	}
	else if (mx > lastMx)
	{
		MoveStick('r');
	}

	lastMx = mx; // current mx is the last mx of next time's mx
}


// start taking user name @ start screen when user presses click mouse inside the text box range & type name and assign it to Player.name
void iKeyboard(unsigned char key)
{
	// To allow user to input text
	if (!hasStartedGame && !hasClickedInTextBox && key == ENTER) hasClickedInTextBox = 1;

	// takes input of user name & store that
	if (!hasStartedGame && hasClickedInTextBox && !hasEnteredUserName())
	{
		static int i = 0;

		if (key == ENTER && i) // '&& i' ensures that at least name of 1 length is given input
		{
			Player.name[i] = NULL;

			hasObtainedUserName = 1;
			hasClickedInTextBox = 0;
		}

		else if (key == BACKSPACE && i) // '&& i' ensures that index is not let -ve
		{
			Player.name[--i] = NULL;
		}

		else // else take the char as name & store it to Player.name
		{
			Player.name[i++] = key;
			Player.name[i] = NULL;
		}

		return;
	}

	// Draw the Bricks & Game Environment but doesn't start ball movement
	if (!hasStartedGame && hasEnteredUserName() && (!showHighScoreList && !showCredits && !showInstuctions) && key == ENTER )//|| (showHighScoreList && key == ENTER)
	{
		hasStartedGame = 1;
		hasStartedBallMovement = 0;
	}

	// deals with pressing of ENTER while user wasn't in pause menu rather user had been in start menu / save high score
	if (!hasPausedGame && showHighScoreList && key == ENTER)
	{
		showHighScoreList = 0;
		hasStartedGame = 0;
	//	hasPausedGame = 0;
		hasFinishedGame = 0;
		hasProceededToSavePlayerInfo = 0;

		setProgStarttingValues();

		return;
	}

	// returns to start menu from high score list
	else if ( (showHighScoreList || showCredits || showInstuctions) && key == ENTER)
	{
		showHighScoreList = 0;
		showCredits = 0;
		showInstuctions = 0;

        if(hasCompletedTheWholeGame() || hasFailedToPassThisLevel())
        {
            showHighScoreList = 0;
            hasStartedGame = 0;
        //	hasPausedGame = 0;
            hasFinishedGame = 0;
            hasProceededToSavePlayerInfo = 0;

            setProgStarttingValues();

        }
		return;
	}

	// Updates the High Score List with user info & shows that
	if ((hasCompletedTheWholeGame() || hasFailedToPassThisLevel()) && !hasUpdatedScore && !showHighScoreList && key == ENTER)
	{
		hasProceededToSavePlayerInfo = 1,
		hasFinishedGame = 0;
	}

	// if user presses enter in pause menu screen then resume game again
	if (hasStartedGame && hasPausedGame && key == ENTER)
	{
		hasPausedGame = !hasPausedGame;
	}

	// Deals with showing game pause & pause menu screen
	if (hasStartedGame && key == SPACEBAR)
	{
		PauseGame();
	}

}

#pragma endregion



// Start The Game with Player's score=0 & initialize the ball movement Timer function()
int main()
{
	//	resetHighScore();

	setProgStarttingValues();

	iSetTimer(BALL_MOVE_TIME_DIFFERENCE, MoveBall);

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "DX Ball'15");
	return 0;
}




#pragma region FileNames

#define FILE_IMG_STARTSCREEN "data/img/Game_Start.bmp"
#define FILE_IMG_STARTSCREEN_OPTIONS "data/img/Game_Start+Box.bmp"
#define FILE_IMG_STARTSCREEN_BOX_PLAY "data/img/Game_Start_Box_Play.bmp"
#define FILE_IMG_STARTSCREEN_BOX_HIGHSCORE "data/img/Game_Start_Box_HighScore.bmp"
#define FILE_IMG_EXITSCREEN "data/img/Game_Exit.bmp"

#define FILE_IMG_CREDITS "data/img/Credits.bmp"
#define FILE_IMG_INSTRUCTIONS "data/img/instruct.bmp"

#define FILE_IMG_GAMEOVER "data/img/GameOver_Incomplete.bmp"
#define FILE_IMG_GAMEFINISHED "data/img/GameOver_Complete.bmp"
#define FILE_IMG_HIGHSCORE_UI "data/img/HighScorer.bmp"

#define FILE_IMG_HIGHSCORE_BACK "data/img/back.bmp"

#define FILE_IMG_POWERUP_LIFE "data/img/power_life.bmp"
#define FILE_IMG_POWERUP_KILL "data/img/power_kill.bmp"
#define FILE_IMG_POWERUP_BIGBALL "data/img/power_bigBall.bmp"
#define FILE_IMG_POWERUP_BREAKBALL "data/img/power_brickBreaker.bmp"
#define FILE_IMG_POWERUP_LEVELUP "data/img/power_levelUp.bmp"
#define FILE_IMG_POWERUP_STICKENLARGE "data/img/power_stickEnlarger.bmp"


#define FILE_IMG_LIVES "data/img/lifePresent.bmp"
#define FILE_IMG_PAUSE "data/img/Pause.bmp"

#define FILE_LEVEL_1 "data/levels/level_1.txt"
#define FILE_LEVEL_2 "data/levels/level_2.txt"
#define FILE_LEVEL_3 "data/levels/level_3.txt"
#define FILE_LEVEL_4 "data/levels/level_4.txt"
#define FILE_LEVEL_5 "data/levels/level_5.txt"

#define FILE_HIGHSCORE "data/user/HighScores.txt"

#pragma endregion


#define HIGHSCORE_MAX_SHOW_LIMIT 15

#pragma region ObjectsSizeAndMovementSpeed

#define BALL_MOVE_TIME_DIFFERENCE 15

#define BALL_SPEED_X 8
#define BALL_SPEED_Y 4

#define BALL_RAD_NORMAL 9//9
#define BALL_RAD_BIG 12

#define STICK_THICKNESS 20
#define STICK_WIDTH_NORMAL 200
#define STICK_WIDTH_ENLARGED 300

#define STICK_SPEED 15

#define BRICK_WIDTH 50
#define BRICK_HEIGHT 20

#define POWER_IMG_WIDTH 40
#define POWER_IMG_HEIGHT 40

#pragma endregion


#pragma region ConversionAndWorkFunctions

#define SCALED_X(x) (x)/BRICK_WIDTH
#define SCALED_Y(y) (y)/BRICK_HEIGHT

#define UNSCALED_x(X) (X)*BRICK_WIDTH
#define UNSCALED_y(Y) (Y)*BRICK_HEIGHT

#define sign(x) ((x)/abs(x))

#define SetDrawingColor(clr) iSetColor(clr.r, clr.g, clr.b)// Set current drawing color to passed argument


#pragma endregion


#pragma region LevelAndScoreRates

#define SCORE_LEVEL1_NORMAL 10
#define SCORE_LEVEL2_NORMAL 20
#define SCORE_LEVEL3_NORMAL 30
#define SCORE_LEVEL4_NORMAL 40
#define SCORE_LEVEL5_NORMAL 50

//#define SCORE_LEVEL1_ESPECIAL 2*SCORE_LEVEL1_NORMAL
//#define SCORE_LEVEL2_ESPECIAL 2*SCORE_LEVEL2_NORMAL
//#define SCORE_LEVEL3_ESPECIAL 2*SCORE_LEVEL3_NORMAL
//#define SCORE_LEVEL4_ESPECIAL 2*SCORE_LEVEL4_NORMAL
//#define SCORE_LEVEL5_ESPECIAL 2*SCORE_LEVEL5_NORMAL

#define LAST_LEVEL 3//5

#define TOTAL_LIVES_INITIAL 3//

#pragma endregion


#pragma region ScreenFixedObjectsCoOrdinates

#define SCREEN_HEIGHT 680
#define SCREEN_WIDTH 1350//1300
#define SIDEBAR_THICKNESS 50


#define BRICK_START_GAPS_DOWN 12//12
#define BRICK_START_GAPS_LEFT SCALED_X(SIDEBAR_THICKNESS)//1

#define BRICK_START_X BRICK_START_GAPS_LEFT // not '-1' coz X starts from 0
#define BRICK_START_Y BRICK_START_GAPS_DOWN // not '-1' coz gaps of 12 row means gap from Y=[0,11] so start from Y=12

#define BRICK_MAX_X (SCALED_X(SCREEN_WIDTH -SIDEBAR_THICKNESS)-1) // '-1' coz
#define BRICK_MAX_Y (SCALED_Y(SCREEN_HEIGHT-SIDEBAR_THICKNESS)-1) // (X,Y) starts from 0


#define PAUSE_BOX_WIDTH 50

#define LIFE_IMG_GAP 5
#define LIFE_IMG_WIDTH 40
#define LIFE_X (SCREEN_WIDTH -LIFE_IMG_WIDTH)
#define LIFE_Y (SCREEN_HEIGHT-LIFE_IMG_WIDTH)


#define DRAW_START_NAME_PROMPT_X 500
#define DRAW_START_NAME_PROMPT_Y 320

#define START_TEXT_BOX_WIDTH 450
#define START_TEXT_BOX_HEIGHT 27

#define SCAN_NAME_START_X 410
#define SCAN_NAME_START_Y 250

#define START_PLAYBOX_START_X 85//50
#define START_PLAYBOX_START_Y 75//50

#define START_PLAYBOX_WIDTH 150
#define START_PLAYBOX_HEIGHT 70//75


#define START_HIGHSCORE_START_X 1040//1000
#define START_HIGHSCORE_START_Y 75//50

#define START_HIGHSCORE_WIDTH 150
#define START_HIGHSCORE_HEIGHT 70//75


#define DRAW_SCORE_X (3+SIDEBAR_THICKNESS)  // 3+ so that left most segment does not coincide with sidebar
#define DRAW_SCORE_Y 635

#define DRAW_NAME_X (SCREEN_WIDTH/2-SIDEBAR_THICKNESS)
#define DRAW_NAME_Y 640


#define DRAW_FINAL_SCORE_INCOMPLETE_X 590
#define DRAW_FINAL_SCORE_INCOMPLETE_Y 200

#define DRAW_FINAL_SCORE_COMPLETE_X 750
#define DRAW_FINAL_SCORE_COMPLETE_Y 52


#define HIGH_SCORE_SHOW_START_X  50
#define HIGH_SCORE_SHOW_START_Y 530

#define HIGH_SCORE_BACK_START_X  SCREEN_WIDTH/2
#define HIGH_SCORE_BACK_START_Y  10
#define HIGH_SCORE_BACK_WIDTH 50

#pragma endregion


#pragma region NumericDigitsSize

#define SEGMENT_RECT_HEIGHT 15
#define SEGMENT_RECT_WIDTH 5

#define TRIANGLE_HEIGHT 2 // Digit's Segment Arrow's length

#define SEGMENT_PEAK_TO_PEAK_DIFFERENCE (SEGMENT_RECT_HEIGHT+2*TRIANGLE_HEIGHT) // total length of a polygon = 1 rectangle + 2 triangles
#define DIGIT_TO_DIGIT_DIFFERENCE 10

#pragma endregion


#pragma region Text

#define TEXT_FONT_LARGE GLUT_BITMAP_TIMES_ROMAN_24
#define TEXT_FONT_LARGE_WIDTH 15
#define TEXT_FONT_LARGE_HEIGHT 24

#define TEXT_TO_TEXT_DIFFERENCE_VERTICAL 7

#define TEXT_UI_WIDTH 270
#define TEXT_UI_HEIGHT (TEXT_FONT_LARGE_HEIGHT + TEXT_TO_TEXT_DIFFERENCE_VERTICAL)

#define TEXT_UI_BOX_DIFFERENCE 30

#define TEXT_BOX_START_Y 400

#define TEXT_START_DIFFERENCE_FROM_BOX 30

#pragma endregion



#pragma region DefinationOfObjectDataTypes

struct Point
{
	int x;
	int y;
};

struct Image
{
	char *fileName;//[25]

	Point start;
	unsigned int isPresent : 1;
};


struct Color
{
	int r;
	int g;
	int b;
};

struct Circle
{
	Point center;
	Color colour;
	int radius;

	int dx;
	int dy;
	int slope;

	char type;
};


struct Rectangle
{
	Point start;
	Color colour;
	int height;
	int width;

	int dx;
	int dy;

	unsigned int isPresent : 1;
};

//#define SetDrawingColor(clr) iSetColor(clr.r,clr.g,clr.b);

enum BRICKS_TYPE
{
	Nor_Red = 01, Nor_Blue, Nor_Green, Nor_Orange, Nor_Fire,
	Esp_Red = 11, Esp_Blue, Esp_Green, Esp_Orange, Esp_Fire,
	Img_Life = 21, Img_Kill, Img_BigBall, Img_BrickBreaker, Img_StickEnlarger, Img_LevelUp,
	Null = 0
};

enum POWER_TYPE
{
	Life = Img_Life - 20, Kill, BigBall, BrickBreaker, StickEnlarger, LevelUp
};


struct Time
{
	int hour;
	int min;
	int sec;
};

struct Date
{
	int year;
	int month;
	int day;
};

struct User
{
	char name[30];
	int score;

	int scoreRate;
	Color colour;

	Date playEvent;
	struct Time playTime;

	int lives;
};

#pragma endregion





#pragma region FunctionLinks

void LoadLevel(int *totalBrickNos, int brickMatrix[][SCALED_Y(SCREEN_HEIGHT)], int *scoreRate, int shift = 0);

#pragma endregion




#pragma region StringKeyWords

#define ENTER '\r'
#define BACKSPACE '\b'
#define SPACEBAR ' '

#pragma endregion

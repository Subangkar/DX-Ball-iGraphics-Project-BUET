
#include "fileIO.h" // also //#include "definations.h"

static int thisLevel = 1; // 'static' used so that when we come back we can get the prev level value

char Levels[LAST_LEVEL + 1][25] = // '+1' to make index eql to fileName
{
	"",
	FILE_LEVEL_1,
	FILE_LEVEL_2,
	FILE_LEVEL_3,
	FILE_LEVEL_4,
	FILE_LEVEL_5,
};

int score_normal_level[LAST_LEVEL + 1] =
{ 0,SCORE_LEVEL1_NORMAL,SCORE_LEVEL2_NORMAL,SCORE_LEVEL3_NORMAL,SCORE_LEVEL4_NORMAL,SCORE_LEVEL5_NORMAL };


void LoadLevel(int *totalBrickNos, int brickMatrix[][SCALED_Y(SCREEN_HEIGHT)], int *scoreRate,int shift)
{
	if (shift==1) thisLevel = 1;
	else if (shift == -1) thisLevel -= 1;

	if (thisLevel <= LAST_LEVEL)
	{
		int lastX = BRICK_MAX_X, lastY = BRICK_MAX_Y; // 26-2,34-12-1

		FILE *fp = fopen(Levels[thisLevel], "rb");

		//for (int row = BRICK_START_Y; row <= lastY; row++) // '<' coz row is index which is y-1
		//{
		//	for (int colm = BRICK_START_X; colm <= lastX && fscanf(fp, "%d", &brickMatrix[colm][row]) != EOF; colm++)
		//	{
		//		if (brickMatrix[colm][row])
		//			(*totalBrickNos)++;
		//	}
		//	if (feof(fp)) break;
		//}

		// read input from file in such a way so that saved pos can be read as like as it is shown in console means pos starting from (X,Y) from lower left corner & going up ++X & going right ++Y
		// to do so take serially input from file but @brickMatrix[][]  Y=row must be assigned in backward direction input & X=colm in forward direction
		for (int Y = BRICK_MAX_Y; Y >= BRICK_START_Y; Y--) // this is reversely taken input
		{
			for (int X = BRICK_START_X; X <= lastX && fscanf(fp, "%d", &brickMatrix[X][Y]) != EOF; X++)
			{
				if (brickMatrix[X][Y])
					(*totalBrickNos)++;
			}
			if (feof(fp)) break;
		}

		fclose(fp);

		*scoreRate = score_normal_level[thisLevel];

		thisLevel++;
	}
}


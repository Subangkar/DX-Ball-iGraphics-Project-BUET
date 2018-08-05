#include "fileIO.h"


/* Empty The File */
void clear(const char* fileName)
{
	FILE *fp = fopen(fileName, "wb");
	fflush(fp);
	fclose(fp);
}

void resetHighScore()
{
	clear(FILE_HIGHSCORE);
}


/* Add 'with' bitsets of data into atPos of toFile */
int insertUser(fpos_t atPos, User thisPlayer)
{
	//	if (atPos == EOF) atPos = 0;

	FILE *fp = fopen(FILE_HIGHSCORE, "rb");
	FILE *fp_tmp = fopen(__DATE__, "wb");

	size_t length = sizeof(User);

	User* temp = (User*)malloc(1 * sizeof(User));

	while (ftell(fp) != atPos)
	{
		fread(temp, sizeof(User), 1, fp);
		fwrite(temp, sizeof(User), 1, fp_tmp);
	}

	// now @ atPos

	fwrite(&thisPlayer, sizeof(User), 1, fp_tmp);

	while (!feof(fp) && fread(temp, sizeof(User), 1, fp))
	{	
		fwrite(temp, sizeof(User), 1, fp_tmp);
	}

	free(temp);
	fclose(fp), fclose(fp_tmp);

	remove(FILE_HIGHSCORE);

	rename(__DATE__, FILE_HIGHSCORE);

	return 0;
}


/* Returns the beginning pos of low scorer than of this Player & if tie assume this Player has low score & if short of low scorer then return the last pos */
fpos_t positionOfLowScoreThanThisPlayer(int thisPlayerScore, size_t* totalScorerBeforeThis)
{
	FILE *fp = fopen(FILE_HIGHSCORE, "rb");

	fpos_t foundPos = EOF, curPos = 0;
	User* readPlayer = (User*)malloc(1 * sizeof(User));

	*totalScorerBeforeThis = 0;
	while ((curPos = ftell(fp)) != EOF && fread(readPlayer, sizeof(User), 1, fp))
	{
		if (readPlayer->score <= thisPlayerScore)
		{
			foundPos = curPos;
			break;
		}
		(*totalScorerBeforeThis)++;
	}

	if (curPos != foundPos && *totalScorerBeforeThis < HIGHSCORE_MAX_SHOW_LIMIT) // 1st check is needed so that player has high score and also total scorer is less than 15
	{
		fseek(fp, 0, SEEK_END);
		foundPos = ftell(fp); // then to append this player at last
	}

	free(readPlayer);
	fclose(fp);
	return foundPos;
}





// find lower score than this & insert this Player Info @ there & even append in case of less HighScorer
void upDateHighScore(User *Player)
{
	size_t personsBefore;
	fpos_t lowScorePos = positionOfLowScoreThanThisPlayer(Player->score, &personsBefore);

	if (lowScorePos != EOF) // personsBefore < HIGHSCORE_MAX_SHOW_LIMIT || 
	{
		insertUser(lowScorePos, *Player);
	}

}

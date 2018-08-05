#include <stdio.h>
#include <stdlib.h>

#include "definations.h"




/* Empty The File */
void clear(const char* fileName);

// cleans the HighScoreFile
void resetHighScore();

// Starts To Find Scores less Than This User's & replace if found any
void upDateHighScore(User *);

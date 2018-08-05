//#include "definations.h"
#include <string.h>


//scan a string & toggles it || ABC > CBA /// These method will only work if memory is allocated
void stringRev(char *str) // Using Loop Third Var Needed
{
	for (int minIndex = 0, maxIndex = strlen(str) - 1; minIndex < maxIndex; minIndex++, maxIndex--)
	{
		*(str + minIndex) = *(str + minIndex) ^ *(str + maxIndex);
		*(str + maxIndex) = *(str + minIndex) ^ *(str + maxIndex);
		*(str + minIndex) = *(str + minIndex) ^ *(str + maxIndex);
	}

	return;
}



// convert an int number to string
char* numberToString(int num)
{
	int numPos = 0;
	char *numStr = new char[33];

	while (num != 0)
	{
		int thisDigit = num % 10;

		*(numStr + numPos) = thisDigit + '0';

		numPos++;
		num /= 10;
	}

	*(numStr + numPos + 1) = '\0'; // End the String

	stringRev(numStr); // Reversing After Making It A String

	return numStr;
}

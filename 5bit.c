/* 5Bit.c
 *	:author Jetharin Kevin Charoenworawat
 *	:UTEID	JKC2237
 *
 *	:intent Program that creates for a standard
 *		of file transfer between different
 *		machines that takes 8-bit chunks 
 *		and encodes them into 5-bit chunks
 *		and vice-versa.
 *	:slips  1 Slip Day Use
 */

// Boolean Type Definition
typedef short Boolean;
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>

FILE* input;
short maxLineLength = 72;

void encode(void);
void decode(void);

// Program's driver Method
int main(int argc, char** argv)
{
	// Unsuccessful Termination as 5Bit.c only supports 
	// 	either encoding or decoding of one file. 
	if(argc > 3)
	{
		fprintf(stderr, "5Bit.c does not support these arguments.\n");
		exit(1);
	}

	// Bypasses the program name as an argument
	argc--;
	argv++;

	// Means that there was no file argument given
	//	so the user must specify one using
	//	stdin
	if(argc == 0)
	{//////////////////////////////////////////////////////////// Need to Fix stdin 
		input = stdin;
		encode();
		exit(0);
	}
	else if(argc == 1)
	{
		input = fopen(*argv, "r");
		if(input == NULL)
		{	
			fprintf(stderr, "Can't open %s\n", *argv);
			exit(1);
		}

		encode();
	}
	else
	{
		// Decreased the number of arguments but is
		//	not necessary to do.
		argc--;

		(* argv)++;
		if(** argv != 'd')
		{
			fprintf(stderr, "This is not a valid command for 5Bit.c\n");
			exit(1);
		}
		else
		{
			// Pass over the command argument
			argv++;
			
			input = fopen(*argv, "r");
			if(input == NULL)
			{
				fprintf(stderr, "Can't open %s\n", *argv);
				exit(1);
			}

			decode();
		}
	}	

	exit(0);
}

// Method that creates a mask based on how many bits
//	are needed
unsigned int getMask(unsigned int numBitsCarry)
{
	unsigned int mask = 0;
	unsigned int numBitsNeeded = 5 - numBitsCarry;
	short shiftAmount = 8 - numBitsNeeded;	

	int i;
	for(i = 0; i < numBitsNeeded; i++)
	{
		mask += 1 << i;
	}

	return mask << shiftAmount;
}

// Method to determine a mask for a carry that has 
//	more than 5 bits
unsigned int getCarryBitsMask(unsigned int numBitsCarry)
{
	unsigned int mask = 0;
	unsigned int numBitsNeeded = numBitsCarry - 5;
	
	// No Need for Shift in order to get at low order bits

	int i;
	for(i = 0; i < numBitsNeeded; i++)
	{
		mask += 1 << i;
	}

	return mask;
}

// Method that returns the number of total bits 
//	that is used to represent the passed number
unsigned int getNumBits(unsigned int bitsInCarry)
{
	unsigned int temp = bitsInCarry;
	unsigned int count = 0;
	
	while(temp != 0)
	{
		if(temp == 1)
		{
			count++;
			return count;
		}
		else
		{
			temp = temp/2;
			count++;
		}
	}
		
	return count;
}
	
void encode(void)
{
	unsigned int c;
	unsigned int oldC;
	unsigned int bitsToEncodeMask;
	unsigned int carryBitsMask;
	unsigned int numBitsInCarry;
	unsigned int numBitsInCarryTemp;
	unsigned int numBitsHave;
	
	// Encoding Value
	unsigned int encodeOffset = 65;
	short  currentLineLength = 0;
	//short originalNumBits = 8;
	//unsigned int numBitsEncoded = 5;	
	unsigned int bitsToEncode = 0;
	unsigned int carryBits = 0;
	short shift;
	unsigned int encodedBits;
fprintf(stderr, "-------------ENCODE-----------------\n");	
	Boolean done = FALSE;
	c = 0;

	while(!done)
	{
		numBitsInCarryTemp = (8 - (5 - numBitsInCarry));
		numBitsInCarry = getNumBits(carryBits);
		if(numBitsInCarryTemp > numBitsInCarry && numBitsInCarryTemp <= 5)
		{
			numBitsInCarry = numBitsInCarryTemp;
		}
		shift = 8 - (5 - numBitsInCarry);

	fprintf(stderr, "%i numBits in carry.\n", numBitsInCarry);
		// Case where there is enough bits to contine the encoding process
		if(numBitsInCarry >= 5)
		{
			encodedBits = (carryBits >> (numBitsInCarry - 5)) + encodeOffset;
		printf("%c", encodedBits);
			
			carryBitsMask = getCarryBitsMask(numBitsInCarry);
			carryBits = (carryBits & carryBitsMask);
		}
		else
		{
			c = getc(input);
			if(c == EOF)
			{
				// Fence Post Problem
				encodedBits = carryBits << (5 - numBitsInCarry);
				printf("%c", encodedBits);

				exit(0);	
			}
		
			// Fixes the carry bits to be in correct magnitude	
			carryBits = carryBits << (5 - numBitsInCarry); 

			bitsToEncodeMask = getMask(numBitsInCarry);
			carryBitsMask = ~bitsToEncodeMask;
		//fprintf(stderr, "%i bitsToEncodeMask.\n", bitsToEncodeMask);
		fprintf(stderr, "%i carryBitMask.\n", carryBitsMask);
			bitsToEncode = ((c & bitsToEncodeMask) >> shift);

		//fprintf(stderr, "%i bitsToEncode.\n", bitsToEncode);
			encodedBits = (carryBits + bitsToEncode) + encodeOffset;

		//printf("%c is the value of c.\n", c);
			// Update the new carry bits resulting from the left over bits of 'c'
			carryBits = (c & carryBitsMask);
					
		printf("%c", encodedBits);
		//printf("%i is the carry bit value.\n", carryBits);
		}
	}

	exit(0);
}

void decode (void)
{
	fprintf(stdout, "OHHH");
	exit(1);
}

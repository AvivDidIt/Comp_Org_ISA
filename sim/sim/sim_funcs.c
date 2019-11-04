#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "sim_funcs.h"


void storeword(int simm, int* regs, char rs, char rd, char *memout_name)
{
	// ********************** Variable Declarations *************************** //

	FILE *memout = fopen(memout_name, "r+") ;	// memout file pointer, open for writing
	int  pos								,	// current position within the file
		 addr								;	// location (PC) in which we wish to store the word

	// ************************************************************************ //

	// extract addr from registers
	addr = regs[HextoDec(rs)] + simm;

	// go to end of file
	fseek(memout, 0, SEEK_END);

	// store line number in pos
	pos = ftell(memout) / 10;

	// if memory file is shorter than addr, add lines of 0 to file
	while (pos < addr)
	{
		fprintf(memout, "%08X\n", 0);
		pos = ftell(memout) / 10;
	}

	// store word in correct location
	fseek(memout, addr*10, SEEK_SET);
	fprintf(memout, "%08X\n", regs[HextoDec(rd)]);

	fclose(memout);
}


void loadword(int simm, int* regs, char rs, char rd, char *memout_name)
{
	// ********************** Variable Declarations *************************** //

	FILE *memout = fopen(memout_name, "r+") ;	// memout file for reading
	int  addr								,	// pc of the instruction we wish to load
		 eof								;	// holds the largest pc in memout
	char temp[10]							;	// holds the instruction that is loaded

	// ************************************************************************ //

	// extract address from regs
	addr = regs[HextoDec(rs)] + simm;

	// go to end of file
	fseek(memout, 0, SEEK_END);

	// store eof line number in eof
	eof = ftell(memout) / 10;

	// if add >= eof then the data in that addr is 0
	if (addr >= eof)
		regs[HextoDec(rd)] = 0;

	// otherwise go to addr and extract data from memory
	else
	{
		fseek(memout, addr * 10, SEEK_SET);

		fgets(temp, 10, memout);
		temp[8] = 0;

		regs[HextoDec(rd)] = HexValue(temp);
	}

	fclose(memout);
}


int HextoDec(char tav)
{
	int	 i = 0;
	char Hexa[17] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','\0' };

	// the index that matches the hex char is it's decimal value
	for (i = 0; Hexa[i] != '\0'; i++)
	{
		if (tav == Hexa[i])
			break;
	}

	return i;
}


int HexValue(char *str)
{
	// ********************** Variable Declarations *************************** //

	int i			   = 1													,	// index
		val			   = 0													,	// integer value of Hexidecimal number
		len			   = strlen(str)										,	// length in Hex Digits 
		signed_val[16] = { 0, 1, 2, 3, 4, 5, 6, 7, -8,-7,-6,-5,-4,-3,-2,-1 };	// array for value of MSB if the number is signed

	// ************************************************************************ //

	// MSB is dependant on sign
	val = signed_val[HextoDec(str[0])] * pow(16, len - 1);

	// calculate the rest of the value
	while (str[i] != '\0')
	{
		val = val + HextoDec(str[i])*pow(16, len - 1 - i);
		i++;
	}

	return val;
}


void branch(int regs[16], char *inst, int *ptrPC, int imm)
{
	// intuitive function, no need to explain
		 if ( (HextoDec(inst[4]) == 0) && (regs[HextoDec(inst[2])] == regs[HextoDec(inst[3])]) )
		*ptrPC = imm;
	else if ( (HextoDec(inst[4]) == 1) && (regs[HextoDec(inst[2])] != regs[HextoDec(inst[3])]) )
		*ptrPC = imm;
	else if ( (HextoDec(inst[4]) == 2) && (regs[HextoDec(inst[2])]  > regs[HextoDec(inst[3])]) )
		*ptrPC = imm;
	else if ( (HextoDec(inst[4]) == 3) && (regs[HextoDec(inst[2])]  < regs[HextoDec(inst[3])]) )
		*ptrPC = imm;
	else if ( (HextoDec(inst[4]) == 4) && (regs[HextoDec(inst[2])] >= regs[HextoDec(inst[3])]) )
		*ptrPC = imm;
	else if ( (HextoDec(inst[4]) == 5) && (regs[HextoDec(inst[2])] <= regs[HextoDec(inst[3])]) )
		*ptrPC = imm;
	else
		*ptrPC += 1 ;

}


void InstCalc(char *inst, int regs[16], int* ptrPC, char *memout)
{
	// ********************** Variable Declarations *************************** //

	int		 imm	= 0					,	// holds the value of imm / simm
			 i		= HextoDec(inst[0])	,	// holds the operation type
			 shift						;	// holds the shift value
	long int padding = 0xFFFFFFFF		;	// used for sra in the case of negative numbers

	// ************************************************************************ //

	// --- immediate field --- //

	// case 1: unsigned imm
	if (i == 4 || i == 5 || i == 7 || i == 11)
		imm = HextoDec(inst[5]) * pow(16, 2) + 16 * HextoDec(inst[6]) + HextoDec(inst[7]);

	// case 2: signed imm
	else
		imm = HexValue(&inst[5]);

	// ----------------------- //

	// switch case for opcode
	switch (i)
	{
	case 0:		// add
		regs[HextoDec(inst[1])] = regs[HextoDec(inst[2])] + regs[HextoDec(inst[3])] + imm;
		*ptrPC += 1;
		break;
	case 1:		// sub
		regs[HextoDec(inst[1])] = regs[HextoDec(inst[2])] - regs[HextoDec(inst[3])] - imm;
		*ptrPC += 1;
		break;
	case 2:		//and
		regs[HextoDec(inst[1])] = regs[HextoDec(inst[2])] & regs[HextoDec(inst[3])] & imm;
		*ptrPC += 1;
		break;
	case 3:		// or
		regs[HextoDec(inst[1])] = regs[HextoDec(inst[2])] | regs[HextoDec(inst[3])] | imm;
		*ptrPC += 1;
		break;
	case 4:		// sll
		regs[HextoDec(inst[1])] = regs[HextoDec(inst[2])] << (regs[HextoDec(inst[3])] + imm);
		*ptrPC += 1;
		break;
	case 5:		// sra
		shift = (regs[HextoDec(inst[3])] + imm);
		if (regs[HextoDec(inst[2])] < 0)
			padding = padding << (32 - shift);
		else
			padding = 0;
		regs[HextoDec(inst[1])] = padding  |  (  regs[HextoDec(inst[2])] >> (regs[HextoDec(inst[3])] + imm)  );
		*ptrPC += 1;
		break;
	case 6:		// mac
		regs[HextoDec(inst[1])] = regs[HextoDec(inst[2])] * regs[HextoDec(inst[3])] + regs[HextoDec(inst[4])] + imm;
		*ptrPC += 1;
		break;
	case 7:		// branch
		branch(regs, inst, ptrPC, imm);
		break;
	case 8:		// reserved
		*ptrPC += 1;
		break;
	case 9:		// reserved
		*ptrPC += 1;
		break;
	case 10:	// reserved
		*ptrPC += 1;
		break;
	case 11:	// jal
		regs[15] = (*ptrPC + 1);
		*ptrPC = imm;
		break;
	case 12:	// lw
		loadword(imm, regs, inst[2], inst[1], memout);
		*ptrPC += 1;
		break;
	case 13:	// sw
		storeword(imm, regs, inst[2], inst[1], memout);
		*ptrPC += 1;
		break;
	case 14:	// jr
		*ptrPC = regs[HextoDec(inst[1])] & 0xFFF;
		break;
	case 15:	// halt
		return;
	}

}


void ptrace(FILE *trace, int pc, char *inst, int regs[16])
{
	int i = 0;	// index

	// print the pc
	fprintf(trace, "%08X %s", pc, inst);

	// print the registers
	while (i < 16) {
		fprintf(trace, " %08X", regs[i]);
		i++;
	}

	// print a new line
	fprintf(trace, "\n");

	return;
}


void pregout(FILE* regout, int regs[16])
{
	int i = 0;	// index

	// print each register onto file
	for (i = 0; i < 16; i++) 
		fprintf(regout, "%08X\n", regs[i]);

	return;
}

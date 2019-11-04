#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "sim_funcs.h"


int main(int argc, char *argv[])
{
	// ********************** Variable Declarations *************************** //

	int  counter = 0,	// counts number of operations for count.txt
		PC = 0,	// holds value of PC
		*ptrPC = &PC,	// enables updating PC within functions
		regs[16] = { 0 };	// holds values of registers
	FILE *memin = NULL,	// memin.txt file pointer  ( argv[1] )
		*memout = NULL;	// memout.txt file pointer ( argv[2] )
	char inst[10],	// holds the instruction read from memin.txt
		ch;	// temp char used to copy memin to meout

   // ************************************************************************ //

   // --- open files for writing --- //

	memout = fopen("memin.txt", "w");

	if (NULL == (memin = fopen("memout.txt", "r")))
	{
		printf("Error Opening File");
		fclose(memin);
	}
	


	// ---------- Main Run ---------- //
	while (!feof(memin))
	{
		fseek(memin, 10 * *ptrPC, SEEK_SET);	// jump to correct position
		fgets(inst, 10, memin);					// read instruction
		inst[8] = 0;							// get rid of '\n'
		fprintf(memout, "\"%s\",", inst);
	}

	// --- close files --- //
	fclose(memin);
	fclose(memout);

	// --- return 0 to symbolize valid run --- //

	return 0;

}




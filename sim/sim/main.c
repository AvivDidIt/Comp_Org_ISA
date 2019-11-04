#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "sim_funcs.h"


int main(int argc, char *argv[])
{
	// ********************** Variable Declarations *************************** //

	int  counter	= 0		,	// counts number of operations for count.txt
		 PC			= 0		,	// holds value of PC
		 *ptrPC		= &PC	,	// enables updating PC within functions
		 regs[16]	= { 0 }	;	// holds values of registers
	FILE *memin		= NULL	,	// memin.txt file pointer  ( argv[1] )
		 *memout	= NULL	,	// memout.txt file pointer ( argv[2] )
		 *regout	= NULL	,	// regout.txt file pointer ( argv[3] )
		 *trace		= NULL	,	// trace.txt file pointer  ( argv[4] )
		 *count		= NULL	;	// count.txt file pointer  ( argv[5] )
	char inst[10]			,	// holds the instruction read from memin.txt
		 ch					;	// temp char used to copy memin to meout
	
	// ************************************************************************ //

	// --- open files for writing --- //

	memout = fopen(argv[2], "w");
	regout = fopen(argv[3], "w");
	trace  = fopen(argv[4], "w");
	count  = fopen(argv[5], "w");

	// --- copy memin to memout --- //
	if (NULL == (memin = fopen(argv[1], "r")))
	{
		printf("Error Opening File");
		fclose(memin);
	}
	while ((ch = fgetc(memin)) != EOF) {
		fputc(ch, memout);
	}
	fclose(memout);

	// reset position in memin to beginning of file
	fseek(memin, 0, SEEK_SET);


	// ---------- Main Run ---------- //
	while (!feof(memin))
	{
		fseek(memin, 10 * *ptrPC, SEEK_SET);	// jump to correct position
		fgets(inst, 10, memin);					// read instruction
		inst[8] = 0;							// get rid of '\n'

		// --- halt --- //
		if (inst[0] == 'F') {
			counter++;
			break;
		}
		// ------------ //

		ptrace(trace, PC, inst, regs);			// print current trace to trace.txt
		InstCalc(inst, regs, ptrPC, argv[2]);	// perform instruction
		counter++;								// update counter
	}

	pregout(regout, regs);						// print register contents to regout.txt
	fprintf(count, "%d", counter);				// print number of operations to count.txt
	
	// --- close files --- //
	fclose(memin);
	fclose(trace);
	fclose(regout);
	fclose(count);

	// --- return 0 to symbolize valid run --- //

	return 0;
}




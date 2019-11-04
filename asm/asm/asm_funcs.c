#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "asm_funcs.h"




void init_op_arr(char* op_arr[])
{
	op_arr[0]  = "add";
	op_arr[1]  = "sub";
	op_arr[2]  = "and";
	op_arr[3]  = "or";
	op_arr[4]  = "sll";
	op_arr[5]  = "sra";
	op_arr[6]  = "mac";
	op_arr[7]  = "branch";
	op_arr[8]  = "in";
	op_arr[9]  = "out";
	op_arr[10] = NULL;
	op_arr[11] = "jal";
	op_arr[12] = "lw";
	op_arr[13] = "sw";
	op_arr[14] = "jr";
	op_arr[15] = "halt";
}



void init_reg_arr(char* reg_arr[])
{
	reg_arr[0]  = "$zero";
	reg_arr[1]  = "$at";
	reg_arr[2]  = "$v0";
	reg_arr[3]  = "$a0";
	reg_arr[4]  = "$a1";
	reg_arr[5]  = "$t0";
	reg_arr[6]  = "$t1";
	reg_arr[7]  = "$t2";
	reg_arr[8]  = "$t3";
	reg_arr[9]  = "$s0";
	reg_arr[10] = "$s1";
	reg_arr[11] = "$s2";
	reg_arr[12] = "$gp";
	reg_arr[13] = "$sp";
	reg_arr[14] = "$fp";
	reg_arr[15] = "$ra";
}



int label_func(char *label_name, Label labels[], int pc, int j, int len)
{
	// assign memory to store the name
	strcpy(labels[j].name, label_name);

	labels[j].name[len - 1] = 0; // get rid of ':' and store label name
	labels[j].pc = pc;			 // store pc
	labels[j+1].name[0] = 0;
	
	return j+1;
}



FILE *word_func(char *words[], FILE *output)
{
	// -------------------- variable declerations ---------------------------- //

	long int pos			,	// position of the file pointer
			 old_pos		,	// position in file when the function is called
			 data			,	// the data is the number given that we wish to print
			 addr			;	// the addr is the pc where we want to print the data
		 int len			,	// holds the len of addr / data in the case where the addr / data is given as a hexidecimal number
			 i				,	// index
			 j				,	// index
			 d_is_hex = 0	;	// flag that is set if data is given in Hex
		char *hex_word		,	// holds addr / data in the case where the addr / data is given as a hexidecimal number
			  hex_chars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	// ----------------------------------------------------------------------- //

	// initialize vars
	addr	 = 0;
	data	 = 0;
	pos      = ftell(output) / 10 ;
	old_pos  = pos;
	hex_word = NULL;

	// case where the addr is given in hex: convert the addr into decimal using formal definition
	if (words[1][1] == 'x')
	{
		hex_word = &words[1][2];
		len = strlen(hex_word);
		for (i = 0; i < len; i++)
		{
			for (j = 0; j < 16; j++)
			{
				if (hex_word[i] == hex_chars[j])
					addr = addr + j * pow(16, len - i - 1);
			}
		}
	}
	// otherwise convert directly do decimal
	else
		addr = atoi(words[1]);

	// case where the data is given in hex: keep as hex and set the flag d_is_hex
	if (words[2][1] == 'x')
	{
		hex_word = &words[2][2];
		d_is_hex = 1;
	}
	// otherwise convert directly do decimal
	else
		data = atoi(words[2]);


	// go to end of file, and save line number in pos
	fseek(output, 0, SEEK_END);
	pos = ftell(output) / 10;

	//case 1: current line # (pos) < addr
	while (pos < addr)
	{
		fprintf(output,"%08X\n", 0);
		pos = ftell(output) / 10 ;
	}

	//case 2:
	if (pos == addr)
	{
		if (d_is_hex)
			fprintf(output, "%08s\n", hex_word);
		else
			fprintf(output,"%08X\n", data);
	}

	//case 3:
	else if (pos > addr)
	{
		fseek(output, 10 * addr, SEEK_SET);
		if (d_is_hex)
			fprintf(output, "%08s\n", hex_word);
		else
			fprintf(output, "%08X\n", data);
	}

	//return to original position in file
	fseek(output, 10 * old_pos, SEEK_SET);

	return output;

}



FILE *print_to_reg(int op, char * words[], char * reg_arr[], FILE * output, Label labels[])
{
	// -------------------- variable declerations ---------------------------- //

	int          i							,	// index
		         j							,	// another index
		         rm							,	// holds rm (because this can be a reg or an int)
				 len						,	// holds length of string hex_imm.
		         flag         = 0			,	// flag for use in loops
		         simm         = 0			;	// signed immediate
	unsigned int imm          = 0			;	// unsigned immediate
	char*        hex_imm      = NULL		;	// immediate given in hex
	char         temp_simm[9] = { NULL }	;	// used to cut simm from 4B to 12b

	// ----------------------------------------------------------------------- //


	// --- print the 4 registers (rd, rs, rt, rm) --- //
	for (j = 1; j <= 4; j++)
	{
		for (i = 0; i <= 15; i++)
		{
			if (strcmp(words[j], reg_arr[i]) == 0)
			{
				if (j == 4)
					flag = 1;
				fprintf(output, "%1X", i);
			}
		}
	}
	// --- rm --- //
	if (flag == 0)
	{
		rm = atoi(words[4]);
		fprintf(output, "%1X", rm);
	}

	// ---------------------------------------------- //


	// --- imm / simm --- //

	if ( words[5][1] == 'x' )
	{
		hex_imm = &words[5][2];
	}
	imm  = atoi(words[5]);
	simm = atoi(words[5]);
	flag = 0;

	// check if imm is label
	for (i = 0; i <= 4096; i++)
	{
		if ( (strlen(labels[i].name) == 0) || (flag == 1) )
			break;
		else
			if (strcmp(words[5], labels[i].name) == 0)
			{
				fprintf(output, "%03X\n", labels[i].pc);
				flag = 1;
			}

	}


	
	// imm is label
	if (flag == 1)
		return output;

	//imm given in hex
	else if (hex_imm != NULL)
	{
		len = strlen(hex_imm);
		//TODO: check if need padding of F's if simm or if 0's is fine
		if (len <= 3)
			fprintf(output, "%03s\n", hex_imm);
		else
			fprintf(output, "%s\n", &hex_imm[len - 3]);
	}

	// imm
	else if ( (op == 4) || (op == 5) || (op == 7) || (op == 11) )
		fprintf(output, "%03X\n", imm);

	// simm
	else
	{
		// simm is positive
		if (simm > 0)
			fprintf(output, "%03X\n", simm);
		// simm is negative
		else
		{
			sprintf(temp_simm, "%08X", simm);
			fprintf(output, "%s\n", &temp_simm[5]);
		}
	}

	return output;
}

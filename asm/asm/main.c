#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "asm_funcs.h"



int main(int argc, char *argv[])
{

	// ------------------ variable definitions ---------------------------------------- //

	FILE *input											,	//stores the input file
         *output										;   //holds the output file
	char *temp											,   //used for strtok
	     *words[10] = {NULL}							,   //holds the words that are split from the sentence
		 *op_arr[16]									,	//array of operations
		  line[11]										,	//used for BASYS Project
		 *reg_arr[16]									;	//array of registers
	int  i												,   //index for loops
		 j = 0											,   //index
		 len = 0										,	//length of first word
		 flag											,   //bit {0,1}: used to increment the pc
		 pc = 0											;	//program counter
	void *sentence = (char*)malloc(sizeof(char) * 500)  ;   //holds current line while reading input file
	Label labels[4096]                                  ;	//stores labels (see struct_utils.h)

	// --------------------------------------------------------------------------------- //

	// open files

	//TODO: error handling with files?
	input = fopen("stopper.asm", "r");// argv[1], "r");
	output = fopen(argv[2], "w")	;

	//initialize variables:

	init_op_arr(op_arr);
	init_reg_arr(reg_arr);


	
	// --- FIRST RUN: get labels --- //

	while (!feof(input))
	{
		
		// read sentence from input file:
		fgets(sentence, 500, input);

		temp = strtok(sentence, " ,\t\n");

		//check if the first word is a label:
		if (temp != NULL)
		{
			len = strlen(temp);
			if (temp[len - 1] == ':')
			{
				j = label_func(temp, labels, pc, j, len);
			}
			else if (strcmp(temp, ".word") == 0)
			{
				pc--;
			}
		}

		temp = strtok(NULL, " ,\t\n");

		flag = 0;
		while ((temp != NULL) && (*temp != '#'))
		{
			flag = 1;
			temp = strtok(NULL, " ,\t\n");
		}

		//update the PC
		pc += flag;

	}

	fclose(input);

	// ------------------------------- //
	

	// --- MAIN RUN: analyze .asm file, extract code words, translate to hex, write to memory --- //

	input = fopen("stopper.asm", "r");//argv[1], "r");
	pc = 0;

	while (!feof(input))
	{
		// read sentence from .asm file:
		fgets(sentence, 500, input);

		// split the sentence into words and store each word in array words[]:
		for (i = 0; i <= 10; i++)
		{
			if (words[i] == NULL)
				break;
			words[i] = NULL;
		}
		temp = strtok(sentence, " ,\t\n");
		i = 0;
		words[0] = temp;

		//check if the first word is a label:
		if (words[0] != NULL)
		{
			len = strlen(words[0]);
			if (words[0][len - 1] != ':')
			{
				i++;
			}
		}
			   
		//continue storing the words in the array:
		flag = 0;
		while ((temp != NULL) && (*temp != '#'))
		{
			flag = 1;
			temp = strtok(NULL, " ,\t\n");
			words[i] = temp;
			i++;
		}

		//update the PC
		pc += flag;
			   
		//check cases:

		if (words[0] != NULL)
		{
			//case 1: check if the line starts with '.word' 
			if (strcmp(words[0], ".word") == 0)
			{

				if (words[2] != NULL)
					output = word_func(words, output);
				pc--;
			}

			//case 2: check if the line is a regular operation
			else
			{
				if (words[1] == NULL)
					continue;
				for (i = 0; i <= 15; i++)
				{
					//reserved opcodes:
					if (op_arr[i] == NULL)
						continue;

					//check if recognized operation
					else if (strcmp(words[0], op_arr[i]) == 0)
					{
						fprintf(output, "%1X", i);
						output = print_to_reg(i, words, reg_arr, output, labels);
						break;
					}
				}
			}
		}

	}
	// ---------------------------------- End of Main Run --------------------------------------- //



	// --- free any allocated memory --- //

	free(sentence);


	// --- close any open files --- //

	fclose(input);
	fclose(output);

	// for BASYS Project
	input = fopen(argv[2], "r");
	output = fopen("stopper_memin.txt", "w");

	while (!feof(input))
	{
		fgets(line, 10, input);
		line[8] = 0;
		fprintf(output, "\"%s\",", line);
	}
	fclose(input);
	fclose(output);
	
	// --- return 0 to signal a valid run --- //

	return 0;

}
#ifndef STRUCT_UTILS_H
#define STRUCT_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "asm_funcs.h"




//We made a struct to hold the labels with their name and pc
typedef struct label {
	char name[50];
	int  pc;
} Label;


void init_op_arr(char* op_arr[]);
// Initiates the array of operations (op_arr)

void init_reg_arr(char* reg_arr[]);
// Initiates the array of registers (reg_arr)


int label_func(char *label_name, Label labels[], int pc, int j, int len);
// Function that is called when a label is found during the first run over the input file.
// The function saves the label's name and pc in the Label struct.


FILE *word_func(char *words[], FILE *output);
// Function that is called when a line starts with '.word'.
// The function prints the proper line as instructed, and returns the file pointer
// to the position it was in before the function was called.


FILE *print_to_reg(int op, char* words[], char* reg_arr[], FILE *output, Label labels[]);
// Function that prints the proper data to the register.
// Handles immediates that are pos / neg integers, and also hexidecimal.

#endif STRUCT_UTILS_H

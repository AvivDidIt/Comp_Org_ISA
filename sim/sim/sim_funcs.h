#ifndef SIM_FUNCS_H
#define SIM_FUNCS_H
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "sim_funcs.h"


void storeword(int simm, int* regs, char rs, char rd, char *memout_name);
// sw function that stores the word in the file memout.txt as 
// instructed in the instruction table.

void loadword(int simm, int* regs, char rs, char rd, char *memout_name);
// lw function that loads the word from the file memout.txt as 
// instructed in the instruction table.

int HextoDec(char tav);
// transforms 4-bit Hexidecimal character to integer

int HexValue(char *str);
// transforms signed Hexidecimal string to integer

void branch(int regs[16], char *inst, int *ptrPC, int imm);
// updates pc to imm using conditions that depend on the rm,
// as instructed in the instruction table.

void InstCalc(char *inst, int regs[16], int* ptrPC, char *memout);
// Instruction Calculator: Analyzes the instruction and calculates
// the value of the immediate and, using a switch case, performs the
// proper operation.

void ptrace(FILE *trace, int pc, char *inst, int regs[16]);
// prints the current trace onto the trace.txt file


void pregout(FILE* regout, int regs[16]);
// prints the register contents onto the regout.txt file




#endif SIM_FUNCS_H
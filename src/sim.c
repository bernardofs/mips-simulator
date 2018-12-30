#include <stdio.h>
#include "shell.h"
#include <inttypes.h>

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */


	uint32_t value = mem_read_32(CURRENT_STATE.PC);

	uint32_t op = value >> 26;

	if(op == 0) {

		// Tipo R

		uint32_t rs = (value >> 21) & 0x1F;
		uint32_t rt = (value >> 16) & 0x1F;
		uint32_t rd = (value >> 11) & 0x1F;
		uint32_t shamt = (value >> 6) & 0x1F;
		// ultimos 6 digitos do value;
		uint32_t func = (value >> 0) & 0x3F;

		//printf("op = %lu, rs = %lu, rt = %lu, rd = %lu, shamt = %lu, func = %lu\n", (unsigned long)op, (unsigned long)rs,(unsigned long)rt,(unsigned long)rd,(unsigned long)shamt,(unsigned long)func);

		NEXT_STATE.PC = CURRENT_STATE.PC + 4;

		if(func == 0x20) {
			// ADD
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + (int32_t)CURRENT_STATE.REGS[rt]; 
		} else if(func == 0x21) {
			// ADDU
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
		} else if(func == 0x24) {
			// AND
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
		} else if(func == 0x08) {
			// JR
			NEXT_STATE.PC = CURRENT_STATE.REGS[31];
		} else if(func == 0x27) {
			// NOR
			NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);	
		} else if(func == 0x25) {
			// OR
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
		} else if(func == 0x26) {
			// XOR
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
		} else if(func == 0x2a) {
			// SLT
			NEXT_STATE.REGS[rd] = ((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt] ? 1 : 0);
		} else if(func == 0x2b) {
			// SLTU
			NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt] ? 1 : 0);
		} else if(func == 0x00) {
			// SLL
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
		} else if (func == 0x04) {
			// SLLV
			uint32_t s = CURRENT_STATE.REGS[rs] & 0x1F;
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << s;
		} else if(func == 0x06) {
			// SRLV 
			uint32_t s = CURRENT_STATE.REGS[rs] & 0x1F;
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> s;
		} else if(func == 0x07) {
			// SRAV
			uint32_t s = CURRENT_STATE.REGS[rs] & 0x1F;
			NEXT_STATE.REGS[rd] = ((int32_t)CURRENT_STATE.REGS[rt]) >> s;
		} else if(func == 0x02) {
			// SRL
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
		} else if(func == 0x22) {
			// SUB
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
		} else if(func == 0x23) {
			// SUBU
			NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
		} else if(func == 0x1a) {
			// DIV
			NEXT_STATE.LO = (int32_t)CURRENT_STATE.REGS[rs] / (int32_t)CURRENT_STATE.REGS[rt];
			NEXT_STATE.HI = (int32_t)CURRENT_STATE.REGS[rs] % (int32_t)CURRENT_STATE.REGS[rt];
		} else if(func == 0x1b) {
			// DIVU
			NEXT_STATE.LO = CURRENT_STATE.REGS[rs] / CURRENT_STATE.REGS[rt];
			NEXT_STATE.HI = CURRENT_STATE.REGS[rs] % CURRENT_STATE.REGS[rt];			
		} else if(func == 0x10) {
			// MFHI
			NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
		} else if(func == 0x12) {
			// MFLO
			NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
		} else if(func == 0x11) {
			// MTHI
			NEXT_STATE.HI = CURRENT_STATE.REGS[rs]; 
		} else if(func == 0x13) {
			// MTLO
			NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
		} else if(func == 0x18) {
			// MULT
			int64_t t = (int32_t)CURRENT_STATE.REGS[rs] * (int32_t)CURRENT_STATE.REGS[rt];
			NEXT_STATE.LO = (((uint64_t)t)&0xFFFFFFFF);
			NEXT_STATE.HI = (((uint64_t)t)>>32);
		} else if(func == 0x19) {
			// MULTU
			uint64_t t = CURRENT_STATE.REGS[rs] * CURRENT_STATE.REGS[rt];
			NEXT_STATE.LO = (t&0xFFFFFFFF);
			NEXT_STATE.HI = (t>>32);
		} else if(func == 0x03) {
			// SRA
			NEXT_STATE.REGS[rd] = ((int32_t)CURRENT_STATE.REGS[rt]) >> shamt;
		} else if(func == 0x09) {
			// JALR
			NEXT_STATE.REGS[rd] = CURRENT_STATE.PC+4;
			NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
		} else if(func == 0x0c) {
			// SYSCALL
			if(CURRENT_STATE.REGS[2] == 10) {
				RUN_BIT = 0;
			}
		}

	} else if(op == 2 || op == 3) {

		uint32_t temp = value & 0x03ffffff;
		//printf("op = %lu, temp = %lu\n",(unsigned long)op, (unsigned long) temp);
		// Tipo J
		if(op == 2) {
			// J
			NEXT_STATE.PC = (CURRENT_STATE.PC & 0xf0000000)|(temp<<2);
		} else {
			// JAL
			NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
			NEXT_STATE.PC = (CURRENT_STATE.PC & 0xf0000000)|(temp<<2);
		}

	} else {

		uint32_t rs = (value >> 21) & 0x1F;
		uint32_t rt = (value >> 16) & 0x1F;
		uint16_t im = (value & 0xFFFF);

		//printf("op = %lu, rs = %lu, rt = %lu, im = ",(unsigned long)op,(unsigned long)rs,(unsigned long)rt);
	    //printf("%" PRIu16 "\n",im);

		// Tipo I
		if(op == 0x04) {
			// BEQ
			if(CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
				NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
			} else {
				NEXT_STATE.PC = CURRENT_STATE.PC + (4);
			}
		} else if(op == 0x05) {
			// BNE
			if(CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
				NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
			} else {
				NEXT_STATE.PC = CURRENT_STATE.PC + (4);
			}
		} else if(op == 0x06) {
			// BLEZ
			if((int32_t)CURRENT_STATE.REGS[rs] <= 0) {
				NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
			} else {
				NEXT_STATE.PC = CURRENT_STATE.PC + (4);
			}
		} else if(op == 0x07) {
			// BGTZ
			if((int32_t)CURRENT_STATE.REGS[rs] > 0) {
				NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
			} else {
				NEXT_STATE.PC = CURRENT_STATE.PC + (4);
			}
		} else if(op == 0x08) {
			// ADDI
			NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + (int16_t)im;
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x09) {
			// ADDIU
			NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + (int16_t)im;
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);			
		} else if(op == 0x0a) {
			// SLTI
			if((int32_t)CURRENT_STATE.REGS[rs] < (int16_t)im) {
				NEXT_STATE.REGS[rt] = 1;
			} else {
				NEXT_STATE.REGS[rt] = 0;
			}
			NEXT_STATE.PC += (4);
		} else if(op == 0x0b) {
			// SLTIU
			if(CURRENT_STATE.REGS[rs] < (int16_t)im) {
				NEXT_STATE.REGS[rt] = 1;
			} else {
				NEXT_STATE.REGS[rt] = 0;
			}
			NEXT_STATE.PC += (4);
		} else if(op == 0x0c) {
			// ANDI
			NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & im;
			NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		} else if(op == 0x0d) {
			// ORI
			NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | im;
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);			
		} else if(op == 0x0e) {
			// XORI
			NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ im;
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);			
		} else if(op == 0x0f) {
			// LUI
			NEXT_STATE.REGS[rt] = (im<<16);
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x20) {
			// LB
			NEXT_STATE.REGS[rt] = (int8_t)(mem_read_32(CURRENT_STATE.REGS[rs] + (int16_t)im) & 0xFF);
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);			
		} else if(op == 0x21) {
			// LH
			NEXT_STATE.REGS[rt] = (int16_t)(mem_read_32(CURRENT_STATE.REGS[rs] + (int16_t)im) & 0xFFFF);
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x23) {
			// LW
			NEXT_STATE.REGS[rt] = mem_read_32(CURRENT_STATE.REGS[rs] + (int16_t)im);
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x24) {
			// LBU
			NEXT_STATE.REGS[rt] = mem_read_32(CURRENT_STATE.REGS[rs] + (int16_t)im) & 0xFF;
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);			
		} else if(op == 0x25) {
			// LHU
			NEXT_STATE.REGS[rt] = mem_read_32(CURRENT_STATE.REGS[rs] + (int16_t)im) & 0xFFFF;
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x28) {
			// SB
			mem_write_32(CURRENT_STATE.REGS[rs]+(int16_t)im,CURRENT_STATE.REGS[rt]&0xff);
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x29) {
			// SH
			mem_write_32(CURRENT_STATE.REGS[rs]+(int16_t)im,CURRENT_STATE.REGS[rt]&0xffff);
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x2b) {
			// SW
			mem_write_32(CURRENT_STATE.REGS[rs]+(int16_t)im,CURRENT_STATE.REGS[rt]);
			NEXT_STATE.PC = CURRENT_STATE.PC + (4);
		} else if(op == 0x01) {
			
			if(rt == 0) {
				// BLTZ
				if((int32_t)CURRENT_STATE.REGS[rs] < 0) {
					NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
				} else {
					NEXT_STATE.PC = CURRENT_STATE.PC + (4);
				}
			} else if(rt == 0x01) {
				// BGEZ
				if((int32_t)CURRENT_STATE.REGS[rs] >= 0) {
					NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
				} else {
					NEXT_STATE.PC = CURRENT_STATE.PC + (4);
				}
			} else if(rt == 0x10) {
				// BLTZAL
				if((int32_t)CURRENT_STATE.REGS[rs] < 0) {
					NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
					NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
				} else {
					NEXT_STATE.PC = CURRENT_STATE.PC + (4);
				}
			} else if(rt == 0x11) {
				// BGEZAL
				if((int32_t)CURRENT_STATE.REGS[rs] >= 0) {
					NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
					NEXT_STATE.PC = CURRENT_STATE.PC + (((int16_t)im)<<2) + 4;
				} else {
					NEXT_STATE.PC = CURRENT_STATE.PC + (4);
				}
			}

		}
	}
}

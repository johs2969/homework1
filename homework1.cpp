#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// registers
unsigned int registers[32];

// program counter
unsigned int pc = 0;

// instruction/data memory
#define INST_MEM_SIZE 32*1024
#define DATA_MEM_SIZE 32*1024
unsigned int inst_mem[INST_MEM_SIZE]; //instruction memory
unsigned int data_mem[DATA_MEM_SIZE]; //data memory

// example instruction set
enum OPCODE {
    ADD = 0b0110011,
    ADDI = 0b0010011,
    LW = 0b0000011,
    SW = 0b0100011,
    BEQ = 0b1100011,
    HALT = 0b1111111,
    LUI = 0b0110111
};
// NOTE:
// add  0110011(51), 000, 0000000       R-Type      
// addi 0010011(19), 000, n.a           I-Type       
// lw   0000011(3), 010, n.a            I-Type      
// sw   0100011(35), 010, n.a           S-Type      
// beq  1100011(99), 000, n.a           SB-Type     
// blt  1100011(99), 100, n.a           SB-Type


int main() {
    ////////     수정금지 영역 시작 (프로그램B의 주석을 해제하는 수정만 가능)    ///////////
   

    // Instructions for Program A
    // ISA: ADD, ADDI, LW, BEQ, HALT
    inst_mem[0] = 0b00000000011000101000001110110011; // add x7 x5 x6
    inst_mem[1] = 0b00000000010100101000001010010011; // addi x5 x5 5
    inst_mem[2] = 0b00000000010100101000001100110011; // add x6 x5 x5
    inst_mem[3] = 0b00000000011000101000001110110011; // add x7 x5 x6
    inst_mem[4] = 0b00010000010100110000000001100011; // beq x6 x5 128
    inst_mem[5] = 0b00000000000010010010001100000011; // lw x6 0(x18) 
    inst_mem[6] = 0b11111111111111111111111111111111; // HALT
  


    // Instructions for Program B 
    /* ISA: ADD, ADDI, LW, BEQ, HALT + (SW, BLT)
    inst_mem[0] = 0b00000000000001000010110000100011; // sw zero 24(x8)
    inst_mem[1] = 0b00000000000001000010101000100011; // sw zero 20(x8)
    inst_mem[2] = 0b00000001010001000010011100000011; // lw x14 20(x8)
    inst_mem[3] = 0b00000000000000000000011110110111; // lui x15 0
    inst_mem[4] = 0b00000000010001111000011110010011; // addi x15 x15 4
    inst_mem[5] = 0b00000010111001111100000001100011; // blt x15 x14 16
    inst_mem[6] = 0b00000001100001000010011110000011; // lw x15 24(x8)
    inst_mem[7] = 0b00000000010001111000011110010011; // addi x15 x15 4
    inst_mem[8] = 0b00000000111101000010110000100011; // sw x15 24(x8)
    inst_mem[9] = 0b00000001010001000010011110000011; // lw x15 20(x8)
    inst_mem[10] = 0b00000000000101111000011110010011; // addi x15 x15 1
    inst_mem[11] = 0b00000000111101000010101000100011; // sw x15 20(x8)
    inst_mem[12] = 0b11111100000000000000110011100011; // beq zero zero -20
    inst_mem[13] = 0b00000001100001000010011110000011; // lw x15 24(x8)
    inst_mem[14] = 0b00000000101001111000011110010011; // addi x15 x15 10
    inst_mem[15] = 0b00000000111101000010111000100011; // sw x15 28(x8)
    inst_mem[16] = 0b11111111111111111111111111111111; // HALT
    */

    // EXIT

    // Given Data for Program A and B
    data_mem[0] = 0b00000000000000000000000000000000; // 0
    data_mem[1] = 0b00000000000000000000000000000001; // 1
    data_mem[2] = 0b00000000000000000000000000000010; // 2
    data_mem[3] = 0b00000000000000000000000000000011; // 3
    data_mem[4] = 0b00000000000000000000000000000100; // 4
    data_mem[5] = 0b00000000000000000000000000000101; // 5
    data_mem[6] = 0b00000000000000000000000000000110; // 6
    data_mem[7] = 0b00000000000000000000000000000111; // 7 

    // Given Register values for Program A and B
    registers[5] = 10;
    registers[6] = 20;
    registers[7] = 30;

    ////////               수정금지 영역 끝             ///////////


    void print_register_values();
    void print_memory_values();

    // flag for end-of-program
    int running = 1;

    while (running) {
        //Fetch 
        int instruction = inst_mem[pc];
        int opcode = (instruction & 0x7F);
        int funct3 = (instruction >> 12) & 0x7;
        int rs1,    // source reg.1
            rs2,    // source reg.2
            rd,     // destination reg.
            imm,    // immediate
            addr;   // base address
        int imm20 = 0;  // upper immediate 20

        switch (opcode) {
        case ADD:
            rd = (instruction >> 7) & 0x1F;
            rs1 = (instruction >> 15) & 0x1F;
            rs2 = (instruction >> 20) & 0x1F;
            registers[rd] = registers[rs1] + registers[rs2];
            pc++;
            break;
        case ADDI:
            rd = (instruction >> 7) & 0x1F;
            rs1 = (instruction >> 15) & 0x1F;
            imm = (instruction >> 20);
            if (!imm20){
            if (imm & 0x800)
                imm |= 0xFFFFF000;
            registers[rd] = registers[rs1] + imm;
            pc++;
            break;
            }
            else {
                registers[rd] = registers[rs1] + imm;
                pc++;
                break;
            }
        case LW:
            rd = (instruction >> 7) & 0x1F;
            rs1 = (instruction >> 15) & 0x1F;
            imm = (instruction >> 20);
            if (imm & 0x800)
                imm |= 0xFFFFF000;
            addr = registers[rs1] + (imm/4);
            registers[rd] = data_mem[addr];
            pc++;
            break;
        case SW:
            rs1 = (instruction >> 15) & 0x1F;
            rs2 = (instruction >> 20) & 0x1F;
            imm = ((instruction >> 25) << 5) | ((instruction >> 7) & 0x1F);
            if (imm & 0x800)
                imm |= 0xFFFFF000;
            addr = registers[rs1] + (imm/4);
            data_mem[addr] = registers[rs2];
            pc++;
            break;
        case BEQ:
            rs1 = (instruction >> 15) & 0x1F;
            rs2 = (instruction >> 20) & 0x1F;
            imm = ((instruction >> 31) << 11) | (((instruction >> 7) & 0x1) << 10) | (((instruction >> 25) & 0x3F) << 4) | ((instruction >> 8) & 0xF);
            if (imm & 0x800)
                imm |= 0xFFFFF000;
            if(!funct3){
            if (registers[rs1] == registers[rs2])
                pc += (imm << 1) / 4;
            else
                pc++;
                break;
            }
            else
            {
                if (registers[rs1] < registers[rs2])
                    pc += (imm << 1) / 4;
                else
                    pc++;
                break;
            }
        case LUI:
            rd = (instruction >> 7) & 0x1F;
            imm20 = (instruction >> 12) << 12;
            registers[rd] = imm20;
            pc++;
            break;
        case HALT:
            running = 0;
            printf(" ** END OF THE PROGRAM ** \n");
            break;
        default:
            printf("Unknown instruction \n");
            running = 0;
            break;
        }

        printf("opcode is: %d\n", opcode);
        print_register_values();
        printf("opcode is: %d\n", opcode);
        print_memory_values();


    }


    // Print final register values
    print_register_values();
    return 0;
}

void print_register_values() {
    printf("Register values:\n");
    int i;
    for (i = 0; i < 32; i++) {
        printf("x%02d: %d\n", i, registers[i]);
    }
}
void print_memory_values() {
    printf("memory values:\n");
    int i;
    for (i = 0; i < 8; i++) {
        printf("x%02d: %d\n", i, data_mem[i]);
    }
}

#include "6502.h"

#include "logger.h"
#include "utils.h"

#include <_stdio.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Parses the location in memory pointed to by 'pc' and creates an Instruction
 *
 * @param mem - The byte array serving as system memory
 * @param pc - The program counter (points to the current instruction in memory)
 *
 * @returns An Instruction with all the information needed about it
 */
Instruction parseInstruction(uint8_t* mem, uint16_t pc) {
    Instruction instruction;
    char* err_msg;

    instruction.opcode = mem[pc];

    switch (instruction.opcode) {
        // ---------- ADC ----------
        case 0x69:  // nice
            instruction.name = "ADC";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0x65:
            instruction.name = "ADC";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x75:
            instruction.name = "ADC";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0x6D:
            instruction.name = "ADC";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0x7D:
            instruction.name = "ADC";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        case 0x79:
            instruction.name = "ADC";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        case 0x61:
            instruction.name = "ADC";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0x71:
            instruction.name = "ADC";
            instruction.addr_mode = INDY;
            instruction.cycles = 5;
            break;
        // ---------- AND ----------
        case 0x29:
            instruction.name = "AND";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0x25:
            instruction.name = "AND";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x35:
            instruction.name = "AND";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0x2D:
            instruction.name = "AND";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0x3D:
            instruction.name = "AND";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        case 0x39:
            instruction.name = "AND";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        case 0x21:
            instruction.name = "AND";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0x31:
            instruction.name = "AND";
            instruction.addr_mode = INDY;
            instruction.cycles = 5;
            break;
        // ---------- ASL ----------
        case 0x0A:
            instruction.name = "ASL";
            instruction.addr_mode = ACCUM;
            instruction.cycles = 2;
            break;
        case 0x06:
            instruction.name = "ASL";
            instruction.addr_mode = ZP;
            instruction.cycles = 5;
            break;
        case 0x16:
            instruction.name = "ASL";
            instruction.addr_mode = ZPX;
            instruction.cycles = 6;
            break;
        case 0x0E:
            instruction.name = "ASL";
            instruction.addr_mode = ABS;
            instruction.cycles = 6;
            break;
        case 0x1E:
            instruction.name = "ASL";
            instruction.addr_mode = ABSX;
            instruction.cycles = 7;
            break;
        // ---------- BCC ----------
        case 0x90:
            instruction.name = "BCC";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- BCS ----------
        case 0xB0:
            instruction.name = "BCS";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- BEQ ----------
        case 0xF0:
            instruction.name = "BEQ";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- BIT ----------
        case 0x24:
            instruction.name = "BIT";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x2C:
            instruction.name = "BIT";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        // ---------- BMI ----------
        case 0x30:
            instruction.name = "BMI";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- BNE ----------
        case 0xD0:
            instruction.name = "BNE";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- BPL ----------
        case 0x10:
            instruction.name = "BPL";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- BRK ----------
        case 0x00:
            instruction.name = "BRK";
            instruction.addr_mode = IMPL;
            instruction.cycles = 7;
            break;
        // ---------- BVC ----------
        case 0x50:
            instruction.name = "BVC";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- BVS ----------
        case 0x70:
            instruction.name = "BVS";
            instruction.addr_mode = REL;
            instruction.cycles = 2;
            break;
        // ---------- CLC ----------
        case 0x18:
            instruction.name = "CLC";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- CLD ----------
        case 0xD8:
            instruction.name = "CLD";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- CLI ----------
        case 0x58:
            instruction.name = "CLI";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- CLV ----------
        case 0xB8:
            instruction.name = "CLV";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- CMP ----------
        case 0xC9:
            instruction.name = "CMP";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0xC5:
            instruction.name = "CMP";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0xD5:
            instruction.name = "CMP";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0xCD:
            instruction.name = "CMP";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0xDD:
            instruction.name = "CMP";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        case 0xD9:
            instruction.name = "CMP";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        case 0xC1:
            instruction.name = "CMP";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0xD1:
            instruction.name = "CMP";
            instruction.addr_mode = INDY;
            instruction.cycles = 5;
            break;
        // ---------- CPX ----------
        case 0xE0:
            instruction.name = "CPX";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0xE4:
            instruction.name = "CPX";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0xEC:
            instruction.name = "CPX";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        // ---------- CPY ----------
        case 0xC0:
            instruction.name = "CPY";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0xC4:
            instruction.name = "CPY";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0xCC:
            instruction.name = "CPY";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        // ---------- DEC ----------
        case 0xC6:
            instruction.name = "DEC";
            instruction.addr_mode = ZP;
            instruction.cycles = 5;
            break;
        case 0xD6:
            instruction.name = "DEC";
            instruction.addr_mode = ZPX;
            instruction.cycles = 6;
            break;
        case 0xCE:
            instruction.name = "DEC";
            instruction.addr_mode = ABS;
            instruction.cycles = 6;
            break;
        case 0xDE:
            instruction.name = "DEC";
            instruction.addr_mode = ABSX;
            instruction.cycles = 7;
            break;
        // ---------- DEX ----------
        case 0xCA:
            instruction.name = "DEX";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- DEY ----------
        case 0x88:
            instruction.name = "DEY";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- EOR ----------
        case 0x49:
            instruction.name = "EOR";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0x45:
            instruction.name = "EOR";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x55:
            instruction.name = "EOR";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0x4D:
            instruction.name = "EOR";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0x5D:
            instruction.name = "EOR";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        case 0x59:
            instruction.name = "EOR";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        case 0x41:
            instruction.name = "EOR";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0x51:
            instruction.name = "EOR";
            instruction.addr_mode = INDY;
            instruction.cycles = 5;
            break;
        // ---------- INC ----------
        case 0xE6:
            instruction.name = "INC";
            instruction.addr_mode = ZP;
            instruction.cycles = 5;
            break;
        case 0xF6:
            instruction.name = "INC";
            instruction.addr_mode = ZPX;
            instruction.cycles = 6;
            break;
        case 0xEE:
            instruction.name = "INC";
            instruction.addr_mode = ABS;
            instruction.cycles = 6;
            break;
        case 0xFE:
            instruction.name = "INC";
            instruction.addr_mode = ABSX;
            instruction.cycles = 7;
            break;
        // ---------- INX ----------
        case 0xE8:
            instruction.name = "INX";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- INY ----------
        case 0xC8:
            instruction.name = "INY";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- JMP ----------
        case 0x4C:
            instruction.name = "JMP";
            instruction.addr_mode = ABS;
            instruction.cycles = 3;
            break;
        case 0x6C:
            instruction.name = "JMP";
            instruction.addr_mode = IND;
            instruction.cycles = 5;
            break;
        // ---------- JSR ----------
        case 0x20:
            instruction.name = "JSR";
            instruction.addr_mode = ABS;
            instruction.cycles = 6;
            break;
        // ---------- LDA ----------
        case 0xA9:
            instruction.name = "LDA";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0xA5:
            instruction.name = "LDA";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0xB5:
            instruction.name = "LDA";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0xAD:
            instruction.name = "LDA";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0xBD:
            instruction.name = "LDA";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        case 0xB9:
            instruction.name = "LDA";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        case 0xA1:
            instruction.name = "LDA";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0xB1:
            instruction.name = "LDA";
            instruction.addr_mode = INDY;
            instruction.cycles = 5;
            break;
        // ---------- LDX ----------
        case 0xA2:
            instruction.name = "LDX";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0xA6:
            instruction.name = "LDX";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0xB6:
            instruction.name = "LDX";
            instruction.addr_mode = ZPY;
            instruction.cycles = 4;
            break;
        case 0xAE:
            instruction.name = "LDX";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0xBE:
            instruction.name = "LDX";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        // ---------- LDY ----------
        case 0xA0:
            instruction.name = "LDY";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0xA4:
            instruction.name = "LDY";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0xB4:
            instruction.name = "LDY";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0xAC:
            instruction.name = "LDY";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0xBC:
            instruction.name = "LDY";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        // ---------- LSR ----------
        case 0x4A:
            instruction.name = "LSR";
            instruction.addr_mode = ACCUM;
            instruction.cycles = 2;
            break;
        case 0x46:
            instruction.name = "LSR";
            instruction.addr_mode = ZP;
            instruction.cycles = 5;
            break;
        case 0x56:
            instruction.name = "LSR";
            instruction.addr_mode = ZPX;
            instruction.cycles = 6;
            break;
        case 0x4E:
            instruction.name = "LSR";
            instruction.addr_mode = ABS;
            instruction.cycles = 6;
            break;
        case 0x5E:
            instruction.name = "LSR";
            instruction.addr_mode = ABSX;
            instruction.cycles = 7;
            break;
        // ---------- NOP ----------
        case 0xEA:
            instruction.name = "NOP";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- ORA ----------
        case 0x09:
            instruction.name = "ORA";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0x05:
            instruction.name = "ORA";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x15:
            instruction.name = "ORA";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0x0D:
            instruction.name = "ORA";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0x1D:
            instruction.name = "ORA";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        case 0x19:
            instruction.name = "ORA";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        case 0x01:
            instruction.name = "ORA";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0x11:
            instruction.name = "ORA";
            instruction.addr_mode = INDY;
            instruction.cycles = 5;
            break;
        // ---------- PHA ----------
        case 0x48:
            instruction.name = "PHA";
            instruction.addr_mode = IMPL;
            instruction.cycles = 3;
            break;
        // ---------- PHP ----------
        case 0x08:
            instruction.name = "PHP";
            instruction.addr_mode = IMPL;
            instruction.cycles = 3;
            break;
        // ---------- PLA ----------
        case 0x68:
            instruction.name = "PLA";
            instruction.addr_mode = IMPL;
            instruction.cycles = 4;
            break;
        // ---------- PLP ----------
        case 0x28:
            instruction.name = "PLP";
            instruction.addr_mode = IMPL;
            instruction.cycles = 4;
            break;
        // ---------- ROL ----------
        case 0x2A:
            instruction.name = "ROL";
            instruction.addr_mode = ACCUM;
            instruction.cycles = 2;
            break;
        case 0x26:
            instruction.name = "ROL";
            instruction.addr_mode = ZP;
            instruction.cycles = 5;
            break;
        case 0x36:
            instruction.name = "ROL";
            instruction.addr_mode = ZPX;
            instruction.cycles = 6;
            break;
        case 0x2E:
            instruction.name = "ROL";
            instruction.addr_mode = ABS;
            instruction.cycles = 6;
            break;
        case 0x3E:
            instruction.name = "ROL";
            instruction.addr_mode = ABSX;
            instruction.cycles = 7;
            break;
        // ---------- ROR ----------
        case 0x6A:
            instruction.name = "ROR";
            instruction.addr_mode = ACCUM;
            instruction.cycles = 2;
            break;
        case 0x66:
            instruction.name = "ROR";
            instruction.addr_mode = ZP;
            instruction.cycles = 5;
            break;
        case 0x76:
            instruction.name = "ROR";
            instruction.addr_mode = ZPX;
            instruction.cycles = 6;
            break;
        case 0x6E:
            instruction.name = "ROR";
            instruction.addr_mode = ABS;
            instruction.cycles = 6;
            break;
        case 0x7E:
            instruction.name = "ROR";
            instruction.addr_mode = ABSX;
            instruction.cycles = 7;
            break;
        // ---------- RTI ----------
        case 0x40:
            instruction.name = "RTI";
            instruction.addr_mode = IMPL;
            instruction.cycles = 6;
            break;
        // ---------- RTS ----------
        case 0x60:
            instruction.name = "RTS";
            instruction.addr_mode = IMPL;
            instruction.cycles = 6;
            break;
        // ---------- SBC ----------
        case 0xE9:
            instruction.name = "SBC";
            instruction.addr_mode = IMM;
            instruction.cycles = 2;
            break;
        case 0xE5:
            instruction.name = "SBC";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0xF5:
            instruction.name = "SBC";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0xED:
            instruction.name = "SBC";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0xFD:
            instruction.name = "SBC";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        case 0xF9:
            instruction.name = "SBC";
            instruction.addr_mode = ABSY;
            instruction.cycles = 4;
            break;
        case 0xE1:
            instruction.name = "SBC";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0xF1:
            instruction.name = "SBC";
            instruction.addr_mode = INDY;
            instruction.cycles = 5;
            break;
        // ---------- SEC ----------
        case 0x38:
            instruction.name = "SEC";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- SED ----------
        case 0xF8:
            instruction.name = "SED";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- SEI ----------
        case 0x78:
            instruction.name = "SEI";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- STA ----------
        case 0x85:
            instruction.name = "STA";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x95:
            instruction.name = "STA";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0x8D:
            instruction.name = "STA";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        case 0x9D:
            instruction.name = "STA";
            instruction.addr_mode = ABSX;
            instruction.cycles = 5;
            break;
        case 0x99:
            instruction.name = "STA";
            instruction.addr_mode = ABSY;
            instruction.cycles = 5;
            break;
        case 0x81:
            instruction.name = "STA";
            instruction.addr_mode = INDX;
            instruction.cycles = 6;
            break;
        case 0x91:
            instruction.name = "STA";
            instruction.addr_mode = INDY;
            instruction.cycles = 6;
            break;
        // ---------- STX ----------
        case 0x86:
            instruction.name = "STX";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x96:
            instruction.name = "STX";
            instruction.addr_mode = ZPY;
            instruction.cycles = 4;
            break;
        case 0x8E:
            instruction.name = "STX";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        // ---------- STY ----------
        case 0x84:
            instruction.name = "STY";
            instruction.addr_mode = ZP;
            instruction.cycles = 3;
            break;
        case 0x94:
            instruction.name = "STY";
            instruction.addr_mode = ZPX;
            instruction.cycles = 4;
            break;
        case 0x8C:
            instruction.name = "STY";
            instruction.addr_mode = ABS;
            instruction.cycles = 4;
            break;
        // ---------- TAX ----------
        case 0xAA:
            instruction.name = "TAX";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- TAY ----------
        case 0xA8:
            instruction.name = "TAY";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- TSX ----------
        case 0xBA:
            instruction.name = "TSX";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- TXA ----------
        case 0x8A:
            instruction.name = "TXA";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- TXS ----------
        case 0x9A:
            instruction.name = "TXS";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- TYA ----------
        case 0x98:
            instruction.name = "TYA";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;

        // ----------- Unofficial Opcodes ----------
        //
        //  I'll be implementing these as necessary
        // to get my target games working. Otherwise
        //   implementing these opcodes is out of
        //          scope for this project
        //
        // -----------------------------------------

        // ---------- NOP ($1A) ----------
        case 0x1A:
            instruction.name = "NOP ($1A)";
            instruction.addr_mode = IMPL;
            instruction.cycles = 2;
            break;
        // ---------- NOP ($1C) ----------
        case 0x1C:
            instruction.name = "NOP ($1C)";
            instruction.addr_mode = ABSX;
            instruction.cycles = 4;
            break;
        // ---------- SLO ($1F) ----------
        case 0x1F:
            instruction.name = "SLO";
            instruction.addr_mode = ABSX;
            instruction.cycles = 7;
            break;
        default:
            asprintf(&err_msg, "$%04x: Invalid opcode 0x%02x", pc, instruction.opcode);
            printLog("CPU", err_msg, "WARNING");
            break;
    }

    switch (instruction.addr_mode) {
        case IMPL:
            instruction.length = 1;
            break;
        case ACCUM:
            instruction.length = 1;
            break;
        case IMM:
            instruction.length = 2;
            instruction.imm = mem[pc + 1];
            break;
        case ZP:
            instruction.length = 2;
            instruction.addr = mem[pc + 1];
            break;
        case ZPX:
            instruction.length = 2;
            instruction.addr = mem[pc + 1];
            break;
        case ZPY:
            instruction.length = 2;
            instruction.addr = mem[pc + 1];
            break;
        case REL:
            instruction.length = 2;
            instruction.offset = mem[pc + 1];
            break;
        case ABS:
            instruction.length = 3;
            instruction.addr = concatenateBytes(mem[pc + 2], mem[pc + 1]);
            break;
        case ABSX:
            instruction.length = 3;
            instruction.addr = concatenateBytes(mem[pc + 2], mem[pc + 1]);
            break;
        case ABSY:
            instruction.length = 3;
            instruction.addr = concatenateBytes(mem[pc + 2], mem[pc + 1]);
            break;
        case IND:
            instruction.length = 3;
            instruction.addr = concatenateBytes(mem[pc + 2], mem[pc + 1]);
            break;
        case INDX:
            instruction.length = 2;
            instruction.addr = mem[pc + 1];
            break;
        case INDY:
            instruction.length = 2;
            instruction.addr = mem[pc + 1];
            break;
    }

    return instruction;
}

/**
 * Execute the given instruction and set the appropriate flags
 *
 * @param instr - The instruction to execute
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
void executeInstruction(Instruction instr, uint8_t** mem, Processor* processor) {
    // Some variables that may or may not be used for some instructions
    int16_t val;
    uint8_t result;
    uint8_t operand;
    uint16_t irq_vector;

    switch (instr.opcode) {
        // ---------- ADC ----------
        case 0x69:  // Immediate
        case 0x65:  // Zero Page
        case 0x75:  // Zero Page X
        case 0x6D:  // Absolute
        case 0x7D:  // Absolute X
        case 0x79:  // Absolute Y
        case 0x61:  // Indirect X (Indexed Indirect)
        case 0x71:  // Indirect Y (Indirect Indexed)
            val = processor->A + getVal(instr, *mem, *processor) + getFlag('C', processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Carry" flag
            setFlag('C', val > 0xFF, processor);

            // Set the "Overflow" flag
            operand = getVal(instr, *mem, *processor);
            setFlag('V', ((processor->A ^ result) & 0x80) && !((processor->A ^ operand) & 0x80),
                    processor);

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->A = result;
            break;
        // ---------- AND ----------
        case 0x29:  // Immediate
        case 0x25:  // Zero Page
        case 0x35:  // Zero Page X
        case 0x2D:  // Absolute
        case 0x3D:  // Absolute X
        case 0x39:  // Absolute Y
        case 0x21:  // Indirect X
        case 0x31:  // Indirect Y
            val = processor->A & getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            uint8_t result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->A = result;
            break;
        // ---------- ASL ----------
        case 0x0A:  // Accumulator
            // Set the "Carry" flag
            setFlag('C', (processor->A & 0x80) >> 7, processor);

            processor->A <<= 1;

            // Set the "Zero" flag
            setFlag('Z', processor->A == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->A & 0x80) >> 7, processor);

            break;
        case 0x06:  // Zero Page
        case 0x16:  // Zero Page X
        case 0x0E:  // Absolute
        case 0x1E:  // Absolute X
            val = getVal(instr, *mem, *processor);

            // Set the "Carry" flag
            setFlag('C', (val & 0x80) >> 7, processor);

            val <<= 1;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            (*mem)[getAddr(instr, *mem, *processor)] = result;
            break;
        // ---------- BCC ----------
        case 0x90:  // Relative
            if (getFlag('C', processor) == 0) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- BCS ----------
        case 0xB0:  // Relative
            if (getFlag('C', processor) == 1) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- BEQ ----------
        case 0xF0:  // Relative
            if (getFlag('Z', processor) == 1) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- BIT ----------
        case 0x24:  // Zero Page
        case 0x2C:  // Absolute
            val = processor->A & getVal(instr, *mem, *processor);

            // Set the "Zero" flag
            setFlag('Z', val == 0, processor);

            // Set the "Overflow" flag
            setFlag('V', (val >> 6) & 1, processor);

            // Set the "Negative" flag
            setFlag('N', (val >> 7) & 1, processor);
            break;
        // ---------- BMI ----------
        case 0x30:  // Relative
            if (getFlag('N', processor) == 1) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- BNE ----------
        case 0xD0:
            if (getFlag('Z', processor) == 0) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- BPL ----------
        case 0x10:
            if (getFlag('N', processor) == 0) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- BRK ----------
        case 0x00:  // Implied
            // Push the program counter onto the stack
            stackPush((processor->PC >> 8) & 0xFF, mem, processor);
            stackPush((processor->PC + 2) & 0xFF, mem, processor);

            // Set the "Break" flag and push the processor status onto the stack
            setFlag('B', 1, processor);
            stackPush(processor->P, mem, processor);

            // Set the program counter to the IRQ interrupt vector
            irq_vector = (*mem)[0xFFFE] | ((*mem)[0xFFFF] << 8);
            if (irq_vector == 0x0000) {
                processor->halted = true;
            }
            processor->PC = irq_vector - instr.length;
            break;
        // ---------- BVC ----------
        case 0x50:  // Relative
            if (getFlag('V', processor) == 0) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- BVS ----------
        case 0x70:  // Relative
            if (getFlag('V', processor) == 1) {
                processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            }
            break;
        // ---------- CLC ----------
        case 0x18:  // Implied
            setFlag('C', 0, processor);
            break;
        // ---------- CLD ----------
        case 0xD8:  // Implied
            setFlag('D', 0, processor);
            break;
        // ---------- CLI ----------
        case 0x58:  // Implied
            setFlag('I', 0, processor);
            break;
        // ---------- CLV ----------
        case 0xB8:  // Implied
            setFlag('V', 0, processor);
            break;
        // ---------- CMP ----------
        case 0xC9:  // Immediate
        case 0xC5:  // Zero Page
        case 0xD5:  // Zero Page X
        case 0xCD:  // Absolute
        case 0xDD:  // Absolute X
        case 0xD9:  // Absolute Y
        case 0xC1:  // Indirect X
        case 0xD1:  // Indirect Y
            val = processor->A - getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Carry" flag
            setFlag('C', val >= 0, processor);

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            break;
        // ---------- CPX ----------
        case 0xE0:  // Immediate
        case 0xE4:  // Zero Page
        case 0xEC:  // Absolute
            val = processor->X - getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Carry" flag
            setFlag('C', val >= 0, processor);

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            break;
        // ---------- CPY ----------
        case 0xC0:  // Immediate
        case 0xC4:  // Zero Page
        case 0xCC:  // Absolute
            val = processor->Y - getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Carry" flag
            setFlag('C', val >= 0, processor);

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            break;
        // ---------- DEC ----------
        case 0xC6:  // Zero Page
        case 0xD6:  // Zero Page X
        case 0xCE:  // Absolute
        case 0xDE:  // Absolute X
            val = getVal(instr, *mem, *processor);
            val--;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            (*mem)[getAddr(instr, *mem, *processor)] = result;
            break;
        // ---------- DEX ----------
        case 0xCA:  // Implied
            processor->X--;

            // Set the "Zero" flag
            setFlag('Z', processor->X == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->X & 0x80) >> 7, processor);

            break;
        // ---------- DEY ----------
        case 0x88:  // Implied
            processor->Y--;

            // Set the "Zero" flag
            setFlag('Z', processor->Y == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->Y & 0x80) >> 7, processor);

            break;
        // ---------- EOR ----------
        case 0x49:  // Immediate
        case 0x45:  // Zero Page
        case 0x55:  // ZEro Page X
        case 0x4D:  // Absolute
        case 0x5D:  // Absolute X
        case 0x59:  // Absolute Y
        case 0x41:  // Indirect X
        case 0x51:  // Indirect Y
            val = processor->A ^ getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->A = result;
            break;
        // ---------- INC ----------
        case 0xE6:  // Zero Page
        case 0xF6:  // Zero Page X
        case 0xEE:  // Absolute
        case 0xFE:  // Absolute X
            val = getVal(instr, *mem, *processor);
            val++;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            (*mem)[getAddr(instr, *mem, *processor)] = result;
            break;
        // ---------- INX ----------
        case 0xE8:  // Implied
            processor->X++;

            // Set the "Zero" flag
            setFlag('Z', processor->X == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->X & 0x80) >> 7, processor);

            break;
        // ---------- INY ----------
        case 0xC8:  // Implied
            processor->Y++;

            // Set the "Zero" flag
            setFlag('Z', processor->Y == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->Y & 0x80) >> 7, processor);

            break;
        // ---------- JMP ----------
        case 0x4C:  // Absolute
        case 0x6C:  // Indirect
            // Subtracting length to make sure that the PC being incremented
            // doesn't skip the next instruction
            processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            break;
        // ---------- JSR ----------
        case 0x20:  // Absolute
            // Push the most significant byte first, then the least significant
            stackPush((processor->PC >> 8) & 0xFF, mem, processor);
            stackPush((processor->PC + 2) & 0xFF, mem, processor);

            processor->PC = getAddr(instr, *mem, *processor) - instr.length;
            break;
        // ---------- LDA ----------
        case 0xA9:  // Immediate
        case 0xA5:  // Zero Page
        case 0xB5:  // Zero Page X
        case 0xAD:  // Absolute
        case 0xBD:  // Absolute X
        case 0xB9:  // Absolute Y
        case 0xA1:  // Indirect X (Indexed Indirect)
        case 0xB1:  // Indirect Y (Indirect Indexed)
            val = getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative flag"
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->A = result;
            break;
        // ---------- LDX ----------
        case 0xA2:  // Immediate
        case 0xA6:  // Zero Page
        case 0xB6:  // Zero Page Y
        case 0xAE:  // Absolute
        case 0xBE:  // Absolute Y
            val = getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative flag"
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->X = result;
            break;
        // ---------- LDY ----------
        case 0xA0:  // Immediate
        case 0xA4:  // Zero Page
        case 0xB4:  // Zero Page X
        case 0xAC:  // Absolute
        case 0xBC:  // Absolute X
            val = getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative flag"
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->Y = result;
            break;
        // ---------- LSR ----------
        case 0x4A:  // Accumulator
            // Set the "Carry" flag
            setFlag('C', (processor->A & 0x01), processor);

            processor->A >>= 1;

            // Set the "Zero" flag
            setFlag('Z', processor->A == 0, processor);
            break;
        case 0x46:  // Zero Page
        case 0x56:  // Zero Page X
        case 0x4E:  // Absolute
        case 0x5E:  // Absolute X
            val = getVal(instr, *mem, *processor);

            // Set the "Carry" flag
            setFlag('C', (val & 0x01), processor);

            val >>= 1;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('Z', (result & 0x80) >> 7, processor);

            (*mem)[getAddr(instr, *mem, *processor)] = result;
            break;
        // ---------- NOP ----------
        case 0xEA:  // Implied
            break;
        // ---------- ORA ----------
        case 0x09:  // Immediate
        case 0x05:  // Zero Page
        case 0x15:  // Zero Page X
        case 0x0D:  // Absolute
        case 0x1D:  // Absolute X
        case 0x19:  // Absolute Y
        case 0x01:  // Indirect X
        case 0x11:  // Indirect Y
            val = processor->A | getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->A = result;
            break;
        // ---------- PHA ----------
        case 0x48:  // Implied
            stackPush(processor->A, mem, processor);
            break;
        // ---------- PHP ----------
        case 0x08:  // Implied
            stackPush(processor->P, mem, processor);
            break;
        // ---------- PLA ----------
        case 0x68:  // Implied
            processor->A = stackPull(mem, processor);

            // Set the "Zero" flag
            setFlag('Z', processor->A == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->A & 0x80) >> 7, processor);
            break;
        // ---------- PLP ----------
        case 0x28:  // Implied
            processor->P = stackPull(mem, processor);
            break;
        // ---------- ROL ----------
        case 0x2A:  // Accumulator
            val = processor->A;
            val <<= 1;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set bit 0 to the current value of the "Carry" flag
            if (getFlag('C', processor) == 1) {
                result |= 0b00000001;
            }

            // Set the "Carry" flag to the old bit 7
            setFlag('C', (val >> 8) & 0x01, processor);

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result >> 7) & 0x01, processor);

            processor->A = result;
            break;
        case 0x26:  // Zero Page
        case 0x36:  // Zero Page X
        case 0x2E:  // Absolute
        case 0x3E:  // Absolute X
            val = getVal(instr, *mem, *processor);
            val <<= 1;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set bit 0 to the current value of the "Carry" flag
            if (getFlag('C', processor) == 1) {
                result |= 0b00000001;
            }

            // Set the "Carry" flag to the old bit 7
            setFlag('C', (val >> 8) & 0x01, processor);

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result >> 7) & 0x01, processor);

            (*mem)[getAddr(instr, *mem, *processor)] = result;
            break;
        // ---------- ROR ----------
        case 0x6A:  // Accumulator
            val = processor->A;

            // Set new bit 7 to the current value of the "Carry" flag
            if (getFlag('C', processor) == 1) {
                val |= 0b100000000;
            }

            // Set the "Carry" flag to the old bit 0
            setFlag('C', val & 0x01, processor);

            val >>= 1;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result >> 7) & 0x01, processor);

            processor->A = result;
            break;
        case 0x66:  // Zero Page
        case 0x76:  // Zero Page X
        case 0x6E:  // Absolute
        case 0x7E:  // Absolute X
            val = getVal(instr, *mem, *processor);

            // Set new bit 7 to the current value of the "Carry" flag
            if (getFlag('C', processor) == 1) {
                val |= 0b100000000;
            }

            // Set the "Carry" flag to the old bit 0
            setFlag('C', val & 0x01, processor);

            val >>= 1;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result >> 7) & 0x01, processor);

            processor->A = result;
            (*mem)[getAddr(instr, *mem, *processor)] = result;
            break;
        // ---------- RTI ----------
        case 0x40:  // Implied
            // Pull status flags
            processor->P = stackPull(mem, processor);

            // Pull program counter (lsb first, then msb)
            val = stackPull(mem, processor);
            val = concatenateBytes(stackPull(mem, processor), val);

            processor->PC = val - instr.length;
            break;
        // ---------- RTS ----------
        case 0x60:  // Implied
            // Pull the least significant byte first, the the most significant
            val = stackPull(mem, processor);
            val = concatenateBytes(stackPull(mem, processor), val);

            processor->PC = val;
            break;
        // ---------- SBC ----------
        case 0xE9:  // Immediate
        case 0xE5:  // Zero Page
        case 0xF5:  // Zero Page X
        case 0xED:  // Absolute
        case 0xFD:  // Absolute X
        case 0xF9:  // Absolute Y
        case 0xE1:  // Indirect X
        case 0xF1:  // Indirect Y
            val = processor->A - getVal(instr, *mem, *processor) - (1 - getFlag('C', processor));

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Carry" flag
            setFlag('C', val >= 0, processor);

            // Set the "Overflow" flag
            operand = getVal(instr, *mem, *processor);
            setFlag('V', ((processor->A ^ result) & 0x80) && ((processor->A ^ operand) & 0x80),
                    processor);

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->A = result;
            break;
        // ---------- SEC ----------
        case 0x38:
            setFlag('C', 1, processor);
            break;
        // ---------- SED ----------
        case 0xF8:
            setFlag('D', 1, processor);
            break;
        // ---------- SEI ----------
        case 0x78:
            setFlag('I', 1, processor);
            break;
        // ---------- STA ----------
        case 0x85:  // Zero Page
        case 0x95:  // Zero Page X
        case 0x8D:  // Absolute
        case 0x9D:  // Absolute X
        case 0x99:  // Absolute Y
        case 0x81:  // Indirect X
        case 0x91:  // Indirect Y
            (*mem)[getAddr(instr, *mem, *processor)] = processor->A;
            break;
        // ---------- STX ----------
        case 0x86:  // Zero Page
        case 0x96:  // Zero Page Y
        case 0x8E:  // Absolute
            (*mem)[getAddr(instr, *mem, *processor)] = processor->X;
            break;
        // ---------- STY ----------
        case 0x84:  // Zero Page
        case 0x94:  // Zero Page X
        case 0x8C:  // Absolute
            (*mem)[getAddr(instr, *mem, *processor)] = processor->Y;
            break;
        // ---------- TAX ----------
        case 0xAA:  // Implied
            processor->X = processor->A;

            // Set the "Zero" flag
            setFlag('Z', processor->X == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->X & 0x80) >> 7, processor);
            break;
        // ---------- TAY ----------
        case 0xA8:  // Implied
            processor->Y = processor->A;

            // Set the "Zero" flag
            setFlag('Z', processor->Y == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->Y & 0x80) >> 7, processor);
            break;
        // ---------- TSX ----------
        case 0xBA:  // Implied
            processor->X = processor->S;

            // Set the "Zero" flag
            setFlag('Z', processor->X == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->X & 0x80) >> 7, processor);
            break;
        // ---------- TXA ----------
        case 0x8A:  // Implied
            processor->A = processor->X;

            // Set the "Zero" flag
            setFlag('Z', processor->A == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->A & 0x80) >> 7, processor);
            break;
        // ---------- TXS ----------
        case 0x9A:  // Implied
            processor->S = processor->X;
            break;
        // ---------- TYA ----------
        case 0x98:  // Implied
            processor->A = processor->Y;

            // Set the "Zero" flag
            setFlag('Z', processor->A == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (processor->A & 0x80) >> 7, processor);
            break;

        // ----------- Unofficial Opcodes ----------
        //
        //  I'll be implementing these as necessary
        // to get my target games working. Otherwise
        //   implementing these opcodes is out of
        //          scope for this project
        //
        // -----------------------------------------

        // ---------- NOP ($1A) ----------
        case 0x1A:  // Implied
            break;
        // ---------- NOP ($1C) ----------
        case 0x1C:
            break;
        // ---------- SLO ($1F) ----------
        case 0x1F:  // Absolute X
            // Essentially a combination of 2 instructions
            // Basically an ASL followed by an ORA

            // ----- ASL -----

            val = getVal(instr, *mem, *processor);

            // Set the "Carry" flag
            setFlag('C', (val & 0x80) >> 7, processor);

            val <<= 1;

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            (*mem)[getAddr(instr, *mem, *processor)] = result;

            // ----- ORA -----

            val = processor->A | getVal(instr, *mem, *processor);

            // Wrap val to 8 bits
            result = val & 0xFF;

            // Set the "Zero" flag
            setFlag('Z', result == 0, processor);

            // Set the "Negative" flag
            setFlag('N', (result & 0x80) >> 7, processor);

            processor->A = result;
            break;
    }

    processor->P |= 0x20;  // Making sure that the unused bit ALWAYS remains 1
}

/**
 * Set the specified flag to the specified value
 *
 * @param flag - The flag to set (N, V, B, D, I, Z, C)
 * @param val - The bit value to set the given flag to
 */
void setFlag(char flag, bool val, Processor* processor) {
    char* err_msg;

    switch (flag) {
        case 'N':
            processor->P = (processor->P & ~0b10000000) | (val ? 0b10000000 : 0);
            break;
        case 'V':
            processor->P = (processor->P & ~0b01000000) | (val ? 0b01000000 : 0);
            break;
        case 'B':
            processor->P = (processor->P & ~0b00010000) | (val ? 0b00010000 : 0);
            break;
        case 'D':
            processor->P = (processor->P & ~0b00001000) | (val ? 0b00001000 : 0);
            break;
        case 'I':
            processor->P = (processor->P & ~0b00000100) | (val ? 0b00000100 : 0);
            break;
        case 'Z':
            processor->P = (processor->P & ~0b00000010) | (val ? 0b00000010 : 0);
            break;
        case 'C':
            processor->P = (processor->P & ~0b00000001) | (val ? 0b00000001 : 0);
            break;
        default:
            asprintf(&err_msg, "Invalid flag %c", flag);
            printLog("CPU", err_msg, "ERROR");
            break;
    }
}

/**
 * Return the value of the given status flag
 *
 * @param flag - The flag to return the status of (N, V, B, D, I, Z, C)
 * @param processor - The processor holding register values
 *
 * @returns The value of the given flag
 */
bool getFlag(char flag, Processor* processor) {
    bool val = 0;
    char* err_msg;

    switch (flag) {
        case 'N':
            val = (processor->P >> 7) & 1;
            break;
        case 'V':
            val = (processor->P >> 6) & 1;
            break;
        case 'B':
            val = (processor->P >> 4) & 1;
            break;
        case 'D':
            val = (processor->P >> 3) & 1;
            break;
        case 'I':
            val = (processor->P >> 2) & 1;
            break;
        case 'Z':
            val = (processor->P >> 1) & 1;
            break;
        case 'C':
            val = processor->P & 1;
            break;
        default:
            asprintf(&err_msg, "Invalid flag %c", flag);
            printLog("CPU", err_msg, "ERROR");
            break;
    }

    return val;
}

/**
 * Return the required value based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The byte of data required by the instruction
 */
uint8_t getVal(Instruction instr, uint8_t* mem, Processor processor) {
    uint8_t val = 0;
    uint8_t lsb_addr;
    uint8_t msb_addr;

    switch (instr.addr_mode) {
        case IMPL:
            break;
        case ACCUM:
            val = processor.A;
            break;
        case IMM:
            val = instr.imm;
            break;
        case ZP:
            val = mem[instr.addr];
            break;
        case ZPX:
            val = mem[instr.addr + processor.X];
            break;
        case ZPY:
            val = mem[instr.addr + processor.Y];
            break;
        case ABS:
            val = mem[instr.addr];
            break;
        case ABSX:
            val = mem[instr.addr + processor.X];
            break;
        case ABSY:

            val = mem[instr.addr + processor.Y];
            break;
        case INDX:
            lsb_addr = mem[instr.addr + processor.X];
            msb_addr = mem[instr.addr + processor.X + 1];
            val = mem[concatenateBytes(msb_addr, lsb_addr)];
            break;
        case INDY:
            lsb_addr = mem[instr.addr];
            msb_addr = mem[instr.addr + 1];
            val = mem[concatenateBytes(msb_addr, lsb_addr) + processor.Y];
            break;
        default:
            printLog("CPU", "Addressing mode doesn't return a value", "ERROR");
            break;
    }

    return val;
}

/**
 * Return the required memory address based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The memory address required by the instruction
 */
uint16_t getAddr(Instruction instr, uint8_t* mem, Processor processor) {
    uint16_t addr = 0;
    uint8_t lsb_addr;
    uint8_t msb_addr;

    switch (instr.addr_mode) {
        case IMPL:
            break;
        case ACCUM:
            break;
        case ZP:
            addr = instr.addr;
            break;
        case ZPX:
            addr = instr.addr + processor.X;
            break;
        case ZPY:
            addr = instr.addr + processor.Y;
            break;
        case REL:
            addr = instr.offset + processor.PC;
            break;
        case ABS:
            addr = instr.addr;
            break;
        case ABSX:
            addr = instr.addr + processor.X;
            break;
        case ABSY:
            addr = instr.addr + processor.Y;
            break;
        case IND:
            lsb_addr = mem[instr.addr];
            msb_addr = mem[instr.addr + 1];
            addr = concatenateBytes(msb_addr, lsb_addr);
            break;
        case INDX:
            lsb_addr = mem[instr.addr + processor.X];
            msb_addr = mem[instr.addr + processor.X + 1];
            addr = concatenateBytes(msb_addr, lsb_addr);
            break;
        case INDY:
            lsb_addr = mem[instr.addr];
            msb_addr = mem[instr.addr + 1];
            addr = concatenateBytes(msb_addr, lsb_addr) + processor.Y;
            break;
        default:
            printLog("CPU", "Instruction doesn't support that addressing mode", "ERROR");
            break;
    }

    return addr;
}

/**
 * Push the given value onto the stack (0x01FF - 0x0100)
 *
 * @param val - The value to be pushed onto the stack
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
void stackPush(uint8_t val, uint8_t** mem, Processor* processor) {
    (*mem)[0x0100 + processor->S] = val;
    processor->S--;
}

/**
 * Return and remove the value at the top of the stack
 *
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
uint8_t stackPull(uint8_t** mem, Processor* processor) {
    processor->S++;
    uint8_t val = (*mem)[0x0100 + processor->S];
    return val;
}

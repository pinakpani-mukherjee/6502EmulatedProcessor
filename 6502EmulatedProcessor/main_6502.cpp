#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>

using Byte = unsigned char;
using Word = unsigned short;

using  u32 = unsigned int;

struct Mem {
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];
	void Initialize() {
		for (u32 i = 0; i < MAX_MEM; i++) {
			Data[i] = 0;
		}
	}
	// basically a read byte function
	Byte operator[](u32 Address) const {
		// assert that Address is < MAX_MEM to prevent overflow errors
		assert(Address < MAX_MEM);
		
		return Data[Address];
	}

	Byte& operator[](u32 Address) {
		// assert that Address is < MAX_MEM to prevent overflow errors
		assert(Address < MAX_MEM);

		return Data[Address];
	}
	/* Write 2 bytes*/
	void WriteWord(Word Value, u32 Address, u32& Cycles ) {
		Data[Address] = Value & 0xFF;
		Data[Address + 1] = (Value >>8);
		Cycles -= 2;
	}
};

struct CPU {


	Word PC; // Program Counter 16 bit
	Word SP; // Stack Pointer 16 bit
	

	Byte A,X,Y; // Registers 8 bit

	Byte C : 1; // Carry Flag
	Byte Z : 1; // Zero Flag
	Byte I : 1; // Interrupt Disable
	Byte D : 1; // Decimal Mode
	Byte B : 1; // Break Command
	Byte V : 1; // Overflow Flag
	Byte N : 1; // Negative Flag

	void Reset(Mem& memory) {
		PC = 0xFFFC; // Set Program Counter
		SP = 0x0100; // Set Stack pointer
		C = Z = I = D = B = V = N = 0; // Clear all bit values
		A = X = Y = 0; // Reset Registers
		memory.Initialize(); // Initialize total memory for CPU
	}
	Byte FetchByte(u32& Cycles,Mem& memory) {
		Byte Data = memory[PC];
		PC++;
		Cycles--;
		return Data;
	}
	Word FetchWord(u32& Cycles, Mem& memory) {

		// 6502 is little Endian
		Word Data = memory[PC];
		PC++;

		Data |= (memory[PC] << 8);
		PC++;

		Cycles -= 2;


		return Data;
	}
	Byte ReadByte(u32& Cycles, Byte Address, Mem& memory) {
		Byte Data = memory[Address];
		Cycles--;
		return Data;
	}
	// opcodes
	static constexpr Byte
		INS_LDA_IM = 0xA9, // Load A Register Immediate 
		INS_LDA_ZP = 0xA5, // Load A Register Zero Page
		INS_LDA_ZPX = 0xB5, // Load A Register Zero Page, with X
		INS_JSR = 0x20; // Jump to Subroutine

	void LDASetStatus() {
		Z = (A == 0);
		N = (A & 0b10000000) > 0;
	}

	void Execute(u32 Cycles,Mem& memory) {
		while (Cycles > 0) {
			Byte Instruction = FetchByte(Cycles, memory);
			switch (Instruction) {
			case INS_LDA_IM: {
				Byte Value = FetchByte(Cycles, memory);
				A = Value;
				LDASetStatus();
			} break;
			case INS_LDA_ZP: {
				Byte ZeroPageAddress = FetchByte(Cycles, memory);
				A = ReadByte(Cycles,ZeroPageAddress,memory);
				LDASetStatus();
			} break;
			case INS_LDA_ZPX: {
				Byte ZeroPageAddress = FetchByte(Cycles, memory);
				ZeroPageAddress += X;
				Cycles--;
				A = ReadByte(Cycles, ZeroPageAddress, memory);
				LDASetStatus();
			} break;
			case INS_JSR: {
				Word JmpSubroutineAddress = FetchWord(Cycles, memory);
				memory.WriteWord(PC - 1, SP, Cycles);
				SP += 1;
				PC = JmpSubroutineAddress;
				Cycles--;
			} break;
			default: {
				std::cout << "Instruction not handled " << Instruction << std::endl;
			}break;
			}
		}
	}

};

int main() { 

	Mem mem;
	CPU cpu;

	cpu.Reset(mem);
	// start - manually inlining program into memory
	mem[0xFFFC] = CPU::INS_JSR;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;
	mem[0x4242] = CPU::INS_LDA_IM;
	mem[0x4243] = 0x69;
	// end - manually inlining program into memory
	cpu.Execute(9,mem);

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

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
	Byte FetchByte(u32& Cycles, Mem& memory) {
		Byte Data = memory[PC];
		PC++;
		Cycles--;
		return Data;
	}

	void Execute(u32 Cycles,Mem& memory) {
		while (Cycles > 0) {
			Byte Instruction = FetchByte(Cycles, memory);
		}
	}

};

int main() {

	Mem mem;
	CPU cpu;

	cpu.Reset(mem);
	cpu.Execute(2,mem);

	return 0;
}
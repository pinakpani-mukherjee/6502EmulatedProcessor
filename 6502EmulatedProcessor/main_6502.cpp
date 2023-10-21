#include <stdio.h>
#include <stdlib.h>

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

};

int main() {

	Mem mem;
	CPU cpu;

	cpu.Reset(mem);

	return 0;
}
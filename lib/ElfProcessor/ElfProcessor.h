//
// Created by Synergiance on 12/10/22.
//

#pragma once

#include <vector>
#include <string>

namespace Syn::Elf {

	enum Command {
		noop, addx
	};

	struct Instruction {
		Command command;
		int operand;
	};

	class Processor {
	public:
		[[nodiscard]] int GetRegisterX() const;
		size_t GetInstructionCount();

		Processor();
		void AddInstruction(Instruction &&instruction);
		static Instruction DecodeInstruction(const std::string &instruction);
		void Cycle();
		void Reset();
	private:
		int mRegisterX;
		std::vector<Instruction> mInstructions;
		int mCurrentCycle;

		static int GetNumCycles(Command &command);
		void AdvanceInstruction();
	};

} // Syn

//
// Created by Synergiance on 12/10/22.
//

#pragma once

#include <vector>
#include <string>

namespace Syn::Elf {

	enum Command {
		none, noop, addx
	};

	struct Instruction {
		Command command;
		int operand;
	};

	class Processor {
	public:
		[[nodiscard]] int GetRegisterX() const;
		size_t GetInstructionCount();

		void AddInstruction(Instruction &&instruction);
		static Instruction DecodeInstruction(const std::string &instruction);
		void Cycle();
	private:
		int mRegisterX;
		Instruction mCurrentInstruction;
		std::vector<Instruction> mInstructions;
		int mCurrentCycle;

		static int GetNumCycles(Command &command);
	};

} // Syn

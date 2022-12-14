//
// Created by Synergiance on 12/10/22.
//

#include "ElfProcessor.h"
#include <Parser.h>

namespace Syn::Elf {
	int Processor::GetRegisterX() const {
		return mRegisterX;
	}

	size_t Processor::GetInstructionCount() {
		return mInstructions.size();
	}

	void Processor::AddInstruction(Instruction &&instruction) {
		mInstructions.emplace_back(instruction);
	}

	Instruction Processor::DecodeInstruction(const std::string &instruction) {
		std::vector<std::string> tokens;
		Parser::Tokenize(instruction, tokens);

		Instruction inst{noop, 0};
		if (tokens.size() < 2 || tokens[0] == "noop") return inst;

		inst.command = addx;
		inst.operand = atoi(tokens[1].c_str());

		return inst;
	}

	void Processor::Cycle() {
		if (mInstructions.empty()) {
			AdvanceInstruction();
			return;
		}

		if (++mCurrentCycle < GetNumCycles(mInstructions[0].command)) return;

		switch (mInstructions[0].command) {
			case addx:
				mRegisterX += mInstructions[0].operand;
				break;
			default:
				break;
		}

		AdvanceInstruction();
	}

	int Processor::GetNumCycles(Command &command) {
		switch (command) {
			case noop:
				return 1;
			case addx:
				return 2;
			default:
				return 0;
		}
	}

	Processor::Processor() {
		mRegisterX = 1;
		mCurrentCycle = 0;
	}

	void Processor::AdvanceInstruction() {
		mCurrentCycle = 0;
		mInstructions.erase(mInstructions.cbegin());
	}

	void Processor::Reset() {
		mRegisterX = 1;
		mCurrentCycle = 0;
	}
} // Syn
//
// Created by Synergiance on 12/1/22.
//

#pragma once

#include <iostream>
#include <fstream>

class Input {
public:
	Input(const char* fileName);
	~Input();
	void Close();
	bool IsOpen();
	bool ReachedEnd();
};

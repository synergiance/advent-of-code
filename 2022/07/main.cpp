#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

#define MAX_FILE_SZ 100000
#define TOTAL_DISK_SPACE 70000000
#define NEEDED_SPACE 30000000

class File {
private:
	std::string name;
	int size;
	bool isDirectory;
	std::vector<File> children;
	File *parent;
public:
	File(std::string name, File *parent, bool isDirectory, int size = 0) {
		this->size = size;
		this->isDirectory = isDirectory;
		this->name = std::move(name);
		this->parent = parent;
	}

	int getSize() {
		if (!isDirectory) return size;

		size = 0;
		for (File child : children)
			size += child.getSize();

		return size;
	}

	bool IsDirectory() {
		return isDirectory;
	}

	File *getChild(const std::string& fileName) {
		for (File &child : children)
			if (child.name == fileName)
				return &child;

		return nullptr;
	}

	std::vector<File> &getChildren() {
		return children;
	}

	void push_back(File &&file) {
		if (getChild(file.name) == nullptr)
			children.push_back(file);
	}

	File *getParent() {
		return parent;
	}
};

int TallySizes(File &directory) {
	if (!directory.IsDirectory()) return 0;

	int directorySz = directory.getSize();
	if (directorySz > MAX_FILE_SZ)
		directorySz = 0;

	for (File &child : directory.getChildren())
		directorySz += TallySizes(child);

	return directorySz;
}

int GetSmallestDirectorySizeOverSize(File &directory, int neededSpace) {
	if (!directory.IsDirectory()) return 0;

	int directorySz = directory.getSize();
	if (directorySz < neededSpace) return 0;

	for (File &child : directory.getChildren()) {
		int childSize = GetSmallestDirectorySizeOverSize(child, neededSpace);
		if (childSize == 0) continue;

		if (childSize < directorySz)
			directorySz = childSize;
	}

	return directorySz;
}

int main() {
	std::ifstream iFile("terminal_output.log");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	File rootDirectory("/", nullptr, true);

	File *currentDirectory = &rootDirectory;

	bool listing = false;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		std::vector<std::string> splitOutput;
		Parser::Tokenize(buffer, splitOutput);

		if (splitOutput[0] != "$") {
			if (listing) {
				if (splitOutput[0] == "dir") {
					currentDirectory->push_back(File(
							splitOutput[1],
							currentDirectory,
							true));
				} else {
					currentDirectory->push_back(File(
							splitOutput[1],
							currentDirectory,
							false,
							atoi(splitOutput[0].c_str())));
				}
			} else {
				std::cerr << "Expected command!\n";
			}

			continue;
		}

		listing = false;

		if (splitOutput[1] == "cd") {
			File *newDirectory = nullptr;

			if (splitOutput[2] == "/") {
				newDirectory = &rootDirectory;
			} else if (splitOutput[2] == "..") {
				newDirectory = currentDirectory->getParent();
			} else {
				newDirectory = currentDirectory->getChild(splitOutput[2]);
			}

			if (newDirectory != nullptr)
				currentDirectory = newDirectory;
		} else if (splitOutput[1] == "ls") {
			listing = true;
		} else {
			std::cerr<<"Unexpected token: "<<splitOutput[1]<<std::endl;
		}
	}

	int freeSpace = TOTAL_DISK_SPACE - rootDirectory.getSize();
	int neededSpace = NEEDED_SPACE - freeSpace;

	std::cout<<"Smallest directory to delete: "<<GetSmallestDirectorySizeOverSize(rootDirectory, neededSpace)<<std::endl;

	iFile.close();
	return 0;
}

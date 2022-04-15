#pragma once

#include <iostream>
#include <string>
#include <filesystem>

#define PRINTLOG(x) std::cout << x << std::endl
#define FULLPATH(x, y) x + "/" + y

bool zipAllinDirectory(const std::string& baseDir, const std::string& fileExt);

bool unzipAllinDirectory(const std::string& baseDir);

bool convertExtenstions(const std::string& baseDir, const char* oldExt, const char* newExt);

std::string getDirectory();

char getOption();

bool chooseFunction(const char getOp, const std::string& getDir);
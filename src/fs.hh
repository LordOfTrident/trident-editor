#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

namespace FS {
	typedef std::vector <std::string> FileList;

	bool Exists(std::string p_Path);
	void Delete(std::string p_Path);
	void Rename(std::string p_Path, std::string p_NewPath);
	void Copy(std::string p_Path, std::string p_NewPath);

	bool IsADirectory(std::string p_Path);

	void CreateDirectory(std::string p_Path);
	std::string ReadFile(std::string p_Path);
	void WriteFile(std::string p_Text, std::string p_Path);

	FileList GetDirectoryFiles(std::string p_Path);
};

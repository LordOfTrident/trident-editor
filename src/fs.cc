#include "fs.hh"

bool FS::Exists(std::string p_Path) {
	return std::filesystem::exists(p_Path);
};

void FS::Delete(std::string p_Path) {
	std::filesystem::remove_all(p_Path);
};

void FS::Rename(std::string p_Path, std::string p_NewPath) {
	std::filesystem::rename(p_Path, p_NewPath);
};

void FS::Copy(std::string p_Path, std::string p_NewPath) {
	std::filesystem::copy(p_Path, p_NewPath);
};

bool FS::IsADirectory(std::string p_Path) {
	return std::filesystem::is_directory(std::filesystem::status(p_Path));
};

void FS::CreateDirectory(std::string p_Path) {
	std::filesystem::create_directory(p_Path);
};

std::string FS::ReadFile(std::string p_Path) {
	std::ifstream fhnd(p_Path);
	std::string fstr;

	if (fhnd.is_open()) {
		std::string fln = "";

		while (getline(fhnd, fln))
			fstr += fln + '\n';

		fhnd.close();
	};

	return fstr;
};

void FS::WriteFile(std::string p_Text, std::string p_Path) {
	std::ofstream fhnd(p_Path);

	fhnd << p_Text;
};

FS::FileList FS::GetDirectoryFiles(std::string p_Path) {
	FileList files = {};

	for (const std::filesystem::directory_entry &entry: std::filesystem::directory_iterator(p_Path))
		files.push_back(entry.path());

	return files;
};

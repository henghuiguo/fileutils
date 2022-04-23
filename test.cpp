#include "fileutils.h"
#include <iostream>
#include <fstream>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif // _WIN32

bool IS_FILE(const std::string& path) {
	FILE* f = fopen(path.c_str(), "r");
	if (f == NULL) return false;
	fclose(f);
	return true;
}

bool IS_DIR(const std::string& path) {
	if (path.empty()) return false;

#ifdef _WIN32
	DWORD attr = GetFileAttributesA(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES) return false;

	return (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
	struct stat st = { 0 };
	if (stat(path.c_str(), &st) == -1)
		return false;

	return (st.st_mode & S_IFDIR);
#endif // _WIN32
}

void CREATE_FILE(const std::string& path) {
	FILE* f = fopen(path.c_str(), "wb");
	if (f == NULL) throw std::runtime_error("create file failed");
	fclose(f);
}

void CREATE_DIR(const std::string& path) {
	try {
		cx::create_directories(path);
	}
	catch (...) {
		throw std::runtime_error("create directory failed");
	}

	if (!IS_DIR(path))
		throw std::runtime_error("create directory failed");
}
	 
void REMOVE_DIR(const std::string& path) {
#ifdef _WIN32
	::RemoveDirectoryA(path.c_str());
#else
	::rmdir(path.c_str());
#endif // _WIN32
}

#define ASSERT(EXP) \
	if(!(EXP)) { \
		printf("\t#################### F A I L E D ####################\nASSERT: %s \n  => FAILED\n", #EXP); ; fflush(stdout); \
		return false; \
	}

#define ASSERT_EXCEPTION(EXP, _EXCEPTION) \
	try { \
		EXP; \
		printf("\t#################### F A I L E D ####################\nASSERT_EXCEPTION: %s \n  => FAILED\n", #EXP); ; fflush(stdout); \
		return false; \
	}\
	catch (const _EXCEPTION&) { }\
	catch (...) {\
		printf("\t#################### F A I L E D ####################\nASSERT_EXCEPTION: %s \n  => FAILED\n", #EXP); ; fflush(stdout); \
		return false; \
	}

// Test path combination.
bool test_path() {
#ifdef _WIN32
	ASSERT(cx::combine_paths("", "") == "");
	ASSERT(cx::combine_paths("\\", "") == "\\");
	ASSERT(cx::combine_paths("\\", "\\") == "\\\\");
	ASSERT(cx::combine_paths("\\", "/") == "\\/");
	ASSERT(cx::combine_paths("a", "b") == "a\\b");
	ASSERT(cx::combine_paths("a\\", "b") == "a\\b");
	ASSERT(cx::combine_paths("a\\\\", "b") == "a\\\\b");
	ASSERT(cx::combine_paths("a", "\\b") == "a\\b");
	ASSERT(cx::combine_paths("a\\", "\\b") == "a\\\\b");
	ASSERT(cx::combine_paths("a", "b", "c") == "a\\b\\c");
	ASSERT(cx::combine_paths("a", "b", "c", "d e") == "a\\b\\c\\d e");
	ASSERT(cx::combine_paths("a ", "b", " c", "d e") == "a \\b\\ c\\d e");
	ASSERT(cx::combine_paths("a ", "b", "", "d e") == "a \\b\\d e");
#else
	ASSERT(cx::combine_paths("", "") == "");
	ASSERT(cx::combine_paths("/", "") == "/");
	ASSERT(cx::combine_paths("/", "/") == "//");
	ASSERT(cx::combine_paths("a", "b") == "a/b");
	ASSERT(cx::combine_paths("a/", "b") == "a/b");
	ASSERT(cx::combine_paths("a//", "b") == "a//b");
	ASSERT(cx::combine_paths("a", "/b") == "a/b");
	ASSERT(cx::combine_paths("a/", "/b") == "a//b");
	ASSERT(cx::combine_paths("a", "b", "c") == "a/b/c");
	ASSERT(cx::combine_paths("a", "b", "c", "d e") == "a/b/c/d e");
	ASSERT(cx::combine_paths("a ", "b", " c", "d e") == "a /b/ c/d e");
	ASSERT(cx::combine_paths("a ", "b", "", "d e") == "a /b/d e");
#endif // _WIN32

	return true;
}

// Test file name parsing, parent directory parsing.
bool test_file() {
#ifdef _WIN32
	ASSERT(cx::get_filename("") == "");
	ASSERT(cx::get_filename("\\") == "");
	ASSERT(cx::get_filename("a\\") == "");
	ASSERT(cx::get_filename("b") == "b");
	ASSERT(cx::get_filename("\\b") == "b");
	ASSERT(cx::get_filename("a\\b") == "b");
	ASSERT(cx::get_filename("a b\\c") == "c");
#endif // _WIN32
	ASSERT(cx::get_filename("") == "");
	ASSERT(cx::get_filename("/") == "");
	ASSERT(cx::get_filename("a/") == "");
	ASSERT(cx::get_filename("b") == "b");
	ASSERT(cx::get_filename("/b") == "b");
	ASSERT(cx::get_filename("a/b") == "b");
	ASSERT(cx::get_filename("a b/c") == "c");

#ifdef _WIN32
	ASSERT(cx::get_filename_without_extention("") == "");
	ASSERT(cx::get_filename_without_extention(".cpp") == "");
	ASSERT(cx::get_filename_without_extention("a b.cpp") == "a b");
	ASSERT(cx::get_filename_without_extention("\\") == "");
	ASSERT(cx::get_filename_without_extention("\\.svn") == "");
	ASSERT(cx::get_filename_without_extention("a\\") == "");
	ASSERT(cx::get_filename_without_extention("a\\.svn") == "");
	ASSERT(cx::get_filename_without_extention("b") == "b");
	ASSERT(cx::get_filename_without_extention("\\b") == "b");
	ASSERT(cx::get_filename_without_extention("\\b.exe") == "b");
	ASSERT(cx::get_filename_without_extention("\\b.c.exe") == "b.c");
	ASSERT(cx::get_filename_without_extention("\\b.c.d.exe") == "b.c.d");
	ASSERT(cx::get_filename_without_extention("a\\b") == "b");
	ASSERT(cx::get_filename_without_extention("a\\b.exe") == "b");
	ASSERT(cx::get_filename_without_extention("a\\b.c.exe") == "b.c");
	ASSERT(cx::get_filename_without_extention("a b\\c") == "c");
	ASSERT(cx::get_filename_without_extention("a b\\c.d.exe") == "c.d");
#else
	ASSERT(cx::get_filename_without_extention("a b/c\\d") == "c\\d");
#endif // _WIN32
	ASSERT(cx::get_filename_without_extention("") == "");
	ASSERT(cx::get_filename_without_extention(".cpp") == "");
	ASSERT(cx::get_filename_without_extention("a b.cpp") == "a b");
	ASSERT(cx::get_filename_without_extention("/") == "");
	ASSERT(cx::get_filename_without_extention("/.svn") == "");
	ASSERT(cx::get_filename_without_extention("a/") == "");
	ASSERT(cx::get_filename_without_extention("a/.svn") == "");
	ASSERT(cx::get_filename_without_extention("b") == "b");
	ASSERT(cx::get_filename_without_extention("/b") == "b");
	ASSERT(cx::get_filename_without_extention("/b.exe") == "b");
	ASSERT(cx::get_filename_without_extention("/b.c.exe") == "b.c");
	ASSERT(cx::get_filename_without_extention("/b.c.d.exe") == "b.c.d");
	ASSERT(cx::get_filename_without_extention("a/b") == "b");
	ASSERT(cx::get_filename_without_extention("a/b.exe") == "b");
	ASSERT(cx::get_filename_without_extention("a/b.c.exe") == "b.c");
	ASSERT(cx::get_filename_without_extention("a b/c") == "c");
	ASSERT(cx::get_filename_without_extention("a b/c.d.exe") == "c.d");

#ifdef _WIN32
	ASSERT(cx::get_parent_directory("") == "");
	ASSERT(cx::get_parent_directory("\\") == "");
	ASSERT(cx::get_parent_directory("a\\") == "a");
	ASSERT(cx::get_parent_directory("b") == "");
	ASSERT(cx::get_parent_directory("\\b") == "");
	ASSERT(cx::get_parent_directory("a\\b") == "a");
	ASSERT(cx::get_parent_directory("a b\\c") == "a b");
#else
	ASSERT(cx::get_parent_directory("a/b\\c") == "a");
#endif // _WIN32
	ASSERT(cx::get_parent_directory("") == "");
	ASSERT(cx::get_parent_directory("/") == "");
	ASSERT(cx::get_parent_directory("a/") == "a");
	ASSERT(cx::get_parent_directory("b") == "");
	ASSERT(cx::get_parent_directory("/b") == "");
	ASSERT(cx::get_parent_directory("a/b") == "a");
	ASSERT(cx::get_parent_directory("a b/c") == "a b");
	ASSERT(cx::get_parent_directory("a b/c/d.exe") == "a b/c");

	return true;
}

bool test_remove_file() {
	ASSERT_EXCEPTION(cx::remove_file(""), std::invalid_argument);
	ASSERT(cx::remove_file("this is not a file.txt") == false);

	const char* testFile = "mytestfile.txt";
	CREATE_FILE(testFile);
	ASSERT(cx::remove_file(testFile) == true);
	ASSERT(IS_FILE(testFile) == false);

	const char* testDir = "mytestdir";
	CREATE_DIR(testDir);
	ASSERT_EXCEPTION(cx::remove_file(testDir), cx::io_exception);


	// open but not close a file.
	// test is_file and remove
	{
		FILE* f = fopen(testFile, "w");
		ASSERT(f != NULL);
		ASSERT(fprintf(f, "%s\n", "HIIIIIIIII") > 0);
		fflush(f);

		ASSERT(cx::is_file(testFile));
#ifdef _WIN32
		// Windows does not allow the file to be deleted.
		ASSERT_EXCEPTION(cx::remove_file(testFile), cx::io_exception);
		ASSERT(cx::is_file(testFile) == true);
		fclose(f);
#else
		// POSIX systems unlink the file name although the file system space is not reclaimed until the last running process closes the file.
		ASSERT(cx::remove_file(testFile) == true);
		ASSERT(cx::is_file(testFile) == false);

		FILE* f2 = fopen(testFile, "w");
		ASSERT(f2 != NULL);
		ASSERT(fprintf(f2, "%s\n", "HIIIIIIIIIABCDE") > 0);
		fclose(f2);
		fclose(f);

		std::ifstream ifs(testFile);
		std::string content;
		ifs >> content;
		ASSERT(content == "HIIIIIIIIIABCDE");
#endif // _WIN32
	}
	
	return true;
}

// Test directory creation, removing, existence checking.
bool test_directory() {
	{
		const char* testFileName = "mytestfile.txt";
		const char* testDir = "mytestdir";
		cx::remove_directories(testDir);

		FILE* f = fopen(testFileName, "w");
		ASSERT(f != NULL);
		fclose(f);

#ifdef _WIN32
		ASSERT(!cx::is_directory(""));
		ASSERT(cx::is_directory("C:"));
		ASSERT(cx::is_directory("C:\\"));
		ASSERT(cx::is_directory("C:\\Windows"));
		ASSERT(cx::is_directory("C:\\Program Files"));

		ASSERT(!cx::is_directory("C:\\Windows\\explorer.exe"));
#else
		ASSERT(!cx::is_directory(""));
		ASSERT(!cx::is_file(""));
		ASSERT(!cx::is_directory("root"));
		ASSERT(!cx::is_file("root"));
		ASSERT(cx::is_directory("/root"));
		ASSERT(!cx::is_file("/root"));
#endif // _WIN32

		ASSERT(cx::is_file(testFileName));
		ASSERT(!cx::is_directory(testFileName));

		cx::create_directory(testDir);
		ASSERT(cx::is_directory(testDir));
		ASSERT(!cx::is_file(testDir));
		ASSERT(!cx::create_directory(testDir));

		ASSERT(cx::remove_directory(testDir) == true);
		ASSERT(!cx::is_directory(testDir));
		ASSERT(!cx::is_file(testDir));
	}

	{
		const char* baseDir = "mytestdir";
#ifdef _WIN32
		const char* testDir = "mytestdir\\a\\b";
		const char* testDir2 = "mytestdir\\a\\c";
		const char* testDir3 = "mytestdir\\b\\d";
		const char* testFile1 = "mytestdir\\b\\d\\file1.txt";
		const char* testFile2 = "mytestdir\\b\\d\\file2.txt";
		const char* testOldFile = "mytestdir\\oldfile.txt";
		const char* testNewFile = "mytestdir\\newfile.txt";
		const char* testOldDir = "mytestdir\\olddir";
		const char* testNewDir = "mytestdir\\newdir";
#else
		const char* testDir = "mytestdir/a/b";
		const char* testDir2 = "mytestdir/a/c";
		const char* testDir3 = "mytestdir/b/d";
		const char* testFile1 = "mytestdir/b/d/file1.txt";
		const char* testFile2 = "mytestdir/b/d/file2.txt";
		const char* testOldFile = "mytestdir/oldfile.txt";
		const char* testNewFile = "mytestdir/newfile.txt";
		const char* testOldDir = "mytestdir/olddir";
		const char* testNewDir = "mytestdir/b/newdir";
#endif // _WIN32
		cx::remove_directories(baseDir);
		cx::create_directories(testDir);
		cx::create_directories(testDir2);
		cx::create_directories(testDir3);

		// test rename file
		{
			cx::remove_file(testNewFile);
			FILE* f = fopen(testOldFile, "w");
			ASSERT(f != NULL);
			ASSERT(fprintf(f, "%s\n", "HIIIIIIIII") > 0);
			fclose(f);

			ASSERT(IS_FILE(testOldFile));
			cx::rename(testOldFile, testNewFile);
			ASSERT(!IS_FILE(testOldFile));
			ASSERT(IS_FILE(testNewFile));
		}

		// test rename file
		{
			cx::remove_file(testNewFile);
			FILE* f = fopen(testOldFile, "w");
			ASSERT(f != NULL);
			fclose(f);

			FILE* f2 = fopen(testNewFile, "w");
			ASSERT(f2 != NULL);
			fclose(f2);

			ASSERT(IS_FILE(testOldFile));
			ASSERT(IS_FILE(testNewFile));

#ifdef _WIN32
			ASSERT_EXCEPTION(cx::rename(testOldFile, testNewFile), cx::io_exception);
#else
			cx::rename(testOldFile, testNewFile);
			ASSERT(!IS_FILE(testOldFile));
			ASSERT(IS_FILE(testNewFile));
#endif // _WIN32

		}

		// test rename directory
		{
			cx::remove_directory(testNewDir);
			cx::create_directories(testOldDir);

			ASSERT(cx::is_directory(testOldDir));
			cx::rename(testOldDir, testNewDir);
			ASSERT(cx::is_directory(testNewDir));
		}

		{
			FILE* f = fopen(testFile2, "w");
			ASSERT(f != NULL);

			ASSERT(cx::remove_directory(testDir3) == false);
			fclose(f);
			ASSERT(IS_FILE(testFile2));
			ASSERT(cx::is_file(testFile2));
		}

		ASSERT(cx::is_directory(testDir));
		ASSERT(cx::remove_directory(baseDir) == false);
		ASSERT(cx::is_directory(testDir));
		ASSERT(cx::is_directory(testDir2));
		ASSERT(cx::is_directory(testDir3));
		ASSERT(cx::remove_directories(baseDir) == true);
		ASSERT(!cx::is_directory(baseDir));
	}
	
	ASSERT(!cx::get_current_directory().empty());

	std::cout << "Currrent Dir: " <<  cx::get_current_directory() << std::endl;
	return true;
}

// Test file enumeration.
bool test_enum_files() {
	{
		const char* baseDir = "mytestdir";
#ifdef _WIN32
		const char* testDir = "mytestdir\\a\\b";
		const char* testDir2 = "mytestdir\\a\\c";
		const char* testDir3 = "mytestdir\\b\\d";
		const char* testFile1 = "mytestdir\\b\\d\\file1.txt";
		const char* testFile2 = "mytestdir\\b\\d\\file2.txt";
#else
		const char* testDir = "mytestdir/a/b";
		const char* testDir2 = "mytestdir/a/c";
		const char* testDir3 = "mytestdir/b/d";
		const char* testFile1 = "mytestdir/b/d/file1.txt";
		const char* testFile2 = "mytestdir/b/d/file2.txt";
#endif // _WIN32
		cx::remove_directories(baseDir);
		cx::create_directories(testDir);
		cx::create_directories(testDir2);
		cx::create_directories(testDir3);

		// create file1
		{
			FILE* f = fopen(testFile1, "w");
			ASSERT(f != NULL);
			fclose(f);
			ASSERT(IS_FILE(testFile1));
		}
		// create file2
		{
			FILE* f = fopen(testFile2, "w");
			ASSERT(f != NULL);
			fclose(f);
			ASSERT(IS_FILE(testFile2));
		}

		// enum directories and files.
		{
			std::vector<std::string> dirs = { testDir, testDir2, testDir3 };
			std::vector<std::string> files = { testFile1, testFile2 };
			cx::enum_all_files(baseDir, [&dirs, &files](const std::string& fileName, cx::EnumFileType fileType, bool& cancelEnum) {
					std::cout << fileName << std::endl;
					if (fileType & cx::EFT_DIR) {
						auto i = std::find(dirs.begin(), dirs.end(), fileName);
						if (i != dirs.end()) dirs.erase(i);
					} else if (fileType & cx::EFT_FILE) {
						auto i = std::find(files.begin(), files.end(), fileName);
						if (i != files.end()) files.erase(i);
					}
				}
			);
			ASSERT(dirs.empty());
			ASSERT(files.empty());
		}
		
		// enum directories.
		{
			std::vector<std::string> dirs = { testDir, testDir2, testDir3 };
			std::vector<std::string> files = { testFile1, testFile2 };
			cx::enum_all_files(baseDir, [&dirs, &files](const std::string& fileName, cx::EnumFileType fileType, bool& cancelEnum) {
					if (fileType & cx::EFT_DIR) {
						auto i = std::find(dirs.begin(), dirs.end(), fileName);
						if (i != dirs.end()) dirs.erase(i);
					} else if (fileType & cx::EFT_FILE) {
						auto i = std::find(files.begin(), files.end(), fileName);
						if (i != files.end()) files.erase(i);
					}
				}, cx::EFT_DIR
			);
			ASSERT(dirs.empty());
			ASSERT(files.size() == 2);
		}

		// enum files.
		{
			std::vector<std::string> dirs = { testDir, testDir2, testDir3 };
			std::vector<std::string> files = { testFile1, testFile2 };
			cx::enum_all_files(baseDir, [&dirs, &files](const std::string& fileName, cx::EnumFileType fileType, bool& cancelEnum) {
					if (fileType & cx::EFT_DIR) {
						auto i = std::find(dirs.begin(), dirs.end(), fileName);
						if (i != dirs.end()) dirs.erase(i);
					} else if (fileType & cx::EFT_FILE) {
						auto i = std::find(files.begin(), files.end(), fileName);
						if (i != files.end()) files.erase(i);
					}
				}, cx::EFT_FILE
			);
			ASSERT(dirs.size() == 3);
			ASSERT(files.empty());
		}

		// count files
		ASSERT(cx::get_file_count(baseDir, cx::EFT_FILE, 0) == 2);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR, 0) == 5);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR | cx::EFT_FILE, 0) == 7);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR | cx::EFT_FILE, 1) == 2);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR, 1) == 2);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR, 2) == 5);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR, 3) == 5);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR, 4) == 5);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_FILE, 0) == 2);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_FILE, 1) == 0);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_FILE, 2) == 0);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_FILE, 3) == 2);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR | cx::EFT_FILE, 2) == 5);
		ASSERT(cx::get_file_count(baseDir, cx::EFT_DIR | cx::EFT_FILE, 3) == 7);

		ASSERT(cx::remove_directories(baseDir));
	}
	 
	return true;
}

bool test_read_write() {
	std::string testfile = "fileutils-test_read_write.txt";
	cx::remove_file(testfile);

	std::vector<unsigned char> data;
	ASSERT_EXCEPTION(cx::read_all_bytes(testfile, data), cx::io_exception);
	
#ifdef _WIN32 
	ASSERT_EXCEPTION(cx::write_all_bytes("", data), std::invalid_argument);
#endif // WIN32
	data.push_back(1);
	data.push_back(2);
	data.push_back(3);
	cx::write_all_bytes(testfile, data);

	std::vector<unsigned char> data2;
	cx::read_all_bytes(testfile, data2);
	ASSERT(data2[0] == 1);
	ASSERT(data2[1] == 2);
	ASSERT(data2[2] == 3);

	cx::remove_file(testfile);
	return true;
}

int main() {
	if (!test_path()) return 1;
	if (!test_file()) return 1;
	if (!test_remove_file()) return 1;
	if (!test_directory()) return 1;
	if (!test_enum_files()) return 1;
	if (!test_read_write()) return 1;
	
	printf("All tests passed!\n");
	return 0;
}

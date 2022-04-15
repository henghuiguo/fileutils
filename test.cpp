#include "fileutils.h"
#include <iostream>
#include <algorithm>

bool IsFile(const std::string& fileName) {
	FILE* f = fopen(fileName.c_str(), "r");
	if (f == NULL) return false;
	fclose(f);
	return true;
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

		// open and close a file.
		// test is_file and remove_file
		{
			FILE* f = fopen(testFile1, "w");
			ASSERT(f != NULL);
			fclose(f);
			ASSERT(IsFile(testFile1));
			ASSERT(cx::is_file(testFile1));
			ASSERT(cx::remove_file(testFile1));
			ASSERT(!IsFile(testFile1));
			ASSERT(!cx::is_file(testFile1));
			ASSERT(cx::remove_file(testFile1) == false);
		}

		// open but not close a file.
		// test is_file and remove_file
		{
			FILE* f = fopen(testFile1, "w");
			ASSERT(fprintf(f, "%s\n", "HIIIIIIIII") > 0);
			fflush(f);
			ASSERT(f != NULL);

			ASSERT(cx::is_file(testFile1));
#ifdef _WIN32
			ASSERT(cx::remove_file(testFile1) == false);
			ASSERT(cx::is_file(testFile1) == true);
#else
			ASSERT(cx::remove_file(testFile1) == true);
			ASSERT(cx::is_file(testFile1) == false);
#endif // _WIN32
			fclose(f);
			cx::remove_file(testFile1);
			ASSERT(cx::is_file(testFile1) == false);
		}

		{
			FILE* f = fopen(testFile2, "w");
			ASSERT(f != NULL);

			ASSERT(cx::remove_directory(testDir3) == false);
			fclose(f);
			ASSERT(IsFile(testFile2));
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
bool test_enum_files()
{
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
			ASSERT(IsFile(testFile1));
		}
		// create file2
		{
			FILE* f = fopen(testFile2, "w");
			ASSERT(f != NULL);
			fclose(f);
			ASSERT(IsFile(testFile2));
		}

		// enum directories and files.
		{
			std::vector<std::string> dirs = { testDir, testDir2, testDir3 };
			std::vector<std::string> files = { testFile1, testFile2 };
			cx::enum_all_files(baseDir, [&dirs, &files](const std::string& fileName, cx::EnumFileType fileType, bool& cancelEnum)
				{
					std::cout << fileName << std::endl;
					if (fileType & cx::EFT_DIR) {
						auto i = std::find(dirs.begin(), dirs.end(), fileName);
						if (i != dirs.end()) dirs.erase(i);
					}
					else if (fileType & cx::EFT_FILE) {
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
			cx::enum_all_files(baseDir, [&dirs, &files](const std::string& fileName, cx::EnumFileType fileType, bool& cancelEnum)
				{
					if (fileType & cx::EFT_DIR) {
						auto i = std::find(dirs.begin(), dirs.end(), fileName);
						if (i != dirs.end()) dirs.erase(i);
					}
					else if (fileType & cx::EFT_FILE) {
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
			cx::enum_all_files(baseDir, [&dirs, &files](const std::string& fileName, cx::EnumFileType fileType, bool& cancelEnum)
				{
					if (fileType & cx::EFT_DIR) {
						auto i = std::find(dirs.begin(), dirs.end(), fileName);
						if (i != dirs.end()) dirs.erase(i);
					}
					else if (fileType & cx::EFT_FILE) {
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

int main()
{
	if (!test_path()) return 1;
	if (!test_file()) return 1;
	if (!test_directory()) return 1;
	if (!test_enum_files()) return 1;
	if (!test_read_write()) return 1;

	printf("All tests passed!\n");
	return 0;
}

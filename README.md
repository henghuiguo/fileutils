# fileutils
This project provides a collection of **cross-platform** file operating utilities without using **STL filesystem** library.
You can use this project in any environment in which the **STL filesystem** is not well supported.


# How to use
1. Copy fileutils.h and fileutils.cpp to your project.
2. #include "fileutils.h"
3. Invoke APIs in namespace **cx**, as follows:
```cplusplus
   // Determines whether the given path refers to a block file.
   bool isFile = cx::is_file(path);
   // Determines whether the given path refers to a directory.
   bool isDir = cx::is_directory(path);
   // Get the file name of the given path.
   std::string filename = cx::get_filename(path);
   // Get parent directory of the given path.
   std::string dir = cx::get_parent_directory(path);
   // Combine names to a path.
   std::string path = cx::combine_paths("a", "b", "c", "d e");
   // Get currrent absolute directory.
   std::string curDir = cx::get_current_directory();
   // ...
```

# Tests
```cplusplus
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
	if(EXP)	/*printf("%s \n  => OK\n", #EXP)*/;\
	else { \
		printf("\t#################### F A I L E D ####################\nASSERT_TRUE: %s \n  => FAILED\n", #EXP); ; fflush(stdout); \
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

		std::vector<std::string> dirs;
		dirs.push_back(testDir);
		dirs.push_back(testDir2);
		dirs.push_back(testDir3);
		std::vector<std::string> files;
		files.push_back(testFile1);
		files.push_back(testFile2);
		cx::enum_all_files(baseDir, [&dirs, &files](const std::string& fileName, cx::EnumFileType fileType, bool& cancelEnum)
			{
				if (fileType & cx::EFT_DIR) {
					auto i = std::find(dirs.begin(), dirs.end(), fileName);
					if(i != dirs.end()) dirs.erase(i);
				}
				else if (fileType & cx::EFT_FILE) {
					auto i = std::find(files.begin(), files.end(), fileName);
					if (i != files.end()) files.erase(i);
				}
			}
		);

		ASSERT(dirs.empty());
		ASSERT(files.empty());

		ASSERT(cx::remove_directories(baseDir));
	}
	 
	return true;
}

int main()
{
	if (!test_path()) return 1;
	if (!test_file()) return 1;
	if (!test_directory()) return 1;
	if (!test_enum_files()) return 1;

	printf("Success\n");
	return 0;
}


```

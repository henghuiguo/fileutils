///////////////////////////////////////////////////////////////////////////////////////
// MIT License
// 
// Copyright (c) 2022 Henghui Guo
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <stdexcept>

/** @brief cx namespace. */
namespace cx {
	/**
	 * @brief I/O exception.
	 */
	class io_exception : public std::exception { };

	/**
	 * @brief Combine two paths together.
	 * @param path1 The first path.
	 * @param path2 The second path.
	 * @return Combined path. path1 + separator + path2.
	 */
	std::string combine_paths(const std::string& path1, const std::string& path2);

	/**
	 * @brief Combine paths together.
	 * @param path1 The first path.
	 * @param path2 The second path.
	 * @param path3 The third path.
	 * @return Combined path. path1 + separator + path2 + separator + path3.
	 */
	std::string combine_paths(const std::string& path1, const std::string& path2, const std::string& path3);

	/**
	 * @brief Combine paths together.
	 * @param path1 The first path.
	 * @param path2 The second path.
	 * @param path3 The third path.
	 * @param path4 The fourth path.
	 * @return Combined path. path1 + separator + path2 + separator + path3 + separator + path4.
	 */
	std::string combine_paths(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4);

	/**
	 * @brief Check whether the file exists.
	 * @param path The file name to check.
	 * @return true if the file exists, or false if not.
	 */
	bool is_file(const std::string& path);

	/**
	 * @brief Check whether the directory exists.
	 * @param path The directory name to check.
	 * @return true if the directory exists, or false if not.
	 */
	bool is_directory(const std::string& path);

	/**
	 * @brief Get file name.
	 * @param path The whole path.
	 * @return The filename of the path.
	 */
	std::string get_filename(const std::string& path);

	/**
	 * @brief Get file name without extention.
	 * @param path The whole path.
	 * @return The filename of the path.
	 */
	std::string get_filename_without_extention(const std::string& path);

	/**
	 * @brief Get parent path of the given path.
	 * @param path The whole path.
	 * @return The filename of the path.
	 */
	std::string get_parent_directory(const std::string& path);

	/**
	 * @brief Create a new directory using the path.
	 * @param path Directory name.
	 * @return true if successful, or false if failed for those reasons:
	 * 1. Directory name already exists.
	 * 2. The parent directory does not exists.
	 * 3. No permission.
	 * @throw invalid_argument When path is empty.
	 */
	bool create_directory(const std::string& path);

	/**
	 * @brief Create a new directory using the path. The parent directories were also created if not exist.
	 * @param path Directory name.
	 * @return true if successful, or false if failed for those reasons:
	 * 1. Directory name already exists.
	 * 2. No permission.
	 * @throw invalid_argument When path is empty.
	 */
	bool create_directories(const std::string& path);

	/**
	 * @brief Remove an empty directory.
	 * @param path Directory name.
	 * @return true if successful, or false if failed for these reasons:
	 * 1. Directory does not exists.
	 * 2. No permission.
	 * @throw invalid_argument When path is empty.
	 */
	bool remove_directory(const std::string& path);

	/**
	 * @brief Remove the directory and all sub-directories.
	 * @param path Directory name.
	 * @return true if successful, or false if failed for these reasons:
	 * 1. Directory does not exists.
	 * 2. No permission.
	 * @throw invalid_argument When path is empty.
	 */
	bool remove_directories(const std::string& path);

	/**
	 * @brief Remove a file.
	 * @param path Directory name.
	 * @return true if successful, or false if the file did not exists.
	 * @throw invalid_argument When path is empty.
	 * @throw io_exception For any other reasons.
	 * 1. The path is not a directory or and other types.
	 * 2. No permission.
	 */
	bool remove_file(const std::string& path);

	/**
	 * @brief Rename or move a file or directory.
	 * @param oldName Old file name.
	 * @param newName New file name.
	 * @throw invalid_argument When name is empty.
	 * @throw io_exception For any other reasons.
	 */
	void rename(const std::string& oldName, const std::string& newName);

	/**
	 * @brief The output file type in enumeration.
	 */
	enum EnumFileType {
		/**
		 * @brief Directory.
		 */
		EFT_DIR = 1,

		/**
		 * @brief File.
		 */
		EFT_FILE = 2
	};

	/**
	 * @brief A simple file enumerator.
	 * Example:
	 * @code
	 * 	file_enumerator fe;
	 * 	if (fe.begin(dirName)) {
	 * 	    do {
	 * 	    	std::cout << fe.filename() << fe.file_type() << std::endl;
	 * 	    } while (fe.next());
	 * 	}
	 * 	fe.end();
	 * @endcode
	 */
	class file_enumerator {
	public:
		file_enumerator();

		/**
		 * @brief Constructor with file type filters.
		 * @param filters File type filters, such as:
		 *   EFT_DIR: only output directories.
		 *   EFT_FILE: only output files.
		 *   EFT_DIR | EFT_FILE: output directories and files.
		 */
		file_enumerator(int filters);

		~file_enumerator();

		/**
		 * @brief Begin file enumeration in the directory.
		 * @param dirName Directory name for enumeration.
		 * @return true if successful, false if no file found:
		 * @throw invalid_argument When dirName is empty.
		 * @throw io_exception When open directory failed for reasons:
		 *  1.Directory not exist.
		 *  2.The entry which dirName refers to is not a directory.
		 */
		bool begin(const std::string& dirName);

		/** 
		 * @brief End enumeration.
		 */
		void end();

		/**
		 * @brief Move to next file.
		 * @return true if got a new file, 
		 *    false if:  (1) no more file found. (2) enumeration did not started.
		 */
		bool next();

		/**
		 * @brief Get filename of current enumeration point.
		 * @return Current filename.
		 */
		const std::string& filename() const;

		/**
		 * @brief Get file type of current enumeration point.
		 * @return Current file type.
		 */
		EnumFileType file_type() const;

		/**
		 * @brief Get file type filters.
		 * @return File type filters.
		 */
		int filters() const;

		/**
		 * @brief Set file type filters.
		 * @param filters File type filters, such as:
		 *   EFT_DIR: only output directories.
		 *   EFT_FILE: only output files.
		 *   EFT_DIR | EFT_FILE: output directories and files.
		 */
		void filters(int newFilters);
	private:
		void* nativeEnumerator;
		bool started;
		std::string fname;
		EnumFileType ftype;
		std::string dname;
		int _Filters;
	public:
		file_enumerator(const file_enumerator&) = delete;
		file_enumerator& operator=(const file_enumerator&) = delete;
	};

	/**
	 * @brief Enumeration output callback function
	 * @param filename Output file name.
	 * @param fileType File type.
	 * @param cancelEnum The value indicating weather the enumeration should be canceled.
	 */
	typedef void (*enum_files_callback)(const std::string& filename, EnumFileType fileType, bool& cancelEnum);

	template<class Callback>
	void _enum_files_by_depth(const std::string& dirName, Callback callbackFun, int filters, int depth, int& currentDepth) {
		if (dirName.empty()) throw std::invalid_argument("dirName");

		if (currentDepth < 1) currentDepth = 1;
		if (depth != 0 && currentDepth > depth) return;

		file_enumerator fe;
		if (filters == EFT_DIR) fe.filters(filters);

		if (!fe.begin(dirName)) return;

		do {
			EnumFileType fileType = fe.file_type();

			bool doCallback = false;
			if (fileType == EFT_DIR) {
				if (filters & EFT_DIR) doCallback = true;
			}
			else if (fileType == EFT_FILE) {
				if (filters & EFT_FILE) doCallback = true;
			}

			if (doCallback) {
				bool cancelEnum = false;
				callbackFun(fe.filename(), fileType, cancelEnum);
				if (cancelEnum) return;
			}

			if (fileType == EFT_DIR && (depth == 0 || currentDepth + 1 <= depth)) {
				currentDepth++;
				_enum_files_by_depth(fe.filename(), callbackFun, filters, depth, currentDepth);
				currentDepth--;
			}
		} while (fe.next());
	}

	/**
	 * @brief Enum files in the directory:
	 * @tparam CallbackFun: Callback function as enum_files_callback.
	 * @param dirName: Directory name.
	 * @param callbackFun: Output callback function.
	 *   The definition is like enum_files_callback:
	 *   void foo(const std::string& filename, EnumFileType fileType, bool& cancelEnum);
	 *      filename: output file name.
	 *      fileType: file type.
	 *      cancelEnum: the value indicating weather the enumeration should be canceled.
	 * @param filters File type filters, such as:
	 *   EFT_DIR: Only output directories.
	 *   EFT_FILE: Only output files.
	 *   EFT_DIR | EFT_FILE: As default, Output directories and files.
	 * @param depth Walk depth. Default: 1.
	 *   0: Max sub-directory depth of this directory.
	 *   >=1: Real depth to walk into.
	 * @throw invalid_argument When dirName is empty.
	 */
	template<class Callback>
	void enum_files(const std::string& dirName, Callback callbackFun, int filters = EFT_DIR | EFT_FILE, int depth = 1) {
		int currentDepth = 1;
		_enum_files_by_depth(dirName, callbackFun, filters, depth, currentDepth);
	}

	/**
	 * @brief Enum all files in the directory:
	 * @tparam CallbackFun: Callback function as enum_files_callback.
	 * @param dirName: Directory name.
	 * @param callbackFun: Output callback function.
	 *   The definition is like enum_files_callback:
	 *   void foo(const std::string& filename, EnumFileType fileType, bool& cancelEnum);
	 *      filename: output file name.
	 *      fileType: file type.
	 *      cancelEnum: the value indicating weather the enumeration should be canceled.
	 * @param filters File type filters, such as:
	 *   EFT_DIR: Only output directories.
	 *   EFT_FILE: Only output files.
	 *   EFT_DIR | EFT_FILE: As default, Output directories and files.
	 * @return true if successful, or false if failed.
	 * @throw invalid_argument When dirName is empty.
	 */
	template<class Callback>
	void enum_all_files(const std::string& dirName, Callback callbackFun, int filters = EFT_DIR | EFT_FILE) {
		int currentDepth = 1;
		_enum_files_by_depth(dirName, callbackFun, filters, 0, currentDepth);
	}

	/**
	 * @brief Get children file count of the given directory.
	 * @param dirName The parent directory.
	 * @param filters File type filters with all following options:
	 *   EFT_DIR: Only count directories.
	 *   EFT_FILE: Only count files.
	 *   EFT_DIR | EFT_FILE: Count directories and files.
	 * @param depth Walk depth. Default: 1.
	 * @return File count: >= 0.
	 * @throw invalid_argument When dirName is empty.
	 * @throw io_exception When open directory failed.
	 */
	int get_file_count(const std::string& dirName, int filters = EFT_DIR | EFT_FILE, int depth = 1);

	/**
	 * @brief Get all children file count of the given directory.
	 * @param dirName The parent directory.
	 * @param filters File type filters with all following options:
	 *   EFT_DIR: Only count directories.
	 *   EFT_FILE: Only count files.
	 *   EFT_DIR | EFT_FILE: Count directories and files.
	 * @return File count: >= 0.
	 * @throw invalid_argument When dirName is empty.
	 * @throw io_exception When open directory failed.
	 */
	int get_all_file_count(const std::string& dirName, int filters = EFT_DIR | EFT_FILE);

	/**
	 * @brief Get application current directory.
	 * @return Current directory.
	 */
	std::string get_current_directory();

	/**
	 * @brief Read all byte to buffer from a file.
	 * @param filename The filename.
	 * @param data The data buffer.
	 * @throw invalid_argument When filename is empty.
	 * @throw io_exception When open file failed.
	 */
	void read_all_bytes(const std::string& filename, std::vector<unsigned char>& data);

	/**
	 * @brief Write all bytes from a buffer to a file.
	 * @param filename The filename.
	 * @param data The data buffer.
	 * @param bAppend true: for append mode, false: for creation mode.
	 * @throw invalid_argument When filename is empty.
	 * @throw io_exception When open file failed.
	 */
	void write_all_bytes(const std::string& filename, const std::vector<unsigned char>& data, bool bAppend = false);

	/**
	 * @brief Write all bytes from a buffer to a file.
	 * @param filename The filename.
	 * @param data The data buffer.
	 * @param length Data length.
	 * @param bAppend true: for append mode, false: for creation mode.
	 * @throw invalid_argument When filename is empty.
	 * @throw io_exception When open file failed.
	 */
	void write_all_bytes(const std::string& filename, const unsigned char* data, size_t length, bool bAppend = false);
}
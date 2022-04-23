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

#include "fileutils.h"
#include <fstream>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif // _WIN32

namespace cx {

#ifdef _WIN32
	static char DIR_SEP = '\\';
	static char DIR_SEP2 = '/';
#else
	static char DIR_SEP = '/';
#endif // _WIN32

	std::string combine_paths(const std::string& path1, const std::string& path2) {
		if (path1.empty()) return path2;
		if (path2.empty()) return path1;

		bool hasSep = false;
		char endCh = path1[path1.size() - 1];
		char startCh = path2[0];
#ifdef _WIN32
		if (endCh == DIR_SEP || endCh == DIR_SEP2) hasSep = true;
		else if (startCh == DIR_SEP || startCh == DIR_SEP2) hasSep = true;
#else
		if (endCh == DIR_SEP) hasSep = true;
		else if (startCh == DIR_SEP) hasSep = true;
#endif // _WIN32

		if (hasSep) return path1 + path2;
		else return path1 + DIR_SEP + path2;
	}

	std::string combine_paths(const std::string& path1, const std::string& path2, const std::string& path3) {
		return combine_paths(combine_paths(path1, path2), path3);
	}

	std::string combine_paths(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4) {
		return combine_paths(combine_paths(path1, path2), combine_paths(path3, path4));
	}

	bool is_file(const std::string& path) {
		if (path.empty()) return false;

#ifdef _WIN32
		DWORD attr = GetFileAttributesA(path.c_str());
		if (attr == INVALID_FILE_ATTRIBUTES) return false;

		return (attr & FILE_ATTRIBUTE_ARCHIVE);
#else
		struct stat st = { 0 };
		if (stat(path.c_str(), &st) == -1)
			return false;

		return (st.st_mode & S_IFREG);
#endif // _WIN32
	}

	bool is_directory(const std::string& path) {
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

	std::string get_filename(const std::string& path) {
#ifdef _WIN32
		size_t id = path.rfind('\\');
		size_t id2 = path.rfind('/');
		if (id == std::string::npos || (id2 != std::string::npos && id < id2)) id = id2;
#else
		size_t id = path.rfind('/');
#endif // _WIN32

		if (id == std::string::npos) return path;
		return path.substr(id + 1);
	}

	std::string get_filename_without_extention(const std::string& path) {
#ifdef _WIN32
		size_t id = path.rfind('\\');
		size_t id2 = path.rfind('/');
		if (id == std::string::npos || (id2 != std::string::npos && id < id2)) id = id2;
#else
		size_t id = path.rfind('/');
#endif // _WIN32

		size_t idex = path.rfind('.');

		if (id == std::string::npos) {
			if (idex == std::string::npos)
				return path;
			else
				return path.substr(0, idex);
		}

		if (idex == std::string::npos)
			return path.substr(id + 1);
		else
			return path.substr(id + 1, idex - id - 1);
	}

	std::string get_parent_directory(const std::string& path) {
#ifdef _WIN32
		size_t id = path.rfind('\\');
		size_t id2 = path.rfind('/');
		if (id == std::string::npos || (id2 != std::string::npos && id < id2)) id = id2;
#else
		size_t id = path.rfind('/');
#endif // _WIN32

		if (id == std::string::npos) return "";
		return path.substr(0, id);
	}

	bool create_directory(const std::string& path) {
		if (path.empty()) throw std::invalid_argument("path");
#ifdef _WIN32
		if (!::CreateDirectoryA(path.c_str(), NULL))
			return false;
		return true;
#else
		return mkdir(path.c_str(), 0) == 0;
#endif // _WIN32
	}

	static bool _do_create_directories(const std::string& path) {
		if (path.empty()) throw std::invalid_argument("path");
		if (is_directory(path)) return false;

		std::string parentDir = get_parent_directory(path);
		if (!parentDir.empty() && !is_directory(parentDir)) {
			// create parent directory
			_do_create_directories(parentDir);
		}

		return create_directory(path);

	}

	bool create_directories(const std::string& path) {
		return _do_create_directories(path);
	}

	bool remove_directory(const std::string& path) {
		if (path.empty()) throw std::invalid_argument("path");
#ifdef _WIN32
		return ::RemoveDirectoryA(path.c_str()) == TRUE;
#else
		return ::rmdir(path.c_str()) == 0;
#endif // _WIN32
	}

	bool do_remove_directories(const std::string& path) {
		if (!is_directory(path)) return false;

		cx::enum_files(path, [](const std::string& filename, cx::EnumFileType fileType, bool& cancelEnum) {
				if (fileType & cx::EFT_DIR) {
					do_remove_directories(filename);
				} else if (fileType & cx::EFT_FILE) {
					remove_file(filename);
				}
			}
		);

		return remove_directory(path);
	}

	bool remove_directories(const std::string& path) {
		if (path.empty()) throw std::invalid_argument("path");
		return do_remove_directories(path);
	}

	bool remove_file(const std::string& path) {
		if (path.empty()) throw std::invalid_argument("path");

#ifdef _WIN32
		BOOL r = ::DeleteFileA(path.c_str());
		if (r == true) return true;
		DWORD lastError = ::GetLastError();
		if (lastError == ERROR_FILE_NOT_FOUND) return false;
#else
		struct stat st = { 0 };
		if (stat(path.c_str(), &st) == -1)
			return false;

		if (st.st_mode & S_IFREG) {
			int r = ::remove(path.c_str());
			if (r == 0) return true;
		}
#endif // _WIN32
		throw io_exception();
	}

	void rename(const std::string& oldName, const std::string& newName) {
		if (oldName.empty()) throw std::invalid_argument("oldName");
		if (newName.empty()) throw std::invalid_argument("newName");

		int r = ::rename(oldName.c_str(), newName.c_str());
		if (r == 0) return;
		throw io_exception();
	}

	file_enumerator::file_enumerator() {
		started = false;
		_Filters = EFT_DIR | EFT_FILE;
	}

	file_enumerator::file_enumerator(int filters) {
		started = false;
		this->_Filters = filters;
	}

	file_enumerator::~file_enumerator() {
		end();
	}

	bool file_enumerator::begin(const std::string& dirName) {
		if (dirName.empty()) throw std::invalid_argument("dirName");
		if (started) return false;

		this->dname = dirName;
#ifdef _WIN32
		WIN32_FIND_DATAA ffd;
		char szDir[MAX_PATH];

		if (StringCchCopyA(szDir, MAX_PATH, dirName.c_str()) != S_OK) return false;
		if (StringCchCatA(szDir, MAX_PATH, "\\*") != S_OK) return false;

		HANDLE hDir = FindFirstFileA(szDir, &ffd);
		if (hDir == INVALID_HANDLE_VALUE)
			throw io_exception();
		
		bool bFound = false;
		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((_Filters & EFT_DIR) == 0) continue;
				if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0) continue;

				ftype = EFT_DIR;
				bFound = true;
				break;
			} else if (ffd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
				if ((_Filters & EFT_FILE) == 0) continue;
				ftype = EFT_FILE;
				bFound = true;
				break;
			}
		} while (FindNextFileA(hDir, &ffd));

		if (bFound) {
			fname = combine_paths(dname, ffd.cFileName);

			nativeEnumerator = hDir;
			started = true;
			return true;
		} else {
			FindClose(hDir);
			return false;
		}
#else
		DIR* hDir = opendir(dirName.c_str());
		if (hDir == NULL)
			throw io_exception();

		bool bFound = false;
		dirent* d = NULL;
		while ((d = readdir(hDir)) != NULL) {
			if (d->d_type & DT_DIR) {
				if ((_Filters & EFT_DIR) == 0) continue;
				if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;

				ftype = EFT_DIR;
				bFound = true;
				break;
			} else if (d->d_type & DT_REG) {
				if ((_Filters & EFT_FILE) == 0) continue;
				ftype = EFT_FILE;
				bFound = true;
				break;
			}
		}

		if (bFound) {
			fname = combine_paths(dname, d->d_name);
			nativeEnumerator = hDir;
			started = true;
			return true;
		} else {
			closedir(hDir);
			return false;
		}
#endif // _WIN32
	}

	void file_enumerator::end() {
		if (!started) return;

#ifdef _WIN32
		HANDLE hDir = (HANDLE)nativeEnumerator;
		FindClose(hDir);
#else 
		DIR* hDir = (DIR*)nativeEnumerator;
		closedir(hDir);
#endif // _WIN32

		started = false;
	}

	bool file_enumerator::next() {
		if (!started) return false;

#ifdef _WIN32
		HANDLE hDir = (HANDLE)nativeEnumerator;

		WIN32_FIND_DATAA ffd;
		bool bFound = false;
		while (FindNextFileA(hDir, &ffd)) {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0) continue;

				ftype = EFT_DIR;
				bFound = true;
				break;
			} else if (ffd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
				ftype = EFT_FILE;
				bFound = true;
				break;
			}
		}

		if (bFound) {
			fname = combine_paths(dname, ffd.cFileName);
			return true;
		} else {
			return false;
		}
#else 
		DIR* hDir = (DIR*)nativeEnumerator;

		bool bFound = false;
		dirent* d = NULL;
		while ((d = readdir(hDir)) != NULL) {
			if (d->d_type & DT_DIR) {
				if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;

				ftype = EFT_DIR;
				bFound = true;
				break;
			} else if (d->d_type & DT_REG) {
				ftype = EFT_FILE;
				bFound = true;
				break;
			}
		}

		if (bFound) {
			fname = combine_paths(dname, d->d_name);
			return true;
		} else {
			return false;
		}
#endif // _WIN32
	}

	const std::string& file_enumerator::filename() const {
		return fname;
	}

	cx::EnumFileType file_enumerator::file_type() const {
		return ftype;
	}

	int file_enumerator::filters() const {
		return this->_Filters;
	}

	void file_enumerator::filters(int newFilters) {
		this->_Filters = newFilters;
	}

	static void _get_file_count_by_depth(const std::string& dirName, int filters, int depth, int& currentDepth, int& counter) {
		if (dirName.empty()) throw std::invalid_argument("dirName");
		if (depth < 0) throw std::invalid_argument("depth");

		if (currentDepth < 1) currentDepth = 1;
		if (depth != 0 && currentDepth > depth) return;

		file_enumerator fe;
		if (filters == EFT_DIR) fe.filters(filters);

		if (!fe.begin(dirName)) return;

		do {
			EnumFileType fileType = fe.file_type();

			bool doCount = false;
			if (fileType == EFT_DIR) {
				if (filters & EFT_DIR) doCount = true;
			} else if (fileType == EFT_FILE) {
				if (filters & EFT_FILE) doCount = true;
			}

			if (doCount) {
				counter++;
			}

			if (fileType == EFT_DIR && (depth == 0 || currentDepth + 1 <= depth)) {
				currentDepth++;
				_get_file_count_by_depth(fe.filename(), filters, depth, currentDepth, counter);
				currentDepth--;
			}
		} while (fe.next());
	}

	int get_file_count(const std::string& dirName, int filters /*= EFT_DIR | EFT_FILE*/, int depth /*= 1*/) {
		int currentDepth = 1;
		int counter = 0;
		_get_file_count_by_depth(dirName, filters, depth, currentDepth, counter);
			return counter;
	}

	int get_all_file_count(const std::string& dirName, int filters /*= EFT_DIR | EFT_FILE*/) {
		int currentDepth = 1;
		int counter = 0;
		_get_file_count_by_depth(dirName, filters, 0, currentDepth, counter);
		return counter;
	}

	std::string get_current_directory() {
#ifdef _WIN32
		char buf[MAX_PATH];
		::GetCurrentDirectoryA(MAX_PATH, buf);
		return buf;
#else 
		char buf[PATH_MAX];
		char* dir = ::getcwd(buf, PATH_MAX);
		if (dir == NULL) return "";

		return dir;
#endif // _WIN32
	}

	void read_all_bytes(const std::string& filename, std::vector<unsigned char>& data) {
		if (filename.empty()) throw std::invalid_argument("filename");

		std::ifstream ifs;
		// ifs.exceptions(std::ios::failbit);
		ifs.open(filename, std::ios::binary | std::ios::ate);
		if (!ifs.is_open()) throw io_exception();

		std::ifstream::pos_type pos = ifs.tellg();
		data.resize((size_t)pos);
		ifs.seekg(0, std::ios::beg);
		ifs.read((char*)data.data(), (size_t)pos);
	}

	void write_all_bytes(const std::string& filename, const std::vector<unsigned char>& data, bool bAppend /*= false*/) {
		if (filename.empty()) throw std::invalid_argument("filename");

		write_all_bytes(filename, data.data(), data.size(), bAppend);
	}

	void write_all_bytes(const std::string& filename, const unsigned char* data, size_t length, bool bAppend /*= false*/) {
		if (filename.empty()) throw std::invalid_argument("filename");
		 
		std::ofstream ofs;
		// ofs.exceptions(std::ios::failbit);
		if (bAppend)
			ofs.open(filename, std::ios::binary | std::ios::ate | std::ios::app);
		else
			ofs.open(filename, std::ios::binary | std::ios::ate | std::ios::out);

		if (!ofs.is_open()) throw io_exception();
		ofs.write((char*)data, length);
	}

}

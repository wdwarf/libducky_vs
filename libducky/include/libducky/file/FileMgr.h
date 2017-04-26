/*
* FileMgr.h
*
*  Created on: Jul 18, 2012
*      Author: ducky
*/

#ifndef LIBDUCKY_FILE_FILEMGR_H_
#define LIBDUCKY_FILE_FILEMGR_H_

#include <libducky/Object.h>
#include <Windows.h>
#include <string>
#include <vector>

using std::_W(string);
using std::vector;

namespace ducky
{
	namespace file
	{

		class FileMgr
		{
		public:
			static int mv(_W(string) oldPath, _W(string) newPath);
			static 	int cp(_W(string) oldPath, _W(string) newPath);
			static int rm(_W(string) path);
			static int rename(_W(string) oldPath, _W(string) newPath);
			static int create(_W(string) path, bool coverExisting = false);
			static int mkdir(_W(string) dirName);
			static int rmdir(_W(string) dirName);
			static bool exist(_W(string) fileName);
			static bool isDir(_W(string) fileName);
			static vector<WIN32_FIND_DATA> listFilesAndDirs(_W(string) fileName);
			static vector<_W(string)> listFiles(_W(string) path, bool recursion = false);

			static _W(string) getModuleFileName(HMODULE hModule = NULL);
			static _W(string) getModuleFilePath(HMODULE hModule = NULL);

		private:
			FileMgr();
			virtual ~FileMgr();
		};

	}
}

#endif /* LIBDUCKY_FILE_FILEMGR_H_ */

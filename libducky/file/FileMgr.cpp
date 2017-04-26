/*
* FileMgr.cpp
*
*  Created on: Jul 18, 2012
*      Author: ducky
*/

#include "stdafx.h"
#include <libducky/file/FileMgr.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <comdef.h>
#include <boost/filesystem.hpp>

#define DBG(str) str

using namespace std;
namespace ducky
{
	namespace file
	{

		//---------------------------------------------------------------------------

		FileMgr::FileMgr()
		{
			//do nothing...

		}

		//---------------------------------------------------------------------------

		FileMgr::~FileMgr()
		{
			//do nothing...
		}

		//---------------------------------------------------------------------------

		int FileMgr::mv(_W(string) oldPath, _W(string) newPath)
		{
			FileMgr::rm(newPath);
			if (MoveFile(oldPath.c_str(), newPath.c_str()))
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}

		//---------------------------------------------------------------------------

		int FileMgr::cp(_W(string) oldPath, _W(string) newPath)
		{
			int result = -1;
			fstream oldFile;
			fstream newFile;
			oldFile.open(oldPath.c_str(), ios::in | ios::binary);
			if (oldFile.is_open())
			{
				newFile.open(newPath.c_str(), ios::out | ios::binary | ios::trunc);
				if (newFile.is_open())
				{
					char buf[1024];
					int readBytes = 0;

					while (!oldFile.eof())
					{
						oldFile.read(buf, 1024);
						readBytes = oldFile.gcount();
						if (readBytes > 0)
						{
							newFile.write(buf, readBytes);
						}
					}

					newFile.close();
					result = 0;
				}
				oldFile.close();
			}
			return result;
		}

		//---------------------------------------------------------------------------

		int FileMgr::rm(_W(string) path)
		{
			if (DeleteFile(path.c_str()))
			{
				return 0;
			}
			return -1;
		}

		//---------------------------------------------------------------------------

		int FileMgr::rename(_W(string) oldPath, _W(string) newPath)
		{
			return FileMgr::mv(oldPath, newPath);
		}

		//---------------------------------------------------------------------------

		int FileMgr::create(_W(string) path, bool coverExisting)
		{
			_W(fstream) file;
			ios_base::openmode om = ios::out | ios::binary;
			if (coverExisting)
			{
				om |= ios::trunc;
			}
			file.open(path.c_str(), om);
			if (file.is_open())
			{
				file.close();
				return 0;
			}
			else
			{
				return -1;
			}
		}

		//---------------------------------------------------------------------------

		_W(string) FileMgr::getModuleFileName(HMODULE hModule)
		{
			TCHAR cPath[1024];
			::ZeroMemory(cPath, 1024);
			GetModuleFileName(hModule, cPath, 1024);
			return cPath;
		}

		//---------------------------------------------------------------------------

		_W(string) FileMgr::getModuleFilePath(HMODULE hModule)
		{
			_W(string) path = FileMgr::getModuleFileName(hModule);
			path = path.substr(0, path.find_last_of(_T("\\")) + 1);
			return path;
		}

		//---------------------------------------------------------------------------

		int FileMgr::mkdir(_W(string) dirName)
		{
			try
			{
				boost::filesystem::path p = dirName;
				boost::filesystem::create_directory(p);
				return 0;
			}
			catch (exception& e)
			{
			}

			/*if (CreateDirectory(dirName.c_str(), NULL))
			{
				return 0;
			}*/
			return -1;
		}

		//---------------------------------------------------------------------------

		int FileMgr::rmdir(_W(string) dirName)
		{
			/*_W(string) cmd = _T("rmdir \"") + dirName + _T("\" /S /Q");
			return system(bstr_t(cmd.c_str()));*/

			try
			{
				boost::filesystem::path p = dirName;
				boost::filesystem::remove_all(p);
				return 0;
			}
			catch (exception& e)
			{
			}

			/*if (RemoveDirectory(dirName.c_str()))
			{
				return 0;
			}*/
			return -1;
		}

		//---------------------------------------------------------------------------

		bool FileMgr::exist(_W(string) fileName)
		{
			if (fileName.empty()) return false;
			DWORD attr = ::GetFileAttributes(fileName.c_str());
			if (INVALID_FILE_ATTRIBUTES != attr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//---------------------------------------------------------------------------

		bool FileMgr::isDir(_W(string) fileName)
		{
			if (fileName.empty()) return false;
			DWORD attr = ::GetFileAttributes(fileName.c_str());
			if (INVALID_FILE_ATTRIBUTES != attr &&
				attr & FILE_ATTRIBUTE_DIRECTORY)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//---------------------------------------------------------------------------

		vector<WIN32_FIND_DATA> FileMgr::listFilesAndDirs(_W(string) fileName)
		{
			vector<WIN32_FIND_DATA> list;
			WIN32_FIND_DATA d;
			memset(&d, 0, sizeof(d));
			HANDLE hFindFile = FindFirstFile(fileName.c_str(), &d);
			if (INVALID_HANDLE_VALUE != hFindFile)
			{
				_W(string) fileName = d.cFileName;
				if (_T("..") != fileName)
				{
					list.push_back(d);
				}
				while (FindNextFile(hFindFile, &d))
				{
					_W(string) fileName = d.cFileName;
					if (_T("..") != fileName)
					{
						list.push_back(d);
					}
				}
				FindClose(hFindFile);
			}
			return list;
		}

		//---------------------------------------------------------------------------

		vector<_W(string)> FileMgr::listFiles(_W(string) path, bool recursion)
		{
			vector<_W(string)> list;
			if (_T('\\') != path[path.length() - 1])
			{
				path += _T("\\");
			}
			_W(string) filePath = path + _T("*");

			vector<WIN32_FIND_DATA> tmpList = FileMgr::listFilesAndDirs(filePath);
			for (vector<WIN32_FIND_DATA>::iterator it = tmpList.begin();
				it != tmpList.end(); ++it)
			{
				_W(string) fileName = (*it).cFileName;
				if (_T("..") == fileName) continue;

				if (!((*it).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					list.push_back(fileName);
				}
				else
					if (recursion)
					{
						vector<_W(string)> tmpList2 = listFiles(path + fileName + _T("\\"), recursion);
						for (vector<_W(string)>::iterator it = tmpList2.begin();
							it != tmpList2.end(); ++it)
						{
							list.push_back(fileName + _T("\\") + *it);
						}
					}
			}

			return list;
		}


	}
}
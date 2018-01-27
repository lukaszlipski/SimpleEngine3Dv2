#include "File.h"
#include "Window.h"

namespace SE3D2
{

	FileHandle* File::OpenRead(const std::string& path)
	{
		HANDLE Handle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (Handle == INVALID_HANDLE_VALUE)
		{
			return nullptr;
		}
		return new SE3D2::FileHandle(Handle);
	}

	FileHandle* File::OpenWrite(const std::string& path)
	{
		HANDLE Handle = CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
		if (Handle == INVALID_HANDLE_VALUE)
		{
			return nullptr;
		}
		return new SE3D2::FileHandle(Handle);
	}

	FileHandle* File::OpenReadWrite(const std::string& path)
	{
		HANDLE Handle = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
		if (Handle == INVALID_HANDLE_VALUE)
		{
			return nullptr;
		}
		return new SE3D2::FileHandle(Handle);
	}

	void File::CloseFile(FileHandle* handle)
	{
		CloseHandle(handle->mHandle);
		if (handle) 
		{
			delete handle;
		}
	}

	bool File::Exists(const std::string& file)
	{
		DWORD dirAttrib = GetFileAttributes(file.c_str());
		return (dirAttrib != INVALID_FILE_ATTRIBUTES && !(dirAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool File::DirectoryExists(const std::string& directory)
	{
		DWORD dirAttrib = GetFileAttributes(directory.c_str());
		return (dirAttrib != INVALID_FILE_ATTRIBUTES && (dirAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	SE3D2::DateTime File::GetLastModificationTime(const std::string& file) const
	{
		FILETIME time;
		SYSTEMTIME sysTime;
		HANDLE Handle = CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (Handle == INVALID_HANDLE_VALUE)
		{
			CloseHandle(Handle);
			return DateTime{ 0,0,0,0,0,0 };
		}
		GetFileTime(Handle, 0, 0, &time);
		FileTimeToSystemTime(&time, &sysTime);

		DateTime result;
		result.Year = sysTime.wYear;
		result.Month = sysTime.wMonth;
		result.Day = sysTime.wDay;
		result.Hour = sysTime.wHour;
		result.Minute = sysTime.wMinute;
		result.Seconds = sysTime.wSecond;

		CloseHandle(Handle);

		return result;
	}

	SE3D2::DateTime File::GetLastModificationTime(FileHandle* handle) const
	{
		FILETIME time;
		SYSTEMTIME sysTime;
		
		GetFileTime(handle->mHandle, 0, 0, &time);
		FileTimeToSystemTime(&time, &sysTime);

		DateTime result;
		result.Year = sysTime.wYear;
		result.Month = sysTime.wMonth;
		result.Day = sysTime.wDay;
		result.Hour = sysTime.wHour;
		result.Minute = sysTime.wMinute;
		result.Seconds = sysTime.wSecond;

		return result;
	}

	int64 File::Size(const std::string& path)
	{
		HANDLE Handle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if (Handle == INVALID_HANDLE_VALUE) 
		{ 
			return -1; 
		}

		LARGE_INTEGER size;
		GetFileSizeEx(Handle, &size);
		CloseHandle(Handle);

		return size.QuadPart;

	}

	int64 File::Size(FileHandle* handle)
	{
		LARGE_INTEGER size;
		GetFileSizeEx(handle->mHandle, &size);
		return size.QuadPart;

	}

	std::string File::CurrentDirectory()
	{
		char Tmp[512];
		GetCurrentDirectory(512, &Tmp[0]);

		std::string Buffer(Tmp);

		for (auto& i : Buffer)
		{
			if (i == '\\')
			{
				i = '/';
			}
			else if (i == '\0')
			{
				break;
			}
		}


		return Buffer;
	}


	bool File::Delete(const std::string& file)
	{
		return DeleteFile(file.c_str());
	}

	bool File::DeleteDirectory(const std::string& directory)
	{
		return RemoveDirectory(directory.c_str());
	}

	bool File::MakeDirectory(const std::string& directory)
	{
		return CreateDirectory(directory.c_str(), 0);
	}

	bool File::GetFiles(std::vector<std::string>& files, std::string directory)
	{
		if (directory.size() == 0) return false;
		if (directory.back() != '/')
		{
			directory.push_back('/');
		}
		directory.push_back('*');
		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile(directory.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE) return false;

		files.clear();
		do
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { files.push_back(findData.cFileName); }
		} 
		while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);
		return true;
	}

	bool File::GetDirectories(std::vector<std::string>& directories, std::string directory)
	{
		if (directory.size() == 0) return false;
		if (directory.back() != '/')
		{
			directory.push_back('/');
		}
		directory.push_back('*');
		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile(directory.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE) return false;

		directories.clear();
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { directories.push_back(findData.cFileName); }
		} 
		while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);
		return true;
	}

	int32 File::Read(void* handle, void* buffer, int32 size, int32 read)
	{
		DWORD Read = read;
		bool result = ReadFile(handle, buffer, size, &Read, 0);
		if (!result) 
		{ 
			return 0; 
		}
		return Read;
	}

	bool File::Write(void* handle, const void* buffer, int32 size, bool append /*= false*/)
	{
		if (append)
		{
			SetFilePointer(handle, 0, NULL, FILE_END);
		}
		DWORD written = 0;
		bool result = WriteFile(handle, buffer, size, &written, 0);
		SetFilePointer(handle, 0, NULL, 0);
		if (!written) 
		{ 
			return false;
		}

		return true;
	}

	bool FileHandle::Read(uint8* buffer, int32 size)
	{
		int32 read = File::Get().Read(mHandle, buffer, size, mRead);
		if (!read)
		{
			return false;
		}
		mRead = read;
		return true;
	}

	bool FileHandle::Read(std::string& buffer, int32 size /*= 0*/)
	{
		if (!size)
		{
			size = static_cast<int32>(File::Get().Size(this));
		}
		buffer.resize(size);
		int32 read = File::Get().Read(mHandle, &buffer[0], size, mRead);
		if (!read)
		{
			return false;
		}
		mRead = read;
		return true;
	}

	bool FileHandle::Write(const uint8* buffer, int32 size, bool append /*= false*/)
	{
		return File::Get().Write(mHandle, buffer, size, append);
	}

	bool FileHandle::Write(const std::string& buffer, bool append /*= false*/)
	{
		return File::Get().Write(mHandle, &buffer[0], static_cast<int32>(buffer.length()), append);
	}

	FileHandle::FileHandle(void* Handle)
		: mHandle(Handle)
	{
	}

}
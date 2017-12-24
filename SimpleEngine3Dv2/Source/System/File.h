#pragma once
#include "..\Utility\Types.h"
#include "..\Utility\DateTime.h"
#include <string>
#include <vector>

namespace SE3D2
{
	class File;

	class FileHandle
	{
	public:
		bool Read(uint8* buffer, int32 size);
		bool Read(std::string& buffer, int32 size = 0);
		bool Write(uint8* buffer, int32 size, bool append = false);
		bool Write(std::string& buffer, bool append = false);

	private:
		void* mHandle;
		int32 mRead;

		FileHandle(void* Handle);
		friend File;

	};

	class File
	{
	public:
		static File& Get()
		{
			static File* instance = new File();
			return *instance;
		}

		bool Startup() { return true; };
		bool Shutdown() { return true; }

		class FileHandle* OpenRead(const std::string& path);
		class FileHandle* OpenWrite(const std::string& path);
		class FileHandle* OpenReadWrite(const std::string& path);
		void CloseFile(FileHandle* handle);

		bool Exists(const std::string& file);
		bool DirectoryExists(const std::string& directory);
		DateTime GetLastModificationTime(const std::string& file) const;
		DateTime GetLastModificationTime(FileHandle* handle) const;
		int64 Size(const std::string& path);
		int64 Size(FileHandle* handle);
		std::string CurrentDirectory();
		bool Delete(const std::string& file);
		bool DeleteDirectory(const std::string& directory);
		bool MakeDirectory(const std::string& directory);

		bool GetFiles(std::vector<std::string>& files, std::string directory);
		bool GetDirectories(std::vector<std::string>& directories, std::string directory);

	private:

		friend FileHandle;
		int32 Read(void* handle, void* buffer, int32 size, int32 read);
		bool Write(void* handle, void* buffer, int32 size, bool append = false);

	};

}
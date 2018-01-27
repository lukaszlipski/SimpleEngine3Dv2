#include "Shader.h"
#include "..\System\File.h"
#include "..\System\Graphics.h"

namespace SE3D2
{

	ParametersBuffer* Shader::FindParametersBuffer(const std::string& name) const
	{
		for (auto& buffer : mParametersBuffers)
		{
			if (buffer->GetName() == name)
			{
				return buffer.get();
			}
		}
		return nullptr;
	}

	bool Shader::TryToFindCompiledFile(const std::string& name, std::string& source)
	{
		std::string CurrentDir = File::Get().CurrentDirectory();

		std::string CompiledFilePath = CurrentDir + "/Shader/" + Graphics::Get().GetContext()->GetName() + "/" + name + "." + GetCompiledExtension();
		DateTime CompileDateTime = File::Get().GetLastModificationTime(CompiledFilePath);
		if (!CompileDateTime.IsValid())
		{
			return false;
		}

		std::string SourceFilePath = CurrentDir + "/Source/Shader/" + Graphics::Get().GetContext()->GetName() + "/" + name + "." + GetExtension();
		DateTime SourceDateTime = File::Get().GetLastModificationTime(SourceFilePath);

		// Source file is newer
		if (SourceDateTime >= CompileDateTime)
		{
			return false;
		}

		// Try to find compiled file
		FileHandle* CompiledFile = File::Get().OpenRead(CompiledFilePath);
		if (CompiledFile)
		{
			CompiledFile->Read(source);
			File::Get().CloseFile(CompiledFile);
			return true;
		}

		return false;
	}

	bool Shader::TryToFindSourceFile(const std::string& name, std::string& source)
	{
		std::string CurrentDir = File::Get().CurrentDirectory();
		std::string SourceFilePath = CurrentDir + "/Source/Shader/" + Graphics::Get().GetContext()->GetName() + "/" + name + "." + GetExtension();

		// Try to find source file
		FileHandle* SourceFile = File::Get().OpenRead(SourceFilePath);
		if (SourceFile)
		{
			SourceFile->Read(source);
			File::Get().CloseFile(SourceFile);
			return true;
		}

		return false;
	}

	bool Shader::SaveCompiledFile(const std::string& name, const void* binary, int32 size)
	{
		std::string CurrentDir = File::Get().CurrentDirectory();
		std::string CompiledFilePath = CurrentDir + "/Shader/" + Graphics::Get().GetContext()->GetName() + "/" + name + "." + GetCompiledExtension();

		FileHandle* SourceFile = File::Get().OpenWrite(CompiledFilePath);
		if (SourceFile)
		{
			SourceFile->Write(reinterpret_cast<const uint8*>(binary),size);
			File::Get().CloseFile(SourceFile);
			return true;
		}

		return false;
	}

}
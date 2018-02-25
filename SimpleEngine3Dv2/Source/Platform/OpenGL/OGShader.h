#pragma once
#include "..\Graphic\API\Shader.h"
#include "GL\glew.h"
#include "..\Utility\Path.h"

namespace SE3D2
{
	class OGFragmentShaderPolicy
	{
	public:
		inline uint32 CreateShader() { return glCreateShader(GL_FRAGMENT_SHADER); }
	};

	class OGVertexShaderPolicy
	{
	public:
		inline uint32 CreateShader() { return glCreateShader(GL_VERTEX_SHADER); }
	};

	template<typename T>
	class OGShader : public Shader
	{

	public:
		OGShader(const std::string& name)
			: Shader(name)
		{ }

		~OGShader()
		{
			glDeleteProgram(mProgram);
		}

		virtual bool Compile(const std::string& name) override;
		virtual void Bind() override;
		virtual void SetParametersBuffer(ParametersBuffer* pb) override;
		virtual std::string GetExtension() const override { return "glsl"; }

		inline uint32 GetProgram() const { return mProgram; }

	protected:
		virtual bool CollectResources(const std::string& source) override;

	private:
		T mShaderPolicy;
		uint32 mProgram;

		bool GenerateShader(uint32& shader);
		bool GenerateProgram(uint32 shader);
		void Save(const std::string& name);

	};

	template<typename T>
	bool OGShader<T>::Compile(const std::string& name)
	{
		if (Path::GetExtension(name) != "")
		{
			return false;
		}

		bool Compiled = false;
		mProgram = glCreateProgram();

		if (TryToFindCompiledFile(name, mSource))
		{
			// Try using binary version

			int32 BinarySize;
			uint32 BinaryFormat;

			memcpy(&BinarySize, &mSource.data()[mSource.size() - sizeof(int32) - sizeof(uint32)], 4);
			memcpy(&BinaryFormat, &mSource.data()[mSource.size() - sizeof(uint32)], 4);

			glProgramBinary(mProgram, BinaryFormat, mSource.data(), BinarySize);

			// Check for error
			GLenum Error = glGetError();
			if (Error != GL_INVALID_OPERATION && Error != GL_INVALID_ENUM)
			{ 
				Compiled = true;
			}

		}

		if (!Compiled)
		{
			if (TryToFindSourceFile(name, mSource))
			{
				
				uint32 Shader;
				if (!GenerateShader(Shader)) { return false; }
				
				if (!GenerateProgram(Shader)) { return false; }
				
				Save(name);

				glDeleteShader(Shader);
				
			}
			else
			{
				return false;
			}
		}


		return CollectResources(mSource);
	}

	template<typename T>
	bool OGShader<T>::GenerateShader(uint32& shader)
	{
		shader = mShaderPolicy.CreateShader();

		const char *Source = mSource.c_str();
		glShaderSource(shader, 1, &Source, 0);
		glCompileShader(shader);

		// check for errors
		int32_t logStatus;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &logStatus);

		if (logStatus != GL_TRUE)
		{
			int32_t logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			std::string error;
			error.reserve(logLength);
			glGetShaderInfoLog(shader, logLength, 0, &error[0]);
			// #TODO : Log errors

			return false;
		}
		return true;
	}


	template<typename T>
	bool OGShader<T>::GenerateProgram(uint32 shader)
	{
		// Create separate program from shader
		glAttachShader(mProgram, shader);
		glProgramParameteri(mProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

		// Enable saving binary version of shaders
		glProgramParameteri(mProgram, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

		// Check for errors
		int32_t logStatus;
		glLinkProgram(mProgram);
		glGetProgramiv(mProgram, GL_LINK_STATUS, &logStatus);

		if (logStatus != GL_TRUE)
		{
			int32_t logLength;
			glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);
			std::string error;
			error.reserve(logLength);
			glGetProgramInfoLog(mProgram, logLength, 0, &error[0]);
			// #TODO : Log errors

			return false;
		}
		return true;
	}


	template<typename T>
	void SE3D2::OGShader<T>::Save(const std::string& name)
	{
		int32 BinarySize;
		glGetProgramiv(mProgram, GL_PROGRAM_BINARY_LENGTH, &BinarySize);

		std::vector<uint8> BinaryProgram;
		uint32 BufferSize = BinarySize + sizeof(int32) + sizeof(uint32);
		BinaryProgram.resize(BufferSize);

		uint32 BinaryFormat;
		glGetProgramBinary(mProgram, BinarySize, 0, &BinaryFormat, &BinaryProgram[0]);

		memcpy(&BinaryProgram[BufferSize - sizeof(int32) - sizeof(uint32)], &BinarySize, 4);
		memcpy(&BinaryProgram[BufferSize - sizeof(uint32)], &BinaryFormat, 4);

		mSource.clear();
		// #TODO : Save binary program to mSource

		SaveCompiledFile(name, BinaryProgram.data(), static_cast<int32>(BinaryProgram.size()));
	}


	template<typename T>
	bool OGShader<T>::CollectResources(const std::string& source)
	{
		return true;
	}

	template<typename T>
	void OGShader<T>::SetParametersBuffer(ParametersBuffer* pb)
	{

	}

	template<typename T>
	void OGShader<T>::Bind()
	{
		glUseProgram(mProgram);
	}

	using OGVertexShader = OGShader<OGVertexShaderPolicy>;
	using OGFragmentShader = OGShader<OGFragmentShaderPolicy>;

}
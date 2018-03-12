#pragma once
#include "..\Graphic\API\Shader.h"
#include "GL\glew.h"
#include "..\Utility\Path.h"
#include "..\System\Graphics.h"
#include <assert.h>
#include "OGParamtersBuffer.h"
#include "..\Graphic\API\Buffer.h"

namespace SE3D2
{
	class OGFragmentShaderPolicy
	{
	public:
		inline uint32 CreateShader() { return glCreateShader(GL_FRAGMENT_SHADER); }
		inline ShaderType GetType() { return ShaderType::PIXEL; }
	};

	class OGVertexShaderPolicy
	{
	public:
		inline uint32 CreateShader() { return glCreateShader(GL_VERTEX_SHADER); }
		inline ShaderType GetType() { return ShaderType::VERTEX; }
	};

	template<typename T>
	class OGShader : public Shader
	{

	public:
		OGShader(const std::string& name)
			: Shader(name)
		{ 
			mType = mShaderPolicy.GetType();
		}

		~OGShader()
		{
			glDeleteProgram(mProgram);
		}

		virtual bool Compile(const std::string& name) override;
		virtual void SetParametersBuffer(ParametersBuffer* pb, uint32 globalSlot = 0) override;
		virtual bool SetStructuredBuffer(const std::string& name, StructuredBuffer* sb) override;
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
	bool OGShader<T>::SetStructuredBuffer(const std::string& name, StructuredBuffer* sb)
	{
		return false;
	}

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
		// Clear previous buffer if there was any
		mParametersBuffers.clear();

		GLint NumUniforms;
		GLint NumBlockUniforms;
		glGetProgramInterfaceiv(mProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &NumUniforms);
		glGetProgramInterfaceiv(mProgram, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &NumBlockUniforms);

		GLenum PropertiesBlock[3] = { GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE };
		for (int32_t i = 0; i < NumBlockUniforms; ++i)
		{
			GLint ValuesBlock[3];
			glGetProgramResourceiv(mProgram, GL_UNIFORM_BLOCK, i, 3, PropertiesBlock, 3, 0, ValuesBlock);
			char* Name = (char*)alloca(ValuesBlock[0]);
			glGetProgramResourceName(mProgram, GL_UNIFORM_BLOCK, i, ValuesBlock[0], &ValuesBlock[0], Name);
			GLuint location = glGetUniformBlockIndex(mProgram, Name);

			// Create parameters buffer and add to mParametersBuffer
			ParametersBuffer* ParamBuffer = Graphics::Get().GetContext()->CreateParametersBuffer(Name, ValuesBlock[2], location);
			if (!ParamBuffer)
			{
				return false;
			}
			std::unique_ptr<ParametersBuffer> UniqueBuffer(ParamBuffer);
			mParametersBuffers.push_back(std::move(UniqueBuffer));
		}

		GLenum Properties[6] = { GL_BLOCK_INDEX, GL_TYPE, GL_OFFSET, GL_NAME_LENGTH, GL_LOCATION, GL_ARRAY_SIZE };
		for (int32_t i = 0; i < NumUniforms; ++i)
		{
			GLint Values[6];
			glGetProgramResourceiv(mProgram, GL_UNIFORM, i, 6, Properties, 6, 0, Values);
			char* Name = (char*)alloca(Values[3]);
			glGetProgramResourceName(mProgram, GL_UNIFORM, i, Values[3], &Values[3], Name);

			Parameter param;
			param.mName = Name;
			param.mOffset = Values[2];
			
			if (Values[1] == GL_FLOAT) 
			{
				param.mType = ParameterType::FLOAT;
			}
			else if (Values[1] == GL_INT)
			{
				param.mType = ParameterType::INT32;
			}
			else if (Values[1] = GL_FLOAT_VEC2)
			{
				param.mType = ParameterType::VECTOR2;
			}
			else if (Values[1] = GL_FLOAT_VEC3)
			{
				param.mType = ParameterType::VECTOR3;
			}
			else if (Values[1] = GL_FLOAT_VEC4)
			{
				param.mType = ParameterType::VECTOR4;
			}
			else if (Values[1] = GL_FLOAT_MAT3)
			{
				param.mType = ParameterType::MATRIX3x3;
			}
			else if (Values[1] = GL_FLOAT_MAT4)
			{
				param.mType = ParameterType::MATRIX4x4;
			}
			else
			{
				// #TODO: add to log (unsupported type)
				param.mType = ParameterType::NONE;
				assert(false);
			}

			for (auto& buffer : mParametersBuffers)
			{
				if (buffer->GetSlot() == Values[0])
				{
					buffer->AddParameter(param);
				}
			}

		}

		return true;
	}

	template<typename T>
	void OGShader<T>::SetParametersBuffer(ParametersBuffer* pb, uint32 globalSlot)
	{
		for (auto& paramBuffer : mParametersBuffers)
		{
			if (paramBuffer->GetName() == pb->GetName() && paramBuffer->GetDataSize() == pb->GetDataSize() && paramBuffer->GetSlot() == pb->GetSlot())
			{
				pb->Update();
				OGParametersBuffer* OgPb = static_cast<OGParametersBuffer*>(pb);
				
				glBindBuffer(GL_UNIFORM_BUFFER, OgPb->GetBuffer()->GetBuffer());
				glUniformBlockBinding(GetProgram(), OgPb->GetSlot(), globalSlot);
				glBindBufferBase(GL_UNIFORM_BUFFER, globalSlot, OgPb->GetBuffer()->GetBuffer());
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
		}
	}

	using OGVertexShader = OGShader<OGVertexShaderPolicy>;
	using OGFragmentShader = OGShader<OGFragmentShaderPolicy>;

}
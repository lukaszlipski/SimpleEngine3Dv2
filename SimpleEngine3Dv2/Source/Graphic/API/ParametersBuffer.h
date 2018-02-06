#pragma once
#include "..\Utility\Types.h"
#include <string>
#include <vector>
#include <memory>

namespace SE3D2
{
	enum class ParameterType
	{
		NONE = 0,
		INT32,
		FLOAT,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		MATRIX3x3,
		MATRIX4x4
	};

	struct Parameter
	{
		std::string mName;
		ParameterType mType;
		int32 mOffset;
	};

	struct ParametersBuffer
	{
	public:
		ParametersBuffer(const std::string& name, int32 size, int32 slot)
			: mDataSize(size), mName(name), mData(new uint8[size]), mSlot(slot), mDirty(false)
		{
			// Clear data memory
			memset(mData.get(), 0, mDataSize);
		}

		ParametersBuffer(const ParametersBuffer& copy)
			: mDataSize(copy.mDataSize), mData(new uint8[copy.mDataSize]), mName(copy.mName), mVariables(copy.mVariables), mSlot(copy.mSlot), mDirty(copy.mDirty)
		{
			memcpy(mData.get(), copy.mData.get(), mDataSize);
		}

		virtual ~ParametersBuffer() = default;

		void AddParameter(const Parameter& param) { mVariables.push_back(param); }
		inline const std::string& GetName() const { return mName; }
		inline int32 GetDataSize() const { return mDataSize; }
		inline int32 GetSlot() const { return mSlot; }
		inline void SetDirty(bool isDirty) { mDirty = isDirty; }

		virtual void ClearResources() = 0;
		virtual void Update() = 0;
		virtual bool SetFloat(const std::string& name, float value) = 0;
		virtual bool SetInt32(const std::string& name, int32 value) = 0;

	protected:
		std::string mName;
		std::unique_ptr<uint8> mData;
		int32 mDataSize;
		std::vector<Parameter> mVariables;
		int32 mSlot;
		bool mDirty;

	};
}
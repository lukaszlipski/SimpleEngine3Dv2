#pragma once
#include "..\Graphic\API\ParametersBuffer.h"
#include "OGBuffer.h"

namespace SE3D2
{
	class OGParametersBuffer : public ParametersBuffer
	{
	public:
		OGParametersBuffer(const std::string& name, int32 size, int32 slot, OGUniformBlockBuffer* buffer)
			: ParametersBuffer(name, size, slot), mBuffer(buffer)
		{ }

		virtual void ClearResources() override
		{
			mBuffer->ClearResource();
		}

		virtual void Update() override
		{
			if (mDirty)
			{
				mBuffer->Update(mData.get());
				SetDirty(false);
			}
		}

		virtual bool SetFloat(const std::string& name, float value) override
		{
			for (auto& var : mVariables)
			{
				if (var.mName == name && var.mType == ParameterType::FLOAT)
				{
					memcpy(mData.get() + var.mOffset, &value, sizeof(float));
					SetDirty(true);
					return true;
				}
			}
			return false;
		}

		virtual bool SetInt32(const std::string& name, int32 value) override
		{
			for (auto& var : mVariables)
			{
				if (var.mName == name && var.mType == ParameterType::INT32)
				{
					memcpy(mData.get() + var.mOffset, &value, sizeof(int32));
					SetDirty(true);
					return true;
				}
			}
			return false;
		}

		OGUniformBlockBuffer* GetBuffer() const { return mBuffer; }

	private:
		OGUniformBlockBuffer* mBuffer;

	};
}
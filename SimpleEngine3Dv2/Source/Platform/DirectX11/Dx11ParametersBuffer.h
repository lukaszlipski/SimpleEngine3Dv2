#pragma once
#include "..\Graphic\API\ParametersBuffer.h"
#include <d3d11.h>
#include "..\System\Graphics.h"
#include "Dx11Context.h"
#include "Dx11Buffer.h"

namespace SE3D2
{
	class Dx11ParametersBuffer : public ParametersBuffer
	{

	public:
		Dx11ParametersBuffer(const std::string& name, int32 size, int32 slot, Dx11ConstantBuffer* buffer)
			: ParametersBuffer(name, size, slot), mBuffer(buffer)
		{ }

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

		ID3D11Buffer* GetConstantBuffer() const { return mBuffer->GetBuffer(); }

		virtual void Update() override
		{
			if (mDirty)
			{
				mBuffer->Update(mBuffer->GetSize(), 0, mData.get());
				SetDirty(false);
			}
		}

		virtual void ClearResources() override
		{
			mBuffer->ClearResource();
		}

	private:
		Dx11ConstantBuffer* mBuffer;

	};
}
#pragma once
#include <string>
#include "Types.h"

namespace SE3D2
{

	class ImageLoader
	{
	public:
		ImageLoader(const char* path)
			: mPath(path)
		{ }

		virtual ~ImageLoader()
		{
			delete[] mPixels;
		}

		virtual bool TryLoad() = 0;

		inline int32 GetWidth() const { return mWidth; }
		inline int32 GetHeight() const { return mHeight; }
		inline int8 GetBytesPerPixel() const { return mBytesPerPixel; }
		inline uint8* GetPixels() const { return mPixels; }

	protected:
		std::string mPath;
		int32 mWidth = 0;
		int32 mHeight = 0;
		int8 mBytesPerPixel = 0;
		uint8* mPixels;

	};

	class TgaLoader : public ImageLoader
	{
	public:
		TgaLoader(const char* path)
			: ImageLoader(path)
		{ }

		virtual bool TryLoad() override;

	private:
		bool Postprocess(int type);

	};


	class Image
	{

	public:
		Image(const char* path);

		bool TryLoad();

		~Image()
		{
			delete mLoader;
		}

		inline int32 GetWidth() const { return mLoader->GetWidth(); }
		inline int32 GetHeight() const { return mLoader->GetHeight(); }
		inline int8 GetBytesPerPixel() const { return mLoader->GetBytesPerPixel(); }
		inline uint8* GetPixels() const { return mLoader->GetPixels(); }

	private:
		ImageLoader* mLoader = nullptr;

	};
}
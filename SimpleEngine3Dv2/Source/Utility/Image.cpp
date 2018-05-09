#include "Image.h"
#include "../System/File.h"

namespace SE3D2
{
	bool TgaLoader::TryLoad()
	{
#pragma pack(push,1)
		struct Header
		{
			int8 IDLength;
			int8 ColorMapType;
			int8 ImageTypeCode;
			int16 ColorMapOrigin;
			int16 ColorMapLength;
			int8 ColorMapDepth;
			int16 OriginX;
			int16 OriginY;
			int16 Width;
			int16 Height;
			int8 BitsPerPixel;
			int8 ImageDescriptor;
		};
#pragma pack(pop)

		if (!File::Get().Exists(mPath))
		{
			// #TODO: add to log
			return false;
		}

		FileHandle* Handle = File::Get().OpenRead(mPath);
		Header HeaderBuffer;
		Handle->Read(reinterpret_cast<uint8*>(&HeaderBuffer), sizeof(Header));

		// #TODO: add support for color map
		if (HeaderBuffer.ColorMapDepth > 0)
		{
			File::Get().CloseFile(Handle);
			return false;
		}
		
		// Save simple properties
		mWidth = HeaderBuffer.Height;
		mHeight = HeaderBuffer.Width;
		mBytesPerPixel = HeaderBuffer.BitsPerPixel / 8;
		mPixels = new uint8[mWidth * mHeight * mBytesPerPixel];
	
		// Pass by unnecessary data
		if(HeaderBuffer.IDLength > 0)
		{
			std::string Tmp;
			Handle->Read(Tmp, HeaderBuffer.IDLength);
		}

		// Uncompressed rgb
		if (HeaderBuffer.ImageTypeCode == 2)
		{
			Handle->Read(mPixels, mWidth * mHeight * mBytesPerPixel);
		}
		else
		{
			// #TODO: add support for other types
			File::Get().CloseFile(Handle);
			return false;
		}

		File::Get().CloseFile(Handle);
		return true;
	}

	Image::Image(const char* path)
	{
		std::string Ext = Path::GetExtension(path);
		
		if (Ext == "tga")
		{
			mLoader = new TgaLoader(path);
		}
		else
		{
			// #TODO: unsupported format
		}
	}

	bool Image::TryLoad()
	{
		if (!mLoader)
		{
			return false;
		}

		return mLoader->TryLoad();
	}

}


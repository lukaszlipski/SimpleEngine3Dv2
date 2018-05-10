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
			Postprocess(2);
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

	bool TgaLoader::Postprocess(int type)
	{
		struct Pixel
		{
			int8 R;
			int8 G;
			int8 B;
			int8 A;
		};

		if (type == 2)
		{
			uint8* LineCpy = new uint8[mBytesPerPixel * mWidth];

			for (int32 h = 0; h < mHeight/2; ++h)
			{
				uint8* CurrentLine = mPixels + (h * mWidth * mBytesPerPixel);
				uint8* OppositeLine = mPixels + ((mHeight - h - 1) * mWidth * mBytesPerPixel);
				int32 LineSize = mWidth * mBytesPerPixel;

				// swap B and R
				for (int32 w = 0; w < mWidth; ++w)
				{
					int32 B = w * mBytesPerPixel;
					int32 R = (w * mBytesPerPixel) + 2;

					uint8 Tmp = CurrentLine[B];
					CurrentLine[B] = CurrentLine[R];
					CurrentLine[R] = Tmp;

					Tmp = OppositeLine[B];
					OppositeLine[B] = OppositeLine[R];
					OppositeLine[R] = Tmp;
				}

				// Swap lines
				memcpy(LineCpy, CurrentLine, LineSize);
				memcpy(CurrentLine, OppositeLine, LineSize);
				memcpy(OppositeLine, LineCpy, LineSize);
			}

			// Odd case
			if (mHeight % 2 != 0)
			{
				uint8* OddLine = mPixels + ((mHeight / 2 + 1) * mWidth * mBytesPerPixel);

				for (int32 w = 0; w < mWidth; ++w)
				{
					int32 B = w * mBytesPerPixel;
					int32 R = (w * mBytesPerPixel) + 2;

					uint8 Tmp = OddLine[B];
					OddLine[B] = OddLine[R];
					OddLine[R] = Tmp;
				}
			}

			return true;
		}
		return false;
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


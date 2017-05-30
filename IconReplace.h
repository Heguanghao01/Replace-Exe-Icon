#ifndef ICONREPLACE_H
#define ICONREPLACE_H
#include <Windows.h>

#include <vector>
#include <string>

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
	BYTE bWidth; // Width, in pixels, of the image
	BYTE bHeight; // Height, in pixels, of the image
	BYTE bColorCount; // Number of colors in image (0 if >=8bpp)
	BYTE bReserved; // Reserved ( must be 0)
	WORD wPlanes; // Color Planes
	WORD wBitCount; // Bits per pixel
	DWORD dwBytesInRes; // How many bytes in this resource?
	DWORD dwImageOffset; // Where in the file is this image?
} sMemIconDirEntry;


typedef struct  
{
	BYTE    bWidth;               // Width, in pixels, of the image   
	BYTE    bHeight;              // Height, in pixels, of the image   
	BYTE    bColorCount;          // Number of colors in image (0 if >=8bpp)   
	BYTE    bReserved;            // Reserved   
	WORD    wPlanes;              // Color Planes   
	WORD    wBitCount;            // Bits per pixel   
	DWORD   dwBytesInRes;         // how many bytes in this resource?   
	WORD    nID;                  // the ID   
}sIconDirEntry;

typedef struct
{  
	WORD idReserved;   // Reserved (must be 0)   
	WORD idType;       // Resource type (1 for icons)   
	WORD idount;      // How many images?   
	sIconDirEntry  idEntries[1]; // The entries for each image   
} sGrpIconDir, *psGrpIconDir;  
#pragma pack( pop )

typedef struct Resourcedata
{
	DWORD dwResourseSize;// 
	BYTE * pData;
	Resourcedata()
	{
		dwResourseSize = 0;
		pData = NULL;
	}
}sResource;



typedef struct
{
	WORD idReserved;   // Reserved (must be 0)
	WORD idType;       // Resource Type (1 for icons)
	WORD idCount;      // How many images?
	sMemIconDirEntry   idEntries[1]; // An entry for each image (idCount of 'em)
}sIconDir,*psIconDir;

typedef struct Iconddatainfo
{
	sMemIconDirEntry memIconDirEntry;
	char *pIconImage;
	Iconddatainfo()
	{
		pIconImage = NULL;
	}

}sIconDataInfo;


typedef struct Laninfo
{
	LPCTSTR pResName;
	WORD idLanId;
	Laninfo()
	{
		pResName = NULL;
		idLanId = 0;
	}
}sLanInfo;


typedef struct Resourceinfo
{
	LPCTSTR pResName;
	DWORD dwResId;
	bool bFlag;
	Resourceinfo()
	{
		pResName = NULL;
		dwResId = 0;
		bFlag = false;
	}

}sResourceInfo;

class IconReplace
{
public:
	   IconReplace(  std::wstring iconfile_path,
		   std::wstring exepath):m_IconFilePath(iconfile_path),m_ExePath(exepath),m_pGroupIcon(NULL),m_IsExistIcon(false)
	   {
	   };
	   ~IconReplace( );
public:
	 bool ParseResource();
	 bool ParseIconFile();
	 bool UpdateResource();
	 bool ReplaceIcon();

private:
public:
	std::vector<sResourceInfo>   m_GrupIcon;
	std::vector<sResourceInfo>   m_VerInfo;
	std::vector<sResourceInfo>   m_ManifestInfo;
	std::vector<sLanInfo>  m_IconLanInfo;
	std::vector<sLanInfo>   m_VerLanInfo;
	std::vector<sLanInfo>   m_ManifestLanInfo;


private:
	std::wstring m_IconFilePath;
	std::wstring m_ExePath;
	std::vector<sIconDataInfo> m_vIconData;//ICON文件中的信息
	psGrpIconDir m_pGroupIcon;
	bool m_IsExistIcon;
	sResource m_VerData;
	sResource m_ManifestData;
};
#endif

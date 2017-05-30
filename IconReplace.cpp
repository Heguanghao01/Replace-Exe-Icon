#include "IconReplace.h"

BOOL CALLBACK enumresprocedure( HMODULE  h_module, LPCTSTR  lpType, LPTSTR  lpszName,LONG_PTR lParam);
BOOL CALLBACK enumreslangproc(HMODULE  h_module,LPCTSTR  lpszType,LPCTSTR  lpszName,WORD  wIDLanguage,LONG_PTR lParam);
IconReplace::~IconReplace( )
{
	for (std::vector<sResourceInfo>::iterator it = m_GrupIcon.begin();it != m_GrupIcon.end();it++)
	{
		if (NULL !=it->pResName)
		{
			delete [] it->pResName;
			it->pResName = NULL;
		}

	}


	for (std::vector<sResourceInfo>::iterator it = m_VerInfo.begin();it != m_VerInfo.end();it++)
	{
		if (NULL !=it->pResName)
		{
			delete [] it->pResName;
			it->pResName = NULL;
		}

	}

	for (std::vector<sResourceInfo>::iterator it = m_ManifestInfo.begin();it != m_ManifestInfo.end();it++)
	{
		if (NULL !=it->pResName)
		{
			delete [] it->pResName;
			it->pResName = NULL;
		}

	}

	for (std::vector<sLanInfo>::iterator it = m_IconLanInfo.begin();it != m_IconLanInfo.end();it++)
	{
		if (NULL !=it->pResName)
		{
			delete [] it->pResName;
			it->pResName = NULL;
		}

	}

	for (std::vector<sLanInfo>::iterator it = m_VerLanInfo.begin();it != m_VerLanInfo.end();it++)
	{
		if (NULL !=it->pResName)
		{
			delete [] it->pResName;
			it->pResName = NULL;
		}

	}

	for (std::vector<sLanInfo>::iterator it = m_ManifestLanInfo.begin();it != m_ManifestLanInfo.end();it++)
	{
		if (NULL !=it->pResName)
		{
			delete [] it->pResName;
			it->pResName = NULL;
		}

	}


	for (std::vector<sIconDataInfo>::iterator it = m_vIconData.begin();it != m_vIconData.end();it++)
	{
		if (NULL !=it->pIconImage)
		{
			delete [] it->pIconImage;
			it->pIconImage = NULL;
		}

	}

	if (NULL != m_pGroupIcon)
	{
		delete [] m_pGroupIcon;
		m_pGroupIcon = NULL;
	}

	if (NULL != m_VerData.pData)
	{
		delete [] m_VerData.pData;
		m_VerData.pData = NULL;
	}

	if (NULL != m_ManifestData.pData)
	{
		delete [] m_ManifestData.pData;
		m_ManifestData.pData = NULL;
	}


};
bool IconReplace::ParseResource( )
{
	if (m_ExePath.empty())
	{
		return false;
	}

	HMODULE h_module = ::LoadLibraryEx(m_ExePath.c_str(),NULL, LOAD_LIBRARY_AS_DATAFILE ); 
	if (NULL == h_module)
	{
		return false;
	}

	//RT_GROUP_ICON
	if (TRUE == EnumResourceNames(h_module,RT_GROUP_ICON,enumresprocedure,(LONG_PTR)this))
	{
		std::vector<sResourceInfo>::iterator it = m_GrupIcon.begin();
		if (it!= m_GrupIcon.end())
		{
			//这个类只处理pe文件的第一个RT_GROUP_ICON，第一个RT_ICON资源
			HRSRC h_resinfo = FindResource(h_module,it->pResName,RT_GROUP_ICON);
			if (NULL!=h_resinfo)
			{
				HGLOBAL h_global = LoadResource(h_module, h_resinfo); 
				if (NULL!=h_global)
				{	
					DWORD dwsize = SizeofResource(h_module, h_resinfo);
					psGrpIconDir lpicon = (psGrpIconDir) ::LockResource(h_global);
					m_pGroupIcon =(psGrpIconDir) new BYTE[dwsize];
					if (NULL == m_pGroupIcon)
					{	FreeLibrary(h_module);
						return false;
					}
					memcpy(m_pGroupIcon,lpicon,dwsize);
					m_IsExistIcon = true;

				}else
				{	FreeLibrary(h_module);
					return false;
				}
			}else
			{	FreeLibrary(h_module);
				return false;
			}

		}
	}else
	{
		if (ERROR_RESOURCE_TYPE_NOT_FOUND!=GetLastError())
		{	FreeLibrary(h_module);
			return false;
		}
		
	
	}
		



	//RT_VERSION
	if (TRUE == EnumResourceNames(h_module,RT_VERSION,enumresprocedure,(LONG_PTR)this))
	{
		std::vector<sResourceInfo>::iterator it = m_VerInfo.begin();
		if (it!= m_VerInfo.end())
		{
			//这个类只处理pe文件的第一个RT_VERSION
			HRSRC h_resinfo = FindResource(h_module,it->pResName,RT_VERSION);
			if (NULL!=h_resinfo)
			{

				HGLOBAL h_global = ::LoadResource(h_module, h_resinfo); 
				if (NULL!=h_global)
				{//处理版本信息
					
					m_VerData.dwResourseSize= ::SizeofResource(h_module, h_resinfo);

					BYTE *ptemp = (BYTE *)::LockResource(h_global);
					if (NULL ==ptemp)
					{	FreeLibrary(h_module);
						return false;
					}
					m_VerData.pData = new BYTE[m_VerData.dwResourseSize];
					if (NULL == m_VerData.pData)
					{	FreeLibrary(h_module);
						return false;
					}
					memcpy(m_VerData.pData,ptemp,m_VerData.dwResourseSize);

				}else
				{	FreeLibrary(h_module);
					return false;
				}
			}else
			{	FreeLibrary(h_module);
				return false;
			}

		}
	}else
	{
		if (ERROR_RESOURCE_TYPE_NOT_FOUND!=GetLastError())
		{	FreeLibrary(h_module);
			return false;
		}
	}


	//RT_MANIFEST
	if (TRUE == EnumResourceNames(h_module,RT_MANIFEST,enumresprocedure,(LONG_PTR)this))
	{
		std::vector<sResourceInfo>::iterator it = m_ManifestInfo.begin();
		if (it!= m_ManifestInfo.end())
		{
			//这个类只处理pe文件的第一个m_ManifestInfo
			HRSRC h_resinfo = FindResource(h_module,it->pResName,RT_MANIFEST);
			if (NULL!=h_resinfo)
			{

				HGLOBAL h_global = ::LoadResource(h_module, h_resinfo); 
				if (NULL!=h_global)
				{//处理manifest信息

					m_ManifestData.dwResourseSize = ::SizeofResource(h_module, h_resinfo);

					BYTE *ptemp = (BYTE *)::LockResource(h_global);
					if (NULL ==ptemp)
					{	FreeLibrary(h_module);
						return false;
					}
					m_ManifestData.pData = new BYTE[m_ManifestData.dwResourseSize];
					if (NULL == m_ManifestData.pData)
					{	
						FreeLibrary(h_module);
						return false;
					}
					memcpy(m_ManifestData.pData,ptemp,m_ManifestData.dwResourseSize);

				}else
				{
					FreeLibrary(h_module);
					return false;
				}
			}else
			{
				FreeLibrary(h_module);
				return false;
			}

		}
	}else
	{
		if (ERROR_RESOURCE_TYPE_NOT_FOUND!=GetLastError())
		{
			FreeLibrary(h_module);
			return false;
		}
	}
	FreeLibrary(h_module);
	return true;

}

BOOL CALLBACK enumresprocedure(HMODULE  h_module,LPCTSTR  lpType, LPTSTR  lpszName, LONG_PTR lParam)
{
	IconReplace * piconreplace = (IconReplace * )lParam;
	

	// Name is from MAKEINTRESOURCE()
	if(IS_INTRESOURCE(lpType)&&( RT_GROUP_ICON == lpType))
	{	sResourceInfo node;
		TCHAR * szBuffer =new TCHAR[256];
		if (NULL==szBuffer)
		{
			return false;
		}
		if (IS_INTRESOURCE(lpszName))
		{
			node.dwResId = (DWORD)lpszName;

			node.bFlag = true;
			unsigned int id = (unsigned int)lpszName;

			swprintf(szBuffer,L"#%d",id);

		}else
		{
			node.bFlag = false;
			wcscpy(szBuffer,lpszName);
		}
		node.pResName = szBuffer;
		piconreplace->m_GrupIcon.push_back(node);
	}else if(IS_INTRESOURCE(lpType)&&( RT_VERSION == lpType))
	{
		sResourceInfo node;
		TCHAR * szBuffer =new TCHAR[256];
		if (NULL==szBuffer)
		{
			return false;
		}

		if (IS_INTRESOURCE(lpszName))
		{
			node.dwResId = (DWORD)lpszName;
			node.bFlag = true;
			unsigned int id = (unsigned int)lpszName;
			swprintf(szBuffer,L"#%d",id);
		}else
		{
			node.bFlag = false;
			wcscpy(szBuffer,lpszName);
		}
		node.pResName = szBuffer;
		piconreplace->m_VerInfo.push_back(node);
	}else if(IS_INTRESOURCE(lpType)&&( RT_MANIFEST == lpType))
	{
		sResourceInfo node;
		TCHAR * szBuffer =new TCHAR[256];
		if (NULL==szBuffer)
		{
			return false;
		}

		if (IS_INTRESOURCE(lpszName))
		{	
			node.bFlag = true;
			node.dwResId = (DWORD)lpszName;
			unsigned int id = (unsigned int)lpszName;
			swprintf(szBuffer,L"#%d",id);
		}else
		{
			node.bFlag = false;
			wcscpy(szBuffer,lpszName);
		}
		node.pResName = szBuffer;
		piconreplace->m_ManifestInfo.push_back(node);
	}
	return TRUE;
}


bool IconReplace::ParseIconFile()
{
	if (m_IconFilePath.empty())
	{
		return false;
	}
	HANDLE hfile = ::CreateFile(m_IconFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ,  
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE==hfile)
	{
		return false;
	}

	// We need an sIconDir to hold the data
	sIconDir icondir;
	// Read the Reserved word
	DWORD dwBytesRead = 0;

	if (!ReadFile( hfile, &icondir.idReserved, sizeof( WORD ), &dwBytesRead, NULL ))
	{
		CloseHandle(hfile);
		return false;
	}

	// Read the Type word - make sure it is 1 for icons
	dwBytesRead = 0;
	if (!ReadFile( hfile, &icondir.idType, sizeof( WORD ), &dwBytesRead, NULL ))
	{
	
		CloseHandle(hfile);
		return false;
	}

	// Read the count - how many images in this file?
	dwBytesRead = 0;
	if (!ReadFile( hfile, &icondir.idCount, sizeof( WORD ), &dwBytesRead, NULL ))
	{
		CloseHandle(hfile);
		return false;
	}
	// Reallocate IconDir so that idEntries has enough room for idCount elements

	psIconDir picondir = (psIconDir)new BYTE[(sizeof(sIconDir)+sizeof( sMemIconDirEntry )*(icondir.idCount-1))];
	if (NULL==picondir)
	{
		CloseHandle(hfile);
		return false;
	}
	picondir->idCount = icondir.idCount;
	picondir->idReserved = icondir.idReserved;
	picondir->idType = icondir.idType;
	// Read the sIconDirEntry elements
	dwBytesRead = 0;
	if (!ReadFile( hfile, picondir->idEntries, picondir->idCount * sizeof(sMemIconDirEntry),&dwBytesRead, NULL ))
	{
		if (NULL != picondir)
		{
			delete [] picondir;
		}
		CloseHandle(hfile);
		return false;
	
	}

	// Loop through and read in each image
	for(int i=0;i<picondir->idCount;i++)
	{


		sIconDataInfo node;
		node.memIconDirEntry = picondir->idEntries[i];
		// Allocate memory to hold the image
		node.pIconImage = new char[picondir->idEntries[i].dwBytesInRes];

		if (NULL==node.pIconImage)
		{
			if (NULL != picondir)
			{
				delete [] picondir;
			}
			CloseHandle(hfile);
			return false;
		}

		// Seek to the location in the file that has the image
		long newpos = SetFilePointer( hfile, picondir->idEntries[i].dwImageOffset, 
			NULL, FILE_BEGIN );
		if (HFILE_ERROR==newpos)
		{
			if (NULL != picondir)
			{
				delete [] picondir;
			}
			CloseHandle(hfile);
			return false;
		}
		// Read the image data
		dwBytesRead = 0;
		if (ReadFile( hfile, node.pIconImage, picondir->idEntries[i].dwBytesInRes,
			&dwBytesRead, NULL ))
		{
			m_vIconData.push_back(node);

		}else
		{
			if (NULL != picondir)
			{
				delete [] picondir;
			}
			CloseHandle(hfile);
			return false;
		}

	}
	if (NULL != picondir)
	{
		delete [] picondir;
	}
	CloseHandle(hfile);
	return true;
}

bool IconReplace::UpdateResource()
{
	//构造RT_GROUP_ICON头部信息
	if (m_vIconData.empty())
	{
		return false;
	}
	unsigned long len = sizeof(sGrpIconDir)+(m_vIconData.size() - 1)*sizeof(sIconDirEntry);
	psGrpIconDir pnewgropicon =(psGrpIconDir) new BYTE[len];
	if (NULL==pnewgropicon)
	{
		return false;
	}
	pnewgropicon->idount =  m_vIconData.size();
	if (NULL != m_pGroupIcon)
	{
		pnewgropicon->idReserved = m_pGroupIcon->idReserved;
		pnewgropicon->idType = m_pGroupIcon->idType;
	}else
	{
		pnewgropicon->idReserved = 0;
		pnewgropicon->idType = 1;
	}

	for (int i = 0; i<pnewgropicon->idount;i++)
	{
		pnewgropicon->idEntries[i].dwBytesInRes = m_vIconData.at(i).memIconDirEntry.dwBytesInRes;
		pnewgropicon->idEntries[i].bWidth = m_vIconData.at(i).memIconDirEntry.bWidth;
		pnewgropicon->idEntries[i].bHeight = m_vIconData.at(i).memIconDirEntry.bHeight;
		pnewgropicon->idEntries[i].bColorCount = m_vIconData.at(i).memIconDirEntry.bColorCount;
		pnewgropicon->idEntries[i].bReserved = m_vIconData.at(i).memIconDirEntry.bReserved;
		pnewgropicon->idEntries[i].wPlanes = m_vIconData.at(i).memIconDirEntry.wPlanes;
		pnewgropicon->idEntries[i].wBitCount = m_vIconData.at(i).memIconDirEntry.wBitCount;
		pnewgropicon->idEntries[i].nID = i+1;

	}



	//查找资源的语言信息
	HMODULE h_module = ::LoadLibraryEx(m_ExePath.c_str(),NULL, LOAD_LIBRARY_AS_DATAFILE ); 
	if (!m_GrupIcon.empty())
	{
		if (NULL==m_pGroupIcon)
		{
			return false;
		}
	
		EnumResourceLanguages(h_module,RT_ICON,MAKEINTRESOURCE(m_pGroupIcon->idEntries[0].nID),enumreslangproc,(LONG_PTR)this);

	
	}
	
	if (!m_VerInfo.empty())
	{
		if (m_VerInfo.at(0).bFlag)
		{
			EnumResourceLanguages(h_module,RT_VERSION,MAKEINTRESOURCE(m_VerInfo.at(0).dwResId),enumreslangproc,(LONG_PTR)this);
		} 
		else
		{
			EnumResourceLanguages(h_module,RT_VERSION,m_VerInfo.at(0).pResName,enumreslangproc,(LONG_PTR)this);
		}
	}

	if (!m_ManifestInfo.empty())
	{
		if (m_ManifestInfo.at(0).bFlag)
		{
			EnumResourceLanguages(h_module,RT_MANIFEST,MAKEINTRESOURCE(m_ManifestInfo.at(0).dwResId),enumreslangproc,(LONG_PTR)this);
		} 
		else
		{
			EnumResourceLanguages(h_module,RT_MANIFEST,m_ManifestInfo.at(0).pResName,enumreslangproc,(LONG_PTR)this);
		}
	}

	FreeLibrary(h_module);

	if (m_IsExistIcon)
	{
		//更新RT_ICON资源

		//替换掉原先的资源
		HANDLE hUpdate = ::BeginUpdateResource(m_ExePath.c_str(), TRUE);
		if (NULL == hUpdate)
		{
			return false;
		}

		if (m_IconLanInfo.empty())
		{
			return false;
		}
	
		if (!::UpdateResource(hUpdate, RT_ICON,MAKEINTRESOURCE(pnewgropicon->idEntries[0].nID),  
			m_IconLanInfo.at(0).idLanId,  
			m_vIconData[0].pIconImage, m_vIconData[0].memIconDirEntry.dwBytesInRes))
		{
			return false;
		}

		if (!::EndUpdateResource(hUpdate, FALSE))
		{
			return false;
		}

		
		hUpdate = ::BeginUpdateResource(m_ExePath.c_str(), FALSE);

		//更新RT_GROUP_ICON
		if (m_GrupIcon.at(0).bFlag)
		{
			if (!::UpdateResource(hUpdate, RT_GROUP_ICON,MAKEINTRESOURCE(m_GrupIcon.at(0).dwResId),  
				m_IconLanInfo.at(0).idLanId,  
				pnewgropicon,len))
			{

				return false;

			}
		} 
		else
		{
			if (L'#'==m_GrupIcon.at(0).pResName[0])
			{//首字符有#号，则忽略掉
				if (!::UpdateResource(hUpdate, RT_GROUP_ICON,m_GrupIcon.at(0).pResName+1,  
					m_IconLanInfo.at(0).idLanId,  
					pnewgropicon,len))
				{

					return false;

				}

			}else
			{
				if (!::UpdateResource(hUpdate, RT_GROUP_ICON,m_GrupIcon.at(0).pResName,  
					m_IconLanInfo.at(0).idLanId,  
					pnewgropicon,len))
				{

					return false;

				}
		
			}
			

			

		}

		//有多个RT_ICON资源时，接着更新RT_ICON资源
		for(unsigned int i =1;i<m_vIconData.size();i++)
		{
			if (!::UpdateResource(hUpdate, RT_ICON,MAKEINTRESOURCE(pnewgropicon->idEntries[i].nID),  
				m_IconLanInfo.at(0).idLanId,  
				m_vIconData[i].pIconImage, m_vIconData[i].memIconDirEntry.dwBytesInRes))
			{
				return false;
			}
		}


		//更新版本资源
		if (!m_VerInfo.empty())
		{
			if (m_VerLanInfo.empty())
			{
				return false;
			}
			if (NULL == m_VerData.pData)
			{
				return false;
			}

			if(m_VerInfo.at(0).bFlag)
			{

				if (!::UpdateResource(hUpdate, RT_VERSION,MAKEINTRESOURCE(m_VerInfo.at(0).dwResId),  
					m_VerLanInfo.at(0).idLanId,  
					m_VerData.pData,m_VerData.dwResourseSize))
				{
					return false;
				}

			}else
			{
				if (L'#'==m_VerInfo.at(0).pResName[0])
				{
					if (!::UpdateResource(hUpdate, RT_VERSION,m_VerInfo.at(0).pResName+1,  
						m_VerLanInfo.at(0).idLanId,  
						m_VerData.pData,m_VerData.dwResourseSize))
					{
						return false;
					}
				}else
				{
			
					if (!::UpdateResource(hUpdate, RT_VERSION,m_VerInfo.at(0).pResName,  
						m_VerLanInfo.at(0).idLanId,  
						m_VerData.pData,m_VerData.dwResourseSize))
					{
						return false;
					}
				}
			
				

			}


		}

		//更新manifest资源
		if (!m_ManifestInfo.empty())
		{

			if (m_ManifestLanInfo.empty())
			{
				return false;
			}
			if (NULL == m_ManifestData.pData)
			{
				return false;
			}
			if (m_ManifestInfo.at(0).bFlag)
			{
				if (!::UpdateResource(hUpdate, RT_MANIFEST,MAKEINTRESOURCE(m_ManifestInfo.at(0).dwResId),  
					m_ManifestLanInfo.at(0).idLanId,  
					m_ManifestData.pData,m_ManifestData.dwResourseSize))
				{
					return false;

				}
			} 
			else
			{
				if (L'#'==m_VerInfo.at(0).pResName[0])
				{
					if (!::UpdateResource(hUpdate, RT_MANIFEST,m_ManifestInfo.at(0).pResName+1,  
						m_ManifestLanInfo.at(0).idLanId,  
						m_ManifestData.pData,m_ManifestData.dwResourseSize))
					{
						return false;

					}
				
				}else
				{
					if (!::UpdateResource(hUpdate, RT_MANIFEST,m_ManifestInfo.at(0).pResName,  
						m_ManifestLanInfo.at(0).idLanId,  
						m_ManifestData.pData,m_ManifestData.dwResourseSize))
					{
						return false;

					}
			
				}
			

			}


		}

		if (!::EndUpdateResource(hUpdate, FALSE))
		{
			return false;
		}


	}else
	{ 
		//exe没有ICON资源，直接添加

		HANDLE hUpdate = ::BeginUpdateResource(m_ExePath.c_str(), FALSE);
		if (NULL == hUpdate)
		{
			return false;
		}
		WORD idLanId;
		if (!m_VerLanInfo.empty())
		{
			idLanId = m_VerLanInfo.at(0).idLanId;
		}else if (!m_ManifestLanInfo.empty())
		{
			idLanId = m_ManifestLanInfo.at(0).idLanId;
		}else 
		{
			idLanId = MAKELANGID(LANG_NEUTRAL,SUBLANG_SYS_DEFAULT);
		
		}
		//更新RT_GROUP_ICON
		//原先没有RT_GROUP_ICON，新增的RT_GROUP_ICON的资源ID设置159
		if (!::UpdateResource(hUpdate, RT_GROUP_ICON,MAKEINTRESOURCE(159),  
				idLanId,pnewgropicon,len))
		{
			return false;
		}
		
		

		//有多个RT_ICON资源时，更新RT_ICON资源
		for(unsigned int i = 0;i<m_vIconData.size();i++)
		{
			if (!::UpdateResource(hUpdate, RT_ICON,MAKEINTRESOURCE(pnewgropicon->idEntries[i].nID),  
				idLanId,  
				m_vIconData[i].pIconImage, m_vIconData[i].memIconDirEntry.dwBytesInRes))
			{
				return false;
			}
		}

		if (!::EndUpdateResource(hUpdate, FALSE))
		{
			return false;
		}


	}
	return true;
}


BOOL CALLBACK enumreslangproc( HMODULE  h_module, LPCTSTR  lpszType, LPCTSTR  lpszName,WORD  wIDLanguage, LONG_PTR lParam)
{
	IconReplace * piconreplace = (IconReplace * )lParam;

	if(IS_INTRESOURCE(lpszType)&&( RT_ICON == lpszType))
	{
		TCHAR * szBuffer = new TCHAR[256];//这里申请的资源不释放，外部统一释放
		sLanInfo node;
		if ( IS_INTRESOURCE(lpszName))
		{
			int id = (int)lpszName;
			swprintf(szBuffer,L"#%d",&id);
		}else
		{
			wcscpy(szBuffer,lpszName);
		}
		node.pResName = szBuffer;
		node.idLanId = wIDLanguage;
		piconreplace->m_IconLanInfo.push_back(node);
	}else if(IS_INTRESOURCE(lpszType)&&( RT_VERSION == lpszType))
	{	
		TCHAR * szBuffer = new TCHAR[256];//这里申请的资源不释放，外部统一释放
		sLanInfo node;
		if ( IS_INTRESOURCE(lpszName))
		{
			int id = (int)lpszName;
			swprintf(szBuffer,L"#%d",&id);
	
		}else
		{
			
			wcscpy(szBuffer,lpszName);
		}
		node.pResName = szBuffer;
		node.idLanId = wIDLanguage;
		piconreplace->m_VerLanInfo.push_back(node);

	}else if (IS_INTRESOURCE(lpszType)&&( RT_MANIFEST == lpszType))
	{
		TCHAR * szBuffer = new TCHAR[256];//这里申请的资源不释放，外部统一释放
		sLanInfo node;
		if ( IS_INTRESOURCE(lpszName))
		{
			int id = (int)lpszName;
			swprintf(szBuffer,L"#%d",&id);
		}else
		{
			wcscpy(szBuffer,lpszName);
		}
		node.pResName = szBuffer;
		node.idLanId = wIDLanguage;
		piconreplace->m_ManifestLanInfo.push_back(node);
	}
	return TRUE;
}

	 bool IconReplace::ReplaceIcon()
	 {
		 if (!ParseResource())
		 {
			 return false;
		 }
		 if (!ParseIconFile())
		 {
			 return false;
		 }

		 if (!UpdateResource())
		 {
			 return false;
		 }
		 return true;
		
	
	 }
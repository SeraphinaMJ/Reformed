#pragma once

class manager_pack;

class gLoader 
{
	public:

		static void preload(const std::string & p_assetPath, const std::string & p_dataPath, manager_pack & p_gPach, bool p_preloadAll);
		

	private:

		static void preloadAll(manager_pack & p_gPach);
		static void preloadSelective( const std::string & p_dataPath, manager_pack & p_gPack);

		static bool modified(LPCSTR p_hFile);
		//static std::vector<HANDLE> getModifiedList(LPCSTR filePath);

		static HANDLE openFile(LPCSTR p_fileName);

		static const LPCSTR assetListName;
};
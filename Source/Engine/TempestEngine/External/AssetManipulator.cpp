#include "AssetManipulator.hpp"
#include "../Utility/AssetManager.hpp"

const std::vector<std::wstring>& assetManipulator::assetList()
{
    return assetManager::assetList();
}

std::string assetManipulator::stringFromWide(const std::wstring & p_convert)
{
    return assetManager::stringFromWide(p_convert);
}

void assetManipulator::reloadAssets()
{
    assetManager::reloadAssetList();
}


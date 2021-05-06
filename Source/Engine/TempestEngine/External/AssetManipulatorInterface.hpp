/*!***************************************************************************************
\file       AssetManipulatorInterface.hpp
\author     Cody Cannell
\date       3/30/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  The definition of the Asset manipulator public interface
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <vector>
#include <string>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\par class: engineMetaManipulatorInterface
\brief   Provides external access to engine metadata.  Mostly read-only.
*****************************************************************************************/
class assetManipulatorInterface
{
public:
    virtual const std::vector<std::wstring> & assetList() = 0;
    virtual std::string stringFromWide(const std::wstring& p_convert) = 0;

    virtual void reloadAssets() = 0;
};


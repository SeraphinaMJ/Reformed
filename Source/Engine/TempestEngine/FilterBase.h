/*!***************************************************************************************
\file       FilterBase.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

class FilterBase
{
public:

	FilterBase();

	virtual void processBlock(float* buffer, int size);


private:

};
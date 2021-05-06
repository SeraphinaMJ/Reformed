/*!***************************************************************************************
\file       ProcessingDisplay.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include "Filter.h"
#include "DSPBase.h"

// Include Specific DSP's you want to display in editor here

#include "Compressor.h"

class FilterDisplay
{
public:

private:

};

class DSPDisplay
{
public:


private:


};

class LowPassDisplay : public FilterDisplay, public Audio::LowPass
{
public:


private:


};

class HighPassDisplay : public FilterDisplay, public Audio::HighPass
{
public:


private:


};

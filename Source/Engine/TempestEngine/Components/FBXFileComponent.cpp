/*!***************************************************************************************
\file       FBXFileComponent.cpp
\author     Henry Brobeck
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "FBXFileComponent.h"

FBXFileComponent::FBXFileComponent(typeRT p_data)
{
}

const std::string& FBXFileComponent::getFBXFilename() const
{
	return m_filename;
}

void FBXFileComponent::setFBXFilename(const std::string& p_filename)
{
	m_filename = p_filename;
}

typeRT FBXFileComponent::toTypeRT() const
{
	typeRT result;
	typeRT string;
	string.setString(getFBXFilename());
	string.setVariableName("filename");
	result.insertMember(string);
	return result;
}

void FBXFileComponent::updateFromTypeRT(typeRT& p_type)
{
	m_filename = p_type.member("filename").getString();
}

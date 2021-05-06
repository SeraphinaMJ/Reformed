/*!***************************************************************************************
\file       FBXFileComponent.h
\author     Henry Brobeck
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include <Component.hpp>
#include <ComponentTypeDetails.hpp>

class FBXFileComponent : public componentCRTP<FBXFileComponent>
{
public:
	FBXFileComponent() = default;
	FBXFileComponent(typeRT p_data);

	static componentType const getType() { static componentType const s_type = componentType::forConcrete<FBXFileComponent>(); return s_type; }
	componentType const type() const override { return getType(); }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "FBXFileComponent", true, true, false); }

	const std::string& getFBXFilename() const;
	void setFBXFilename(const std::string& p_filename);
	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT& p_type) override;
private:
	std::string m_filename;
};

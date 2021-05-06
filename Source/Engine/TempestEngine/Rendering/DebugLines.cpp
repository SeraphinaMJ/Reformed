/*!***************************************************************************************
\file       DebugLines.cpp
\author     Patrick Cook
\date       9/24/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  component for storing and displaying multiple line segments
*****************************************************************************************/

#include "DebugLines.hpp"
#include "GameObject.hpp"

#include <algorithm> // find

componentHandle<debugLines> debugLines::getLinesByTag(std::shared_ptr<gameObject> obj, std::string const & p_tag)
{
	auto l_components = obj->getComponents<debugLines>();

	// find component with the specified tag
	auto l_findResult = std::find_if(l_components.begin(), l_components.end(),
		[&p_tag](componentHandle<debugLines> l_component) noexcept -> bool
		{
			return (l_component)->getTag() == p_tag;
		}
	);
	// found it? great
	if (l_findResult != l_components.end())
	{
		return (*l_findResult);
	}
	// create one if it didn't exist
	else
	{
		auto l_component = obj->addComponent<debugLines>();
		l_component->setTag(p_tag);
		return l_component;
	}
}

debugLines::debugLines(std::string && tag)
	: m_buffer{}
	, m_tag{std::move(tag)}
{
}

debugLines::debugLines(typeRT & p_type)
	: m_buffer{}
	, m_tag{ "INVALID" }
{
}

typeRT debugLines::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	return result;
}

void debugLines::updateFromTypeRT(typeRT & )
{
}

std::vector<lineSegment> & debugLines::getLineBuffer() noexcept
{
	return m_buffer.getBuffer();
}

std::vector<lineSegment> const & debugLines::getLineBuffer() const noexcept
{
	return m_buffer.peekBuffer();
}

std::string const & debugLines::getTag() const noexcept
{
	return m_tag;
}

void debugLines::setTag(std::string const p_tag)
{
	m_tag = p_tag;
}

std::string debugLines::getQueryableType()
{
	return std::string(type().name());
}

std::string debugLines::getQueryableID()
{
	if (!getGameObject().expired()) { return std::to_string(getGameObject().lock()->getID()); }
	else { return "orphan debug lines component"; }
}

std::string debugLines::getQueryableProperty(std::string p_name)
{
	//convert this object to a typeRT
	typeRT temp = toTypeRT();

	//access the member with the name given
	typeRT temp2 = temp.member(p_name);

	//returns a typeRT equivalent to the type of the member variable

	//return string consisting of each data using string conversion defined in typeRT class
	std::string result;

	result += temp2.member("Tag").to_csv_format();

	return result;
}

std::vector<std::string> debugLines::getQueryablePropertyNames()
{
	return m_property_names;
}


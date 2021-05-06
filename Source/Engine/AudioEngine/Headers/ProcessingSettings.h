/*!***************************************************************************************
\file       ProcessingSettings.h
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		
*****************************************************************************************/
#pragma once

#include <vector>
#include <string>

namespace Audio
{
	class DSPBase;
	class FilterBase;

	class ProcessingSettings
	{
	public:
		ProcessingSettings();
		ProcessingSettings(const ProcessingSettings& rhs_P);
		~ProcessingSettings();

		ProcessingSettings& operator=(const ProcessingSettings& rhs_p);

		void setFilter(FilterBase* newFilter_p);
		// TODO: Figure out if we should have a collision here and 
		// disallow multiple DSP's of the same type
		void addDSP(DSPBase* newDSP_p); 
		void removeDSP(const std::string& DSPName_p);
		DSPBase* detachDSP(const std::string& DSPName_p);

		FilterBase* getFilter() const;
		DSPBase* getDSP(const std::string& DSPName_p) const;
		const std::vector<DSPBase*>& getDSPList() const;

	private:
		FilterBase* activeFilter;
		std::vector<DSPBase*> dspList;

	};


} // namespace Audio


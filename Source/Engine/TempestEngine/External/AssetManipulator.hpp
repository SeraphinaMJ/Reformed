#pragma once
#include "AssetManipulatorInterface.hpp"

class assetManipulator : public assetManipulatorInterface
{
    public:
        virtual ~assetManipulator() = default;

    private:
    // Inherited via assetManipulatorInterface
    virtual const std::vector<std::wstring>& assetList() override;

    virtual std::string stringFromWide(const std::wstring& p_convert) override;
    void reloadAssets() override;

};

#pragma once
#include <vector>
#include <string>

struct viewportGeometry;

class graphicsManipulatorInterface
{
public:
	virtual std::vector<std::string> getRenderTextureNames() = 0;

	// -1 = do not show a texture on-screen
	virtual void setRenderTextureIndex(int index) = 0;
	virtual int getRenderTextureIndex() = 0;

	virtual void reloadShaders() = 0;

	virtual bool isDisplayingWireframes() = 0;
	virtual void displayWireframes(bool) = 0;

	virtual bool isDisplayingDebugLines() = 0;
	virtual void displayDebugLines(bool) = 0;

	virtual viewportGeometry const& getViewport() = 0;
};
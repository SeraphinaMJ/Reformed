/*!***************************************************************************************
\file       Windowing.hpp
\author     Aaron Damyen
\date       5/7/2018
\copyright  All content ? 2017-2018 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  Definition of the windowing system interface.  This is needed to represent the game on the
screen in a specified area.
*****************************************************************************************/
#pragma once

#include "../SystemBase.hpp"
#include "GraphicsSystem.hpp"

struct GLFWwindow;

/////////========================================================================/////////
/////////  Class                                                                 /////////
/////////========================================================================/////////
/*!*************************************************************************************
\par class: windowing
\brief   This is the class responsible for creating and maintaining a window within the containing operating system
***************************************************************************************/
class windowing final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
	public:

        static constexpr const char * SYSTEM_NAME = "windowing"; //!< Name for looking up this system

        const std::string& name() const override { static std::string s(SYSTEM_NAME); return s; }

	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
	protected:

		//////============================================================================//////
		//////     Functions                                                              //////
		//////============================================================================//////
        void onInitialize() override;
        void onStartFrame() override;
        void onShutdown() override;
        void onUpdate() override;
        void onEndFrame() override;

		/*!***************************************************************************************
		\brief  Handles the message from the engine that the windowing system should initialize
		*****************************************************************************************/
		//virtual void onInitialize() override;

		/*!***************************************************************************************
		\brief  Handles the message from the engine that the windowing system should update
		*****************************************************************************************/
		//virtual void onUpdate() override;

		/*!***************************************************************************************
		\brief  Handles the message from the engine that the windowing system should shut down.
		*****************************************************************************************/
		//virtual void onShutdown() override;

		//======================================================================================//
		//         Helper                                                                      ///
		//======================================================================================//

		//////============================================================================//////
		//////     Data                                                                   //////
		//////============================================================================//////

		////==================================================================================////
		///        Configurable                                                               ////
		////==================================================================================////

		////==================================================================================////
		///        Non-Configurable                                                           ////
		////==================================================================================////


	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
	private:
		///////==========================================================================///////
		///////    Static                                                                ///////
		///////==========================================================================///////

		//////============================================================================//////
		//////     Functions                                                              //////
		//////============================================================================//////

		//======================================================================================//
		//         Helper                                                                      ///
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Retrieves the handle to the current window.  This is private so it can only be
		        used by friends.
		\return GLFWwindow * - Reference to the current window.
		*****************************************************************************************/
		GLFWwindow * getWindowHandle() const { return m_windowHandle; }

		//////============================================================================//////
		//////     Data                                                                   //////
		//////============================================================================//////
        std::string m_windowTitle;      //!< Title of the window (Titlebar)
        unsigned m_windowWidth = 0;     //!< Width of the window
        unsigned m_windowHeight = 0;    //!< Height of the window
        int m_bufferWidth = 0;          //!< Width of the buffer for glViewport
        int m_bufferHeight = 0;         //!< Height of the buffer for glViewport
        GLFWwindow * m_windowHandle;    //!< Handle to the window
        //std::shared_ptr<graphicsSystem> m_graphics;

		////==================================================================================////
		///        Configurable                                                               ////
		////==================================================================================////

		////==================================================================================////
		///        Non-Configurable                                                           ////
		////==================================================================================////

        friend class inputSystem;
};

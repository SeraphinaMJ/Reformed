rem /*!***************************************************************************************
rem \file       Prebuild.bat
rem \author     David Roberts
rem \date       7/31/18
rem \copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
rem \par        Project: Boomerang
rem \brief  
rem *****************************************************************************************/
TempestPrebuilder.exe %1
IF %errorlevel% geq 0 (echo "prebuild successful") ELSE (echo "prebuild failed") 
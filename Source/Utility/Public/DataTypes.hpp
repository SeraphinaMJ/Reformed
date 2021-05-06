/*!***************************************************************************************
\file       DataTypes.hpp
\author     Michael-Paul Moore
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Tools in your utility belt.
*****************************************************************************************/

#pragma once
//========3rd Party Includes============================================================//
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include <chrono>

/*=======================================================================================/
/         From CS250's Utilities.h                                                       /
//======================================================================================*/

// Typedefs for consistency (used sparingly throughout the framework...you do
// not have to use these at all).
typedef unsigned char      u8;
typedef char               s8;
typedef unsigned int       u32;
typedef int                s32;
typedef unsigned long long u64;
typedef long long          s64;
typedef float              f32;
typedef double             f64;
typedef uint64_t    objID;

std::string ReadInFile(const std::string &path);

/*!***************************************************************************************
		\brief  Creates new UUID
		\return unsigned long objID - new ID
*****************************************************************************************/
objID getNextID();

std::string getFileNameFromAssetPath(const std::string &path);

/*!***************************************************************************************
		\brief  Takes an objID and coverts it to a std::string
		\param p_id - The objId to convert
		\return std::string
*****************************************************************************************/
std::string objIDtoString(objID p_id);


/*=======================================================================================/
/         FPS & Timing                                                                   /
//======================================================================================*/

typedef unsigned char fps; //!< Type used for Frames Per-Second
#define FPS_MAX fps(-1)

/*!***************************************************************************************
\brief  Converts an fps to duration in nanoseconds.
\param p_fps - fps to convert
\return The how many nanoseconds in between each frame.
*****************************************************************************************/
std::chrono::duration<int, std::nano> FPStoFrequency(fps p_fps);

typedef unsigned char ticks; //!< Ticks are used by huristic, change type so huristics can store more data


// TODO(MP): Test then move
//template <typename T>
//class huristic
//{
//	void NewSeshion()
//	{
//		data_points = 0;
//		data_total = 0;
//		m_data.clear();
//	}
//
//	void addData(T data)
//	{
//		if (data_points == ticks(-1)) // reset the sesion, it hit max
//			NewSeshion();
//
//		++data_points;
//		data_total += data_total;
//		m_data.push_back(data_points);
//	}
//
//	//T data_per_tick()
//	//{
//	//	if (data_points)
//	//		return m_frame_time / frames;
//	//	return 0;
//	//}
//
//	T mean()
//	{
//		return data_total / data_points;
//	}
//
//	T standard_deviation()
//	{
//		T sum_of_distance_to_mean = {};
//		// find the square of its distance to the mean
//		for (const auto & data_point : data_points)
//		{
//			T distance_to_mean = data_point - mean();
//			sum_of_distance_to_mean += (distance_to_mean * distance_to_mean);
//		}
//		return sqrt(sum_of_distance_to_mean / data_point_count);
//	}
//
//	T              data_total;       //! total data in session
//	std::vector<T> data_points;      //! all data points
//	ticks          data_point_count; //! ticks in sesion
//};

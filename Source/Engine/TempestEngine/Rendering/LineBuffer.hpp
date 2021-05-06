/*!***************************************************************************************
\file       LineMesh.hpp
\author     Patrick Cook
\date       9/16/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Holds segment information for a line segment
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Vector3.hpp"
#include "TypeData.hpp"
#include "LineSegment.hpp"
//======== 3rd Party Includes ==========================================================//
#include <vector>
#include <string>
#include <array>
#include <algorithm> // std::copy_n
#include <memory> // std::shared_ptr
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
using GLuint = unsigned int;
class vertexArrayObject;
/*!***************************************************************************************
\par class: lineBuffer
\brief   Holds segment information for a line segment
*****************************************************************************************/
class lineBuffer
{
public:



	/*!***************************************************************************************
	\brief conversion / non-default ctor. Moves segment data into buffer
	\param p_label name of buffer
	\param p_segments segment buffer to move (optional)
	*****************************************************************************************/
	lineBuffer(std::vector<lineSegment>&& p_segments = {});

	/*!***************************************************************************************
	\brief non-default ctor. Copies segment data into buffer
	\param p_label name of buffer
	\param p_segments segment buffer to copy
	*****************************************************************************************/
	lineBuffer(std::vector<lineSegment> const& p_segments);

	~lineBuffer() noexcept  = default;
	lineBuffer(lineBuffer &&) noexcept = default;

	// for rendering
	size_t getBufferSize() const;
	void build();
	void update();
	void render(const std::shared_ptr<vertexArrayObject> p_modelVAO);
	void bind();
	void unbind() const;
	void shutdown();

	/*!***************************************************************************************
	\brief forwards to std::vector::reserve
	\param p_count amount of things expected to be in the mesh
	*****************************************************************************************/
	void reserve(size_t p_count);

	/*!***************************************************************************************
	\brief forwards to std::vector::resize
	\param p_count amount of things you need in the mesh
	\param p_val fills in added values with this
	*****************************************************************************************/
	void resize(size_t p_count, lineSegment const& p_val);

	// a bunch of ways to add segments
	// listed from most basic to most obscure

	lineSegment& operator[](size_t idx);
	lineSegment const& operator[](size_t idx) const;



	// single additions //

	/*!***************************************************************************************
	\brief forwards to std::vector::push_back
	\param p_segment segment information
	*****************************************************************************************/
	void add_segment(lineSegment const& p_segment);

	/*!***************************************************************************************
	\brief forwards to std::vector::push_back by rvalue reference
	\param p_segment segment information
	*****************************************************************************************/
	void add_segment(lineSegment&& p_segment);

	/*!***************************************************************************************
	\brief forwards to std::vector::emplace_back
	\param p_args forwarded to vector3 ctor
	\return the returned value from regular emplace_back
	*****************************************************************************************/
	template<typename... Args>
	decltype(auto) add_segment(Args&&... p_args)
	{
		m_dirty = true;
		return (m_segments.emplace_back(std::forward<Args>(p_args)...));
	}
	
	// STL containers //

	/*!***************************************************************************************
	\brief copies std::array to the buffer
	\param p_segments std::array to copy
	*****************************************************************************************/
	template<size_t size>
	void add_segments(std::array<lineSegment, size> const& p_segments)
	{
		add_segments(p_segments, size);
	}

	/*!***************************************************************************************
	\brief copies std::vector to the buffer
	\param p_segments std::vector to copy
	*****************************************************************************************/
	void add_segments(std::vector<lineSegment> const& p_segments);

	// STL iterators //

	/*!***************************************************************************************
	\brief copies range to the buffer
	\param p_begin STL-compliant input iterator
	\param p_end STL-compliant output iterator
	*****************************************************************************************/
	template<typename InIter>
	void add_segments(InIter p_begin, InIter p_end)
	{
		m_segments.reserve(count() + static_cast<size_t>(std::distance(p_begin, p_end)));
		std::copy(p_begin, p_end, std::back_inserter(m_segments));
		m_dirty = true;
	}

	// raw pointer //

	/*!***************************************************************************************
	\brief copies c-style array to buffer
	\param p_segments pointer to the beginning of the array
	\param size number of segments to copy
	*****************************************************************************************/
	template<size_t size>
	void add_segments(lineSegment const p_segments[size])
	{
		add_segments(p_segments, size);
	}

	// other accessors //

	/*!***************************************************************************************
	\brief forwards to std::vector::clear clearing the buffer
	*****************************************************************************************/
	void clear() noexcept;

	// getters //

	/*!***************************************************************************************
	\brief data pointer getter
	\return pointer to the beginning of the segment array
	*****************************************************************************************/
	lineSegment const* data() const noexcept;

	std::vector<lineSegment> const& peekBuffer() const noexcept;
	std::vector<lineSegment>& getBuffer() noexcept;

	/*!***************************************************************************************
	\brief array size getter
	\return number of elements in the segment array (identical to count)
	*****************************************************************************************/
	size_t size() const noexcept;

	/*!***************************************************************************************
	\brief array count getter
	\return number of elements in the segment array (identical to size)
	*****************************************************************************************/
	size_t count() const noexcept;
	
	/*!***************************************************************************************
	\brief flags whether buffer is m_dirty
	\return m_dirty flag
	*****************************************************************************************/
	bool isDirty() const noexcept;

	// Setters //

	/*!***************************************************************************************
	\brief manually unsets m_dirty flag (don't use if you don't understand the consequences!)
	*****************************************************************************************/
	void unsetDirty() const noexcept;

	std::string const& getAssetHandle() const;
	void setAssetHandle(std::string const& p_handle) ;

private: 

	/*!***************************************************************************************
	\brief generic segment buffer copying. auto-reserves as needed
	\param p_container STL-compliant container with std::cbegin
	\param p_size number of elements to copy from container
	*****************************************************************************************/
	template<typename T>
	void add_segments(T const& p_container, size_t p_size)
	{
		m_segments.reserve(size() + p_size);
		std::copy_n(std::cbegin(p_container), p_size, std::back_inserter(m_segments));
		m_dirty = true;
	}

	//! vertex array data
	std::vector<lineSegment> m_segments{};
	//! flags whether buffer should be resync'd with GPU
	mutable bool m_dirty{ true };
	mutable bool m_built{ false };

	GLuint m_vaoHandle{ 0 };
	GLuint m_vboHandle{ 0 };
	
	std::string m_vaoAssetHandle{ CUBE_FILE };
	mutable bool m_assetChanged{ true };
};

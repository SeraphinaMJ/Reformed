/*!****************************************************************************
@file EventFireError.h
@author Tyler Robbins

@brief A file defining an exception type which is thrown when an error occurs
       with firing an event.

@par All content © 2018 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include <stdexcept> // std::exception

namespace EventSystem3
{
    enum class Priority;

    /**
     * @brief An exception type which describes an error ocurring when an event
     *        is fired.
     */
    class EventFireError: public std::exception
    {
        public:
            /**
             * @brief An enumeration which describes the type of event firing
             *        error ocurred.
             */
            enum class Type
            {
                NULLPTR
            };

            EventFireError(Type, Priority, float);

            Type getType() const;
            Priority getPriority() const;
            float getTime() const;

            virtual const char* what() const noexcept;
        private:
            //! The error type.
            Type m_err_type;
            //! The event priority.
            Priority m_priority;
            //! The remaining delay on the event.
            float m_time;
    };
}


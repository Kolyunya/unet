//  Unix-network (unet) - Object-oriented high-level C++ Unix network framework.
//  Copyright (C) 2013 Oleynikov Nikolay
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Author email: OleynikovNY@mail.ru

#ifndef _UNET_EXCEPTION_HPP_
#define _UNET_EXCEPTION_HPP_

#include <stdexcept>            //  std::exception
#include <cerrno>               //  errno
#include <cstring>              //  strrerror
#include <xtd/string.hpp>       //  std::string
                                //  std::number_to_string

namespace Unet
{
    template <typename ExceptionType>
    class Exception
        :
            public std::exception
    {
        public:
            explicit                Exception ( const std::string& thrownFrom , bool systemError = false );
            virtual                 ~Exception ( void ) noexcept = default;
            const std::string&      getMessage ( void ) const noexcept;
            virtual const char*     what() const noexcept override;
        protected:
            void                    makeMessage ( const std::string& thrownFrom , bool systemError = false );
            std::string             message;
    };
}

#include "Exception.tpp"

#define EXCEPTION(ExceptionType) Exception<ExceptionType>(__PRETTY_FUNCTION__)

#define SYSTEM_EXCEPTION(ExceptionType) Exception<ExceptionType>(__PRETTY_FUNCTION__,true)

#endif  //  _UNET_EXCEPTION_HPP_
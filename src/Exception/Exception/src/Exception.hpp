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

#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <stdexcept>            //  std::exception
#include <cerrno>               //  errno
#include <cstring>              //  strrerror
#include <string>               //  std::string
#include <initializer_list>     //  std::initializer_list

namespace Unet
{
    class Exception
        :
            public std::exception
    {
        public:
            explicit                Exception ( void );
            explicit                Exception ( const std::string& message , bool systemError = false );
            explicit                Exception ( const std::initializer_list<std::string>& messages , bool systemError = false );
            virtual                 ~Exception ( void ) noexcept = default;
            const std::string&      getMessage ( void ) const noexcept;
        protected:
            void                    makeMessage ( const std::string& message , bool systemError = false );
            void                    makeMessage ( const std::initializer_list<std::string>& messages , bool systemError = false );
            void                    appendSystemErrorMessage ( bool systemError = false );
            std::string             message;
    };
}

#endif  //  _EXCEPTION_HPP_
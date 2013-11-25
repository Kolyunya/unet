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

#include "Exception.hpp"

namespace Unet
{

                            Exception::Exception ( void )
    {

    }

                            Exception::Exception ( const std::string& message , bool systemError )
    {

        this->makeMessage(message,systemError);

    }

                            Exception::Exception ( const std::initializer_list<std::string>& messages , bool systemError )
    {

        this->makeMessage(messages,systemError);

    }

    const std::string&      Exception::getMessage ( void ) const noexcept
    {

        return this->message;

    }

    const char*             Exception::what() const noexcept
    {

        return this->message.data();

    }

    void                    Exception::makeMessage ( const std::string& message , bool systemError )
    {

        this->message = message;
        this->appendSystemErrorMessage(systemError);

    }

    void                    Exception::makeMessage ( const std::initializer_list<std::string>& messages , bool systemError )
    {

        for ( auto message : messages )
        {

            this->message += message;

        }

        this->appendSystemErrorMessage(systemError);

    }

    void                    Exception::appendSystemErrorMessage ( bool systemError )
    {

        if ( systemError )
        {

            this->message += " ";
            this->message += strerror(errno);
            this->message += ".";

        }

    }

}
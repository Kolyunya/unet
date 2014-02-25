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

#include "Message.hpp"

namespace Unet
{

            Message ( const Message& message )
                :
                    Datagram(message),
                    protocol(message.protocol)
    {

    }

            Message ( Message&& message )
    {

    }


            Message::Message ( const std::string& data , AddressUniPtr addressUniPtr , Protocol protocol )
        :
            Datagram(data,addressUniPtr),
            protocol(protocol)
    {

    }

    void    Message::swap ( Message&& message )
    {

    }

}
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

#include "Protocol.hpp"

namespace Unet
{

                    Protocol::Protocol ( ProtocolType type )
    {
        this->setType(type);
    }

    bool            Protocol::operator== ( const Protocol& protocol ) const
    {
        ProtocolType lhsProtocolType = this->getType();
        ProtocolType rhsProtocolType = protocol.getType();
        bool protocolTypesAreEqual = lhsProtocolType == rhsProtocolType;
        return protocolTypesAreEqual;
    }

    bool            Protocol::operator!= ( const Protocol& protocol ) const
    {
        bool protocolsAreEqual = (*this) == protocol;
        bool protocolsAreNotEqual = ! protocolsAreEqual;
        return protocolsAreNotEqual;
    }

    ProtocolType    Protocol::getType ( void ) const
    {
        ProtocolType protocolType = this->type;
        return protocolType;
    }

    void            Protocol::setType ( ProtocolType type )
    {
        Protocol::validateType(type);
        this->type = type;
    }

    bool            Protocol::getTypeIsValid ( ProtocolType type )
    {
        bool typeIsValid = type < ProtocolTypeLimit;
        return typeIsValid;
    }

    bool            Protocol::getTypeIsNotValid ( ProtocolType type )
    {
        bool typeIsValid = Protocol::getTypeIsValid(type);
        bool typeIsNotValid = ! typeIsValid;
        return typeIsNotValid;
    }

    void            Protocol::validateType ( ProtocolType type )
    {
        bool typeIsNotValid = Protocol::getTypeIsNotValid(type);
        if ( typeIsNotValid )
        {
            throw -1;
        }
    }

}
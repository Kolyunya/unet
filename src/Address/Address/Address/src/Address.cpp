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

#include "Address.hpp"

namespace Unet
{

                        Address::Address ( void )
    {

        this->clear();

    }

                        Address::Address ( const Address& address )
    {

        this->copyAddress(address);

    }

    Address&            Address::operator= ( const Address& address )
    {

        this->copyAddress(address);
        return *this;

    }

    bool                Address::operator== ( const Address& address ) const
    {

        return  (
                    this->size == address.size
                        &&
                    0 == memcmp(&this->data,&address.data,address.size)
                );

    }

    sa_family_t         Address::getFamily ( void ) const
    {

        return this->data.ss_family;

    }

    const sockaddr*     Address::getDataPtr ( void ) const
    {

        return reinterpret_cast <const sockaddr*> (&this->data);

    }

    sockaddr*           Address::getDataPtr ( void )
    {

        return reinterpret_cast <sockaddr*> (&this->data);

    }

    const socklen_t&    Address::getSizeRef ( void ) const
    {

        return this->size;

    }

    socklen_t&          Address::getSizeRef ( void )
    {

        return this->size;

    }

    const socklen_t*    Address::getSizePtr ( void ) const
    {

        return &this->size;

    }

    socklen_t*          Address::getSizePtr ( void )
    {

        return &this->size;

    }

    socklen_t           Address::getSizeLimit ( void )
    {

        return sizeof(sockaddr_storage);

    }

    bool                Address::isEmpty ( void ) const
    {

        return this->size == 0;

    }

    bool                Address::isNotEmpty ( void ) const
    {

        return ! this->isEmpty();

    }

    void                Address::clear ( void )
    {

        memset(&this->data,0,sizeof(this->data));
        this->size = 0;

    }

    void                Address::setFamily ( sa_family_t family )
    {
        this->data.ss_family = family;
    }


    void                Address::copyAddress ( const Address& address )
    {

        socklen_t addressSize = address.getSizeRef();
        memcpy(&this->data,address.getDataPtr(),addressSize);
        this->size = addressSize;

    }

}
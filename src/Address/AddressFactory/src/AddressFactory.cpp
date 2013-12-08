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

#include "AddressFactory.hpp"

namespace Unet
{

                        AddressFactory::AddressFactory ( void )
    {
        this->reset();
    }

    sockaddr*           AddressFactory::getDataPtr ( void ) const
    {

        return reinterpret_cast<sockaddr*>(&this->addressData);

    }

    socklen_t*          AddressFactory::getSizePtr ( void ) const
    {

        return &this->addressSize;

    }

    AddressUniPtr       AddressFactory::getProduct ( void )
    {
        //  Extract address family from raw data
        sa_family_t addressFamily = this->addressData.ss_family;

        //  A unique pointer to an Address object
        AddressUniPtr addressUniPtr;

        // Concrete address type depends on it's family
        switch ( addressFamily )
        {
            case AF_INET:
            {
                addressUniPtr.reset(new Ipv4Address());
                break;
            }
            case AF_INET6:
            {
                addressUniPtr.reset(new Ipv6Address());
                break;
            }
            default:
            {
                //  Invalid address family
                throw -1;
            }
        }

        //  Copy data to the new Address object
        this->copyAddress(addressUniPtr);

        return addressUniPtr;

    }

    void                AddressFactory::reset ( void )
    {
        this->resetAddressData();
        this->resetAddressSize();
    }

    void                AddressFactory::resetAddressData ( void )
    {

        memset(&this->addressData,0,sizeof(this->addressData));

    }

    void                AddressFactory::resetAddressSize ( void )
    {

        this->addressSize = Address::getSizeLimit();

    }

    void                AddressFactory::copyAddress ( const AddressUniPtr& addressUniPtr ) const
    {
        this->copyAddressData(addressUniPtr);
        this->copyAddressSize(addressUniPtr);
    }

    void                AddressFactory::copyAddressData ( const AddressUniPtr& addressUniPtr ) const
    {
        memcpy(addressUniPtr->getDataPtr(),&this->addressData,sizeof(this->addressData));
    }

    void                AddressFactory::copyAddressSize ( const AddressUniPtr& addressUniPtr ) const
    {
        addressUniPtr->getSizeRef() = this->addressSize;
    }

}
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

    bool                AddressFactory::hasProduct ( void ) const
    {

        return this->addressUniPtr.get();

    }

    void                AddressFactory::makeProduct ( void )
    {

        // Extract address family from raw data
        sa_family_t addressFamily = this->addressData.ss_family;

        // Concrete address type depends on it's faamily
        switch ( addressFamily )
        {
            case AF_INET:
            {
                this->addressUniPtr.reset(new Ipv4Address());
                break;
            }
            case AF_INET6:
            {
                this->addressUniPtr.reset(new Ipv6Address());
                break;
            }
        }

        // Copy data to the address object which has just been created
        this->copyAddress(this->addressUniPtr.get());

    }

    AddressUniPtr       AddressFactory::getProductByUniPtr ( void )
    {
        this->checkHasProduct();
        return this->getAddressCopy();
    }

    AddressShrPtr       AddressFactory::getProductByShrPtr ( void )
    {
        return this->getProductByUniPtr();
    }

    void                AddressFactory::reset ( void )
    {

        this->resetAddressUniPtr();
        this->resetAddressData();
        this->resetAddressSize();

    }

    void                AddressFactory::checkHasProduct ( void ) const
    {
        if ( false == this->hasProduct() )
        {
            //!!!
            throw -1;
        }
    }

    void                AddressFactory::resetAddressUniPtr ( void )
    {

        this->addressUniPtr.reset();

    }

    void                AddressFactory::resetAddressData ( void )
    {

        memset(&this->addressData,0,sizeof(this->addressData));

    }

    void                AddressFactory::resetAddressSize ( void )
    {

        this->addressSize = Address::getSizeLimit();

    }

    void                AddressFactory::copyAddress ( Address* addressPtr ) const
    {

        this->copyAddressData(addressPtr);
        this->copyAddressSize(addressPtr);

    }

    void                AddressFactory::copyAddressData ( Address* addressPtr ) const
    {

        memcpy(addressPtr->getDataPtr(),&this->addressData,sizeof(this->addressData));

    }

    void                AddressFactory::copyAddressSize ( Address* addressPtr ) const
    {

        addressPtr->getSizeRef() = this->addressSize;

    }

    AddressUniPtr       AddressFactory::getAddressCopy ( void ) const
    {
        return this->addressUniPtr->getCopyByUniPtr();
    }

}
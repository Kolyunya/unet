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

#include "Datagram.hpp"

namespace Unet
{
                    Datagram::Datagram ( const Datagram& datagram )
    {
        this->deepCopy(datagram);
    }

                    Datagram::Datagram ( Datagram&& datagram )
    {
        this->swap(datagram);
    }

                    Datagram::Datagram ( const std::string& message , AddressUniPtr addressUniPtr )
                        :
                            message(message),
                            addressUniPtr(std::move(addressUniPtr))
    {

std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" <<std::endl;
    }

    Datagram&       Datagram::operator= ( Datagram datagram )
    {
        // Copy-and-swap idiom
        this->swap(datagram);
        return *this;
    }

    void            Datagram::swap ( Datagram& datagram )
    {
        this->message.swap(datagram.message);
        std::swap(this->addressUniPtr,datagram.addressUniPtr);
    }

    bool            Datagram::operator== ( const Datagram& datagram ) const
    {
        return  (
                    this->message == datagram.message
                        &&
                    *(this->addressUniPtr) == *(datagram.addressUniPtr)
                );
    }

    bool            Datagram::hasAddress ( void ) const
    {
        if ( this->addressUniPtr == nullptr )
        {
            return false;
        }

        return this->addressUniPtr->isEmpty();
    }

    void            Datagram::clear ( void )
    {
        this->message.clear();
        this->addressUniPtr.reset();
    }

    void            Datagram::deepCopy ( const Datagram& datagram )
    {
        this->deepCopyMessage(datagram);
        this->deepCopyAddress(datagram);
    }

    void            Datagram::deepCopyMessage ( const Datagram& datagram )
    {
        this->message = datagram.message;
    }

    void            Datagram::deepCopyAddress ( const Datagram& datagram )
    {
        // No need to preliminary reset the shared pointer.
        this->addressUniPtr = datagram.addressUniPtr->getCopyByUniPtr();
    }
}
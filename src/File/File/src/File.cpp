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

#include "File.hpp"

namespace Unet
{

                File::File ( int descriptor ) noexcept
    {
        /***
            @description    Constructor which creates a file object which manages file refered by "descriptor"
        ***/

        if ( descriptor < -1 )
        {
            descriptor = -1;
        }

        this->descriptor = descriptor;
    }

                File::File ( File&& file )
    {
        this->swap(file);
    }

    File&       File::operator= ( File&& file )
    {
        this->swap(file);
        return *this;
    }

    void        File::swap ( File& file )
    {
        std::swap(this->descriptor,file.descriptor);
    }

    void        File::open ( void )
    {

        if ( this->descriptor >= 0 )
        {

            throw Exception(ExcFileDescriptorIsAlreadyOpened);

        }

        try
        {

            int descriptor = this->allocate();
            this->setDescriptor(descriptor);

        }

        catch ( ... )
        {

            throw Exception(ExcFileDescriptorCouldNotBeOpened);

        }

    }

    void        File::close ( void )
    {

        if ( this->descriptor < 0 )
        {

            throw Exception(ExcFileDescriptorIsAlreadyClosed);

        }

        try
        {

            this->deallocate();
            this->descriptor = -1;

        }

        catch ( ... )
        {

            throw Exception(ExcFileDescriptorCouldNotBeClosed);

        }

    }

    int         File::getDescriptor ( void ) const
    {

        if ( this->descriptor < 0 )
        {

            throw Exception(ExcFileDescriptorIsEmpty);

        }

        return this->descriptor;

    }

    void        File::setDescriptor ( int descriptor )
    {

        if ( descriptor < 0 )
        {

            throw Exception({__PRETTY_FUNCTION__,ExcFileDescriptorIsInvalid});

        }

        if ( this->descriptor >= 0 )
        {

            throw Exception(ExcFileDescriptorIsAlreadyOpened);

        }

        this->descriptor = descriptor;

    }

    bool        File::isReady ( unsigned char readyFor ) const
    {


        if ( readyFor == 0 )
        {

            throw Exception(ExcFileDescriptorOptionsListIsEmpty);

        }

        int fileDescriptor = this->descriptor;

        fd_set descriptorsRead;
        fd_set descriptorsWrite;
        fd_set descriptorsException;

        fd_set* descriptorsReadPtr = nullptr;
        fd_set* descriptorsWritePtr = nullptr;
        fd_set* descriptorsExceptionPtr = nullptr;

        if ( ( readyFor & FD_IS_READY_FOR_READ ) == FD_IS_READY_FOR_READ )
        {

            FD_ZERO(&descriptorsRead);
            FD_SET(fileDescriptor,&descriptorsRead);
            descriptorsReadPtr = &descriptorsRead;

        }

        if ( ( readyFor & FD_IS_READY_FOR_WRITE ) == FD_IS_READY_FOR_WRITE )
        {

            FD_ZERO(&descriptorsWrite);
            FD_SET(fileDescriptor,&descriptorsWrite);
            descriptorsWritePtr = &descriptorsWrite;

        }

        if ( ( readyFor & FD_IS_READY_FOR_EXCEPTION ) == FD_IS_READY_FOR_EXCEPTION )
        {

            FD_ZERO(&descriptorsException);
            FD_SET(fileDescriptor,&descriptorsException);
            descriptorsExceptionPtr = &descriptorsException;

        }

        struct timeval waitFor;
        waitFor.tv_sec = 0;
        waitFor.tv_usec = 0;

        int select_result = ::select(fileDescriptor+1,descriptorsReadPtr,descriptorsWritePtr,descriptorsExceptionPtr,&waitFor);

        if ( select_result < 0 )
        {

            throw Exception(ExcFileDescriptorStatusCouldNotBeRetrieved,true);

        }

        if
        (
            (
                descriptorsReadPtr != nullptr
                    &&
                FD_ISSET(fileDescriptor,descriptorsReadPtr) == false
            )
                ||
            (
                descriptorsWritePtr != nullptr
                    &&
                FD_ISSET(fileDescriptor,descriptorsWritePtr) == false
            )
                ||
            (
                descriptorsExceptionPtr != nullptr
                    &&
                FD_ISSET(fileDescriptor,descriptorsExceptionPtr) == false
            )
        )
        {

            return false;

        }

        return true;

    }

    bool        File::isReadyForRead ( void ) const
    {

        return this->isReady(FD_IS_READY_FOR_READ);

    }

    bool        File::isReadyForWrite ( void ) const
    {

        return this->isReady(FD_IS_READY_FOR_WRITE);

    }

    bool        File::checkOptions ( int options ) const
    {

        int optionsActual = this->getOptions();
        bool optionsAreSet = ( ( optionsActual & options ) == options );
        return optionsAreSet;

    }

    int         File::getOptions ( void ) const
    {

        int optionsActual = fcntl(this->descriptor,F_GETFL);

        if ( optionsActual == -1 )
        {

            throw Exception(ExcFileDescriptorOptionsCouldNotBeRetrieved,true);

        }

        return optionsActual;

    }

    void        File::setOptions ( int options )
    {

        int overrideOptionsResult = fcntl(this->descriptor,F_SETFL,options);

        if ( overrideOptionsResult == -1 )
        {

            throw Exception(ExcFileDescriptorOptionsCouldNotBeSet,true);

        }

    }

    void        File::enableOptions ( int options )
    {

        int optionsActual = this->getOptions();
        optionsActual |= options;
        this->setOptions(optionsActual);

    }

    void        File::disableOptions ( int options )
    {

        int optionsActual = this->getOptions();
        optionsActual &= ~options;
        this->setOptions(optionsActual);

    }

    void        File::deallocate ( void )
    {

        int closeSuccess = ::close(this->descriptor);

        if ( closeSuccess == -1 )
        {

            throw Exception(ExcFileDescriptorCouldNotBeClosed,true);

        }

    }

}
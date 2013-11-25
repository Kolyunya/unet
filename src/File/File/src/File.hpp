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

#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <sys/select.h>                 //  select
                                        //  timeval
                                        //  fd_set
                                        //  FD_SET
                                        //  FD_ZERO
                                        //  FD_ISSET
#include <unistd.h>                     //  close
#include <fcntl.h>                      //  fcntl
                                        //  F_GETFL
                                        //  F_SETFL
#include <utility>                      //  std::swap
#include <Unet/FileException.hpp>       //  Unet::Exception

#define FD_IS_READY_FOR_READ 1          //  0 0 1   check if file descritop is ready for read
#define FD_IS_READY_FOR_WRITE 2         //  0 1 0   check if file descritop is ready for write
#define FD_IS_READY_FOR_EXCEPTION 4     //  1 0 0

namespace Unet
{
    class File
    {
        public:
            explicit        File ( int descriptor = -1 ) noexcept;
                            File ( File&& file );
            File&           operator= ( File&& file );
            void            swap ( File& file );
            virtual         ~File ( void ) noexcept = default;
            void            open ( void );
            void            close ( void );
            int             getDescriptor ( void ) const;
            void            setDescriptor ( int descriptor );
            bool            isReady ( unsigned char readyFor ) const;
            bool            isReadyForRead ( void ) const;
            bool            isReadyForWrite ( void ) const;
            bool            checkOptions ( int options ) const;
            int             getOptions ( void ) const;
            void            setOptions ( int options );
            void            enableOptions ( int options );
            void            disableOptions ( int options );
        protected:
            virtual int     allocate ( void ) = 0;
            virtual void    deallocate ( void );
            int             descriptor;
    };
}

#endif  //  _FILE_HPP_
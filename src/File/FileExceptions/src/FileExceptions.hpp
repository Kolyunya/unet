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

#ifndef _UNET_FILE_EXCEPTIONS_HPP_
#define _UNET_FILE_EXCEPTIONS_HPP_

#include <Unet/Exception.hpp>

namespace Unet
{
    struct FileDescriptorIsEmpty { constexpr static char const * const message = "FileDescriptorIsEmpty"; };
    struct FileDescriptorIsInvalid { constexpr static char const * const message = "FileDescriptorIsInvalid"; };
    struct FileDescriptorIsAlreadyOpened { constexpr static char const * const message = "FileDescriptorIsAlreadyOpened"; };
    struct FileDescriptorIsAlreadyClosed { constexpr static char const * const message = "FileDescriptorIsAlreadyClosed"; };
    struct FileDescriptorCouldNotBeOpened { constexpr static char const * const message = "FileDescriptorCouldNotBeOpened"; };
    struct FileDescriptorCouldNotBeClosed { constexpr static char const * const message = "FileDescriptorCouldNotBeClosed"; };
    struct FileDescriptorStatusCouldNotBeRetrieved { constexpr static char const * const message = "FileDescriptorStatusCouldNotBeRetrieved"; };
    struct FileDescriptorOptionsListIsEmpty { constexpr static char const * const message = "FileDescriptorOptionsListIsEmpty"; };
    struct FileDescriptorOptionsCouldNotBeRetrieved { constexpr static char const * const message = "FileDescriptorOptionsCouldNotBeRetrieved"; };
    struct FileDescriptorOptionsCouldNotBeSet { constexpr static char const * const message = "FileDescriptorOptionsCouldNotBeSet"; };
}

#endif  //  _UNET_FILE_EXCEPTIONS_HPP_
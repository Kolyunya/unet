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

#ifndef _FILE_EXCEPTION_HPP_
#define _FILE_EXCEPTION_HPP_

#include <Unet/Exception.hpp>

namespace Unet
{
    static std::string ExcFileDescriptorIsEmpty = "ExcFileDescriptorIsEmpty";
    static std::string ExcFileDescriptorIsInvalid = "ExcFileDescriptorIsInvalid";
    static std::string ExcFileDescriptorIsAlreadyOpened = "ExcFileDescriptorIsAlreadyOpened";
    static std::string ExcFileDescriptorIsAlreadyClosed = "ExcFileDescriptorIsAlreadyClosed";
    static std::string ExcFileDescriptorCouldNotBeOpened = "ExcFileDescriptorCouldNotBeOpened";
    static std::string ExcFileDescriptorCouldNotBeClosed = "ExcFileDescriptorCouldNotBeClosed";
    static std::string ExcFileDescriptorStatusCouldNotBeRetrieved = "ExcFileDescriptorStatusCouldNotBeRetrieved";
    static std::string ExcFileDescriptorOptionsListIsEmpty = "ExcFileDescriptorOptionsListIsEmpty";
    static std::string ExcFileDescriptorOptionsCouldNotBeRetrieved = "ExcFileDescriptorOptionsCouldNotBeRetrieved";
    static std::string ExcFileDescriptorOptionsCouldNotBeSet = "ExcFileDescriptorOptionsCouldNotBeSet";
}

#endif  //  _FILE_EXCEPTION_HPP_
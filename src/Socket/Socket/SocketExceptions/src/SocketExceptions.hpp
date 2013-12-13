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

#ifndef _UNET_SOCKET_EXCEPTIONS_HPP_
#define _UNET_SOCKET_EXCEPTIONS_HPP_

#include <Unet/Exception.hpp>

namespace Unet
{
    struct SocketOptionCouldNotBeSet { constexpr static char const * const message = "ExcSocketOptionCouldNotBeSet"; };
    struct SocketOptionValueCouldNotBeRetrieved { constexpr static char const * const message = "SocketOptionValueCouldNotBeRetrieved"; };
    struct SocketCouldNotBeOpened { constexpr static char const * const message = "ExcSocketCouldNotBeOpened"; };
    struct SocketCouldNotBeBound { constexpr static char const * const message = "ExcSocketCouldNotBeBound"; };
    struct SocketCouldNotBeConnected { constexpr static char const * const message = "ExcSocketCouldNotBeConnected"; };
    struct SocketCouldNotBeShutdown { constexpr static char const * const message = "ExcSocketCouldNotBeShutdown"; };
    struct IncommingDataIsNotAvailableNow { constexpr static char const * const message = "IncommingDataIsNotAvailableNow"; };
    struct CouldNotRetrieveSocketLocalAddress { constexpr static char const * const message = "ExcCouldNotRetrieveSocketLocalAddress"; };
    struct CouldNotRetrieveSocketPeerAddress { constexpr static char const * const message = "ExcCouldNotRetrieveSocketPeerAddress"; };
    struct IncommingDataSizeCouldNotBeRetrieved { constexpr static char const * const message = "ExcIncommingDataSizeCouldNotBeRetrieved"; };
}

#endif  //  _UNET_SOCKET_EXCEPTIONS_HPP_
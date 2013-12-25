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

#ifndef _UNET_TCP_SOCKET_EXCEPTIONS_HPP_
#define _UNET_TCP_SOCKET_EXCEPTIONS_HPP_

#include <Unet/Exception.hpp>

namespace Unet
{
    struct InvalidConnectionsLimitNumber { constexpr static char const * const message = "InvalidConnectionsLimitNumber"; };
    struct InvalidMessageSize { constexpr static char const * const message = "InvalidMessageSize"; };
    struct SocketCouldNotBeListened { constexpr static char const * const message = "SocketCouldNotBeListened"; };
    struct SocketCouldNotAcceptConnection { constexpr static char const * const message = "SocketCouldNotAcceptConnection"; };
    struct MessageHasNotBeenDelieveredYet { constexpr static char const * const message = "MessageHasNotBeenDelieveredYet"; };
    struct OutgoingDataCouldNotBeSentCompletely { constexpr static char const * const message = "OutgoingDataCouldNotBeSentCompletely"; };
    struct PeerDisconnected { constexpr static char const * const message = "PeerDisconnected"; };
    struct BrokenPipe { constexpr static char const * const message = "BrokenPipe"; };
}

#endif  //  _UNET_TCP_SOCKET_EXCEPTIONS_HPP_
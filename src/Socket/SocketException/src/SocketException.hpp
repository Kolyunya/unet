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

#ifndef _SOCKET_EXCEPTION_HPP_
#define _SOCKET_EXCEPTION_HPP_

#include <Unet/Exception.hpp>

namespace Unet
{
    static std::string ExcStringDoesNotRepresentValidNetworkAddress = "ExcStringDoesNotRepresentValidNetworkAddress";
    static std::string ExcStringCouldNotBeConvertedToNetworkAddress = "ExcStringCouldNotBeConvertedToNetworkAddress";
    static std::string ExcNetworkAddressCouldNotBeconvertedToString = "ExcNetworkAddressCouldNotBeconvertedToString";
    static std::string ExcNetworkAddressTypeIsNotSupported = "ExcNetworkAddressTypeIsNotSupported";
    static std::string ExcNetworkAddressFamilyIsNotSupported = "ExcNetworkAddressFamilyIsNotSupported";
    static std::string ExcNetworkAddressIsInvalid = "ExcNetworkAddressIsInvalid";
    static std::string ExcSocketOptionCouldNotBeSet = "ExcSocketOptionCouldNotBeSet";
    static std::string ExcSocketCouldNotBeBound = "ExcSocketCouldNotBeBound";
    static std::string ExcSocketCouldNotBeListened = "ExcSocketCouldNotBeListened";
    static std::string ExcSocketCouldNotBeConnected = "ExcSocketCouldNotBeConnected";
    static std::string ExcSocketCouldNotBeShutdown = "ExcSocketCouldNotBeShutdown";
    static std::string ExcSocketCouldNotBeOpened = "ExcSocketCouldNotBeOpened";
    static std::string ExcSocketHasNoIncommingData = "ExcSocketHasNoIncommingData";
    static std::string ExcIncommingDataSizeCouldNotBeRetrieved = "ExcIncommingDataSizeCouldNotBeRetrieved";
    static std::string ExcIncommingDataCouldNotBeRetrieved = "ExcIncommingDataCouldNotBeRetrieved";
    static std::string ExcOutgoingDataCouldNotBeSent= "ExcOutgoingDataCouldNotBeSent";
    static std::string ExcOutgoingDataCouldNotBeSentCompletely= "ExcOutgoingDataCouldNotBeSentCompletely";
    static std::string ExcMessageHasNotBeenDelieveredYet = "ExcMessageHasNotBeenDelieveredYet";
    static std::string ExcCouldNotRetrieveSocketLocalAddress = "ExcCouldNotRetrieveSocketLocalAddress";
    static std::string ExcCouldNotRetrieveSocketPeerAddress = "ExcCouldNotRetrieveSocketPeerAddress";
    static std::string ExcMessageDelimiterIsNotDefined = "ExcMessageDelimiterIsNotDefined";
    static std::string ExcConnectionsLimitIsNotDefined = "ExcMessageDelimiterIsNotDefined";
    static std::string ExcSocketCouldNotAcceptConnection = "ExcSocketCouldNotAcceptConnection";
}

#endif  //  _SOCKET_EXCEPTION_HPP_
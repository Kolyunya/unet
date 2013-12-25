{
    'includes'  :   [
                        './../system/gyp/includes/includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'             :   'Unet',
                        'UNIT_NEEDS_BUILDING'   :   'true',
                        'UNIT_NEEDS_TESTSING'   :   'false',
                        'UNIT_SOURCES'          :   [
                                                    ],
                        'UNIT_DEPENDENCIES'     :   [
                                                        './../src/Address/Address/Address/build/Address.gyp:Address',
                                                        './../src/Address/Address/AddressFactory/build/AddressFactory.gyp:AddressFactory',
                                                        './../src/Address/NullAddress/build/NullAddress.gyp:NullAddress',
                                                        './../src/Address/Ipv4Address/build/Ipv4Address.gyp:Ipv4Address',
                                                        './../src/Address/Ipv6Address/build/Ipv6Address.gyp:Ipv6Address',
                                                        './../src/Datagram/Datagram/build/Datagram.gyp:Datagram',
                                                        './../src/File/File/build/File.gyp:File',
                                                        './../src/Socket/Socket/Socket/build/Socket.gyp:Socket',
                                                        './../src/Socket/UdpSocket/UdpSocket/build/UdpSocket.gyp:UdpSocket',
                                                        './../src/Socket/TcpSocket/TcpSocket/build/TcpSocket.gyp:TcpSocket',
                                                        './../src/SocketServer/SocketServer/SocketServer/build/SocketServer.gyp:SocketServer',
                                                        './../src/SocketServer/TcpServer/TcpServer/TcpServer/build/TcpServer.gyp:TcpServer',
                                                        './../src/SocketServer/UdpServer/UdpServer/UdpServer/build/UdpServer.gyp:UdpServer',
                                                    ],
                        'UNIT_CFLAGS'           :   [
                                                    ],
                        'UNIT_LDFLAGS'          :   [
                                                    ],

                    },
}

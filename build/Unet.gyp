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
                                                        './../src/Exception/Exception/build/Exception.gyp:Exception',
                                                        './../src/Address/Address/build/Address.gyp:Address',
                                                        './../src/Address/Ipv4Address/build/Ipv4Address.gyp:Ipv4Address',
                                                        './../src/Address/Ipv6Address/build/Ipv6Address.gyp:Ipv6Address',
                                                        './../src/Datagram/Datagram/build/Datagram.gyp:Datagram',
                                                        './../src/File/File/build/File.gyp:File',
                                                        './../src/Socket/Socket/build/Socket.gyp:Socket',
                                                        './../src/Socket/UdpSocket/build/UdpSocket.gyp:UdpSocket',
                                                        './../src/Socket/TcpSocket/build/TcpSocket.gyp:TcpSocket',
                                                    ],
                    },
}
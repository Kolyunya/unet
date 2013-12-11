{
    'includes'  :   [
                        './../../../system/gyp/includes/example_includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'         :   'TcpEchoServer',
                        'UNIT_SOURCES'      :   [
                                                    '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).cpp',
                                                ],
                        'UNIT_DEPENDENCIES' :   [
                                                    './../../../src/Address/Ipv4Address/build/Ipv4Address.gyp:Ipv4Address',
                                                    './../../../src/Address/Ipv6Address/build/Ipv6Address.gyp:Ipv6Address',
                                                    './../../../src/SocketServer/TcpServer/TcpEchoServer/build/TcpEchoServer.gyp:TcpEchoServer',
                                                ],
                        'UNIT_CFLAGS'       :   [
                                                    '-pthread',
                                                ],
                        'UNIT_LDFLAGS'      :   [
                                                    '-pthread',
                                                ],
                    },
}
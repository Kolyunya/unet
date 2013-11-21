{
    'includes'  :   [
                        './../../../system/gyp/includes/example_includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'         :   'UdpEchoServerTwo',
                        'UNIT_SOURCES'      :   [
                                                    '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).cpp',
                                                ],
                        'UNIT_DEPENDENCIES' :   [
                                                    './../../../src/Server/UdpServer/build/UdpServer.gyp:UdpServer',
                                                    './../../../src/Socket/UdpSocket/build/UdpSocket.gyp:UdpSocket',
                                                    './../../../src/Address/Ipv4Address/build/Ipv4Address.gyp:Ipv4Address',
                                                ],
                        'UNIT_CFLAGS'       :   [
                                                    '-pthread',
                                                ],
                        'UNIT_LDFLAGS'      :   [
                                                    '-pthread',
                                                ],
                    },
}
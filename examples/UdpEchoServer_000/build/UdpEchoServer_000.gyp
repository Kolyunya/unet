{
    'includes'  :   [
                        './../../../system/gyp/includes/example_includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'         :   'UdpEchoServer_000',
                        'UNIT_SOURCES'      :   [
                                                    '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).cpp',
                                                ],
                        'UNIT_DEPENDENCIES' :   [
                                                    './../../../src/Socket/UdpSocket/UdpSocket/build/UdpSocket.gyp:UdpSocket',
                                                    './../../../src/Address/Ipv4Address/build/Ipv4Address.gyp:Ipv4Address',
                                                    './../../../src/Address/Ipv6Address/build/Ipv6Address.gyp:Ipv6Address',
                                                ],
                        'UNIT_CFLAGS'       :   [
                                                ],
                        'UNIT_LDFLAGS'      :   [
                                                ],
                    },
}
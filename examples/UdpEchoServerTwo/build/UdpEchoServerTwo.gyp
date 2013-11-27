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
                                                    './../../../src/Address/Ipv4Address/build/Ipv4Address.gyp:Ipv4Address',
                                                    './../../../src/Server/UdpEchoServer/build/UdpEchoServer.gyp:UdpEchoServer',
                                                ],
                        'UNIT_CFLAGS'       :   [
                                                    '-pthread',
                                                ],
                        'UNIT_LDFLAGS'      :   [
                                                    '-pthread',
                                                ],
                    },
}
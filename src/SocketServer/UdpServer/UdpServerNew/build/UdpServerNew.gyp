{
    'includes'  :   [
                        './../../../../../system/gyp/includes/includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'             :   'UdpServerNew',
                        'UNIT_NEEDS_BUILDING'   :   'true',
                        'UNIT_NEEDS_TESTSING'   :   'true',
                        'UNIT_SOURCES'          :   [
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).hpp',
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).cpp',
                                                    ],
                        'UNIT_DEPENDENCIES'     :   [
                                                        './../../../../Exception/Exception/build/Exception.gyp:Exception',
                                                        './../../../../Socket/UdpSocket/build/UdpSocket.gyp:UdpSocket',
														'./../../../SocketServer/SocketServer/build/SocketServer.gyp:SocketServer',
                                                    ],
                        'UNIT_CFLAGS'           :   [
                                                    ],
                        'UNIT_LDFLAGS'          :   [
                                                    ],
                    },
}
sgdhsdhsh
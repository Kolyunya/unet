{
    'includes'  :   [
                        './../../../../../../system/gyp/includes/includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'             :   'TcpServer',
                        'UNIT_NEEDS_BUILDING'   :   'true',
                        'UNIT_NEEDS_TESTSING'   :   'false',
                        'UNIT_SOURCES'          :   [
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).hpp',
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).cpp',
                                                    ],
                        'UNIT_DEPENDENCIES'     :   [
														'./../../../../SocketServer/SocketServer/build/SocketServer.gyp:SocketServer',
                                                        './../../../../../Socket/TcpSocket/TcpSocket/build/TcpSocket.gyp:TcpSocket',
                                                        './../../../../../Datagram/Datagram/build/Datagram.gyp:Datagram',
                                                    ],
                        'UNIT_CFLAGS'           :   [
                                                    ],
                        'UNIT_LDFLAGS'          :   [
                                                    ],
                    },
}
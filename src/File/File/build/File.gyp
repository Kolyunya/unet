{
    'includes'  :   [
                        './../../../../system/gyp/includes/includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'             :   'File',
                        'UNIT_NEEDS_BUILDING'   :   'true',
                        'UNIT_NEEDS_TESTSING'   :   'false',
                        'UNIT_SOURCES'          :   [
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).hpp',
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).cpp',
                                                    ],
                        'UNIT_DEPENDENCIES'     :   [
                                                        './../../../Exception/Exception/build/Exception.gyp:Exception',
                                                    ],
                        'UNIT_CFLAGS'           :   [
                                                    ],
                        'UNIT_LDFLAGS'          :   [
                                                    ],
                    },
}
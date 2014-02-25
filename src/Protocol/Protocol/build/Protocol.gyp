{
    'includes'  :   [
                        './../../../../system/gyp/includes/includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'             :   'Protocol',
                        'UNIT_NEEDS_BUILDING'   :   'true',
                        'UNIT_NEEDS_TESTSING'   :   'false',
                        'UNIT_SOURCES'          :   [
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).hpp',
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).cpp',
                                                    ],
                        'UNIT_DEPENDENCIES'     :   [
                                                    ],
                        'UNIT_CFLAGS'           :   [
                                                    ],
                        'UNIT_LDFLAGS'          :   [
                                                    ],
                    },
}
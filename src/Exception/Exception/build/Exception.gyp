{
    'includes'  :   [
                        './../../../../system/gyp/includes/includes.gypi',
                    ],
    'variables' :   {
                        'UNIT_NAME'             :   'Exception',
                        'UNIT_NEEDS_BUILDING'   :   'false',
                        'UNIT_NEEDS_TESTSING'   :   'false',
                        'UNIT_SOURCES'          :   [
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).hpp',
                                                        '<(UNIT_SOURCES_DIR)/<(UNIT_NAME).tpp',
                                                    ],
                        'UNIT_DEPENDENCIES'     :   [
                                                    ],
                        'UNIT_CFLAGS'           :   [
                                                    ],
                        'UNIT_LDFLAGS'          :   [
                                                    ],
                    },
}
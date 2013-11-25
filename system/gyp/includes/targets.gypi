{
    'conditions'    :   [
                            [
                                'UNIT_NEEDS_BUILDING == "true"',
                                {
                                    'targets'   :   [
                                                        {
                                                            'target_name'   :   '<(UNIT_NAME)',
                                                            'type'          :   'static_library',
                                                        },
                                                    ],
                                },
                            ],
                            [
                                'UNIT_NEEDS_TESTSING == "true"',
                                {
                                    'targets'   :   [
                                                        {
                                                            'target_name'   :   '<(UNIT_NAME)Tests',
                                                            'type'          :   'executable',
                                                            'dependencies'  :   [
                                                                                    '<(UNIT_NAME)',
                                                                                ],
                                                            'sources'       :   [
                                                                                    '<(UNIT_TESTS_SOURCE_FILE)',
                                                                                ],
                                                            'include_dirs'  :   [
                                                                                    '$(GTEST_HOME)/include/',
                                                                                ],
                                                            'ldflags'       :   [
                                                                                    '<(GTEST_OBJECT_FILE)',
                                                                                    '-lpthread',                # Needed by gtest
                                                                                ],
                                                        },
                                                    ],
                                },
                            ],
                        ],
}
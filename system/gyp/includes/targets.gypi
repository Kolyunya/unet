{
    'conditions'    :   [
                            [
                                'UNIT_NEEDS_BUILDING == "true"',
                                {
                                    'targets'   :   [
                                                        {
                                                            'target_name'   :   '<(UNIT_NAME)',
                                                            'type'          :   'static_library',
                                                            'dependencies'  :   [
                                                                                    '<@(UNIT_DEPENDENCIES)',
                                                                                ],
                                                        },
                                                    ],
                                },
                            ],
                            [
                                'UNIT_NEEDS_TESTSING == "true"',
                                {
                                    'targets'   :   [
                                                        {
                                                            'target_name'   :   '<(UNIT_NAME)_tests',
                                                            'type'          :   'executable',
                                                            'ldflags'       :   [
                                                                                    '<(GTEST_OBJECT_FILE)',
                                                                                ],
                                                            'sources'       :   [
                                                                                    '<(UNIT_TESTS_SOURCE_FILE)',
                                                                                ],
                                                            'include_dirs'  :   [
                                                                                    '$(GTEST_HOME)/include/',
                                                                                ],
                                                            'conditions'    :   [
                                                                                    [
                                                                                        'UNIT_NEEDS_BUILDING == "true"',
                                                                                        {
                                                                                            'dependencies'  :   [
                                                                                                                    '<(UNIT_NAME)',
                                                                                                                ],
                                                                                        },
                                                                                    ],
                                                                                    [
                                                                                        'OS == "linux"',
                                                                                        {
                                                                                            'ldflags'   :   [
                                                                                                                '-lpthread',
                                                                                                            ],
                                                                                        },
                                                                                    ],
                                                                                ],
                                                        },
                                                    ],
                                },
                            ],
                        ],
}
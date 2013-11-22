{
    'targets'   :   [
                        {
                            'target_name'   :   '<(UNIT_NAME)',
                            'type'          :   'executable',
                            'dependencies'  :   [
                                                    '<@(UNIT_DEPENDENCIES)',
                                                ],
                        },
                    ],
}
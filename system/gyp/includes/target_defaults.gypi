{
    'target_defaults'   :   {
                                'cflags'            :   [
                                                            '<@(CFLAGS_STANDARD)',
                                                            '<@(UNIT_CFLAGS)',
                                                        ],
                                'ldflags'           :   [
                                                            '<@(LDFLAGS_STANDARD)',
                                                            '<@(UNIT_LDFLAGS)',
                                                        ],
                                'include_dirs'      :   [
                                                            '$(STD_HOME)/include',
                                                            '$(UTIL_HOME)/include',
                                                            '$(UNET_HOME)/include',
                                                        ],
                                'sources'           :   [
                                                            '<@(UNIT_SOURCES)',
                                                        ],
                                'configurations'    :   {
                                                            'debug'     :   {
                                                                                'cflags'    :   [
                                                                                                    '<@(CFLAGS_DEBUG)',
                                                                                                ],
                                                                            },
                                                            'release'   :   {
                                                                                'cflags'    :   [
                                                                                                    '<@(CFLAGS_RELEASE)',
                                                                                                ],
                                                                            },
                                                        },
                            },
}
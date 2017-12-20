{
	'variables': {
		'conditions': [
			['OS == "win"', {
				'component%': 'static_library',
			}],
			['OS == "mac"', {
				'component%': 'shared_library',
			}],
			['OS != "win" and OS != "mac"', {
				'component%': 'shared_library',
			}],
		],
	},
	'target_defaults': {
		"include_dirs" : [ 
			'lib/CPP-Metadata/include',
			'lib/CPP-SystemRT/include',
			'include'
		],
		'cflags': ['-DDUK_USE_CPP_EXCEPTIONS=1'],
		'msbuild_settings': {
			'ClCompile': {
				#'WarningLevel': 'Level4', # /W4
				'PreprocessorDefinitions': [
					'_WIN32_WINNT=0x0600', # Windows Vista SP2 or later
					'WIN32_LEAN_AND_MEAN',
					'NOMINMAX',
					'DUK_USE_CPP_EXCEPTIONS=1'
				],
			},
		},
		'default_configuration': 'Release_x64',
        'configurations':
        {
			'Debug': {
				'defines': ['DEBUG=1'],
				'cflags': ['-g', '-O0', '-std=c++11'],
				'msbuild_settings': {
					'ClCompile': {
						'Optimization': 'Disabled', # /Od
						'conditions': [
							['OS == "win" and component == "shared_library"', {
								'RuntimeLibrary': 'MultiThreadedDebugDLL', # /MDd
							}, {
								'RuntimeLibrary': 'MultiThreadedDebug', # /MTd
							}],
						],
					},
					'Link': {
					#'LinkIncremental': 'true', # /INCREMENTAL
					},
				},
			}, # Debug
			'Release': {
				'defines': ['NDEBUG=1'],
				'cflags': ['-O3', '-std=c++11'],
				'msbuild_settings': {
					'ClCompile': {
						'Optimization': 'MaxSpeed', # /O2
						'InlineFunctionExpansion': 'AnySuitable', # /Ob2
						'conditions': [
							['OS == "win" and component == "shared_library"', {
								'RuntimeLibrary': 'MultiThreadedDLL', # /MD
							}, {
								'RuntimeLibrary': 'MultiThreaded', # /MT
							}],
						],
					},
					'Link': {
						#'LinkIncremental': 'false', # /INCREMENTAL:NO
						'OptimizeReferences': 'true', # /OPT:REF
					},
				},
			}, # Release
            'Debug_x64': {
                'inherit_from': ['Debug'],
                'msvs_configuration_platform': 'x64'
            },
            'Release_x64': {
                'inherit_from': ['Release'],
                'msvs_configuration_platform': 'x64'
            },
        },
	},
	
	'targets': [
		{
			'target_name': 'cppfastcgi-daemon',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib', '-lws2_32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				"lib/CPP-Metadata/build.gyp:CPP-Metadata-lib",
				"lib/CPP-SystemRT/build.gyp:CPP-SystemRT-lib"
			],
			'sources': [
				'src/daemon.cpp',
				'src/daemon_main.cpp'
			],
		},
	],
}
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
		'target_conditions': [
		  ['_type=="shared_library" or _type=="static_library"', {'cflags': ['-fPIC'], 'ldflags': ['-fPIC']}],
		],
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
			'target_name': 'mod_python',
			'type': 'shared_library',
			'defines': ['PYTHON_MODULE=1'],
			'conditions': [
				['OS == "win"', {
				#	'libraries': ['-ladvapi32.lib', '-lws2_32.lib'],
				}],
				['OS == "linux"', {
				#	'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				'CPP-FastCGI-lib',
				"lib/CPP-SystemRT/build.gyp:CPP-SystemRT-lib"
			],
			'sources': [
				'mod/python.cpp',
			],
		},
		{
			'target_name': 'CPP-FastCGI-lib',
			'type': 'static_library',
			'sources': [
				'src/record.cpp',
				'src/requestpipe.cpp',
				'src/request.cpp',
				'src/requesthandler.cpp',
				'src/requestmodule.cpp',
				'src/server.cpp',
			],
		},
		{
			'target_name': 'cppfastcgi-daemon',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib', '-lws2_32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread', '-ldl'],
				}]
			],
			"dependencies": [
				'CPP-FastCGI-lib',
				"lib/CPP-Metadata/build.gyp:CPP-Metadata-lib",
				"lib/CPP-SystemRT/build.gyp:CPP-SystemRT-lib"
			],
			'sources': [
				'src/daemon_main.cpp'
			],
		},
	],
}
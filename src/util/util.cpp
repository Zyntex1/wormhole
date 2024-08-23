#include "util.h"

#include "memory.h"
#include "string.h"

#include <algorithm>
#include <cstring>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifdef _WIN32
#include <psapi.h>
#else
#include <link.h>
#endif

void util::console::alloc( ) {
#ifdef _WIN32
	AllocConsole( );

	freopen_s( reinterpret_cast< _iobuf** >( __acrt_iob_func( 0 ) ), "conin$", "r", static_cast< _iobuf* >( __acrt_iob_func( 0 ) ) );
	freopen_s( reinterpret_cast< _iobuf** >( __acrt_iob_func( 1 ) ), "conout$", "w", static_cast< _iobuf* >( __acrt_iob_func( 1 ) ) );
	freopen_s( reinterpret_cast< _iobuf** >( __acrt_iob_func( 2 ) ), "conout$", "w", static_cast< _iobuf* >( __acrt_iob_func( 2 ) ) );
#endif
}

void util::console::free( ) {
#ifdef _WIN32
	fclose( static_cast< _iobuf* >( __acrt_iob_func( 0 ) ) );
	fclose( static_cast< _iobuf* >( __acrt_iob_func( 1 ) ) );
	fclose( static_cast< _iobuf* >( __acrt_iob_func( 2 ) ) );

	FreeConsole( );
#endif
}

static std::vector< util::module_info_t > g_module_info;

bool util::get_module_info( const char* module_name, module_info_t* module_info ) {
	if ( g_module_info.empty( ) ) {
#ifdef _WIN32
		HMODULE mods[ 1024 ];
		HANDLE  proc = GetCurrentProcess( );
		DWORD   cb_needed;

		if ( EnumProcessModules( proc, mods, sizeof( mods ), &cb_needed ) ) {
			for ( unsigned int i = 0; i < ( cb_needed / sizeof( HMODULE ) ); ++i ) {
				char sz_mod_name[ MAX_PATH ];

				if ( GetModuleFileNameA( mods[ i ], sz_mod_name, sizeof( sz_mod_name ) ) ) {
					auto info = MODULEINFO( );

					if ( !GetModuleInformation( proc, mods[ i ], &info, sizeof( info ) ) )
						continue;

					auto name  = std::string( sz_mod_name );
					auto index = name.find_last_of( "\\/" );
					name       = name.substr( index + 1, name.length( ) - index );

					module_info_t mod_info;

					std::snprintf( mod_info.name, sizeof( mod_info.name ), "%s", name.c_str( ) );
					std::snprintf( mod_info.path, sizeof( mod_info.path ), "%s", sz_mod_name );
					mod_info.addr = ( std::uintptr_t ) info.lpBaseOfDll;
					mod_info.size = ( std::size_t ) info.SizeOfImage;

					g_module_info.push_back( mod_info );
				}
			}
		}
#else
		dl_iterate_phdr( []( struct dl_phdr_info* info, std::size_t, void* ) {
			module_info_t mod_info;

			auto name  = std::string( info->dlpi_name );
			auto index = name.find_last_of( "\\/" );
			name       = name.substr( index + 1, name.length( ) - index );

			std::strncpy( mod_info.name, name.c_str( ), sizeof( mod_info.name ) );
			std::strncpy( mod_info.path, info->dlpi_name, sizeof( mod_info.path ) );
			mod_info.addr = info->dlpi_addr + info->dlpi_phdr[ 0 ].p_vaddr;
			mod_info.size = info->dlpi_phdr[ 0 ].p_memsz;

			g_module_info.push_back( mod_info );

			return 0;
		},
		                 nullptr );
#endif
	}

	for ( const auto& info : g_module_info ) {
		if ( _stricmp( info.name, module_name ) )
			continue;

		if ( module_info != nullptr ) {
			*module_info = info;
		}
		return true;
	}

	return false;
}

std::uint8_t* util::pattern_scan( const char* module_name, const char* signature ) noexcept {
	module_info_t module_info;

	if ( !get_module_info( module_name, &module_info ) )
		return nullptr;

	static auto pattern_to_byte = []( const char* pattern ) {
		auto bytes = std::vector< int >{ };
		auto start = const_cast< char* >( pattern );
		auto end   = const_cast< char* >( pattern ) + std::strlen( pattern );

		for ( auto current = start; current < end; ++current ) {
			if ( *current == '?' ) {
				++current;

				if ( *current == '?' )
					++current;

				bytes.push_back( -1 );
			} else {
				bytes.push_back( std::strtoul( current, &current, 16 ) );
			}
		}
		return bytes;
	};

	auto size_of_image = module_info.size;

	auto pattern_bytes = pattern_to_byte( signature );
	auto scan_bytes    = reinterpret_cast< std::uint8_t* >( module_info.addr );

	auto s = pattern_bytes.size( );
	auto d = pattern_bytes.data( );

	for ( auto i = 0ul; i < size_of_image - s; ++i ) {
		bool found = true;

		for ( auto j = 0ul; j < s; ++j ) {
			if ( scan_bytes[ i + j ] != d[ j ] && d[ j ] != -1 ) {
				found = false;
				break;
			}
		}
		if ( found )
			return &scan_bytes[ i ];
	}
}

std::string util::ssprintf( const char* fmt, ... ) {
	va_list ap1, ap2;
	va_start( ap1, fmt );
	va_copy( ap2, ap1 );
	size_t sz = vsnprintf( NULL, 0, fmt, ap1 ) + 1;
	va_end( ap1 );
	char* buf = ( char* ) malloc( sz );
	vsnprintf( buf, sz, fmt, ap2 );
	va_end( ap2 );
	std::string str( buf );
	free( buf );
	return str;
}

bool util::replace( std::string& str, const std::string& from, const std::string& to ) {
	size_t start_pos = str.find( from );
	if ( start_pos == std::string::npos )
		return false;
	str.replace( start_pos, from.length( ), to );
	return true;
}

std::string util::to_upper( std::string str ) {
	std::transform( str.begin( ), str.end( ), str.begin( ), ::toupper );
	return str;
}

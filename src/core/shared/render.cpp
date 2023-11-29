#include "wormhole-sdk/wormhole.h"

void c_render::draw_filled_rect( int x, int y, int w, int h, sdk::color_t color ) {
	wh->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	wh->portal2->surface->draw_filled_rect( x, y, x + w, y + h );
}

void c_render::draw_outlined_rect( int x, int y, int w, int h, sdk::color_t color ) {
	wh->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	wh->portal2->surface->draw_outlined_rect( x, y, x + w, y + h );
}

void c_render::draw_line( int x, int y, int w, int h, sdk::color_t color ) {
	wh->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	wh->portal2->surface->draw_line( x, y, x + w, y + h );
}

bool c_render::create_font( sdk::h_font &font, const char *font_name, int size, bool bold, int flags ) {
	font = wh->portal2->surface->create_font( );
	return wh->portal2->surface->set_font_glyph_set( font, font_name, size, bold ? 800 : 0, 0, 0, flags );
}

void c_render::destruct_font( sdk::h_font font ) {
	for ( const auto &font_range : wh->portal2->font_manager->font_amalgams[ font ].fonts ) {
		const int idx = wh->portal2->font_manager->win32_fonts.find( font_range.win32_font );

		wh->portal2->mem_alloc->free( wh->portal2->font_manager->win32_fonts[ idx ] );
		wh->portal2->font_manager->win32_fonts.remove( idx );
	}

	wh->portal2->font_manager->font_amalgams.remove( font );
}

void c_render::draw_text( int x, int y, sdk::h_font font, sdk::color_t color, bool center, const char *text ) {
	int text_x = x;
	int text_y = y;

	if ( center ) {
		auto text_size = get_text_size( font, text );
		text_x = x - text_size.x / 2;
	}

	wh->portal2->surface->draw_colored_text( font, text_x, text_y, color.r, color.g, color.b, color.a, text );
}

sdk::vec2_t c_render::get_text_size( sdk::h_font font, const char *text ) {
	int text_width, text_height;

	std::string stxt = text;
	wh->portal2->surface->get_text_size( font, std::wstring( stxt.begin( ), stxt.end( ) ).c_str( ), text_width, text_height );

	return sdk::vec2_t( text_width, text_height );
}

void c_render::draw_texture( int x, int y, int w, int h, const char *texture, sdk::color_t color ) {
	int id = wh->portal2->surface->draw_get_texture_id( texture );
	if ( !id ) {
		id = wh->portal2->surface->create_new_texture_id( true );
		wh->portal2->surface->draw_set_texture_file( id, texture, false, true );
	}
	wh->portal2->surface->draw_set_texture( id );
	wh->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	wh->portal2->surface->draw_textured_rect( x, y, x + w, y + h );
}

sdk::vec2_t c_render::get_screen_size( ) {
	int w, h;

	wh->portal2->surface->get_screen_size( w, h );

	return sdk::vec2_t( w, h );
}

sdk::h_font c_render::get_font( unsigned long id ) {
	auto default_font = wh->portal2->scheme->get_font( "DefaultFixedOutline" );

	return default_font + id;
}

sdk::vec2_t c_render::normalize( sdk::vec2_t vec ) {
	const auto screen_size = get_screen_size( );

	return sdk::vec2_t( vec.x / screen_size.x, vec.y / screen_size.y );
}

sdk::vec2_t c_render::to_screen( sdk::vec2_t vec ) {
	const auto screen_size = get_screen_size( );

	return sdk::vec2_t( vec.x * screen_size.x, vec.y * screen_size.y );
}

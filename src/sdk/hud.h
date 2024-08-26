#pragma once

#include "source_sdk.h"

#include <vector>

namespace photon_api {
	class i_hud {
	public:
		vec2_t pos;     // values between (0, 1).
		vec2_t anchor;  // values between (0, 1).
		float  scale;   // scale value to multiply by. (currently useless)
		vec2_t bounds;  // screen boundaries.

		virtual void paint( ) = 0;
	};
}  // namespace photon_api

class c_hud {
public:
	virtual void reg( photon_api::i_hud* hud );    // register a hud component by pointer.
	virtual void unreg( photon_api::i_hud* hud );  // unregister a hud component by pointer.

	virtual void draw_begin( photon_api::i_hud* hud );
	virtual void draw_end( );
	virtual void draw_filled_rect( int x, int y, int w, int h, color_t color );
	virtual void draw_outlined_rect( int x, int y, int w, int h, color_t color, int stroke_width = 1 );
	virtual void draw_line( int x, int y, int w, int h, color_t color );
	virtual void draw_polygon( int n, vertex_t* vertices, color_t color );
	virtual void draw_rounded_rect( int x, int y, int w, int h, color_t color, int rounding );
	virtual void draw_circle( int x, int y, int radius, color_t color );
	virtual void draw_text( int x, int y, h_font font, color_t color, bool center, const char* text );
	virtual void draw_texture( int x, int y, int w, int h, const char* texture, color_t color = color_t( 255, 255, 255, 255 ) );
	virtual void draw_gradient( int x, int y, int w, int h, color_t color1, color_t color2, bool horizontal );
};

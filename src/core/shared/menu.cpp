#include "wormhole-sdk/menu.h"

#include "core/menu/framework.h"

#include <array>
#include <iostream>
#include <vector>

bool c_menu::button( sdk::vec2_t size, const char *label ) {
	return gui::framework::button( size, label );
}
void c_menu::checkbox( bool &val, const char *label ) {
	return gui::framework::checkbox( val, label );
}
void c_menu::slider( int &val, int min, int max, const char *label ) {
	return gui::framework::slider( val, min, max, label );
}
void c_menu::sliderf( float &val, float min, float max, const char *label ) {
	return gui::framework::sliderf( val, min, max, label );
}
void c_menu::colorpicker( sdk::color_t &val, bool &open, const char *label ) {
}
void c_menu::combo( int &val, bool &open, const char *items[], int items_count, const char *label ) {
	std::vector<std::string> items_vector;
	for ( std::size_t i = 0; i < items_count; ++i ) {
		items_vector.push_back( items[ i ] );
	}

	return gui::framework::combo( val, open, items_vector, label );
}
void c_menu::multicombo( int &val, bool &open, const char *items[], int items_count, const char *label ) {
}

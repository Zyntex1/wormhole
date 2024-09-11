#include "signals.h"

e_return_action calc_view_model_lag( e_callback_type type, signal_params_t* params ) {
	return e_return_action::Supercede;
}

bool signals::initialize( ) {
	photon->signal->create( "calc_view_model_lag" )
					->with_callconv( Thiscall )
					->with_parameters( { Pointer, Pointer, Pointer, Pointer } )
					->in_module( MODULE( "client" ) )
					->from_pattern( OS( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 1C 56 6A 00", "56 53 83 EC 24 8B 74 24 30 8B 5C 24 34 6A 00" ) )
					->enable( );

	photon->signal->get( "calc_view_model_lag" )->add_callback( e_callback_type::Pre, &calc_view_model_lag );

	return true;
}

void signals::uninitialize( ) {
	photon->signal->get( "calc_view_model_lag" )->remove_callback( e_callback_type::Pre, &calc_view_model_lag );
	photon->signal->remove( "calc_view_model_lag" );
}

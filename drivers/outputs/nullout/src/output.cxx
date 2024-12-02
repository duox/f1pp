/*output.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::output::nullout::output::output( const core::Iidentifier * ident ) noexcept:
	super( ident )
{
}

/*const status_t f1::pp::output::nullout::output::set_output_filename(const char * filename, size_t length)
{
	// Do not create output file for null output

	// Exit
	__unreferenced_parameter( filename );
	__unreferenced_parameter( length );
	return STATUS_SUCCESS;
}*/
const status_t f1::pp::output::nullout::output::write( const char * string, size_t length )
{
	// Do not output anything for null output

	// Exit
	__unreferenced_parameter( string );
	__unreferenced_parameter( length );
	return STATUS_SUCCESS;
}

const status_t f1::pp::output::nullout::output::begin_output() noexcept
{
	return STATUS_SUCCESS;
}
const status_t f1::pp::output::nullout::output::end_output() noexcept
{
	return STATUS_SUCCESS;
}

/*END OF output.cxx*/

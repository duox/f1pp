/*switch_standard.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_standard::switch_standard( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_standard::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_standard::get_long_name() const NOEXCEPT
{
	return "standard";
}
const char * f1::pp::frontend::f1pp::switch_standard::get_desc() const NOEXCEPT
{
	return "Specify standard for parser";
}
/*const char * f1::pp::frontend::f1pp::switch_standard::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_standard::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_standard::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	status_t status;

	// Check current state
	if( nullchar == *rest )
	{
		application::g_app->fatal_error( _T("%c%s: expected standard name"), switchar, core::string(name).c_str() );
		return STATUS_SUCCESS;
	}

	// Find standard by its name or GUID
	const char * ep;
	core::Tref<pp::Istandard> standard = nullptr;
	if( '{' == *rest )
	{
		f1::guid_t guid;
		guid.parseA( rest.data(), rest.length(), &ep );
		status = pp::service::get().get_standard( standard, guid );
	}
	else
	{
		ep = "";
		status = pp::service::get().get_standard( standard, rest.data(), rest.length() );
	}
	if( status.failed() )
	{
		application::g_app->fatal_error( _T("%c%s: standard not found: %s"), switchar, core::string(name).c_str(), core::string(rest).c_str() );
		return STATUS_SUCCESS;
	}

	core::Tref<pp::context> ctx = application::get_app()->get_context();

	core::Tref<pp::Isyntax> syntax = ctx->extract_syntax( *standard );
#if 1
	if( nullptr != syntax )
	{
		if( _T('-') == switchar )
			ctx->disable_syntax( *syntax );
		else
			ctx->set_syntax( syntax );
	}

	{
		if( _T('-') == switchar )
			ctx->add_preinit_parser_command( *new(std::nothrow) pp::Iparser::cmd_remove_standard( *standard ) );
		else
			ctx->add_preinit_parser_command( *new(std::nothrow) pp::Iparser::cmd_append_standard( *standard ) );
	}
#else
	if( _T('-') != switchar )
		m_standard_list.append( standard );
	else
		m_standard_list.remove( standard );
#endif

	if( nullptr != ep && nullchar != *ep )
		application::g_app->fatal_error( _T("%c%s: unexpected characters after the switch name"), switchar, core::string(name).c_str() );
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_standard::apply( pp::context & ctx )
{
	ctx.append_standards( m_standard_list );
	return STATUS_SUCCESS;
}*/

/*END OF switch_standard.cxx*/

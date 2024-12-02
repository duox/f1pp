/*switch_character_encoding.cxx*/
#include "../pch.h"
#include "../main.h"

f1::pp::frontend::f1pp::switch_character_encoding::switch_character_encoding( unsigned flags ) NOEXCEPT:
	super( flags )
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::pp::frontend::f1pp::switch_character_encoding::get_name() const NOEXCEPT
{
	return nullcstr;
}
const char * f1::pp::frontend::f1pp::switch_character_encoding::get_long_name() const NOEXCEPT
{
	return "char-encoding";
}
const char * f1::pp::frontend::f1pp::switch_character_encoding::get_desc() const NOEXCEPT
{
	return "Specify character literals encoding name";
}
/*const char * f1::pp::frontend::f1pp::switch_character_encoding::get_params() const NOEXCEPT
{
	return nullcstr;
}*/
static const char help_text[] = {
	""
};
const char * f1::pp::frontend::f1pp::switch_character_encoding::get_help_text() const NOEXCEPT
{
	return help_text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

status_t f1::pp::frontend::f1pp::switch_character_encoding::process( char_t switchar, const f1::core::const_string & name, const f1::core::const_string & rest ) noexcept
{
	core::Tconst_ref<nls::Iencoding> enc;
	if( application::get_app()->parse_encoding( enc, rest.data(), rest.length() ).failed() )
		application::g_app->fatal_error( _T("%c%s: expected encoding or codepage name"), switchar, core::string(name).c_str() );
	else
		application::get_app()->get_context()->set_character_encoding( enc );

	return STATUS_SUCCESS;
}
/*const status_t f1::pp::frontend::f1pp::switch_character_encoding::apply( pp::context & ctx )
{
	ctx.get_macros_to_undefine().append( m_macros_to_undefine );

	return STATUS_SUCCESS;
}*/

/*END OF switch_character_encoding.cxx*/

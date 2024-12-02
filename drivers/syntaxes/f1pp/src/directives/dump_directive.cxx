/*dump_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::dump_directive::dump_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::dump_directive::get_name() const
{
	return "dump";
}
// {2BCA0BE4-505D-43B4-934D-CA49DE513343}
const f1::guid_t f1::f1pp::dump_directive::m_guid =
{ 0x2bca0be4, 0x505d, 0x43b4, { 0x93, 0x4d, 0xca, 0x49, 0xde, 0x51, 0x33, 0x43 } };
const f1::guid_t & f1::f1pp::dump_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::dump_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_c_extension) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

enum dump_operand_type_t
{
	dot_auto, dot_macro, dot_namespace
};

const status_t f1::f1pp::dump_directive::parse( pp::Iparser & parser )
{
	status_t status;
	pp::string ident;
	dump_operand_type_t dot = dot_auto;
	core::Tconst_ref<ns::name_space> ns = parser.get_ns_context().get_current_namespace();

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Fetch macro identifier, type and equal sign
	core::Tref<pp::Imacro>		mac;
	bool append = f1par->exists_directive_attribute( "append" );
	f1par->parse_identifier_and_type( mac, &append );

	// Try to dump operand class
	f1par->skip_real_spaces_and_comments();
	if( !par->peek_char( '.' ) )
	{
		f1par->parse_macro_identifier( ident );
		if( ident.empty() )
		{
			f1par->skip_to_newline();
			return f1par->message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		}

		if( "macro" == ident )
		{
			ident.clear();
			dot = dot_macro;
		}
		else if( "namespace" == ident )
		{
			ident.clear();
			dot = dot_namespace;
		}
	}

	// Dump all objects
	if( !ident.empty() )
		goto process;
	for(;;)
	{
		// Check for exit conditions
		f1par->skip_real_spaces_and_comments();
		if( par->is_nl() )
			break;

		// Parse root namespace marker
		bool was_dot = false;
		if( par->skip_char( '.' ) )
		{
			was_dot = true;
			ns = parser.get_ns_context().get_current_namespace();
		}

		// Fetch next identifier
		f1par->parse_macro_identifier( ident );
		if( !was_dot && ident.empty() )
		{
			f1par->skip_to_newline();
			return f1par->message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		}

		// Dump object
	process:
		switch( dot )
		{
		case dot_auto:		dump_object( *f1par, *ns, ident,  mac, append );	break;
		case dot_macro:		dump_macro( *f1par, *ns, ident, mac, append );		break;
		case dot_namespace:	dump_namespace( *f1par, *ns, ident, mac, append );	break;
		default:
			return STATUS_ERROR_INTERNAL;
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::dump_directive::dump_object( f1::f1pp::parser & f1par,
	const ns::name_space & ns_parent, const pp::string & ident,
	pp::Imacro * target_macro, bool append ) const
{
	core::Tref<pp::Imacro> mac;
	if( f1par.find_macro( mac, nullptr, ident ).succeeded() )
		return dump_macro( f1par, *mac, target_macro, append );

	core::Tconst_ref<ns::name_space> ns;
	if( ns_parent.find_subnamespace( ns, ident ).succeeded() )
		return dump_namespace( f1par, *ns, target_macro, append );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::dump_directive::dump_macro( f1::f1pp::parser & f1par,
	const ns::name_space & ns_parent, const pp::string & ident,
	pp::Imacro * target_macro, bool append ) const
{
	core::Tconst_ref<pp::Imacro> mac = nullptr;
	f1par.find_macro( mac, &ns_parent, ident );
	if( nullptr == mac )
		return f1par.message( syntax::IMSG_ERROR_NOT_FOUND_S, ident.c_str() );
	return dump_macro( f1par, *mac, target_macro, append );
}
const status_t f1::f1pp::dump_directive::dump_macro( f1::f1pp::parser & f1par,
	const pp::Imacro & macro,
	pp::Imacro * target_macro, bool append ) const
{
	const pp::string & value = macro.get_value();
	if( nullptr != target_macro )
	{
		if( !append )
			target_macro->set_value( value );	// reset macro value
		else
			target_macro->append_value( value );	// modify macro value
	}
	else
	{
		f1par.get_msgout_context().print( "%s\n", value.c_str() );
	}

	const_cast<pp::Imacro *>(&macro)->on_macro_referenced();

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::dump_directive::dump_namespace( f1::f1pp::parser & f1par,
	const ns::name_space & ns_parent, const pp::string & ident,
	pp::Imacro * target_macro, bool append ) const
{
	core::Tconst_ref<ns::name_space> ns = nullptr;
	ns_parent.find_subnamespace( ns, ident.get_stringslice() );
	if( nullptr == ns )
		return f1par.message( syntax::IMSG_ERROR_NOT_FOUND_S, ident.c_str() );
	return dump_namespace( f1par, *ns, target_macro, append );
}
const status_t f1::f1pp::dump_directive::dump_namespace( f1::f1pp::parser & f1par,
	const ns::name_space & ns,
	pp::Imacro * target_macro, bool append ) const
{
	if( nullptr != target_macro )
	{
		pp::string buf;
		array_foreach( f1::ns::name_list_t::const_iterator, it, ns.get_subname_list() )
		{
			buf.add_part( it->get_printable_name().c_str() );
			buf.add_part( "\n", 1 );
		}

		if( !append )
			target_macro->set_value( buf );	// reset macro value
		else
			target_macro->append_value( buf );	// modify macro value
	}
	else
	{
		array_foreach( f1::ns::name_list_t::const_iterator, it, ns.get_subname_list() )
			f1par.get_msgout_context().print( "%s\n", it->get_printable_name().c_str() );
	}
	return STATUS_SUCCESS;
}

/*END OF dump_directive.cxx*/

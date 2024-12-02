/*pragma_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c99::pragma_directive::pragma_directive( unsigned flags ) __noexcept:
	super( flags )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c99::pragma_directive::get_name() const
{
	return "pragma";
}
// {F6BBC496-C411-4AE6-B085-F924E10F1696}
const f1::guid_t f1::c_pp::c99::pragma_directive::m_guid =
{ 0xf6bbc496, 0xc411, 0x4ae6, { 0xb0, 0x85, 0xf9, 0x24, 0xe1, 0x0f, 0x16, 0x96 } };
const f1::guid_t & f1::c_pp::c99::pragma_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c99::pragma_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return cap_keep_text_eol & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c99::pragma_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#if 1 //def _DEBUG
	core::Tref<f1::c_pp::c99::pragma_directive> drc = type_cast<f1::c_pp::c99::pragma_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, drc->m_pragma_name, pp::string(buf), drc->m_target_pragma );
#else
	return static_cast<f1::c_pp::c99::pragma_directive *>( param )->parse( parser, drc->m_pragma_name, buf );
#endif
}

const status_t f1::c_pp::c99::pragma_directive::parse_pragma( c_pp::parser & cpp_par,
	pp::string & target_ident, core::Tref<c_pp::Ipragma> & target_prg )
{
	const char * s = cpp_par.get_parser()->get_ptr();
	const char * end_s = cpp_par.get_parser()->get_end_ptr();
	status_t status = parse_pragma( cpp_par, s, end_s, target_ident, target_prg );
	cpp_par.get_parser()->set_ptr( s );
	return status;
}

const status_t f1::c_pp::c99::pragma_directive::parse_pragma( c_pp::parser & cpp_par,
	const char *& s, const char * const end_s,
	pp::string & ident, core::Tref<c_pp::Ipragma> & target_prg )
{
	// Check current state
	core::Tref<f1::parser::Iparser> par = cpp_par.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Parse pragmas
	unsigned count = 0;
	const char * old_s;
	pp::string prev_ident;
	core::Tconst_ref<c_pp::Isuper_pragma> super_pragma;
	for(;;)
	{
		// Fetch pragma name
		cpp_par.skip_real_spaces_and_comments( s, end_s );
		old_s = s;

		prev_ident.move( ident );
		if( !cpp_par.parse_macro_identifier( ident, s, end_s ) )
		{
			ident.move( prev_ident );
			break;
		}
		++ count;

		// Find pragma by name
		core::Tref<c_pp::Ipragma> prg;
		if( nullptr != super_pragma )
			super_pragma->find_subpragma( prg, ident );
		else
			cpp_par.find_pragma( prg, ident );
		if( nullptr == prg )
		{
			if( !prev_ident.empty() )
			{
				ident.move( prev_ident );
				s = old_s;
			}
			break;
		}
		target_prg = prg;

		super_pragma = prg.dynamic_pointer_cast<c_pp::Isuper_pragma>();
		if( nullptr == super_pragma )
			break;
	}
	if( 0 == count )
	{
		s = end_s;
		return STATUS_INFORMATION_NOT_PROCESSED;
	}

	return STATUS_SUCCESS;
}

const status_t f1::c_pp::c99::pragma_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::c_pp::parser> cpp_par = type_cast<f1::c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_INVALID_TYPE;

#if 1
	pp::string ident;
	core::Tref<c_pp::Ipragma> target_prg;
	status = parse_pragma( *cpp_par, ident, target_prg );
	if( STATUS_INFORMATION_NOT_PROCESSED == status )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
	if( status.failed() )
		return status;
#else
	// Find pragmas
	unsigned count = 0;
	pp::string ident, prev_ident;
	core::Tref<c_pp::Ipragma> target_prg;
	core::Tconst_ref<c_pp::Isuper_pragma> super_pragma;
	for(;;)
	{
		// Fetch pragma name
		cpp_par->skip_real_spaces_and_comments();
		par->push_pos();
		prev_ident.move( ident );
		if( cpp_par->parse_macro_identifier( ident ).failed() )
		{
			par->pop_pos();
			break;
		}
		++ count;

		// Find pragma by name
		core::Tref<c_pp::Ipragma> prg;
		if( nullptr != super_pragma )
			super_pragma->find_subpragma( prg, ident );
		else
			cpp_par->find_pragma( prg, ident );
		if( nullptr == prg )
		{
			if( !prev_ident.empty() )
			{
				par->pop_pos();
				ident.move( prev_ident );
			}
			break;
		}
		par->pop_discard_pos();
		target_prg = prg;

		super_pragma = prg.dynamic_pointer_cast<c_pp::Isuper_pragma>();
		if( nullptr == super_pragma )
			break;
	}
	if( 0 == count )
	{
		par->skip_to_nl();
		parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}
#endif
	// Parse pragma
	if( nullptr != target_prg && 0 != target_prg->get_caps( Ipragma::fl_self_parse_arglist ) )
	{
		// Skip to end of line
		pp::string param_str;
		cpp_par->parse_to_newline( param_str, true );

		// Execute pragma parser directly
		status = target_prg->parse( parser, param_str );
	}
	else
	{
		// Capture preprocessed line
		m_pragma_name.move( ident );
		m_target_pragma.move( target_prg );
		status = cpp_par->capture_line( line_capture_callback, this );
	}

	// Exit
	return status;
}
const status_t f1::c_pp::c99::pragma_directive::execute( pp::Iparser & parser, const pp::string & name, const pp::string & args, c_pp::Ipragma * target_pragma )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<f1::c_pp::parser *>( &parser );
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_INVALID_TYPE;

	// Find pragma and parse
	if( nullptr != target_pragma )
		return target_pragma->parse( parser, args );

	// Find pragma and transfer it to the output
	if( !name.is_empty() )
	{
		bool found = false;
		array_foreach( core::const_string_list_t::const_iterator, it, cpp_par->get_known_pragma_list() )
		{
			if( name.get_stringslice().equal( *it ) )
			{
				found = true;
				break;
			}
		}
		if( !found )	// If pragma wasn't found, issue a warning
		{
			const pp::Istandard * standard = pp::service::get().find_item_standard( Ipragma::m_item_class_guid, name );
			if( nullptr != standard )
				parser.message( syntax::IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS, name.c_str(), standard->get_description_string() );
			else
				parser.message( syntax::IMSG_WARNING_UNKNOWN_PRAGMA_S, name.c_str() );
		}
	}
	else
		parser.message( syntax::IMSG_WARNING_UNKNOWN_PRAGMA );

	// Write out pragma and its arguments (if we are not parsing an expression)
	if( null == parser.get_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode ) )
	{
		core::Tthin_ptr<pp::Ioutput> outp = parser.get_output();

		if( !cpp_par->m_new_line )
			outp->write( "\n", 1 );

		outp->write( "#pragma " );
		if( !name.empty() )
			outp->write( name.get_string().c_str() );
		outp->write( args.data(), args.length() );

		outp->write( "\n", 1 );
		cpp_par->m_new_line = true;
	}

	// Exit
	return STATUS_SUCCESS;
}

/*END OF pragma_directive.cxx*/

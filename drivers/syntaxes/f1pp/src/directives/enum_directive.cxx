/*enum_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::enum_directive::enum_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::enum_directive::get_name() const
{
	return "enum";
}

// {B2B2C4D0-E426-48f2-B7AC-8FB84182B5AA}
const f1::guid_t f1::f1pp::enum_directive::m_guid =
{ 0xb2b2c4d0, 0xe426, 0x48f2, { 0xb7, 0xac, 0x8f, 0xb8, 0x41, 0x82, 0xb5, 0xaa } };
const f1::guid_t & f1::f1pp::enum_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::enum_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return 0;
}

// Operations
const status_t f1::f1pp::enum_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	f1::ns::context & ns_ctx = parser.get_ns_context();

	// Fetch enumeration identifier
	pp::string enum_ident;
	f1par->skip_real_spaces_and_comments();
	f1par->parse_definition_identifier( enum_ident );
	if( enum_ident.is_empty() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
		return STATUS_SUCCESS;
	}
	if( !parser.check_limit_value_min( pp::limit_t::li_maximum_identifier_length, enum_ident.length() ) )
		parser.message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );

	// Choose namespace for the enumeration
	core::Tref<ns::name_space> previous_ns = ns_ctx.get_current_namespace();
	if( f1par->exists_directive_attribute( "global" ) )
		ns_ctx.set_current_namespace( &ns_ctx.get_root_namespace() );
	else if( f1par->exists_directive_attribute( "public" ) )
		ns_ctx.move_to_parent_namespace();

	// Create object
	core::Tref<enumeration>	en;
	core::Tref<ns::name>	nm;
	if( ns_ctx.find_name( nm, enum_ident.data(), enum_ident.length() ).succeeded() )
		parser.message( f1pp::syntax::IMSG_ERROR_ALREADY_DEFINED_S, enum_ident.c_str() );
	else
	{
		if( f1par->create_enumeration( en, enum_ident.data(), enum_ident.length() ).failed_status( status ) )
			return status;
		en->set_definition_location( *f1par );

		if( f1par->exists_directive_attribute( "bitwise" ) )
			en->modify_flags( enumeration::fl_bitwise );
		if( f1par->exists_directive_attribute( "deprecated" ) )
			en->modify_flags( enumeration::fl_deprecated );
		if( f1par->exists_directive_attribute( "open" ) )
			en->modify_flags( enumeration::fl_open );
		if( f1par->exists_directive_attribute( "sealed" ) )
			en->modify_flags( enumeration::fl_sealed );
	}

	if( previous_ns != ns_ctx.get_current_namespace() )
		ns_ctx.set_current_namespace( previous_ns );

	// Parse enumeration base enumerations
	f1par->skip_real_spaces_and_comments();
	if( par->skip_char( ':' ) )
	{
		for(;;)
		{
			// Parse base enumeration identifier
			pp::string base_ident;
			f1par->skip_real_spaces_and_comments();
			f1par->parse_macro_identifier( base_ident );
			if( base_ident.is_empty() )
			{
				parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
				f1par->skip_to_newline();
			}

			// Append base identifier
			if( nullptr != en )
			{
				switch( en->append_base_enumeration( base_ident ).get_value() )
				{
					case STATUS_ERROR_ALREADY_EXISTS:
						parser.message( f1pp::syntax::IMSG_ERROR_ALREADY_USED_S, base_ident.c_str() );
						break;
					case STATUS_ERROR_DISABLED:
						parser.message( f1pp::syntax::IMSG_ERROR_ENUM_IS_SEALED_S, base_ident.c_str() );
						break;
					default:
						break;
				}
			}

			// Iterate
			f1par->skip_real_spaces_and_comments();
			if( !par->skip_char( ',' ) )
				break;
		}
	}

	// Fetch expression string and expand macros
	frame * frm = new(std::nothrow) frame( *f1par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_enum = en;
	frm->m_output.initialize();
	frm->m_storage.initialize( *parser.get_context() );

	f1par->modify_f1pp_flags(
		f1::f1pp::parser::f1pp_flags_t::fl_enum_mode,
		null,
		&frm->m_old_f1pp_flags
		);
	f1par->modify_c_flags(
		f1::c_pp::parser::c_flags_t::fl_drop_pass_through_directives,
		null,
		&frm->m_old_c_pp_flags
		);
	f1par->activate_enum_callback( frm->m_enum_callback );
	parser.set_output( &frm->m_output, &frm->m_old_output );
	parser.set_storage( &frm->m_storage, &frm->m_old_storage );
	parser.enter_directive( *frm );

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::f1pp::enum_directive::enum_callback::on_next_item( f1pp::parser & parser )
{
	status_t status;

	// Check current state
	if( m_ident.empty() )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );

	enum_directive::frame * frm = type_cast<enum_directive::frame *>( parser.get_directive_frame( enum_directive::m_guid ) );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "#enum" );
	if( frm->get_closing_directive_guid() != endenum_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "#enum" );

	// Get current output
	const pp::string & buf = frm->m_storage.get_string();
	const char *			s = buf.data();
	const char * const	end_s = buf.data_end();
	pp::variant value;

	parser.skip_spaces_and_comments( s, end_s );
	if( s < end_s )
	{
		if( parser.parse_expression( value, s, end_s ).failed_status( status ) )
			return status;
		parser.skip_spaces_and_comments( s, end_s );
		if( s < end_s )
			parser.message( syntax::IMSG_ERROR_INVALID_VALUE );
	}
	else
	{
		if( !frm->m_enum->get_next_value( parser, value, status ) )
			status = value.set_int( 0 );
	}

	if( frm->m_enum->append_element( m_ident, value, m_attrs ).failed_status( status ) )
	{
		if( STATUS_ERROR_ALREADY_EXISTS == status )
			parser.message( f1pp::syntax::IMSG_ERROR_ALREADY_DEFINED_S, m_ident.c_str() );
	}

	// Done
	m_ident.clear();
	m_attrs = nullptr;
	frm->m_storage.reset();

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::enum_directive::enum_callback::append_attributes( f1pp::parser & parser, pp::attribute_list & attrs )
{
	// Check current state
	if( !m_ident.empty() )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_ATTRIBUTES );

	// Attach attributes
	if( nullptr != m_attrs )
		m_attrs->append_attributes( attrs );
	else
		m_attrs = &attrs;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::enum_directive::enum_callback::set_identifier( f1pp::parser & parser, const pp::string & str )
{
	// Check current state
	if( !m_ident.empty() )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_IDENTIFIER );

	// Copy string
	m_ident = str;

	// Exit
	return STATUS_SUCCESS;
}

f1::pp::Iframe * f1::f1pp::enum_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return nullptr;
	return new(std::nothrow) frame( *f1par );
}
/*const status_t f1::f1pp::enum_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	const char *		   s = (const char *) out_buf.get_ptr();
	const char * const end_s = (const char *) out_buf.get_end_ptr();

	// Fetch macro identifier
	pp::string ident;
	f1par->skip_real_spaces_and_comments( s, end_s );
	f1par->parse_macro_identifier( ident, s, end_s );
	if( ident.is_empty() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
	}
	else
	{
		// Fetch optional value
		pp::variant value;
		f1par->skip_real_spaces_and_comments( s, end_s );
		if( '=' == *s )
		{
			++ s;
			f1par->skip_real_spaces_and_comments( s, end_s );
			status = f1par->parse_expression( value, s, end_s );
		}
		else if( nullptr != m_enum )
		{
			if( m_enum->get_element_list().empty() )
			{
				status = value.set_int( 0 );
			}
			else
			{
				const pp::variant & last_value = (*m_enum->get_element_list().rbegin())->get_value_variant();
				switch( last_value.get_type() )
				{
				case pp::variant::ty_int:
					status = value.set_int( last_value.as_int() + 1 );
					break;
				case pp::variant::ty_uint:
					status = value.set_uint( last_value.as_uint() + 1 );
					break;
				case pp::variant::ty_float:
					status = value.set_float( last_value.as_float() + 1 );
					break;
				default:
					parser.message( f1pp::syntax::IMSG_ERROR_ENUM_REQUIRES_INITIALIZER );
					status = STATUS_ERROR_INVALID_FORMAT;
					break;
				}
			}
		}
		else
			status = STATUS_ERROR_INVALID_FORMAT;

		// Add enumeration element
		if( status.succeeded() )
		{
			if( m_enum->append_element( ident, value ).failed_status( status ) )
			{
				if( STATUS_ERROR_ALREADY_EXISTS == status )
					parser.message( f1pp::syntax::IMSG_ERROR_ALREADY_DEFINED_S, ident.c_str() );
				else
					return status;
			}
		}

		// Check for unexpected characters after the statement
		f1par->skip_real_spaces_and_comments( s, end_s );
		//if( !par->is_eof() && !par->is_nl() )
		if( s < end_s )
		{
			parser.message( c_pp::syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );
			//f1par->skip_to_newline();
		}
	}

	// Fetch next element
	f1par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::enum_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::f1pp::enum_directive> drc = type_cast<f1::f1pp::enum_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::enum_directive *>( param )->execute( parser, buf );
#endif
}*/
f1::f1pp::enum_directive::frame::frame( f1pp::parser & par ):
	super( par )
{
}
const char * f1::f1pp::enum_directive::frame::get_openning_directive_name() const
{
	return "enum";
}
const char * f1::f1pp::enum_directive::frame::get_closing_directive_name() const
{
	return "endenum";
}
const f1::guid_t & f1::f1pp::enum_directive::frame::get_openning_directive_guid() const
{
	return enum_directive::m_guid;
}
const f1::guid_t & f1::f1pp::enum_directive::frame::get_closing_directive_guid() const
{
	return endenum_directive::m_guid;
}

/*END OF enum_directive.cxx*/

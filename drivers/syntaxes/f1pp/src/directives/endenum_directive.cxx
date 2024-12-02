/*endenum_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::endenum_directive::endenum_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::endenum_directive::get_name() const
{
	return "endenum";
}
// {D4D8C7F5-10AA-4d29-97CF-0BC63EA0D3A6}
const f1::guid_t f1::f1pp::endenum_directive::m_guid =
{ 0xd4d8c7f5, 0x10aa, 0x4d29, { 0x97, 0xcf, 0xb, 0xc6, 0x3e, 0xa0, 0xd3, 0xa6 } };
const f1::guid_t & f1::f1pp::endenum_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::endenum_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_c_extension) & mask;
}

// Operations
const status_t f1::f1pp::endenum_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INVALID_TYPE;

	enum_directive::frame * frm = type_cast<enum_directive::frame *>( parser.get_directive_frame() );
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endenum" );
	if( frm->get_closing_directive_guid() != endenum_directive::m_guid )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#endenum" );

	f1par->deactivate_enum_callback( frm->m_enum_callback );

#if __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__
	f1par->end_subsection();
#endif // __F1_C_PP__CONFIG__SUPPORT_MACRO_SUBSECTIONS__

	frm->check_different_sources();	// warn if sources differ

	if( nullptr != frm->m_enum )
	{
		// Parse all elements
		const pp::string & buf = frm->m_storage.get_string();
		const char *			s = buf.data();
		const char * const	end_s = buf.data_end();
		while( s < end_s )
		{
			// Parse attribute list, if present
			core::Tref<pp::attribute_list> attrs;
			f1par->skip_spaces_and_comments( s, end_s );
			if( s >= end_s )
				break;
			if( '[' == *s && f1par->parse_attribute_list( attrs, s, end_s ).failed() )
			{
				f1par->skip_to_newline( s, end_s );
				continue;
			}

			// Parse identifier
			pp::string ident;
			f1par->skip_real_spaces_and_comments( s, end_s );
			f1par->parse_definition_identifier( ident, s, end_s );
			if( ident.is_empty() )
			{
				parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
				f1par->skip_to_newline( s, end_s );
				continue;
			}
			if( !parser.check_limit_value_min( pp::limit_t::li_maximum_identifier_length, ident.length() ) )
				parser.message( syntax::IMSG_WARNING_TOO_LONG_IDENTIFIER );

			// Parse optional '='
			pp::variant value;
			f1par->skip_real_spaces_and_comments( s, end_s );
			if( '=' == *s || (s < end_s && ',' != *s && '\n' != *s && '\r' != *s) )
			{
				if( '=' == *s )
				{
					++ s;
					f1par->skip_real_spaces_and_comments( s, end_s );
				}

				// Parse expression
				if( f1par->parse_expression( value, s, end_s ).failed_status( status ) )
				{
					f1par->skip_to_newline( s, end_s );
					continue;
				}
			}
			else
			{
				if( !frm->m_enum->get_next_value( *f1par, value, status ) )
				{
					if( status.failed() )
					{
						f1par->skip_to_newline( s, end_s );
						continue;
					}
					status = value.set_int( 0 );
				}
			}

			// Append element
			if( status.succeeded() )
			{
				if( frm->m_enum->append_element( ident, value, attrs ).failed_status( status ) )
				{
					if( STATUS_ERROR_ALREADY_EXISTS == status )
						parser.message( f1pp::syntax::IMSG_ERROR_ALREADY_DEFINED_S, ident.c_str() );
					else
						break;
				}
			}

			// Iterate
			f1par->skip_real_spaces_and_comments( s, end_s );
			if( ',' == *s )
			{
				++ s;
				f1par->skip_real_spaces_and_comments( s, end_s );
			}
		}
	}

	// Close the directive
	parser.set_output( frm->m_old_output );
	parser.set_storage( frm->m_old_storage );
	f1par->modify_c_flags( frm->m_old_c_pp_flags, ~frm->m_old_c_pp_flags );
	f1par->modify_f1pp_flags( frm->m_old_f1pp_flags, ~frm->m_old_f1pp_flags );
	parser.leave_directive();

	// Exit
	return STATUS_SUCCESS;
}

/*END OF endenum_directive.cxx*/

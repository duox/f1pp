/*embed_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::embed_directive::embed_directive( unsigned flags ):
	super( flags )
{
}

const status_t f1::f1pp::embed_directive::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Add predefined parameters
	m_column_count_parameter.initialize();
	append_parameter( m_column_count_parameter );

	m_element_byte_size_parameter.initialize();
	append_parameter( m_element_byte_size_parameter );

	m_element_prefix_parameter.initialize();
	append_parameter( m_element_prefix_parameter );

	m_element_suffix_parameter.initialize();
	append_parameter( m_element_suffix_parameter );

	m_element_separator_parameter.initialize();
	append_parameter( m_element_separator_parameter );

	m_offset_parameter.initialize();
	append_parameter( m_offset_parameter );

	m_radix_parameter.initialize();
	append_parameter( m_radix_parameter );

	m_timeout_parameter.initialize();
	append_parameter( m_timeout_parameter );

	// Exit
	return STATUS_SUCCESS;
}

unsigned f1::f1pp::embed_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol | cap_override) & mask;
}

// Operations
const status_t f1::f1pp::embed_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;
	resource_state	state;

	// Check current state
	core::Tref<f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Process all attributes
	array_foreach( parameter_list_t::const_iterator, it, m_parameter_list )
	{
		const parameter_desc & desc = *it;
		core::Tref<pp::attribute> attr;
		if( f1pp_par->find_directive_attribute( attr, desc.m_parameter->get_name() ).succeeded() ||
			f1pp_par->find_directive_attribute( attr, desc.m_parameter->get_alt_name() ).succeeded() )
		{
			const pp::arg_list * args = attr->get_arg_list();
			const pp::arg_list::arg_desc_t * arg = nullptr;
			if( nullptr == args ||
				0 == args->get_arg_list().size() ||
				nullptr == (arg = args->get_arg_desc( 0 ))
				)
			{
				if( 0 == desc.m_parameter->get_caps( Iparameter::cap_no_parameter ) )
					parser.message( syntax::IMSG_ERROR_EXPECTED_ARG_LIST );
				else
					desc.m_parameter->parse_argument( *f1pp_par, state, pp::string() );
			}
			else
			{
				desc.m_parameter->parse_argument(
					*f1pp_par,
					state,
					arg->m_expanded_value.empty() ? arg->m_value : arg->m_expanded_value
					);
			}
		}
	}

	// Exit
	return super::execute( parser, buf );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::embed_directive::column_count_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	const char * ep = nullcstr;
	uint64_t val = _strntoui64( arg.data(), arg.length(), &ep, 0 );
	if( nullcstr != ep )
	{
		state.m_columns = unsigned(val);
		if( state.m_columns != val )
			return parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
	}
	parser.skip_real_spaces_and_comments( ep, arg.data_end() );

	if( nullcstr == ep || size_t(ep - arg.data()) != arg.length() )
		parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::embed_directive::element_byte_size_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	const char * ep = nullcstr;
	uint64_t val = _strntoui64( arg.data(), arg.length(), &ep, 0 );
	if( nullcstr != ep )
	{
		state.m_bytes_per_element = uint8_t(val);
		if( state.m_bytes_per_element != val )
			return parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
	}
	parser.skip_real_spaces_and_comments( ep, arg.data_end() );

	if( nullcstr == ep || size_t(ep - arg.data()) != arg.length() )
		parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::embed_directive::element_prefix_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	state.m_element_prefix = arg;

	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::embed_directive::element_suffix_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	state.m_element_suffix = arg;

	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::embed_directive::element_separator_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	state.m_element_separator = arg;

	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::embed_directive::offset_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	const char * ep = nullcstr;
	uint64_t val = _strntoui64( arg.data(), arg.length(), &ep, 0 );
	if( nullcstr != ep )
	{
		state.m_offset = val;
		if( state.m_offset != val )
			return parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
	}
	parser.skip_real_spaces_and_comments( ep, arg.data_end() );

	if( nullcstr == ep || size_t(ep - arg.data()) != arg.length() )
		parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::embed_directive::radix_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	const char * ep = nullcstr;
	uint64_t val = _strntoui64( arg.data(), arg.length(), &ep, 0 );
	if( nullcstr != ep )
	{
		state.m_radix = uint8_t(val);
		if( state.m_radix != val )
			return parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
	}
	parser.skip_real_spaces_and_comments( ep, arg.data_end() );

	if( nullcstr == ep || size_t(ep - arg.data()) != arg.length() )
		parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::embed_directive::timeout_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	const char * ep = nullcstr;
	uint64_t val = _strntoui64( arg.data(), arg.length(), &ep, 0 );
	if( nullcstr != ep )
	{
		state.m_timeout = uint32_t(val);
		if( state.m_timeout != val )
			return parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
	}
	parser.skip_real_spaces_and_comments( ep, arg.data_end() );

	if( nullcstr == ep || size_t(ep - arg.data()) != arg.length() )
		parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

	return STATUS_SUCCESS;
}

/*END OF embed_directive.cxx*/

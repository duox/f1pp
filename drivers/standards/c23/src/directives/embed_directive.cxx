/*embed_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::c_pp::c23::embed_directive * f1::c_pp::c23::embed_directive::m_instance = nullptr;

f1::c_pp::c23::embed_directive::embed_directive( unsigned flags ):
	super( flags )
{
	if( nullptr == m_instance )
		m_instance = this;
}
f1::c_pp::c23::embed_directive::~embed_directive()
{
	if( m_instance == this )
		m_instance = nullptr;
}

//f1::c_pp::c23::embed_directive::Iparameter * f1::c_pp::c23::embed_directive::m_intrinsic_parameters[] = {
//	&f1::c_pp::c23::embed_directive::m_if_empty_parameter
//};


const status_t f1::c_pp::c23::embed_directive::initialize()
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Add predefined parameters
	m_if_empty_parameter.initialize();
	append_parameter( m_if_empty_parameter );

	m_limit_parameter.initialize();
	append_parameter( m_limit_parameter );

	m_prefix_parameter.initialize();
	append_parameter( m_prefix_parameter );

	m_suffix_parameter.initialize();
	append_parameter( m_suffix_parameter );

	// Exit
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::c_pp::c23::embed_directive::get_name() const
{
	return "embed";
}

// {08F7DFFC-D0EB-49B9-B859-852D312BE89D}
const f1::guid_t f1::c_pp::c23::embed_directive::m_guid =
{ 0x08f7dffc, 0xd0eb, 0x49b9, { 0xb8, 0x59, 0x85, 0x2d, 0x31, 0x2b, 0xe8, 0x9d } };
const f1::guid_t & f1::c_pp::c23::embed_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::c_pp::c23::embed_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_keep_text_eol) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parameters

const status_t f1::c_pp::c23::embed_directive::append_parameter( Iparameter & param )
{
	// Check current state
	const char * name = param.get_name();
	__debugbreak_if( nullptr == name )
		return STATUS_ERROR_INVALID_PARAMETER;

	// Check that there's no parameter with such name is defined
	core::Tconst_ref<Iparameter> existing_param;
	__debugbreak_if( find_parameter( existing_param, pp::string( name ) ).succeeded() )
		return STATUS_ERROR_ALREADY_EXISTS;

	// Append parameter
	parameter_desc * desc = m_parameter_list.append_new();
	__debugbreak_if( nullptr == desc )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;
	desc->m_parameter = &param;
	desc->m_identifier.assign( name );

	name = param.get_alt_name();
	if( nullptr != name )
	{
		desc = m_parameter_list.append_new();
		__debugbreak_if( nullptr == desc )
			return STATUS_ERROR_INSUFFICIENT_MEMORY;
		desc->m_parameter = &param;
		desc->m_identifier.assign( name );
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::remove_parameter( Iparameter & param )
{
	parameter_list_t	params;
	array_foreach( parameter_list_t::iterator, it, m_parameter_list )
	{
		parameter_desc & desc = *it;
		if( &param != desc.m_parameter )
		{
			parameter_desc * new_desc = params.append_new();
			__debugbreak_if( nullptr == new_desc )
				return STATUS_ERROR_INSUFFICIENT_MEMORY;
			new_desc->m_parameter. move( desc.m_parameter );
			new_desc->m_identifier.move( desc.m_identifier );
		}
	}
	m_parameter_list.move( params );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::remove_parameter( const char * name, size_t len )
{
	if( size_t(-1) == len )
		len = core::string::length( name );

	parameter_list_t	params;
	array_foreach( parameter_list_t::iterator, it, m_parameter_list )
	{
		parameter_desc & desc = *it;
		if( !desc.m_identifier.equal( name, len ) )
		{
			parameter_desc * new_desc = params.append_new();
			__debugbreak_if( nullptr == new_desc )
				return STATUS_ERROR_INSUFFICIENT_MEMORY;
			new_desc->m_parameter. move( desc.m_parameter );
			new_desc->m_identifier.move( desc.m_identifier );
		}
	}
	m_parameter_list.move( params );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::remove_all_parameters()
{
	return m_parameter_list.remove_all();
}
// TODO: process spaces like in "f1 :: param"
const status_t f1::c_pp::c23::embed_directive::find_parameter( core::Tconst_ref<Iparameter> & res, const pp::string & name ) const
{
	array_foreach( parameter_list_t::const_iterator, it, m_parameter_list )
	{
		const parameter_desc & desc = *it;
		if( desc.m_identifier.equal( name.data(), name.length() ) )
		{
			res = desc.m_parameter;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::c_pp::c23::embed_directive::parse( pp::Iparser & parser )
{
	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Expand macros in the string
	cpp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
#ifdef _DEBUG
	core::Tref<f1::c_pp::c23::embed_directive> drc = type_cast<f1::c_pp::c23::embed_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, pp::string( buf ) );
#else
	return static_cast<f1::c_pp::c23::embed_directive *>( param )->execute( parser, pp::string( buf ) );
#endif
}
const status_t f1::c_pp::c23::embed_directive::execute( pp::Iparser & parser, const pp::string & buf )
{
	status_t status;
	resource_state	state;

	// Check current state
	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// If nothing is specified, just bail out
	const char *			s = buf.data();
	const char * const	end_s = buf.data_end();

	cpp_par->skip_real_spaces_and_comments( s, end_s );
	if( s >= end_s )
		return cpp_par->message( f1::c_pp::syntax::IMSG_FATAL_ERROR_INVALID_FILE_NAME );

	// Parse filename
	if( cpp_par->parse_include_multi_filename( state.m_file_name, state.m_include_flags, s, end_s ).failed_status( status ) )
		return status;

	// Parse and execute all embed parameters
	if( parse_parameter_list( state, *cpp_par, s, end_s ).failed_status( status ) )
		return status;

	// Check for unexpected characters in the end
	cpp_par->skip_real_spaces_and_comments();
	if( s < end_s )
	{
		parser.message( syntax::IMSG_WARNING_UNEXPECTED_CHARACTERS );
//		cpp_par->skip_to_newline();
	}

	// Execute the directive
	if( execute( *cpp_par, state ).failed_status( status ) )
		return status;

	// Exit
	return status;
}
const status_t f1::c_pp::c23::embed_directive::parse_parameter_list( resource_state & state, c_pp::parser & parser, const char * & s, const char * end_s )
{
	status_t status;

	// Parse and execute all embed parameters
	for(;;)
	{
		// Parse argument identifier and its argument
		parser.skip_real_spaces_and_comments( s, end_s );
		if( s >= end_s )
			break;

		pp::string str;
		if( !parser.parse_macro_identifier( str, s, end_s ) )
			break;

		// Find parameter
		core::Tconst_ref<Iparameter> param;
		if( find_parameter( param, str ).failed() )
			parser.message( syntax::IMSG_ERROR_UNSUPPORTED_S, str.c_str() );

		// Check whether there's a parameter
		pp::string arg;
		parser.skip_real_spaces_and_comments( s, end_s );
		if( nullptr != param && 0 == param->get_caps( Iparameter::cap_no_parameter ) )
		{
			if( s >= end_s || '(' != *s )
			{
				parser.message( syntax::IMSG_ERROR_EXPECTED_ARG_LIST );
				param = nullptr;
			}
			else
			{
				// Fetch argument
				const char * start = ++ s;
				if( parser.skip_to_separator( s, end_s, ')' ).failed_status( status ) )
					return status;
				arg.add_part( start, s - start );
				if( ')' == *s )
					++ s;
				else
					parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, ")" );
			}
		}

		// Process argument and exit
		if( nullptr != param )
		{
			if( param->parse_argument( parser, state, arg ).failed_status( status ) )
				return status;
		}
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::parse_parameter_list( resource_state & state, c_pp::parser & parser )
{
	const char * s = parser.get_parser()->get_ptr();
	const char * const end_s = parser.get_parser()->get_end_ptr();	// capture end pointer for the current patch or source

	status_t status = parse_parameter_list( state, parser, s, end_s );
	if( s <= end_s )	// only set pointer if there's something left (otherwise pointer from completed patch will raise an ASSERT)
		parser.get_parser()->set_ptr( s );
	return status;
}

f1::c_pp::c23::embed_directive::resource_state::resource_state():
	m_include_flags( null ),
	m_element_separator( "," ),
	m_offset( 0 ), m_length( uint64_t(-1) ),
	m_buffer_size( 0 ), m_timeout( 0 ), m_radix( 10 ), m_bytes_per_element( 1 ), m_columns( 0 )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned f1::c_pp::c23::embed_directive::Iparameter::get_caps( unsigned mask ) const
{
	__unreferenced_parameter( mask );
	return 0;
}
const char * f1::c_pp::c23::embed_directive::Iparameter::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullptr;
}
const char * f1::c_pp::c23::embed_directive::Iparameter::get_alt_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullptr;
}
const status_t f1::c_pp::c23::embed_directive::Iparameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( state );
	__unreferenced_parameter( arg );
	return STATUS_ERROR_PURE_CALL;
}

const status_t f1::c_pp::c23::embed_directive::if_empty_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	state.m_if_empty = arg;

	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::limit_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	const char * ep = nullcstr;
	uint64_t val = _strntoui64( arg.data(), arg.length(), &ep, 0 );
	if( nullcstr != ep )
	{
		state.m_length = val;
		if( state.m_length != val )
			return parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );
	}
	parser.skip_real_spaces_and_comments( ep, arg.data_end() );

	if( nullcstr == ep || size_t(ep - arg.data()) != arg.length() )
		parser.message( syntax::IMSG_ERROR_INVALID_INTEGER_OPERAND );

	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::prefix_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	state.m_prefix = arg;

	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}
const status_t f1::c_pp::c23::embed_directive::suffix_parameter::parse_argument(
	f1::c_pp::parser & parser, resource_state & state, const pp::string & arg ) const
{
	state.m_suffix = arg;

	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execution

const status_t f1::c_pp::c23::embed_directive::execute( f1::c_pp::parser & parser, const resource_state & state )
{
	status_t status;

	// Locate the file
	core::string	full_file_name;
	if( parser.resolve_include_filename( full_file_name, state.m_file_name, state.m_include_flags ).failed() )
		return parser.message( syntax::IMSG_FATAL_ERROR_FILE_NOT_FOUND, state.m_file_name.c_str() );

	// Open the file
	f1::io::file_istream is( full_file_name );
	if( is.bad() )
		return parser.message( syntax::IMSG_FATAL_ERROR_CANNOT_OPEN_FILE, full_file_name.c_str() );

	// Now perform various checks
	__debugbreak_if( state.m_radix <= 1 && 10 + 26 < state.m_radix )
		return parser.message( syntax::IMSG_FATAL_ERROR_INVALID_PARAMETER );
	__debugbreak_if( state.m_bytes_per_element < 1 && 8 < state.m_bytes_per_element )
		return parser.message( syntax::IMSG_FATAL_ERROR_INVALID_PARAMETER );
	__debugbreak_if( !__is_power_of_two( state.m_bytes_per_element ) )
		return parser.message( syntax::IMSG_FATAL_ERROR_INVALID_PARAMETER );

	const f1::io::length_t file_length = is.get_size();

	if( 0 == file_length )
	{
		if( !state.m_if_empty.empty() )
			parser.output( state.m_if_empty );
		return STATUS_SUCCESS;
	}

	f1::io::length_t resource_length = state.m_length;
	__debugbreak_if( 0 != file_length % state.m_bytes_per_element )
		return parser.message( syntax::IMSG_FATAL_ERROR_INVALID_PARAMETER );
	__debugbreak_if( state.m_offset >= file_length )
		return parser.message( syntax::IMSG_FATAL_ERROR_INVALID_PARAMETER );
	if( uint64_t(-1) == resource_length )
		resource_length = file_length - state.m_offset;
	else
	{
		__debugbreak_if( resource_length >= file_length )
			return parser.message( syntax::IMSG_FATAL_ERROR_INVALID_PARAMETER );
		__debugbreak_if( file_length - resource_length < state.m_offset )
			return parser.message( syntax::IMSG_FATAL_ERROR_INVALID_PARAMETER );
	}
	if( 0 != state.m_offset )
		is.seek( state.m_offset );

	// Allocate space for buffer
	unsigned buffer_byte_size = unsigned(state.m_buffer_size)*state.m_bytes_per_element;
	if( 0 == state.m_buffer_size )
		buffer_byte_size = 512;
	else __debugbreak_if( 64*1024 < buffer_byte_size )
		buffer_byte_size = 64*1024;
	buffer_byte_size = (buffer_byte_size / state.m_bytes_per_element) * state.m_bytes_per_element;

	core::buffer tmp_buf;
	if( tmp_buf.set_data_size( buffer_byte_size ).failed_status( status ) )
		return status;
	void * buf = tmp_buf.get_writable_ptr();

	// Output prefix
	if( !state.m_prefix.empty() )
		parser.output( state.m_prefix );

	// Start reading data
	bool first = true;
	char static_buf[256];
	unsigned column_index = 0;
	f1::io::length_t size_left = resource_length;
	time_t start_time = ::time( nullptr );
	while( size_left > 0 )
	{
		// Read next data block into the buffer
		const size_t block_size = (size_t) __min( size_left, buffer_byte_size );
		size_t nread;
		if( is.read( buf, block_size, &nread ).failed_status( status ) )
			break;
		if( 0 == nread )
			break;
		size_left -= nread;

		// Check for end of line
		if( 0 != state.m_columns && column_index >= state.m_columns )
		{
			parser.output( "\n", 1 );
			column_index = 0;
		}

		// Print the elements
		for( pc_uint8_t s = (pc_uint8_t) buf, end_s = s + nread; s < end_s; ++ s )
		{
			// Output element prefix and element separator
			if( !state.m_element_prefix.empty() )
				parser.output( state.m_element_prefix );
			if( !first && !state.m_element_separator.empty() )
				parser.output( state.m_element_separator );
			first = false;

			// Fetch the data
			uint64_t data;
			switch( state.m_bytes_per_element )
			{
			case 1:	data = *PCB(s);	break;
			case 2:	data = *PCW(s);	break;
			case 4:	data = *PCD(s);	break;
			case 8:	data = *PCQ(s);	break;
			default:
				__assert( "!invalid element byte size" );
				return STATUS_ERROR_INTERNAL;
			}
			char * ptr = _imaxtostr( static_buf, countof(static_buf), data, state.m_radix );
			__debugbreak_ifnot( nullptr != ptr )
				parser.output( ptr, countof(static_buf) - 1 - (ptr - static_buf) );

			// Output element suffix
			if( !state.m_element_suffix.empty() )
				parser.output( state.m_element_suffix );

			// Check for timeout
			if( 0 != state.m_timeout )
			{
				time_t current_time = ::time( nullptr );
				if( uint32_t(current_time - start_time) >= state.m_timeout )
					break;
			}
		}
	}

	// Output suffix
	if( !state.m_suffix.empty() )
		parser.output( state.m_suffix );

	// Exit
	return status;
}

/*END OF embed_directive.cxx*/

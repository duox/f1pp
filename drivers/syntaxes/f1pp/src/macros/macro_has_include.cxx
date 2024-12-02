/*macro_has_include.cxx*/
// TODO: subject for deletion (same pseudo-function is used in C++17 driver)
#include "../pch.h"
#include "../main.h"

const status_t f1::f1pp::macro_has_include::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Check current state
	core::Tref<pp::param_list> pars;
	if( nullptr == params )
	{
		pars.createT<pp::param_list>();
		pars->append_param( 0, "filename" );
		params = pars;
	}

	// Call base class
	if( super::initialize( attrs, params, value, flags | macro_flags_t::fl_immutable | macro_flags_t::fl_predefined | macro_flags_t::fl_expression_only ).failed_status( status ) )
		return status;

	// Initialize object

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char * f1::f1pp::macro_has_include::get_name() const
{
	return "__has_include";
}

const status_t f1::f1pp::macro_has_include::expand( pp::Iparser & parser, pp::string & result, const pp::arg_list * args ) const
{
	status_t status;

	// Check current state
	__debugbreak_if( nullptr == args || 1 != args->get_arg_list().size() )
		return parser.message( syntax::IMSG_ERROR_EXPECTED_S, "__has_include( filename )" );

	core::Tref<f1::c_pp::parser> cpp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == cpp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get macro value
	const pp::arg_list::arg_desc_t & arg = *args->get_arg_list().begin();
	const pp::string & val = arg.m_expanded_value.is_empty() ? arg.m_value : arg.m_expanded_value;

	const char * s = val.data();
	const char * end = val.data_end();

	// Parse include file name
	pp::string	file_name;
	c_pp::parser::include_flags_t include_flags = null;
	if( cpp_par->parse_include_multi_filename( file_name, include_flags, s, end ).failed_status( status ) )
		return status;
	if( nullptr == file_name.data() )
		return parser.message( f1::c_pp::syntax::IMSG_FATAL_ERROR_INVALID_FILE_NAME );

	// Trim left and right spaces if present
	if( isspace( file_name.peek( 0 ) ) || isspace( file_name.rpeek( 0 ) ) )
	{
		const char * path_start = file_name.data();
		size_t path_length = file_name.length();
		pp::service::trim_spaces( path_start, path_length );

		file_name.clear();
		file_name.add_part( path_start, path_length );
	}

	// Find the file
	bool exists = false;
	core::string full_file_name;
	if( cpp_par->resolve_include_filename( full_file_name, file_name, include_flags ).succeeded() )
	{
		io::file_istream fis( full_file_name.c_str() );
		exists = !fis.bad();
	}
	result.add_part_alloc( exists ? "1" : "0" );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF macro_has_include.cxx*/

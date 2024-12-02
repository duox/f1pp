/*Iparser.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::Iparser::Iparser( const core::Iidentifier * ident ) __noexcept:
	super( ident ),
	m_parser_flags( null ), m_global_line( 0 ), m_last_message_line( 0 ),
	m_parser_hooks( nullptr ),
	// m_maximum_include_depth( 0 ), m_warning_maximum_count( 0 ), m_error_maximum_count( 0 ),
	m_disable_output_count( 0 )
{
//	m_filename_change_callback.m_parser = this;
//	m_lineno_change_callback.m_parser = this;
	m_event_handler.initialize();
	memset( m_limit, 0, sizeof(m_limit) );

//	m_parser_flags =
//		parser_flags_t::fl_dump_macros_on_error | parser_flags_t::fl_dump_macros_on_warning |
//		parser_flags_t::fl_dump_macro_stack_on_error | parser_flags_t::fl_dump_macro_stack_on_warning;
}
f1::pp::Iparser::~Iparser() __noexcept
{
	remove_all_macro();

	set_output( nullptr );
	set_storage( nullptr );

	if( nullptr != m_parser )
	{
		m_parser->unregister_event_handler(
			f1::parser::event_type_t(f1::parser::etype_filename_changed | f1::parser::etype_line_changed),
			m_event_handler, this );
//		m_parser->get_filename_change_callbacks().remove_callback( m_filename_change_callback );
//		m_parser->get_lineno_change_callbacks().remove_callback( m_lineno_change_callback );
	}

	m_context = nullptr;
	m_syntax = nullptr;
	m_base_output = nullptr;
	m_parser = nullptr;
}

void f1::pp::Iparser::addref() NOEXCEPT
{
	return super::addref();
}

const status_t f1::pp::Iparser::initialize( pp::context & context, pp::Isyntax & syntax,
	pp::Ioutput & outp, pp::Istorage & stg, parser::Isource & source )
{
	// Initialize object
	m_context = &context;
	m_syntax = &syntax;
	set_output( &outp );
	set_storage( &stg );

	// Exit
	return _initialize( &source );
}
const status_t f1::pp::Iparser::initialize( pp::context & context, pp::Isyntax & syntax,
	pp::Ioutput & outp, pp::Istorage & stg, parser::Iparser * parser, const char * base_source )
{
	status_t status;

	// Initialize object
	m_context = &context;
	m_syntax = &syntax;
	m_base_output = &outp;
	set_output( &outp );
	set_storage( &stg );
	m_parser = parser;

//	m_msgout_context.initialize();
	m_ns_context.initialize();
	m_ns_context.set_path_separators( ".", 1 );

	// Initialize low level parser
	core::Tref<parser::Isource> f = nullptr;
	if( nullptr != base_source && get_parser_context().create_file_source( f, get_fs_context(), base_source ).failed_status( status ) )
		return status;

	// Exit
	return _initialize( f );
}
const status_t f1::pp::Iparser::_initialize( parser::Isource * source )
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Initialize object
//	m_msgout_context.initialize();
	m_ns_context.initialize();

	// Initialize low level parser
	if( nullptr == m_parser )
	{
		core::Tref<parser::Iparser> parser;
		get_parser_context().create_parser( parser, source );
		m_parser = parser;
	}

	m_parser->register_event_handler(
		f1::parser::event_type_t(f1::parser::etype_filename_changed | f1::parser::etype_line_changed),
		m_event_handler, this );
//	m_parser->get_filename_change_callbacks().append_callback( m_filename_change_callback );
//	m_parser->get_lineno_change_callbacks().append_callback( m_lineno_change_callback );
	if( nullptr != source )
	{
		if( nullptr != m_context->get_default_input_encoding() )
			source->set_encoding( m_context->get_default_input_encoding() );

		on_filename_changed();
		on_lineno_changed();
	}

	// Initialize file system
	const fs::path & base_dir = m_context->get_output_base_directory();
	if( !base_dir.is_empty() )
	{
		core::string str;
		get_fs_context().get_full_pathname( str, base_dir );
		m_base_path.set_string_move( str );
	}

	// Initialize output
//	m_msgout_context.append_output( f1::msgout::console_output::m_guid );
//	m_msgout_context.set_formatter( f1::msgout::msvs_formatter::m_guid );

	// Execute context commands
	array_foreach( core::Tobject_array<Iparser::Icommand>::const_iterator, it, m_context->get_preinit_parser_commands() )
		(*it).ref_const_cast()->execute( *this );
	m_context->get_preinit_parser_commands().clear();

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Iparser::modify_parser_flags( parser_flags_t flags_to_set, parser_flags_t flags_to_clear, parser_flags_t * prev_flags )
{
	if( nullptr != prev_flags )
		*prev_flags = m_parser_flags;
	m_parser_flags = (m_parser_flags | flags_to_set) & ~flags_to_clear;
	return STATUS_SUCCESS;
}
f1::pp::Iparser::parser_flags_t f1::pp::Iparser::get_parser_flags( parser_flags_t mask ) const
{
	return m_parser_flags & mask;
}

const f1::pp::context * f1::pp::Iparser::get_context() const
{
	return m_context;
}
f1::pp::context * f1::pp::Iparser::get_context()
{
	return m_context;
}

const f1::pp::Isyntax * f1::pp::Iparser::get_syntax() const
{
	return m_syntax;
}
f1::pp::Isyntax * f1::pp::Iparser::get_syntax()
{
	return m_syntax;
}

/*const f1::pp::Istandard * f1::pp::Iparser::get_standard() const
{
	return m_standard;
}
const status_t f1::pp::Iparser::set_standard( const f1::pp::Istandard & standard )
{
	if( nullptr != m_standard )
		m_standard->on_detach_from_parser( *this );
	m_standard = &standard;
	m_standard->on_attach_to_parser( *this );

	return STATUS_SUCCESS;
}*/


const f1::pp::Ioutput * f1::pp::Iparser::get_base_output() const
{
	return m_base_output;
}
const f1::pp::Ioutput * f1::pp::Iparser::get_output() const
{
	return m_output;
}
f1::pp::Ioutput * f1::pp::Iparser::get_output()
{
	return m_output;
}
const status_t f1::pp::Iparser::set_output( f1::pp::Ioutput * outp, core::Tref<pp::Ioutput> * old_outp )
{
	if( nullptr != old_outp )
		*old_outp = m_output;

	if( nullptr != m_output )
		m_output->on_detach_from_parser( *this );
	m_output = outp;
	if( nullptr != m_output )
		m_output->on_attach_to_parser( *this );

	return STATUS_SUCCESS;
}

const f1::pp::Istorage * f1::pp::Iparser::get_storage() const
{
	return m_storage;
}
f1::pp::Istorage * f1::pp::Iparser::get_storage()
{
	return m_storage;
}
const status_t f1::pp::Iparser::set_storage( f1::pp::Istorage * stg, core::Tref<pp::Istorage> * old_stg )
{
	if( nullptr != old_stg )
		*old_stg = m_storage;
	m_storage = stg;
	return STATUS_SUCCESS;
}

const f1::parser::context & f1::pp::Iparser::get_parser_context() const
{
	return m_context->get_parser_context();
}
f1::parser::context & f1::pp::Iparser::get_parser_context()
{
	return m_context->get_parser_context();
}
const f1::parser::Iparser * f1::pp::Iparser::get_parser() const
{
	return m_parser;
}
f1::parser::Iparser * f1::pp::Iparser::get_parser()
{
	return m_parser;
}

const f1::fs::context & f1::pp::Iparser::get_fs_context() const
{
	return m_context->get_fs_context();
}
f1::fs::context & f1::pp::Iparser::get_fs_context()
{
	return m_context->get_fs_context();
}
const f1::fs::path & f1::pp::Iparser::get_base_directory() const
{
	return m_base_path;
}

const f1::msgout::context & f1::pp::Iparser::get_msgout_context() const
{
	return m_context->get_msgout_context();
	//return m_msgout_context;
}
f1::msgout::context & f1::pp::Iparser::get_msgout_context()
{
	return m_context->get_msgout_context();
	//return m_msgout_context;
}

/*unsigned f1::pp::Iparser::get_maximum_include_depth() const
{
	if( 0 != m_maximum_include_depth )
		return m_maximum_include_depth;
	__debugbreak_ifnot( nullptr != m_context )
		return m_context->get_maximum_include_depth();
	return 0;
}
const status_t f1::pp::Iparser::set_maximum_include_depth( unsigned maximum_level )
{
	m_maximum_include_depth = maximum_level;
	return STATUS_SUCCESS;
}*/
intmax_t f1::pp::Iparser::get_limit_value( limit_t lim ) const
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return 0;
	if( 0 != m_limit[lim] )
		return m_limit[lim];
	__debugbreak_ifnot( nullptr != m_context )
		return m_context->get_limit_value( lim );
	return 0;
}
const status_t f1::pp::Iparser::set_limit_value( limit_t lim, intmax_t value )
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return STATUS_ERROR_INVALID_PARAMETER;

	m_limit[lim] = value;

	if( limit_t::li_error_maximum_count == lim )
		_check_message_count( value, f1::msgout::severity_error, 0 ); //, Isyntax::IMSG_FATAL_ERROR_TOO_MANY_ERRORS );
	else if( limit_t::li_warning_maximum_count == lim )
		_check_message_count( value, f1::msgout::severity_warning, 0 );//, Isyntax::IMSG_FATAL_ERROR_TOO_MANY_WARNINGS );

	return STATUS_SUCCESS;
}
// set limit if current value is greater
const status_t f1::pp::Iparser::set_limit_value_min( limit_t lim, intmax_t value )
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return STATUS_ERROR_INVALID_PARAMETER;

	if( m_limit[lim] <= value )
		return STATUS_SUCCESS;
	return set_limit_value( lim, value );
}
// set limit if current value is less
const status_t f1::pp::Iparser::set_limit_value_max( limit_t lim, intmax_t value )
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return STATUS_ERROR_INVALID_PARAMETER;

	if( m_limit[lim] >= value )
		return STATUS_SUCCESS;
	return set_limit_value( lim, value );
}
// limit value must be greater or equal than the passed value
bool f1::pp::Iparser::check_limit_value_min( limit_t lim, intmax_t value ) const
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return true;	// ignore unknown limit

	const intmax_t limit_value = get_limit_value( lim );
	if( 0 == limit_value )
		return true;	// limit not set
	return limit_value >= value;
}
// limit value must be less or equal than the passed value
bool f1::pp::Iparser::check_limit_value_max( limit_t lim, intmax_t value ) const
{
	__debugbreak_if( _countof(m_limit) <= size_t(lim) )
		return true;	// ignore unknown limit

	const intmax_t limit_value = get_limit_value( lim );
	if( 0 == limit_value )
		return true;	// limit not set
	return limit_value <= value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Commands

const status_t f1::pp::Iparser::Icommand::execute( pp::Iparser & parser )
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	return STATUS_ERROR_PURE_CALL;
}

f1::pp::Iparser::cmd_modify_flags::cmd_modify_flags( parser_flags_t flags_to_set, parser_flags_t flags_to_clear ):
	m_flags_to_set( flags_to_set ), m_flags_to_clear( flags_to_clear )
{
}
const status_t f1::pp::Iparser::cmd_modify_flags::execute( pp::Iparser & parser )
{
	parser.modify_parser_flags( m_flags_to_set, m_flags_to_clear );
	return STATUS_SUCCESS;
}

f1::pp::Iparser::cmd_append_standard::cmd_append_standard( Istandard & std )
{
	append_standard( std );
}
const status_t f1::pp::Iparser::cmd_append_standard::execute( pp::Iparser & parser )
{
	array_foreach( standard_list_t::iterator, it, m_standard_list )
		it->attach_to_parser( parser );
	m_standard_list.clear();
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::cmd_append_standard::append_standard( Istandard & std )
{
	return m_standard_list.append( &std );
}

f1::pp::Iparser::cmd_remove_standard::cmd_remove_standard( Istandard & std )
{
	append_standard( std );
}
const status_t f1::pp::Iparser::cmd_remove_standard::execute( pp::Iparser & parser )
{
	array_foreach( standard_list_t::iterator, it, m_standard_list )
		parser.detach_standard( **it );
	m_standard_list.clear();
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::cmd_remove_standard::append_standard( Istandard & std )
{
	return m_standard_list.append( &std );
}

f1::pp::Iparser::cmd_include_path::cmd_include_path( include_path_flags_t flags, const f1::core::const_string & file_names, bool remove ):
	m_include_flags( flags )
{
	__assert( 0 != m_include_flags );

	if( remove )
		remove_include_path( file_names );
	else
		append_include_path( file_names );
}
const status_t f1::pp::Iparser::cmd_include_path::append_include_path( const f1::core::const_string & file_names )
{
	return append_path( m_includes_to_add, file_names );
}
const status_t f1::pp::Iparser::cmd_include_path::remove_include_path( const f1::core::const_string & file_names )
{
	return append_path( m_includes_to_remove, file_names );
}
const status_t f1::pp::Iparser::cmd_include_path::append_path( core::string & includes, const f1::core::const_string & file_names )
{
	if( !includes.empty() && ';' != *includes.rbegin() )
		includes.append_char( ';' );
	return includes.append( file_names );
}
const status_t f1::pp::Iparser::cmd_include_path::execute( pp::Iparser & parser )
{
	if( !m_includes_to_add.empty() )
		execute( parser, m_includes_to_add, false );
	if( !m_includes_to_remove.empty() )
		execute( parser, m_includes_to_remove, true );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::cmd_include_path::execute( pp::Iparser & parser, const core::string & list, bool remove_paths )
{
	core::Tarray<int>	quote_stack;
	core::string buf;
	const char * s = list.data();
	const char * const end_s = list.data_end();
	const char * start = s;

	__assert( 0 != m_include_flags );

	// Add include paths
	for( ; ; ++ s )
	{
		if( s >= end_s || ';' == *s )
		{
			if( s != start )
				buf.append( start, s - start );

			if( remove_paths )
				parser.remove_include_path( m_include_flags, buf );
			else
				parser.append_include_path( m_include_flags, buf );

			if( s >= end_s )
				break;

			buf.clear();
			start = s + 1;
		}
		else if( '\"' == *s || '\'' == *s )
		{
			if( s != start )
				buf.append( start, s - start );

			if( quote_stack.empty() || *s != quote_stack.back() )
				quote_stack.push_back( *s );
			else
				quote_stack.pop_back();

			start = s + 1;
		}
	}

	// Exit
	return STATUS_SUCCESS;
}

f1::pp::Iparser::cmd_set_directive_flags::cmd_set_directive_flags( const core::stringslice & name, unsigned flags_to_set, unsigned flags_to_clear )
{
	append_directive_flags( name, flags_to_set, flags_to_clear );
}
const status_t f1::pp::Iparser::cmd_set_directive_flags::execute( pp::Iparser & parser )
{
	array_foreach( core::Tarray<directive_state>::iterator, it, m_directive_states )
	{
		directive_state & ds = *it;

		core::Tref<pp::Idirective> drct;
		parser.find_directive( drct, 0, pp::string(ds.m_directive_name) );
		if( nullptr != drct )
		{
			drct->modify_flags( ds.m_flags_to_set, ds.m_flags_to_clear );
		}
	}
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::cmd_set_directive_flags::append_directive_flags( const core::stringslice & name, unsigned flags_to_set, unsigned flags_to_clear )
{
	directive_state * ds = m_directive_states.append_new();
	__debugbreak_if( nullptr == ds )
		return STATUS_ERROR_INSUFFICIENT_MEMORY;
	ds->m_directive_name = name;
	ds->m_flags_to_clear= flags_to_clear;
	ds->m_flags_to_set	= flags_to_set;
	return STATUS_SUCCESS;
}

f1::pp::Iparser::cmd_system_include_base::cmd_system_include_base( const core::const_string & base_dir ):
	m_base_dir( base_dir )
{
}
const status_t f1::pp::Iparser::cmd_system_include_base::execute( pp::Iparser & parser )
{
	return parser.set_system_include_base_directory( m_base_dir );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

const status_t f1::pp::Iparser::set_parameter( const f1::guid_t & param_guid, const core::variant & val )
{
	__unreferenced_parameter( param_guid );
	__unreferenced_parameter( val );
	return STATUS_SUCCESS;
}

f1::parser::lineno_t f1::pp::Iparser::get_global_line() const
{
	return m_global_line;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Character flags

const status_t f1::pp::Iparser::modify_char_flags( int ch, unsigned flags_to_set, unsigned flags_to_clear, unsigned * prev_flags )
{
	__debugbreak_if( unsigned(ch) >= countof(m_char_table) )
		return STATUS_ERROR_INVALID_PARAMETER;
	__debugbreak_if( 0 != (flags_to_set & fl_char_flags_mask) )
		return STATUS_ERROR_INVALID_PARAMETER;
	__debugbreak_if( 0 != (flags_to_clear & fl_char_flags_mask) )
		return STATUS_ERROR_INVALID_PARAMETER;

	if( nullptr != prev_flags )
		*prev_flags = m_char_table[ch];
	m_char_table[ch] = uint8_t( (m_char_table[ch] | flags_to_set) & ~flags_to_clear );

	return STATUS_SUCCESS;
}
unsigned f1::pp::Iparser::get_char_flags( int ch, unsigned mask ) const
{
	__debugbreak_if( unsigned(ch) >= countof(m_char_table) )
		return 0;
	return m_char_table[ch] & mask;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Messages

bool f1::pp::Iparser::is_message_printed_at_line( f1::parser::lineno_t line, f1::msgout::msgid_t msg )
{
	if( line != m_last_message_line )
	{
		m_last_message_line  = line;
		m_line_messages.clear();
	}
	else
	{
		if( m_line_messages.exists( msg ) )
			return true;
	}

	m_line_messages.append( msg );

	return false;
}
const char * f1::pp::Iparser::get_message_file_name( const char * file ) const
{
	if( m_context->get_flags( context::fl_msg_full_path ) )
		file = m_full_current_file_name.c_str();
	else
		file = m_parser->get_current_file_name();

	if( m_context->get_flags( context::fl_message_file_names_only ) )
		file = PathFindFileName( file );

	return file;
}

const status_t f1::pp::Iparser::message( f1::msgout::msgid_t msg, ... )
{
	va_list ap;
	va_start( ap, msg );
	status_t status = message(
		nullcstr,
		m_parser->get_line(),
		msg, ap );
	va_end( ap );
	return status;
}
const status_t f1::pp::Iparser::message( const char * file, f1::parser::lineno_t line, f1::msgout::msgid_t msg, ... )
{
	va_list ap;
	va_start( ap, msg );
	status_t status = message( file, line, msg, ap );
	va_end( ap );
	return status;
}
const status_t f1::pp::Iparser::message( const f1::parser::Iparser::location & loc, f1::msgout::msgid_t msg, ... )
{
	va_list ap;
	va_start( ap, msg );
	status_t status = message(
		nullptr == loc.m_source ? "<unknown>" : loc.m_source->get_filename(),
		loc.m_line,
		msg, ap );
	va_end( ap );
	return status;
}

const status_t f1::pp::Iparser::message( const char * file, f1::parser::lineno_t line, f1::msgout::msgid_t msg, va_list ap )
{
	if( nullptr == file &&
		!m_parser->is_eof()		// on EOF, show all messages
		)
	{
		if( is_message_printed_at_line( line, msg ) )
			return STATUS_SUCCESS;
	}

	if( nullptr != f1::util::application::g_app )
		f1::util::application::g_app->print_utility_header();

	file = get_message_file_name( file );

	// Update filename and line
	get_msgout_context().on_file_changed( file );
	get_msgout_context().on_line_changed( line );

	// Display message
	status_t status = get_msgout_context().message_ap( msg, ap );

	// Restore file and line
	get_msgout_context().on_file_changed( file );
	get_msgout_context().on_line_changed( m_parser->get_line() );

	// Process fatal error
	const msgout::severity_t severity = get_msgout_context().get_message_severity( msg );
	if( msgout::severity_fatal_error == severity )
	{
		get_msgout_context().modify_flags( f1::msgout::context::bf_fatal_error );
		abort_parser();
	}

	// Check maximum warning and error count
	if( f1::msgout::warning_group::DEFAULT_MIN <= msg && msg <= f1::msgout::warning_group::DEFAULT_MAX )
		_check_message_count( m_limit[pp::limit_t::li_warning_maximum_count], f1::msgout::severity_warning, 0 );//, Isyntax::IMSG_FATAL_ERROR_TOO_MANY_WARNINGS );
	else if( f1::msgout::error_group::DEFAULT_MIN <= msg && msg <= f1::msgout::error_group::DEFAULT_MAX )
		_check_message_count( m_limit[pp::limit_t::li_error_maximum_count], f1::msgout::severity_error, 0 );//, Isyntax::IMSG_FATAL_ERROR_TOO_MANY_ERRORS );

	// Dump macros on error or warning
	if( ((msgout::severity_fatal_error == severity || msgout::severity_error == severity) && m_parser_flags & parser_flags_t::fl_dump_macros_on_error) ||
		 (msgout::severity_warning	   == severity && m_parser_flags & parser_flags_t::fl_dump_macros_on_warning)
		)
	{
		array_foreach( core::Tobject_array<macro_object>::const_iterator, it, m_macro_list )
		{
			const macro_object & mobj = **it;
			const Imacro & mac = **mobj.m_macro_list.begin();

			if( mac.get_flags( Imacro::macro_flags_t::fl_predefined | Imacro::macro_flags_t::fl_self_parse_arglist | Imacro::macro_flags_t::fl_expression_only | Imacro::macro_flags_t::fl_operator ) )
				continue;

			core::string buf;
			mobj.get_printable_full_name( buf );
			get_msgout_context().print( "MACRO %s", buf.c_str() );

			const f1::pp::param_list * params = mac.get_param_list();
			if( nullptr != params )
			{
				buf.clear();
				params->format( buf );
				get_msgout_context().print( "(%s)", buf.c_str() );
			}

			const f1::pp::string & value = mac.get_value();
			const char * value_string = value.c_str();
			if( nullptr == value_string )
				value_string = "";
			get_msgout_context().print( "=%s\n", value_string );
		}
	}

	// Dump macro stack on error or warning
	if( ((msgout::severity_fatal_error == severity || msgout::severity_error == severity) && m_parser_flags & parser_flags_t::fl_dump_macro_stack_on_error) ||
		 (msgout::severity_warning	   == severity && m_parser_flags & parser_flags_t::fl_dump_macro_stack_on_warning)
		)
	{
		dump_macro_stack( get_msgout_context() );
	}

	// Exit
	return status;
}

/*unsigned f1::pp::Iparser::get_warning_maximum_count() const
{
	return m_warning_maximum_count;
}
unsigned f1::pp::Iparser::get_error_maximum_count() const
{
	return m_error_maximum_count;
}
const status_t f1::pp::Iparser::set_warning_maximum_count( unsigned count )
{
	m_warning_maximum_count = count;
	_check_message_count( m_warning_maximum_count, f1::msgout::severity_warning, 0 );//, Isyntax::IMSG_FATAL_ERROR_TOO_MANY_WARNINGS );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::set_error_maximum_count( unsigned count )
{
	m_error_maximum_count = count;
	_check_message_count( m_error_maximum_count, f1::msgout::severity_error, 0 ); //, Isyntax::IMSG_FATAL_ERROR_TOO_MANY_ERRORS );
	return STATUS_SUCCESS;
}*/
void f1::pp::Iparser::_check_message_count( intmax_t maximum_count, msgout::severity_t severity, unsigned msgid )
{
	if( 0 != maximum_count && maximum_count <= get_msgout_context().get_severity_message_count( severity ) )
	{
		message( msgid );
		abort_parser();
	}
}
unsigned f1::pp::Iparser::get_common_error_count() const
{
	return	get_msgout_context().get_severity_message_count( msgout::severity_error ) +
			get_msgout_context().get_severity_message_count( msgout::severity_fatal_error );
}

void f1::pp::Iparser::on_filename_changed()
{
	m_output->on_file_changed( m_parser->get_current_file_name(), m_parser->get_line() );
	get_fs_context().get_full_pathname( m_full_current_file_name, m_parser->get_current_file_name() );
}
void f1::pp::Iparser::on_lineno_changed()
{
	on_line_changed( m_parser->get_line() );
}
const status_t f1::pp::Iparser::on_line_changed( f1::parser::lineno_t line )
{
	__debugbreak_if( nullptr == m_parser_hooks )
		return STATUS_ERROR_INTERNAL;
	__debugbreak_if( nullptr == m_output )
		return STATUS_ERROR_INTERNAL;
	if( line == m_output->get_line() )	// INFO: these two lines ignore new-lines inside arguments
		return STATUS_SUCCESS;
	m_global_line += line - m_output->get_line();
	return m_parser_hooks->on_line_changed( *this, line );
}

const status_t f1::pp::Iparser::event_handler::invoke( f1::parser::event_type_t type_mask, void * handler_param, void * event_param, f1::parser::Iparser & parser )
{
	core::Tref<f1::pp::Iparser> c_par = type_cast<f1::pp::Iparser *>(reinterpret_cast<core::object *>(handler_param));
	__debugbreak_if( nullptr == c_par )
		return STATUS_ERROR_INVALID_PARAMETER;

	if( type_mask & f1::parser::etype_filename_changed )
		c_par->on_filename_changed();
	else if( type_mask & f1::parser::etype_line_changed )
		c_par->on_lineno_changed();

	__unreferenced_parameter( event_param );
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes

const status_t f1::pp::Iparser::set_system_include_base_directory( const core::string & base_dir )
{
	m_system_include_base_directory = base_dir;
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::set_system_include_base_directory( const core::stringslice & base_dir )
{
	m_system_include_base_directory = base_dir;
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::set_system_include_base_directory( const core::const_string & base_dir )
{
	m_system_include_base_directory = base_dir;
	return STATUS_SUCCESS;
}
const f1::core::string & f1::pp::Iparser::get_system_include_base_directory() const
{
	return m_system_include_base_directory;
}

const status_t f1::pp::Iparser::modify_include_path( include_path_flags_t flags, const char * path, size_t len, bool remove )
{
	status_t status;

	// Check current state
	if( size_t(-1) == len )
		len = core::string::length( path );
	__debugbreak_if( 0 == len )
		return STATUS_ERROR_INVALID_PARAMETER;	// a caller should check for empty paths and print diagnostic

	// Get full pathname
	core::string full_path;
#if 1
	// Add path as is and resolve it when used
	full_path.assign( path, len );
#else
	// Get full path name relatively to the current source file directory
	if( resolve_source_relative_file_path( full_path, path, len ).failed_status( status ) )
		return status;
#endif
	if( full_path.empty() )
		return STATUS_ERROR_INTERNAL;	// should return appopriate status code

	// Append directory path and exit
	if( flags & include_path_flags_t::fl_system )
		return remove ?
			m_sys_search_dir_list.remove_search_path( full_path ):
			m_sys_search_dir_list.append_search_path( full_path );
	__assert( flags & include_path_flags_t::fl_user );
	return remove ?
		m_usr_search_dir_list.remove_search_path( full_path ):
		m_usr_search_dir_list.append_search_path( full_path );
}

const status_t f1::pp::Iparser::append_include_path( include_path_flags_t flags, const char * path, size_t len )
{
	return modify_include_path( flags, path, len, false );
}
const status_t f1::pp::Iparser::append_include_path( include_path_flags_t flags, const core::string & path )
{
	return modify_include_path( flags, path.data(), path.length(), false );
}
const status_t f1::pp::Iparser::append_include_path( include_path_flags_t flags, const pp::string & path )
{
	return modify_include_path( flags, path.data(), path.length(), false );
}

const status_t f1::pp::Iparser::remove_include_path( include_path_flags_t flags, const char * path, size_t len )
{
	return modify_include_path( flags, path, len, true );
}
const status_t f1::pp::Iparser::remove_include_path( include_path_flags_t flags, const core::string & path )
{
	return modify_include_path( flags, path.data(), path.length(), true );
}
const status_t f1::pp::Iparser::remove_include_path( include_path_flags_t flags, const pp::string & path )
{
	return modify_include_path( flags, path.data(), path.length(), true );
}

const status_t f1::pp::Iparser::remove_all_include_paths( include_path_flags_t flags )
{
	if( flags & include_path_flags_t::fl_system )
		return m_sys_search_dir_list.remove_all_search_paths();
	__assert( flags & include_path_flags_t::fl_user );
	return m_usr_search_dir_list.remove_all_search_paths();
}

const status_t f1::pp::Iparser::resolve_source_relative_file_path( core::string & res, const pp::string & file_name ) const
{
	return resolve_source_relative_file_path( res, file_name.data(), file_name.length() );
}
const status_t f1::pp::Iparser::resolve_source_relative_file_path( core::string & res, const core::stringslice & file_name ) const
{
	return resolve_source_relative_file_path( res, file_name.data(), file_name.length() );
}
const status_t f1::pp::Iparser::resolve_source_relative_file_path( core::string & res, const char * file_name, size_t len ) const
{
	core::Tref<f1::parser::Isource> src = m_parser->get_current_source();
	__debugbreak_if( nullptr == src )
		return STATUS_ERROR_NOT_INITIALIZED;

	fs::path path = src->get_filename();
	path.remove_filename();
	path.combine( file_name, len );
	path.move_to( res );

	return STATUS_SUCCESS;
}

const f1::fs::search_dir_list & f1::pp::Iparser::get_search_dir_list( include_path_flags_t flags ) const
{
	if( flags & include_path_flags_t::fl_system )
		return m_sys_search_dir_list;
//	__assert( flags & include_path_flags_t::fl_user );
	return m_usr_search_dir_list;
}
f1::fs::search_dir_list & f1::pp::Iparser::get_search_dir_list( include_path_flags_t flags )
{
	if( flags & include_path_flags_t::fl_system )
		return m_sys_search_dir_list;
//	__assert( flags & include_path_flags_t::fl_user );
	return m_usr_search_dir_list;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::Iparser::parse()
{
	DBG_FAIL_ON_PURE_CALL();
	return STATUS_ERROR_PURE_CALL;
}
const status_t f1::pp::Iparser::abort_parser()
{
	DBG_FAIL_ON_PURE_CALL();
	return STATUS_ERROR_PURE_CALL;
}

const status_t f1::pp::Iparser::set_parser_hooks( Iparser_hooks & intf )
{
	m_parser_hooks = &intf;
	return STATUS_SUCCESS;
}
f1::pp::Iparser::Iparser_hooks * f1::pp::Iparser::get_parser_hooks() const
{
	return m_parser_hooks;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool f1::pp::Iparser::root_parser_hooks::is_eof( pp::Iparser & parser ) const
{
	return parser.get_parser()->is_eof();
}
const status_t f1::pp::Iparser::root_parser_hooks::on_esc_char( pp::Iparser & parser )
{
	__unreferenced_parameter( parser );
	return STATUS_ERROR_NOT_SUPPORTED;
}

const status_t f1::pp::Iparser::root_parser_hooks::output( pp::Iparser & parser, const char * start, size_t len )
{
	return parser.get_output()->write( start, len );
}
const status_t f1::pp::Iparser::root_parser_hooks::on_line_changed( pp::Iparser & parser, int line )
{
	return parser.get_output()->on_line_changed( line );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool f1::pp::Iparser::line_parser_hooks::is_eof( pp::Iparser & parser ) const
{
	return parser.get_parser()->is_nl();
}

// TODO: optimize for case if start == m_parser->get_ptr()
const status_t f1::pp::Iparser::line_parser_hooks::output( pp::Iparser & parser, const char * start, size_t len )
{
	m_buffer.add_part( start, len );
	__unreferenced_parameter( parser );
	return STATUS_SUCCESS;;
}

const status_t f1::pp::Iparser::output( const char * str, size_t len )
{
	__debugbreak_if( nullptr == m_parser_hooks )
		return STATUS_ERROR_INTERNAL;
	if( 0 != m_disable_output_count )
		return STATUS_SUCCESS;
	if( 0 == len )
		return STATUS_SUCCESS;
	return m_parser_hooks->output( *this, str, len );
}
const status_t f1::pp::Iparser::output( const pp::string & str )
{
	return output( str.data(), str.size() );
}
const status_t f1::pp::Iparser::output( const core::string & str )
{
	return output( str.data(), str.size() );
}

const status_t f1::pp::Iparser::enable_output( bool enable )
{
	if( enable )
	{
		__debugbreak_if( 0 == m_disable_output_count )
			return STATUS_ERROR_INTERNAL;
		-- m_disable_output_count;
	}
	else
	{
		__assert( 10000 > m_disable_output_count );
		++ m_disable_output_count;
	}
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::pp::Iparser::enter_directive( Iframe & ectx )
{
	status_t status;

	const Iframe * frm = m_frame_stack.empty() ? nullptr : *m_frame_stack.rbegin();
	if( nullptr != frm && 0 != frm->get_flags( pp::Iframe::fl_initially_disabled ) )
		ectx.modify_flags( pp::Iframe::fl_initially_disabled );
	else
		ectx.modify_flags( 0, pp::Iframe::fl_initially_disabled );

	if( m_frame_stack.append( &ectx ).failed_status( status ) )
		return status;

	ectx.enter();

	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::leave_directive()
{
	if( m_frame_stack.empty() )
		return STATUS_ERROR_STACK_IS_EMPTY;

	Iframe * ectx = *m_frame_stack.rbegin();
	m_frame_stack.pop_back();
	ectx->leave();
//	ectx->delete_self();

	return STATUS_SUCCESS;
}
const f1::pp::Iframe * f1::pp::Iparser::get_directive_frame() const
{
	if( m_frame_stack.empty() )
		return nullptr;
	return *m_frame_stack.rbegin();
}
f1::pp::Iframe * f1::pp::Iparser::get_directive_frame()
{
	if( m_frame_stack.empty() )
		return nullptr;
	return *m_frame_stack.rbegin();
}
f1::pp::Iframe * f1::pp::Iparser::get_directive_frame( const f1::guid_t & openning_guid )
{
	array_foreach( core::Tarray<Iframe *>::reverse_iterator, it, m_frame_stack )
	{
		Iframe * ctx = *it;
		if( ctx->get_openning_directive_guid() == openning_guid )
			return ctx;
	}
	return nullptr;
}

const status_t f1::pp::Iparser::set_frame_stack( const core::Tarray<Iframe *> & stack )
{
	// Find common ground
	core::Tarray<Iframe *>::const_iterator it_old, it_new;
	array_foreach_i2( core::Tarray<Iframe *>::const_iterator, it_old, it_new, m_frame_stack, stack )
	{
		if( *it_old != *it_new )
			break;
	}

	// Leave all levels that are above common ground
	size_t size_left = m_frame_stack.size();
	array_foreach( core::Tarray<Iframe *>::reverse_iterator, it, m_frame_stack )
	{
		Iframe * exc = *it;

		exc->leave();
		exc->delete_self();

		__assert( 0 != size_left );
		-- size_left;

		if( *it_old == *it )
			break;
	}
	m_frame_stack.resize( size_left );

	// Add new levels
	array_foreach_i( core::Tarray<Iframe *>::const_iterator, it_new, stack )
	{
		Iframe * exc = *it_new;

		exc->add_ref();
		exc->enter();
		m_frame_stack.push_back( exc );
	}

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Directives

const f1::pp::directive_list * f1::pp::Iparser::get_directive_list() const
{
	return &m_directive_list;
}
f1::pp::directive_list * f1::pp::Iparser::get_directive_list()
{
	return &m_directive_list;
}
const status_t f1::pp::Iparser::find_directive( core::Tref<Idirective> & res, const f1::guid_t & guid ) const
{
	return m_directive_list.find_directive( res, guid );
}
const status_t f1::pp::Iparser::find_directive( core::Tref<Idirective> & res, unsigned flags, const char * name, size_t length ) const
{
	return m_directive_list.find_directive( res, flags, name, length );
}
const status_t f1::pp::Iparser::find_directive( core::Tref<Idirective> & res, unsigned flags, const pp::string & name ) const
{
	return m_directive_list.find_directive( res, flags, name.data(), name.length() );
}

bool f1::pp::Iparser::is_directive_available( const pp::Idirective & directive ) const
{
	core::Tref<Idirective> res;
	if( find_directive( res, directive.get_guid() ).failed() )
		return false;
	return true;
}

const status_t f1::pp::Iparser::on_execute_directive( Idirective & obj )
{
	return obj.parse( *this );
}
const status_t f1::pp::Iparser::on_unknown_directive( const pp::string & ident )
{
	// TODO: issue a warning
	return STATUS_SUCCESS;
	/*skip_to_newline();

	// Display standard for which directive is defined
	const pp::Istandard * standard = pp::service::get().find_item_standard( Idirective::m_item_class_guid, ident );
	if( nullptr != standard )
		message( syntax::IMSG_WARNING_NOT_AVAILABLE_STANDARD_SS, ident.c_str(), standard->get_description_string() );
	else
		// Show error message
		message( syntax::IMSG_ERROR_UNKNOWN_DIRECTIVE, ident.c_str() );

	// Exit
	return STATUS_SUCCESS;*/
}

const char * f1::pp::Iparser::get_directive_start() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullcstr;
}
const char * f1::pp::Iparser::get_directive_name_start() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullcstr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macros

const status_t f1::pp::Iparser::create_macro( core::Tref<pp::Imacro> & res,
	const pp::string & ident, const pp::string & value, Imacro::macro_flags_t flags
	)
{
	return create_macro( res, ident, nullptr, nullptr, &value, flags );
}
const status_t f1::pp::Iparser::create_macro( core::Tref<pp::Imacro> & res,
	const pp::string & ident, const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string & value, Imacro::macro_flags_t flags
	)
{
	return create_macro( res, ident, attrs, params, &value, flags );
}
const status_t f1::pp::Iparser::create_macro( core::Tref<pp::Imacro> & res,
	const pp::string & ident, const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, Imacro::macro_flags_t flags
	)
{
	status_t status;

	// Check current state
	__debugbreak_if( nullptr == m_syntax )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Create macro object
	core::Tref<pp::Iuser_macro> obj = nullptr;
	if( m_syntax->new_macro( obj ).failed_status( status ) )
		return status;
	if( obj->initialize( ident, attrs, params, value, flags ).failed_status( status ) )
		return status;

	// Append object
	if( append_macro( *obj ).failed_status( status ) )
		return status;

	// Exit
	res = obj;
	return STATUS_SUCCESS;
}

// The method is usually used for adding internal macros
const status_t f1::pp::Iparser::append_macro( pp::Imacro & mac )
{
	status_t status;

#if 1
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(( "INFO: adding macro: %s\n", mac.get_name() ));
	}

	// Check current state
	core::Tref<ns::name_space> nsp = m_ns_context.get_current_namespace();
	__debugbreak_if( nullptr == nsp )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check whether the macro already exists
	core::Tref<ns::name> nm = nullptr;
	core::Tref<macro_object> mobj;
	if( nsp->find_subname( nm, mac.get_name() ).succeeded() )
	{
		mobj = type_cast<macro_object *>( nm->get_object() );
		if( nullptr == mobj )
			return STATUS_ERROR_INVALID_TYPE;

		core::Tconst_ref<pp::param_list> params = mac.get_param_list();

		// Check for the macro parameter list
		array_foreach( const_macro_list_t::const_iterator, it, mobj->m_macro_list )
		{
			__debugbreak_if( &mac == *it )
				return STATUS_SUCCESS;
			if( nullptr == it->get_param_list() )
			{
				if( nullptr == params )
					return STATUS_ERROR_ALREADY_EXISTS;
				continue;
			}
			if( nullptr == params )
				continue;
			if( params->equal( *it->get_param_list() ) )
				return STATUS_SUCCESS;
		}
	}
	else
	{
		if( nsp->create_subname_by_path( nm, nullptr, 0, mac.get_name() ).failed_status( status ) )
			return status;
	}

	// Create reference object
	if( nullptr == mobj )
	{
		mobj.createT<macro_object>();
		mobj->m_macro_list.append( &mac );
		mobj->append_name( *nm );
		mac.on_attach_to_named_object( *mobj );
		m_macro_list.append( mobj );
	}
	else
		mobj->m_macro_list.append( &mac );
#else
	// Check for the macro
	array_foreach( core::Tobject_array<macro_object>::iterator, it, m_macro_list )
	{
		__debugbreak_if( &mac == it->m_macro )
			return STATUS_SUCCESS;
	}

	// Create name object
	core::Tref<ns::name> nm = nullptr;
	if( m_ns_context.get_current_namespace()->create_subname_by_path( nm, nullptr, 0, mac.get_name() ).failed_status( status ) )
		return status;

	// Create reference object
	core::Tref<macro_object> mobj;
	mobj.createT<macro_object>( std::cref(mac) );
	mobj->append_name( *nm );
	m_macro_list.append( mobj );
#endif

	// Done
	if( get_context()->is_event_enabled( pp::context::event_id_t::ev_macro_added ) )
		get_context()->invoke_event( pp::context::event_id_t::ev_macro_added, &mac );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::append_macros( pp::macro_list_t & list )
{
	array_foreach( pp::macro_list_t::iterator, it, list )
		append_macro( **it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::remove_macro( pp::Imacro & mac )
{
	core::Tref<pp::Imacro> lck( &mac );
#if 1
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(( "INFO: removing macro: %s\n", mac.get_name() ));
	}

	core::Tref<ns::name_space> nsp = m_ns_context.get_current_namespace();
	__debugbreak_if( nullptr == nsp )
		return STATUS_ERROR_NOT_INITIALIZED;

#if 0
	core::Tref<ns::name> nm = nullptr;
	__debugbreak_if( nsp->find_subname( nm, mac.get_name() ).failed() )
		return STATUS_ERROR_NOT_FOUND;

	core::Tref<macro_object> mobj = type_cast<macro_object *>(nm->get_object());
#elif 0
	core::Tref<macro_object> mobj = type_cast<macro_object *>(mac.get_named_object());
#else
	core::Tref<macro_object> mobj;
	array_foreach( core::Tobject_array<macro_object>::iterator, mo_it, m_macro_list )
	{
		array_foreach( pp::const_macro_list_t::iterator, m_it, mo_it->m_macro_list )
		{
			if( *m_it == &mac )
			{
				mobj = *mo_it;
				goto found;
			}
		}
	}
found:
#endif
	__debugbreak_if( nullptr == mobj )
		return STATUS_ERROR_INVALID_TYPE;

	mobj->remove_all_names();
	mac.on_detach_from_named_object( *mobj );
	mobj->m_macro_list.remove( &mac );

	// Done
	if( get_context()->is_event_enabled( pp::context::event_id_t::ev_macro_removed ) )
		get_context()->invoke_event( pp::context::event_id_t::ev_macro_removed, &mac );

	// Exit
	return STATUS_SUCCESS;
#else
	// Find macro object
	// TODO: speed things up
	core::Tref<macro_object> obj = nullptr;
	core::Tobject_array<macro_object>::iterator it;
	array_foreach_i( core::Tobject_array<macro_object>::iterator, it, m_macro_list )
	{
		if( &mac == it->m_macro )
		{
			obj = *it;
			break;
		}
	}
	__debugbreak_if( nullptr == obj )
		return STATUS_ERROR_NOT_FOUND;

	// Remove object
	obj->remove_all_names();
	m_macro_list.erase( it );

	// Exit
	return STATUS_SUCCESS;
#endif
}
const status_t f1::pp::Iparser::remove_macros( pp::macro_list_t & list )
{
	array_foreach( pp::macro_list_t::iterator, it, list )
		remove_macro( **it );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::remove_all_macro()
{
	if( service::get().get_flags( service::fl_debug_output ) )
	{
		DBG_PRINTF(( "INFO: removing all macros\n" ));
	}

	m_macro_list.remove_all();
	m_ns_context.get_root_namespace().remove_all_subnames();
	m_ns_context.get_root_namespace().remove_all_subnamespaces();

	if( get_context()->is_event_enabled( pp::context::event_id_t::ev_macro_removed_all ) )
		get_context()->invoke_event( pp::context::event_id_t::ev_macro_removed_all );

	return STATUS_SUCCESS;
}

const status_t f1::pp::Iparser::get_macro_name_object( core::Tref<macro_object> & res, const pp::string & ident ) const
{
	// Check current state
	core::Tconst_ref<ns::name_space> nsp = m_ns_context.get_current_namespace();
	__debugbreak_if( nullptr == nsp )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check whether the macro already exists
	core::Tref<ns::name> nm = nullptr;
	if( nsp->find_subname( nm, ident ).failed() )
		return STATUS_ERROR_NOT_FOUND;

	core::Tref<macro_object> mobj = type_cast<macro_object *>(nm->get_object());
	if( nullptr == mobj )
		return STATUS_ERROR_INVALID_TYPE;

	// Exit
	res = mobj;
	return STATUS_SUCCESS;
}
/*const status_t f1::pp::Iparser::query_macro( const pp::string & ident, unsigned & query_result ) const
{
	status_t status;

	// Get macro object
	core::Tref<macro_object> mobj;
	if( get_macro_name_object( mobj, ident ).failed_status( status ) )
		return status;

	// Analyze the macro list
	query_result = 0;
	array_foreach( const_macro_list_t::const_iterator, it, mobj->m_macro_list )
	{
		if( nullptr == it->get_param_list() )
			query_result |= fl_macro_query_hasnot_params;
		else
			query_result |= fl_macro_query_has_params;
	}

	// Exit
	return STATUS_SUCCESS;
}*/
bool f1::pp::Iparser::exists_macro( const pp::string & ident, Imacro::macro_flags_t flags, Imacro::macro_flags_t mask ) const
{
	// Check current state
	core::Tconst_ref<ns::name_space> nsp = m_ns_context.get_current_namespace();
	__debugbreak_if( nullptr == nsp )
		return false;

	// Check whether the macro already exists
	core::Tref<ns::name> nm = nullptr;
	if( nsp->find_subname( nm, ident ).failed() )
		return false;

	core::Tref<macro_object> mobj = type_cast<macro_object *>( nm->get_object() );
	if( nullptr == mobj )
		return false;

	// Check for flags
	if( 0 == flags && 0 == mask )
		return true;
	array_foreach( const_macro_list_t::const_iterator, it, mobj->m_macro_list )
	{
		if( flags == it->get_flags( mask ) )
			return true;
	}

	// Exit
	return false;
}

const status_t f1::pp::Iparser::find_macro( core::Tconst_ref<pp::Imacro> & res,
	const ns::name_space * ns_parent, const core::stringslice & ident,
	find_macro_flags_t search_flags, core::Tref<ns::name> * pname ) const
{
	status_t status;

	res = nullptr;

	// Find named object in the current and upper namespaces
	core::Tconst_ref<ns::name> nm = nullptr;
	status = find_macro_name( nm, ns_parent, ident, search_flags );
	if( nullptr == nm )
		return _ask_for_macro( res, ident, status );
	if( nullptr != pname )
		*pname = nm;

	// Find macro with correct arguments
	core::Tconst_ref<pp::Imacro> macro;
	if( get_macro_from_name( macro, *nm ).failed_status( status ) )
		return status;
	if( nullptr == macro )
		return STATUS_ERROR_INTERNAL;

	// Convert to macro
	if( _finish_macro_search( res, *macro, ident, search_flags ).failed_status( status ) )
		return status;

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::find_macro( core::Tconst_ref<pp::Imacro> & res,
	const ns::name_space * ns_parent, const core::stringslice & ident, const pp::arg_list & args,
	find_macro_flags_t search_flags, core::Tref<ns::name> * pname ) const
{
	status_t status;

	res = nullptr;

	// Find named object in the current and upper namespaces
	core::Tconst_ref<ns::name> nm = nullptr;
	status = find_macro_name( nm, ns_parent, ident, search_flags );
	if( nullptr == nm )
		return _ask_for_macro( res, ident, status );
	if( nullptr != pname )
		*pname = nm;

	// Find macro with correct arguments
	core::Tconst_ref<pp::Imacro> macro;
	if( get_macro_from_name( res, *nm, args ).failed_status( status ) )
		return status;
	if( nullptr == macro )
		return STATUS_ERROR_INTERNAL;

	// Convert to macro
	if( _finish_macro_search( macro, *macro, ident, search_flags ).failed_status( status ) )
		return status;

	// Exit
	res = macro;
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::find_macro( core::Tconst_ref<pp::Imacro> & res,
	const ns::name_space * ns_parent, const core::stringslice & ident, const pp::param_list & params,
	find_macro_flags_t search_flags, core::Tref<ns::name> * pname ) const
{
	status_t status;

	res = nullptr;

	// Find named object in the current and upper namespaces
	core::Tconst_ref<ns::name> nm = nullptr;
	find_macro_name( nm, ns_parent, ident, search_flags );
	if( nullptr == nm )
		return _ask_for_macro( res, ident, status );
	if( nullptr != pname )
		*pname = nm;

	// Find macro with correct arguments
	core::Tconst_ref<pp::Imacro>	macro;
	if( get_macro_from_name( macro, *nm, params ).failed_status( status ) )
		return status;
	if( nullptr == macro )
		return STATUS_ERROR_INVALID_TYPE;

	// Convert to macro
	if( _finish_macro_search( macro, *macro, ident, search_flags ).failed_status( status ) )
		return status;

	// Exit
	res = macro;
	return STATUS_SUCCESS;
}

const status_t f1::pp::Iparser::_finish_macro_search( core::Tconst_ref<pp::Imacro> & res, const pp::Imacro & macro, const core::stringslice & ident, find_macro_flags_t search_flags ) const
{
	res = &macro;
//	if( macro.get_flags( pp::Imacro::macro_flags_t::fl_being_expanded ) )	// TODO: sometimes it may be too restrictive
//		return STATUS_ERROR_BUSY;
	if( !(search_flags & find_macro_flags_t::fl_macro_search_force) && macro.get_flags( pp::Imacro::macro_flags_t::fl_undefined ) )
		return _ask_for_macro( res, ident, STATUS_ERROR_NOT_FOUND );
	if( !(search_flags & find_macro_flags_t::fl_macro_search_all_defined) )
	{
		if( ( macro.get_flags( pp::Imacro::macro_flags_t::fl_expression_only )	&& !(m_parser_flags & parser_flags_t::fl_expression_mode) && !(search_flags & find_macro_flags_t::fl_macro_search_test_mode) ) ||
			( macro.get_flags( pp::Imacro::macro_flags_t::fl_operator )			&& !(m_parser_flags & parser_flags_t::fl_expression_mode) )
			)
		{
			res = nullptr;
			if( _ask_for_macro( res, ident, STATUS_ERROR_NOT_FOUND ).failed() )
			{
				//const_cast<self *>(this)->message( context::MSGID_MACRO_NOT_AVAILABLE, core::string( ident ).c_str() );
				return STATUS_ERROR_NOT_FOUND;
			}
		}
	}

	return STATUS_SUCCESS;
}

const status_t f1::pp::Iparser::find_macro_name( core::Tconst_ref<ns::name> & res,
	const ns::name_space * ns_parent, const core::stringslice & ident,
	find_macro_flags_t search_flags ) const
{
	status_t status;

	res = nullptr;

	// Find named object in the current and upper namespaces
	core::Tconst_ref<ns::name_space> ns = nullptr != ns_parent ? ns_parent : m_ns_context.get_current_namespace();
	core::Tconst_ref<ns::name> nm = nullptr;
	while( nullptr != ns )
	{
		if( ns->find_subname( nm, ident ).succeeded_status( status ) )
			break;
		if( search_flags & find_macro_flags_t::fl_macro_search_current_namespace_only )
			break;
		nm = ns->get_default_name();
		if( nullptr == nm )
			break;
		ns = nm->get_namespace();
	}

	res = nm;
	return nullptr == nm ? STATUS_ERROR_NOT_FOUND : STATUS_SUCCESS;
}

const status_t f1::pp::Iparser::get_macro_from_name( core::Tconst_ref<pp::Imacro> & res, const ns::name & nm ) const
{
	core::Tconst_ref<macro_object> mac = type_cast<const macro_object *>(nm.get_object());
	if( nullptr == mac )
		return STATUS_ERROR_INVALID_TYPE;
#if 0
	array_foreach( pp::const_macro_list_t::const_iterator, it, mac->m_macro_list )
	{
		if( nullptr == it->get_param_list() )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}

	if( !mac->m_macro_list.empty() )
		res = *mac->m_macro_list.rbegin();

	return STATUS_ERROR_SUCCESS;
#else
	if( !mac->m_macro_list.empty() )
		res = *mac->m_macro_list.rbegin();
	return STATUS_SUCCESS;
#endif
}
const status_t f1::pp::Iparser::get_macro_from_name( core::Tconst_ref<pp::Imacro> & res, const ns::name & nm, const pp::arg_list & args ) const
{
	core::Tconst_ref<macro_object> mac = type_cast<const macro_object *>(nm.get_object());
	if( nullptr == mac )
		return STATUS_ERROR_INVALID_TYPE;
#if 0
	array_foreach( pp::const_macro_list_t::const_iterator, it, mac->m_macro_list )
	{
		if( nullptr == it->get_param_list() )
			continue;
		if( it->get_param_list()->match_arg_list( args ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
#else
	if( !mac->m_macro_list.empty() )
		res = *mac->m_macro_list.rbegin();
	__unreferenced_parameter( args );
	return STATUS_SUCCESS;
#endif
	//res = mac->m_macro;
	//return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::get_macro_from_name( core::Tconst_ref<pp::Imacro> & res, const ns::name & nm, const pp::param_list & params ) const
{
	core::Tconst_ref<macro_object> mac = type_cast<const macro_object *>(nm.get_object());
	if( nullptr == mac )
		return STATUS_ERROR_INVALID_TYPE;
#if 0
	array_foreach( pp::const_macro_list_t::const_iterator, it, mac->m_macro_list )
	{
		if( nullptr == it->get_param_list() )
			continue;
		if( it->get_param_list()->equal( *params ) )
		{
			res = *it;
			return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
#else
	if( !mac->m_macro_list.empty() )
		res = *mac->m_macro_list.rbegin();
	__unreferenced_parameter( params );
	return STATUS_SUCCESS;
#endif
	//res = mac->m_macro;
	//return STATUS_SUCCESS;
}

const status_t f1::pp::Iparser::on_macro_expanded( pp::Imacro & mac )
{
//	if( mac.get_flags( Imacro::bf_deprecated ) )
//		message( syntax::IMSG_W)

	mac.on_macro_referenced();

	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::_ask_for_macro( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident, status_t default_status ) const
{
	if( on_unknown_macro_identifier( res, ident ).succeeded() && nullptr != res )
		return STATUS_SUCCESS;

	if( m_parser_flags & parser_flags_t::fl_expression_mode )
	{
		if( on_unknown_macro( res, ident ).succeeded() && nullptr != res )
			return STATUS_SUCCESS;
	}

	return default_status;
}
const status_t f1::pp::Iparser::on_unknown_macro_identifier( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident ) const
{
	__unreferenced_parameter( res );
	__unreferenced_parameter( ident );
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::on_unknown_macro( core::Tconst_ref<pp::Imacro> & res, const core::stringslice & ident ) const
{
	__unreferenced_parameter( res );
	__unreferenced_parameter( ident );
	return STATUS_SUCCESS;
}

const status_t f1::pp::Iparser::push_macros( const core::Tarray<pp::string> & ident_list )
{
	status_t status;

	macro_stack_entry * entry = m_macro_stack.append_new();

	array_foreach( core::Tarray<pp::string>::const_iterator, it, ident_list )
	{
		const pp::string & ident = *it;

		core::Tref<pp::Imacro>	mac;
		core::Tref<ns::name>	nm;
		if( find_macro( mac, nullptr, ident, null, &nm ).failed_status( status ) )
			return status;
		__debugbreak_if( nullptr == mac )
			return STATUS_ERROR_INTERNAL;

		macro_desc * desc = entry->m_macro_desc_list.append_new();
		desc->m_ident = ident;
		desc->m_macro_object = type_cast<macro_object *>(nm->get_object());
		desc->m_namespace = nm->get_namespace();
	}

	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::pop_macros()
{
	// Check if macro stack contains saved macros
	__debugbreak_if( m_macro_stack.empty() )
		return STATUS_ERROR_STACK_IS_EMPTY;

	// Get element from the top of the stack
	macro_stack_entry * entry = m_macro_stack.get_last();
	__debugbreak_if( nullptr == entry )
		return STATUS_ERROR_INVALID_STATE;

	// Restore all macros
	array_foreach( core::Tarray<macro_desc>::iterator, it, entry->m_macro_desc_list )
	{
		macro_desc & desc = *it;

		// Try to find name which is currently may be used by another macro object
		core::Tref<pp::Imacro>	mac;
		core::Tref<ns::name>	nm;
		if( find_macro( mac, nullptr, desc.m_ident, null, &nm ).failed() )
		{
			// Create new name in case it has already been deleted
			desc.m_namespace->create_subname( nm, nullptr, 0, desc.m_ident.c_str(), desc.m_ident.size() );
		}
		__debugbreak_if( nullptr == nm )
			return STATUS_ERROR_INTERNAL;

		// Replace previous macro object with the new one
		nm->set_object( desc.m_macro_object );
	}

	// Exit
	return STATUS_SUCCESS;
}

const status_t f1::pp::Iparser::dump_macros( io::text_ostream & str )
{
	core::string buf;
	array_foreach( core::Tobject_array<macro_object>::const_iterator, it, m_macro_list )
	{
		it->get_printable_full_name( buf );
		str.format( "%s\n", buf.c_str() );
	}
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::dump_macro( io::text_ostream & str, const pp::Imacro & mac )
{
	core::string buf;

	// Display macro name
	str.format( "NAME: %s\n", mac.get_name() );

	// Display attributes
	core::Tconst_ref<f1::pp::attribute_list> al = mac.get_attribute_list();
	if( nullptr != al && !al->get_attribute_list().empty() )
	{
		str.format( "ATTRIBUTES:\n" );
		array_foreach( f1::pp::const_attribute_list_t::const_iterator, it, al->get_attribute_list() )
		{
			str.format( " %s\n", buf.c_str() );
		}
	}

	// Display value
	const f1::pp::string & value = mac.get_value();

	str.format( "VALUE:\n" );
	str.format( "%s\n", value.c_str() );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::dump_macro_stack( msgout::context & ctx )
{
	// Exit
	__unreferenced_parameter( ctx );
	return STATUS_SUCCESS;
}

const f1::ns::context & f1::pp::Iparser::get_ns_context() const
{
	return m_ns_context;
}
f1::ns::context & f1::pp::Iparser::get_ns_context()
{
	return m_ns_context;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions

/*const status_t f1::pp::Iparser::append_function( const pp::Ifunction & func )
{
	std::string name( func.get_name() );
	function_map_t::iterator it = m_function_map.find( name );
	__debugbreak_if( m_function_map.end() != it )
		return STATUS_ERROR_ALREADY_OWNED;

	m_function_map.insert( function_pair_t( name, &func ) );
	m_function_list.append( &func );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::remove_function( const pp::Ifunction & func )
{
	std::string name( func.get_name() );
	function_map_t::iterator it = m_function_map.find( name );
	__debugbreak_if( m_function_map.end() == it )
		return STATUS_ERROR_NOT_FOUND;

	m_function_map.erase( it );
	return m_function_list.remove( &func );
}
const status_t f1::pp::Iparser::remove_all_function()
{
	for( function_map_t::iterator it = m_function_map.begin(); it != m_function_map.end(); ++ it )
		it->second.clear();
	m_function_map.clear();

	return m_function_list.remove_all();
}

const status_t f1::pp::Iparser::find_function( core::Tconst_ref<Ifunction> & res, const f1::pp::string & name ) const
{
	return find_function( res, name.data(), name.length() );
}
const status_t f1::pp::Iparser::find_function( core::Tconst_ref<Ifunction> & res, const f1::core::stringslice & name ) const
{
	return find_function( res, name.data(), name.length() );
}
const status_t f1::pp::Iparser::find_function( core::Tconst_ref<Ifunction> & res, const char * str, size_t len ) const
{
	std::string name( str, len );
	function_map_t::const_iterator it = m_function_map.find( name.c_str() );
	if( m_function_map.end() == it )
	{
		res = nullptr;
		return STATUS_ERROR_NOT_FOUND;
	}

	res = it->second;
	return STATUS_SUCCESS;
}
*/
const status_t f1::pp::Iparser::dump_includes( msgout::context & ctx, dump_flags_t flags ) const
{
	__debugbreak_if( nullptr == m_parser )
		return STATUS_ERROR_NOT_INITIALIZED;
	__debugbreak_if( nullptr == m_parser->get_root_source() )
		return STATUS_ERROR_NOT_INITIALIZED;
	return dump_include( ctx, *m_parser->get_root_source(), flags );
}
const status_t f1::pp::Iparser::dump_include( msgout::context & ctx, f1::parser::Isource & src, dump_flags_t flags ) const
{
	status_t status;

	// Dump self
	if( dump_include_info( ctx, src, flags ).failed_status( status ) )
		return status;

	// Dump sub-sources
	ctx.begin_level();
	array_foreach( f1::parser::source_list_t::const_iterator, it, src.get_source_list() )
	{
		core::Tref<parser::Isource> sub_src = *it;
		__debugbreak_ifnot( nullptr != sub_src )
			dump_include( ctx, *sub_src, flags );
	}
	ctx.end_level();

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::Iparser::dump_include_info( msgout::context & ctx, f1::parser::Isource & src, dump_flags_t flags ) const
{
#if 1
	// Calculate identation
	unsigned ident = 0;
	for( core::Tref<f1::parser::Isource> s = src.get_parent(); nullptr != s; s = s->get_parent() )
		++ ident;

	// Print string
	if( 0 != ident )
		ctx.print( "%*c", ident - 1, 0x20, src.get_filename() );
#else
	ctx.print( "%s\n", src.get_filename() );
#endif
	// Exit
	__unreferenced_parameter( flags );
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Standards

const status_t f1::pp::Iparser::attach_standard( const Istandard & standard )
{
	status_t status;

	if( m_standard_list.append_unique( &standard ).failed_status( status ) )
		return status;

	const_cast<Istandard*>(&standard)->on_attach_to_parser( *this );	// TODO: fix const

	return status;
}
const status_t f1::pp::Iparser::detach_standard( const Istandard & standard )
{
	status_t status;

	if( !m_standard_list.exists( &standard ) )
		return STATUS_SUCCESS;

	const_cast<Istandard*>(&standard)->on_detach_from_parser( *this );	// TODO: fix const

	if( m_standard_list.remove( &standard ).failed_status( status ) )
		return status;

	return status;
}

bool f1::pp::Iparser::is_standard_attached( const Istandard & standard ) const
{
	return m_standard_list.exists( &standard );
}
const f1::pp::const_standard_list_t & f1::pp::Iparser::get_standard_list() const
{
	return m_standard_list;
}

const f1::pp::Istandard * f1::pp::Iparser::find_item_standard( const f1::guid_t & item_class_guid, const pp::string & name ) const
{
	// Check this standard first
	array_foreach( const_standard_list_t::const_iterator, it, m_standard_list )
	{
		const f1::pp::Istandard * standard = m_context->find_item_standard( item_class_guid, **it, name );
		if( nullptr != standard )
			return standard;
	}

	// Exit
	return nullptr;
}

/*END OF Iparser.cxx*/

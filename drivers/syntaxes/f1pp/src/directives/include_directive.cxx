/*include_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::include_directive::include_directive( unsigned flags ):
	super( flags )
{
	m_parser_event_handler.m_include = this;
}

// Properties
const char * f1::f1pp::include_directive::get_name() const
{
	return "include";
}

// {34527A69-6BEA-4582-8F29-52798635EEB3}
const f1::guid_t f1::f1pp::include_directive::m_guid =
{ 0x34527a69, 0x6bea, 0x4582, { 0x8f, 0x29, 0x52, 0x79, 0x86, 0x35, 0xee, 0xb3 } };
const f1::guid_t & f1::f1pp::include_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::include_directive::get_caps( unsigned mask ) const __noexcept
{
	return (super::get_caps() | Idirective::cap_override) & mask;
}

// Operations
const status_t f1::f1pp::include_directive::parse( pp::Iparser & parser )
{
	return parse( parser, true );
}
const status_t f1::f1pp::include_directive::parse( pp::Iparser & parser, bool first_run )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = f1par->get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Skip spaces and check for EOL
	f1par->skip_real_spaces_and_comments();
	if( f1par->get_parser()->is_nl() )
	{
		if( first_run )
			return f1par->message( f1::c_pp::syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );
		return execute_tasks( *f1par, *f1par->get_parser() );
	}

	// Parse comma
	if( !first_run )
	{
		if( !par->skip_char( ',' ) )
		{
			f1par->skip_to_newline();
			return f1par->message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "," );
		}
	}

	// Parse attributes
	bool append = false;
	m_attributes = 0;
	if( f1par->exists_directive_attribute( "append" ) )
	{
		append = true;
		m_attributes |= attribute_flags_t::fl_attr_append;
	}
	if( f1par->exists_directive_attribute( "once" ) )
		m_attributes |= attribute_flags_t::fl_attr_once;
	if( f1par->exists_directive_attribute( "multi" ) )
		m_attributes |= attribute_flags_t::fl_attr_multi;
	if( f1par->exists_directive_attribute( "next" ) )
		m_attributes |= attribute_flags_t::fl_attr_next;
	if( f1par->exists_directive_attribute( "nooutput" ) )
		m_attributes |= attribute_flags_t::fl_attr_no_output;
	if( f1par->exists_directive_attribute( "raw" ) )
		m_attributes |= attribute_flags_t::fl_attr_raw;
	if( f1par->exists_directive_attribute( "silent" ) )
		m_attributes |= attribute_flags_t::fl_attr_silent;

	// Parse macro name first
	f1par->skip_real_spaces_and_comments();
	if( f1par->parse_identifier_and_type( m_target_macro, &append ).failed_status( status ) )
		return status;

	if( append & !(m_attributes & attribute_flags_t::fl_attr_append) )
		m_attributes |= attribute_flags_t::fl_attr_append;
	else if( !append & (m_attributes & attribute_flags_t::fl_attr_append) )
		m_attributes &= ~attribute_flags_t::fl_attr_append;

	// Find length of the include sub-task
	size_t len = 0;
	f1par->skip_real_spaces_and_comments();
	f1par->find_separator( len, ',' );

	// Capture line
	f1par->modify_c_flags( c_pp::parser::c_flags_t::fl_disable_string_escapes );
	return f1par->capture_line( line_capture_callback, this, len );
}

const status_t f1::f1pp::include_directive::execute( f1::c_pp::parser & cpp_par, const char * & s, const char * end_s )
{
	status_t status = STATUS_SUCCESS;

	// Check current state
	core::Tref<f1::f1pp::parser> f1par = type_cast<f1pp::parser *>(&cpp_par);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = f1par->get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	if( nullcstr == s )
		return STATUS_SUCCESS;

	// Parse all include file names
	include_task * tsk = m_include_tasks.append_new();
	tsk->m_include_flags = null;
	tsk->m_target_macro.move( m_target_macro );
	tsk->m_attributes = m_attributes;

	// Parse include file name
	// TODO: Warning: copy-paste from C99
	f1par->skip_real_spaces_and_comments( s, end_s );
	if( parse_include_filename( *f1par, tsk->m_file_path_name, tsk->m_include_flags, s, end_s ).failed_status( status ) )
		return STATUS_SUCCESS;	// drop status code
	if( tsk->m_file_path_name.empty() )
	{
		if( s < end_s )
			return f1par->message( f1::c_pp::syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );
			//return f1par->message( f1::c_pp::syntax::IMSG_FATAL_ERROR_INVALID_FILE_NAME );
		return STATUS_SUCCESS;
	}

	// Parse argument list
	f1par->skip_real_spaces_and_comments( s, end_s );
	if( s < end_s && '(' == *s )
	{
		if( f1par->parse_arg_list( tsk->m_args, nullptr, s, end_s, 0, c_pp::parser::fl_parse_args_allow_newlines ).failed_status( status ) )
			return status;
		f1par->skip_real_spaces_and_comments( s, end_s );
	}

	// Iterate
	f1par->skip_real_spaces_and_comments( s, end_s );		// skip comments and spaces, EXCLUDING newline
	if( s < end_s )
		return f1par->message( c_pp::syntax::IMSG_ERROR_EXPECTED_S, "," );

	// Launch next iteration
	return parse( *f1par, false );							// start expanding macros
}

const status_t f1::f1pp::include_directive::execute_tasks( f1::f1pp::parser & f1par, f1::parser::Iparser & par )
{
	status_t status;

	// Check that something was parsed
	if( m_include_tasks.empty() )
		return f1par.message( f1::c_pp::syntax::IMSG_ERROR_EXPECTED_INCLUDE_FILENAME );

	// Single include variant (multi-include will require postpone inclusion of second and following include files
	// until the first file is finished.
	par.register_event_handler(
		f1::parser::event_type_t(f1::parser::etype_source_included | f1::parser::etype_source_entered),
		m_parser_event_handler, this );
	//m_parser_event_handler.m_last_source_end_offset = size_t(-1);
	m_parser_event_handler.m_parser = &f1par;
	m_parser_event_handler.m_parent_source = par.get_current_source();

	m_parser_event_handler.m_include_tasks.move( m_include_tasks );
	m_parser_event_handler.m_include_tasks_iterator.reset( m_parser_event_handler.m_include_tasks );

	const include_task * tsk = &*m_parser_event_handler.m_include_tasks_iterator;
	m_parser_event_handler.m_macro = tsk->m_target_macro;
	m_parser_event_handler.m_args = tsk->m_args;
	m_parser_event_handler.m_include_tasks_iterator.next();
	m_parser_event_handler.m_attributes = tsk->m_attributes;

	f1::c_pp::parser::include_desc * desc = nullptr;
	if( include_file( f1par, tsk->m_file_path_name, tsk->m_include_flags, &desc ).failed_status( status ) )
		status = on_include_not_found( f1par, tsk->m_file_path_name, tsk->m_include_flags );
	else if( nullptr != desc )
	{
		if( tsk->m_attributes & attribute_flags_t::fl_attr_once )
			desc->m_flags |= f1::c_pp::parser::include_flags_t::fl_once_included;
		if( tsk->m_attributes & attribute_flags_t::fl_attr_silent )
			desc->m_flags |= f1::c_pp::parser::include_flags_t::fl_silent;
	}

	//m_parser_event_handler.invoke( f1::parser::etype_source_entered, nullptr, m_parser_event_handler.m_parent_source, *par );

	/*array_foreach( core::Tarray<include_task>::const_iterator, it, m_include_tasks )
	{
		const include_task & tsk = *it;

		__assert( nullptr == m_parser_event_handler.m_macro );	// should have already been processed
		m_parser_event_handler.m_macro = tsk.m_macro;
		m_parser_event_handler.m_args = tsk.m_args;

		// Include file
		if( include_file( f1par, tsk.m_file_path_name, tsk.m_include_path_flags_t ).failed_status( status ) )
		{
			status = on_include_not_found( f1par, tsk.m_file_path_name, tsk.m_include_path_flags_t );
			break;
		}
	}

	par->unregister_event_handler( f1::parser::etype_source_included, m_parser_event_handler, this );*/

//	f1par.skip_to_newline( s, end_s );	// skip characters in the case of an error

	// Exit
	return status;
}
const status_t f1::f1pp::include_directive::parser_event_handler::invoke(
	f1::parser::event_type_t type_mask,
	void * handler_param,
	void * event_param,
	f1::parser::Iparser & parser )
{
	status_t status;

	core::Tref<f1::parser::Isource> src = type_cast<f1::parser::Isource *>( reinterpret_cast<core::object *>( event_param ) );
	__debugbreak_if( nullptr == src )
		return STATUS_ERROR_INTERNAL;
	core::Tref<f1::f1pp::parser> f1par = m_parser.dynamic_pointer_cast<f1::f1pp::parser>();
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_INTERNAL;

	if( type_mask & f1::parser::etype_source_included )
	{
		//core::stringslice data;
		//src->get_source_data( data );

		//m_last_source_end_offset = src->get_parent_offset() + data.length();

		if( src->get_parent() == m_parent_source )
		{
			parser::source_context * ctx = f1par->get_source_context( *src );
			//parser::source_context * ctx = f1par->create_source_context( *src );
			__debugbreak_if( nullptr == ctx )
				return STATUS_ERROR_INTERNAL;
			ctx->m_args.move( m_args );
			ctx->m_macro.move( m_macro );
			ctx->m_target_macro_append_mode = !!(m_attributes & attribute_flags_t::fl_attr_append);
		}

		/*ctx->m_output.initialize();
		ctx->m_storage.initialize();
		if( nullptr != ctx->m_macro )
		{
			parser.set_output( &ctx->m_output, &ctx->m_old_output );
			parser.set_storage( &ctx->m_storage, &ctx->m_old_storage );
		}*/

		return STATUS_SUCCESS;
	}
	if( type_mask & f1::parser::etype_source_entered )
	{
		/*parser::source_context * ctx = f1par->get_source_context( *src );
		__debugbreak_if( nullptr == ctx )
			return STATUS_ERROR_INTERNAL;

		if( nullptr != ctx->m_macro )
		{
			parser.set_output( &ctx->m_old_output );
			parser.set_storage( &ctx->m_old_storage );
		}*/

		if( src == m_parent_source )
		{
			if( m_include_tasks_iterator.valid() )
			{
				include_task & tsk = *m_include_tasks_iterator;
				m_macro.move( tsk.m_target_macro );
				m_args.move( tsk.m_args );
				m_attributes = tsk.m_attributes;
				m_include_tasks_iterator.next();
				if( m_include->include_file( *f1par, tsk.m_file_path_name, tsk.m_include_flags ).failed_status( status ) )
					status = m_include->on_include_not_found( *f1par, tsk.m_file_path_name, tsk.m_include_flags );
			}
			else
			{
				m_include_tasks.clear();
				parser.unregister_event_handler(
					f1::parser::event_type_t(f1::parser::etype_source_included | f1::parser::etype_source_entered),
					*this, m_include );
			}
		}
		return STATUS_SUCCESS;
	}

	__unreferenced_parameter( handler_param );
	return STATUS_ERROR_INTERNAL;
}

// Hooks
const status_t f1::f1pp::include_directive::include_file( f1::c_pp::parser & par, const pp::string & file_path_name, c_pp::parser::include_flags_t include_flags, c_pp::parser::include_desc ** inc_desc_ptr )
{
	return par.include_file( include_flags, file_path_name, size_t(-1)/*m_parser_event_handler.m_last_source_end_offset*/, inc_desc_ptr );
}
/*const status_t f1::f1pp::include_directive::on_complete( f1::c_pp::parser & par, const char *& s, const char * end_s )
{
	status_t status;


	// Exit
	return STATUS_SUCCESS;
}
*/
/*END OF include_directive.cxx*/

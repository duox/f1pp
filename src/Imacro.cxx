/*Imacro.cxx*/
#include "pch.h"
#include "main.h"

// {54FD0EE9-CF00-4705-9E04-B7104F0AD11D}
const f1::guid_t f1::pp::Imacro::m_item_class_guid =
{ 0x54fd0ee9, 0xcf00, 0x4705, { 0x9e, 0x04, 0xb7, 0x10, 0x4f, 0x0a, 0xd1, 0x1d } };

f1::pp::Imacro::Imacro( const core::Iidentifier * ident ) NOEXCEPT:
	super( ident ),
	m_expand_count( 0 )
{
}
f1::pp::Imacro::~Imacro() NOEXCEPT
{
}

const status_t f1::pp::Imacro::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, const pp::string * value, macro_flags_t flags )
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Parse attributes
	macro_flags_t attr_flags = null;
	if( nullptr != attrs )
	{
		if( attrs->exists_attribute( "const" ) )
			attr_flags |= Imacro::macro_flags_t::fl_immutable;
		if( attrs->exists_attribute( "deprecated" ) )
			attr_flags |= Imacro::macro_flags_t::fl_deprecated;
		if( attrs->exists_attribute( "disabled" ) )
			attr_flags |= Imacro::macro_flags_t::fl_disabled;
		if( attrs->exists_attribute( "protected" ) )
			attr_flags |= Imacro::macro_flags_t::fl_immutable;
		if( attrs->exists_attribute( "weak" ) )
			attr_flags |= Imacro::macro_flags_t::fl_weak;
	}

	// Create initial definition
	definition * def = m_def_stack.append_new();

	set_attribute_list( attrs );
	def->m_param_list = params;
	def->m_macro_flags = flags | attr_flags;

	if( nullptr != value )
	{
		set_value( *value );
		/*def->m_value = *value;
		pp::service::trim_spaces( def->m_value );

		def->m_variant.set_string( def->m_value.data(), def->m_value.size() );

		post_change_value( def->m_value, def->m_variant );*/
	}

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::initialize( macro_flags_t flags )
{
	return initialize( nullptr, nullptr, nullptr, flags );
}
const status_t f1::pp::Imacro::initialize( const f1::pp::attribute_list * attrs,
	const pp::param_list * params, macro_flags_t flags )
{
	return initialize( attrs, params, nullptr, flags );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

//const f1::pp::context * f1::pp::Imacro::get_context() const
//{
//	if( nullptr != m_parser )
//		return m_parser->get_context();
//	__return_fail nullptr;
//}
/*f1::pp::context * f1::pp::Imacro::get_context()
{
	return m_context;
}*/

/*const f1::pp::Iparser * f1::pp::Imacro::get_parser() const
{
	return m_parser;
}
f1::pp::Iparser * f1::pp::Imacro::get_parser()
{
	return m_parser;
}*/

const status_t f1::pp::Imacro::modify_flags( macro_flags_t flags_to_set, macro_flags_t flags_to_clear, macro_flags_t * prev_flags )
{
	__debugbreak_if( m_def_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;	// empty definition stack is invalid

	core::Tbitflags<macro_flags_t>::make_flags( (*m_def_stack.rbegin()).m_macro_flags, flags_to_set, flags_to_clear, prev_flags );

	return STATUS_SUCCESS;
}
f1::pp::Imacro::macro_flags_t f1::pp::Imacro::get_flags( macro_flags_t mask ) const
{
	__debugbreak_if( m_def_stack.empty() )
		return null;	// empty definition stack is invalid
	const definition & def = *m_def_stack.rbegin();

	return (def.m_macro_flags | def.m_attr_flags) & mask;
}

const char * f1::pp::Imacro::get_name() const
{
	DBG_FAIL_ON_PURE_CALL();
	return nullcstr;
}

const f1::pp::attribute_list * f1::pp::Imacro::get_attribute_list() const
{
	__debugbreak_if( m_def_stack.empty() )
		return nullptr;	// empty definition stack is invalid
	return (*m_def_stack.rbegin()).m_attribute_list;
}
f1::pp::attribute_list * f1::pp::Imacro::get_attribute_list()
{
	__debugbreak_if( m_def_stack.empty() )
		return nullptr;	// empty definition stack is invalid
	return (*m_def_stack.rbegin()).m_attribute_list;
}
const status_t f1::pp::Imacro::set_attribute_list( const f1::pp::attribute_list * al )
{
	__debugbreak_if( m_def_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;	// empty definition stack is invalid
	definition & def = *m_def_stack.rbegin();

	def.m_attribute_list = al;
	if( nullptr != al )
	{
		if( al->exists_attribute( "deprecated" ) )
			def.m_attr_flags |= macro_flags_t::fl_deprecated;
		else
			def.m_attr_flags &= ~macro_flags_t::fl_deprecated;

		if( al->exists_attribute( "disabled" ) )
			def.m_attr_flags |= macro_flags_t::fl_disabled;
		else
			def.m_attr_flags &= ~macro_flags_t::fl_disabled;

		if( al->exists_attribute( "ignored" ) )
			def.m_attr_flags |= macro_flags_t::fl_ignore;
		else
			def.m_attr_flags &= ~macro_flags_t::fl_ignore;

		if( al->exists_attribute( "weak" ) )
			def.m_attr_flags |= macro_flags_t::fl_weak;
		else
			def.m_attr_flags &= ~macro_flags_t::fl_weak;
	}
	else
	{
		def.m_attr_flags = null;
	}

	return STATUS_SUCCESS;
}

const f1::pp::param_list * f1::pp::Imacro::get_param_list() const
{
	__debugbreak_if( m_def_stack.empty() )
		return nullptr;	// empty definition stack is invalid
	return (*m_def_stack.rbegin()).m_param_list;
}
f1::pp::param_list * f1::pp::Imacro::get_param_list()
{
	__debugbreak_if( m_def_stack.empty() )
		return nullptr;	// empty definition stack is invalid
	return (*m_def_stack.rbegin()).m_param_list;
}
const status_t f1::pp::Imacro::set_param_list( const f1::pp::param_list * pl )
{
	__debugbreak_if( m_def_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;	// empty definition stack is invalid
	(*m_def_stack.rbegin()).m_param_list = pl;
	return STATUS_SUCCESS;
}

const status_t f1::pp::Imacro::set_definition_location( parser::Isource & src, parser::lineno_t line )
{
	__debugbreak_if( m_def_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;	// empty definition stack is invalid

	(*m_def_stack.rbegin()).m_definition_source = &src;
	(*m_def_stack.rbegin()).m_definition_line = line;

	return STATUS_SUCCESS;
}
f1::parser::lineno_t f1::pp::Imacro::get_definition_line() const
{
	__debugbreak_if( m_def_stack.empty() )
		return 0;	// empty definition stack is invalid
	const definition & def = *m_def_stack.rbegin();
	return def.m_definition_line;
}
const f1::parser::Isource * f1::pp::Imacro::get_definition_source() const
{
	__debugbreak_if( m_def_stack.empty() )
		return nullptr;	// empty definition stack is invalid
	return (*m_def_stack.rbegin()).m_definition_source;
}
f1::parser::Isource * f1::pp::Imacro::get_definition_source()
{
	__debugbreak_if( m_def_stack.empty() )
		return nullptr;	// empty definition stack is invalid
	return (*m_def_stack.rbegin()).m_definition_source;
}

const f1::pp::variant & f1::pp::Imacro::get_value_variant() const
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return f1::pp::variant::null;

	const definition & def = *m_def_stack.rbegin();
	return def.m_variant;
}
const f1::pp::string & f1::pp::Imacro::get_value() const
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return f1::pp::string::null;

	const definition & def = *m_def_stack.rbegin();
	return def.m_value;
}
const f1::core::md5hash::digest_t & f1::pp::Imacro::get_value_hash() const
{
	const definition & def = *m_def_stack.rbegin();
	if( !def.m_digest_valid )
	{
		definition * d = const_cast<definition *>(&def);
		if( def.m_value.empty() )
			memset( d->m_digest, 0, sizeof(def.m_digest) );
		else
			core::md5hash::calc_blob_hash( d->m_digest, def.m_value.data(), def.m_value.size() );
		d->m_digest_valid = true;
	}
	return def.m_digest;
}

const status_t f1::pp::Imacro::set_value_hash( core::md5hash::digest_t & hash )
{
	definition & def = *m_def_stack.rbegin();
	memcpy( def.m_digest, &hash, sizeof(def.m_digest) );
	def.m_digest_valid = true;
	return STATUS_SUCCESS;
}

const status_t f1::pp::Imacro::set_value_variant( const f1::pp::variant & value )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;

	pp::string altered_value_string;
	if( pp::variant::ty_none != value.get_type() )
		value.format( altered_value_string );
	else
		altered_value_string.clear();
	pp::variant altered_value_variant( value );
	if( !pre_change_value( altered_value_string, &altered_value_variant ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_variant.move( altered_value_variant );
	def.m_value.move( altered_value_string );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::set_value_variant_move( f1::pp::variant & value )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;

	pp::string altered_value_string;
	if( pp::variant::ty_none != value.get_type() )
		value.format( altered_value_string );
	else
		altered_value_string.clear();
	pp::variant altered_value_variant;
	altered_value_variant.move( value );
	if( !pre_change_value( altered_value_string, &altered_value_variant ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_variant.move( altered_value_variant );
	def.m_value.move( altered_value_string );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::set_value_variant( const f1::pp::variant & value, const f1::pp::string & value_string )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;
	__debugbreak_if( !value_string.valid() )
		return STATUS_ERROR_INTERNAL;

	pp::string altered_value_string( value_string );
	pp::variant altered_value_variant( value );
	if( !pre_change_value( altered_value_string, &altered_value_variant ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_variant.move( altered_value_variant );
	def.m_value.move( altered_value_string );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::set_value_variant_move( f1::pp::variant & value, f1::pp::string & value_string )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;
	__debugbreak_if( !value_string.valid() )
		return STATUS_ERROR_INTERNAL;

	pp::string altered_value_string;	altered_value_string.move( value_string );
	pp::variant altered_value_variant;	altered_value_variant.move( value );
	if( !pre_change_value( altered_value_string, &altered_value_variant ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_variant.move( altered_value_variant );
	def.m_value.move( altered_value_string );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::set_value( const char * value, size_t len )
{
	return set_value( f1::pp::string( value, len ) );	// TODO: static variant
}
const status_t f1::pp::Imacro::set_value( const f1::pp::string & value )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;
	__debugbreak_if( !value.valid() )
		return STATUS_ERROR_INTERNAL;

	pp::string altered_value( value );
	if( !pre_change_value( altered_value ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_variant.set_string( altered_value.data(), altered_value.size() );
	def.m_value.move( altered_value );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::set_value_move( f1::pp::string & value )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;
	__debugbreak_if( !value.valid() )
		return STATUS_ERROR_INTERNAL;

	pp::string altered_value;
	altered_value.move( value );
	if( !pre_change_value( altered_value ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_value.move( altered_value );
	def.m_variant.set_string( altered_value.data(), altered_value.size() );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::append_value( const char * value, size_t len )
{
	return append_value( f1::pp::string( value, len ) );	// TODO: static variant
}
const status_t f1::pp::Imacro::append_value( const f1::pp::string & value )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;
	__debugbreak_if( !value.valid() )
		return STATUS_ERROR_INTERNAL;

	pp::string altered_value = value;
	if( !pre_change_value( altered_value ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_value.add_part( value );
	def.m_variant.set_string( def.m_value.data(), def.m_value.size() );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::append_value_move( f1::pp::string & value )
{
	__debugbreak_if( m_def_stack.empty() )	// cannot allow empty value stack
		return STATUS_ERROR_INVALID_STATE;
	__debugbreak_if( !value.valid() )
		return STATUS_ERROR_INTERNAL;

	pp::string altered_value;
	altered_value.move( value );
	if( !pre_change_value( altered_value ) )
		return STATUS_SUCCESS;

	definition & def = *m_def_stack.rbegin();
	def.m_value.add_part( value );
	def.m_variant.set_string( def.m_value.data(), def.m_value.size() );

	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}

bool f1::pp::Imacro::pre_change_value( f1::pp::string & string_value, f1::pp::variant * variant_value )
{
	__unreferenced_parameter( string_value );
	__unreferenced_parameter( variant_value );
	return true;
}
void f1::pp::Imacro::post_change_value( const f1::pp::string & string_value, const f1::pp::variant & variant_value )
{
	__unreferenced_parameter( string_value );
	__unreferenced_parameter( variant_value );
	return;
}

const status_t f1::pp::Imacro::on_macro_referenced()
{
	__debugbreak_ifnot( uint32_t(-1) != m_expand_count )
		++ m_expand_count;

	return STATUS_SUCCESS;
}
uint32_t f1::pp::Imacro::get_reference_count() const
{
	return m_expand_count;
}

const status_t f1::pp::Imacro::set_deprecation_text( const f1::pp::string & text )
{
	m_deprecation_text = text;
	return STATUS_SUCCESS;
}
const f1::pp::string & f1::pp::Imacro::get_deprecation_text() const
{
	return m_deprecation_text;
}

const status_t f1::pp::Imacro::on_attach_to_named_object( ns::named_object & nobj )
{
	return m_named_object_list.append_unique( &nobj );
}
const status_t f1::pp::Imacro::on_detach_from_named_object( ns::named_object & nobj )
{
	return m_named_object_list.remove( &nobj );
}
const f1::ns::named_object_weak_list_t & f1::pp::Imacro::get_named_object_list() const
{
	return m_named_object_list;
}
f1::ns::named_object_weak_list_t & f1::pp::Imacro::get_named_object_list()
{
	return m_named_object_list;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro value

const status_t f1::pp::Imacro::push_value( const parser::Isource * src, int line )
{
	definition * desc = m_def_stack.append_new();
	*desc = *(m_def_stack.rbegin() + 1);	// copy value from previous definition
	desc->m_alloc_source = src;
	desc->m_alloc_line = line;
	return STATUS_SUCCESS;
}
const status_t f1::pp::Imacro::pop_value()
{
	__debugbreak_if( m_def_stack.empty() )
		return STATUS_ERROR_INVALID_STATE;

	if( m_def_stack.size() == 1 )
		return STATUS_ERROR_INVALID_STATE;

	m_def_stack.pop_back();

	definition & def = *m_def_stack.rbegin();
	post_change_value( def.m_value, def.m_variant );

	return STATUS_SUCCESS;
}
const f1::core::Tarray<f1::pp::Imacro::definition> & f1::pp::Imacro::get_definition_stack() const
{
	return m_def_stack;
}
const f1::pp::Imacro::definition * f1::pp::Imacro::get_first_definition() const
{
	if( m_def_stack.empty() )
		return nullptr;
	return &*m_def_stack.begin();
}
const f1::pp::Imacro::definition * f1::pp::Imacro::get_last_definition() const
{
	if( m_def_stack.empty() )
		return nullptr;
	return &*m_def_stack.rbegin();
}

f1::pp::Imacro::definition::definition() noexcept:
	m_macro_flags(0), m_attr_flags(0), m_digest_valid( false ),
	m_definition_line(0), m_alloc_line(0), m_was_undefined(false)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::pp::Imacro::parse( pp::Iparser & parser, pp::string & buf ) const
{
	DBG_FAIL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( buf );
	return STATUS_ERROR_NOT_SUPPORTED;
}
const status_t f1::pp::Imacro::expand( pp::Iparser & parser, pp::string & buf, const pp::arg_list * args ) const
{
	DBG_FAIL_ON_PURE_CALL();
	__unreferenced_parameter( parser );
	__unreferenced_parameter( buf );
	__unreferenced_parameter( args );
	return STATUS_ERROR_PURE_CALL;
}
bool f1::pp::Imacro::can_undefine() const
{
	return 1 == m_def_stack.size();
}
void f1::pp::Imacro::on_all_names_removed()
{
}

const status_t f1::pp::Imacro::format_macro_signature( core::string & buf, const char * ident ) const
{
	if( nullcstr == ident )
		ident = get_name();
	buf.append( ident );
	if( nullptr != get_param_list() )
	{
		buf.append( "( " );
		array_foreach( pp::param_list::param_desc_list_t::const_iterator, it, get_param_list()->get_param_list() )
		{
			const pp::param_list::param_desc_t & param = *it;

			if( param.m_flags & pp::param_list::fl_vararg )
			{
				__assert( it.is_last() );
				buf.append( "..." );
			}
			else
			{
				buf.append( param.m_name );
				if( param.m_flags & pp::param_list::fl_default_value )
				{
					__assert( !param.m_default_value.empty() );
					buf.append( " = " );
					buf.append( param.m_default_value );
				}
			}

			if( !it.is_last() )
				buf.append( ", " );
		}
		buf.append( " )" );
	}

	return STATUS_SUCCESS;
}

/*END OF Imacro.cxx*/

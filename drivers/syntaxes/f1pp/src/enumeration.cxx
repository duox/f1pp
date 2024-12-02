/*enumeration.cxx*/
#include "pch.h"
#include "main.h"

f1::f1pp::enumeration::enumeration(/* const core::identifier * ident ):
	super( ident */)
{
}

const status_t f1::f1pp::enumeration::initialize( f1pp::parser & par, const pp::string & name )
{
	status_t status;

	// Call base class
	if( super::initialize().failed_status( status ) )
		return status;

	// Store parameters
	m_parser = &par;
	//m_ns.initialize();

	core::Tref<ns::name_space> current_ns = par.get_ns_context().get_current_namespace();
	core::Tref<ns::name> nm;
	current_ns->create_subname( nm, this, 0, name.data(), name.length() );
	current_ns->append_subnamespace( *this );

	// Exit
	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const f1::f1pp::parser * f1::f1pp::enumeration::get_parser() const
{
	return m_parser;
}
f1::f1pp::parser * f1::f1pp::enumeration::get_parser()
{
	return m_parser;
}

const status_t f1::f1pp::enumeration::modify_flags( unsigned flags_to_set, unsigned flags_to_clear, unsigned * prev_flags )
{
	m_flags.modify( flags_to_set, flags_to_clear, prev_flags );
	return STATUS_SUCCESS;
}
unsigned f1::f1pp::enumeration::get_flags( unsigned mask ) const
{
	return m_flags & mask;
}

const f1::ns::name_space & f1::f1pp::enumeration::get_element_namespace() const
{
	//return m_ns;
	return *this;
}
f1::ns::name_space & f1::f1pp::enumeration::get_element_namespace()
{
	//return m_ns;
	return *this;
}

const status_t f1::f1pp::enumeration::set_definition_location( f1pp::parser & parser )
{
	return set_definition_location( *parser.get_parser()->get_current_source(), parser.get_parser()->get_line() );
}
const status_t f1::f1pp::enumeration::set_definition_location( f1::parser::Isource & src, f1::parser::lineno_t line )
{
	m_definition_source = &src;
	m_definition_line = line;

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base enumerations

const status_t f1::f1pp::enumeration::append_base_enumeration( const char * name, size_t len )
{
	status_t status;
	core::Tconst_ref<enumeration> en;
	if( m_parser->find_enumeration( en, name, len ).failed_status( status ) )
		return status;
	if( 0 != en->get_flags( fl_sealed ) )
		return STATUS_ERROR_DISABLED;
	return append_base_enumeration( *en );
}
const status_t f1::f1pp::enumeration::append_base_enumeration( const pp::string & name )
{
	status_t status;
	core::Tconst_ref<enumeration> en;
	if( m_parser->find_enumeration( en, name ).failed_status( status ) )
		return status;
	return append_base_enumeration( *en );
}
const status_t f1::f1pp::enumeration::append_base_enumeration( const enumeration & en )
{
	// Check whether there is base enumeration
	if( _is_base_enumeration( en ) )
		return STATUS_ERROR_ALREADY_EXISTS;

	use_namespace( *const_cast<enumeration *>(&en) );

	// Exit
	return m_base_enums.append_unique( &en );
}

const f1::f1pp::const_enumeration_list_t & f1::f1pp::enumeration::get_base_enumerations() const
{
	return m_base_enums;
}

bool f1::f1pp::enumeration::_is_base_enumeration( const enumeration & en ) const
{
	if( m_base_enums.find( en ).valid() )
		return true;

	array_foreach( const_enumeration_list_t::const_iterator, it, m_base_enums )
	{
		if( it->_is_base_enumeration( en ) )
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Elements

const status_t f1::f1pp::enumeration::append_element( const pp::string & name, const pp::variant & value, const pp::attribute_list * attrs )
{
	status_t status;

	core::Tref<ns::name> nm;
	__debugbreak_if( /*m_ns.*/find_subname( nm, name.data(), name.length() ).succeeded() )
		return STATUS_ERROR_ALREADY_EXISTS;

	ns::context & ns_context = m_parser->get_ns_context();

	pp::Imacro::macro_flags_t macro_flags = null;
	if( m_flags & fl_deprecated )
		macro_flags |= pp::Imacro::macro_flags_t::fl_deprecated;

	pp::string str;
	value.format( str );

	core::Tref<ns::name_space> old_ns = ns_context.get_current_namespace();
	ns_context.set_current_namespace( this );//&m_ns );

	core::Tref<enum_macro> mac;
	mac.createT<enum_macro>();
	if( mac->initialize( *this, name, attrs, nullptr, nullptr, macro_flags ).failed_status( status ) )
	//if( m_parser->create_macro<enum_macro>( mac, name, attrs, nullptr, nullptr, macro_flags ).failed_status( status ) )
		return status;
	__debugbreak_if( nullptr == mac )	// TODO: set old namespace
		return STATUS_ERROR_INTERNAL;
	m_parser->append_macro( *mac );

//	create_name( nm, name, mac
	mac->set_value_variant( value, str );

	ns_context.set_current_namespace( old_ns );
	if( m_flags & fl_open )
		ns_context.get_root_namespace().use_namespace( *this );

	m_element_list.append( mac );

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::enumeration::append_element( const char * name, size_t len, const pp::variant & value, const pp::attribute_list * attrs )
{
	return append_element( pp::string( name, len ), value, attrs );
}
const status_t f1::f1pp::enumeration::remove_element( pp::Imacro & macro )
{
	m_element_list.remove( &macro );

	core::Tref<ns::name> nm;
	find_name( nm, macro.get_name() );
	if( nullptr == nm )
		return STATUS_SUCCESS;

	core::Tref<ns::named_object> nobj = nm->get_object();
	if( nullptr == nobj )
		return remove_subname( *nm );

	nobj->remove_all_names();

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::enumeration::remove_all_elements()
{
	m_element_list.remove_all();
	remove_all_subnames();

	return STATUS_SUCCESS;
}

const f1::pp::Imacro * f1::f1pp::enumeration::get_element( const char * name, size_t len ) const
{
	core::Tref<ns::name> nm;
	if( /*m_ns.*/find_subname( nm, name, len ).failed() )
		return nullptr;
	__debugbreak_if( nullptr == nm )
		return nullptr;

	core::Tconst_ref<pp::Imacro> mac;
	if( m_parser->get_macro_from_name( mac, *nm ).failed() )
		return nullptr;

	return mac;
}
f1::pp::Imacro * f1::f1pp::enumeration::get_element( const char * name, size_t len )
{
	return const_cast<pp::Imacro *>( const_cast<const self *>(this)->get_element( name, len ) );
}
const f1::pp::Imacro * f1::f1pp::enumeration::get_element( const pp::variant & value ) const
{
	array_foreach( pp::macro_list_t::const_iterator, it, m_element_list )
	{
		if( (*it)->get_value_variant() == value )
			return *it;
	}
	return nullptr;
}
f1::pp::Imacro * f1::f1pp::enumeration::get_element( const pp::variant & value )
{
	return const_cast<pp::Imacro *>( const_cast<const self *>(this)->get_element( value ) );
}

const f1::pp::macro_list_t & f1::f1pp::enumeration::get_element_list() const
{
	return m_element_list;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

const status_t f1::f1pp::enumeration::display_enumeration_location( pp::Iparser & par, msgout::msgid_t msgid ) const
{
	if( nullptr != m_definition_source )
		par.message( m_definition_source->get_filename(), m_definition_line, msgid );
	else
		par.message( c_pp::syntax::IMSG_INFO_DEFINED_IN_COMMAND_LINE );
	return STATUS_SUCCESS;
}

bool f1::f1pp::enumeration::get_next_value( f1pp::parser & par, pp::variant & value, status_t & status ) const
{
	if( get_element_list().empty() )
	{
		if( get_base_enumerations().empty() )
		{
			value.set_int( 0 );
			return true;
		}

		status = STATUS_SUCCESS;
		array_foreach( const_enumeration_list_t::const_reverse_iterator, it, get_base_enumerations() )
		{
			if( (*it)->get_next_value( par, value, status ) )
				return true;
			if( status.failed() )
				break;
		}
		par.message( f1pp::syntax::IMSG_ERROR_ENUM_REQUIRES_INITIALIZER );
		status = STATUS_ERROR_INVALID_FORMAT;
	}
	else
	{
		const pp::variant & last_value = (*get_element_list().rbegin())->get_value_variant();
		switch( last_value.get_type() )
		{
		case pp::variant::ty_int:{
			intmax_t val = last_value.as_int();
			if( get_flags( enumeration::fl_bitwise ) )
				val = 0 == val ? 1 : val << 1;
			else
				val ++;
			status = value.set_int( val );
			}break;
		case pp::variant::ty_uint:{
			uintmax_t val = last_value.as_uint();
			if( get_flags( enumeration::fl_bitwise ) )
				val = 0 == val ? 1 : val << 1;
			else
				val ++;
			status = value.set_uint( val );
			}break;
		case pp::variant::ty_float:{
			max_float_t val = last_value.as_float();
			if( get_flags( enumeration::fl_bitwise ) )
				val = 0 == val ? 1 : val * 2;
			else
				val += 1;
			status = value.set_float( val );
			}break;
		default:
			par.message( f1pp::syntax::IMSG_ERROR_ENUM_REQUIRES_INITIALIZER );
			status = STATUS_ERROR_INVALID_FORMAT;
			break;
		}
	}
	return status.succeeded();
}

/*END OF enumeration.cxx*/

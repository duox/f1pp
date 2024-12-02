/*foreach_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::foreach_directive::foreach_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::foreach_directive::get_name() const
{
	return "foreach";
}
// {3E6C4780-C813-465A-B8F7-3B5139964EFE}
const f1::guid_t f1::f1pp::foreach_directive::m_guid =
{ 0x3e6c4780, 0xc813, 0x465a, { 0xb8, 0xf7, 0x3b, 0x51, 0x39, 0x96, 0x4e, 0xfe } };
const f1::guid_t & f1::f1pp::foreach_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::foreach_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_hard_edge | cap_invariant | cap_keep_text_eol | cap_structured) & mask;
}

// Operations
const status_t f1::f1pp::foreach_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;
	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Check whether identifier is specified
	// Macro specified in such a way belongs to the upper namespace in order to be examined
	// after the loop finishes
	f1pp_par->skip_real_spaces_and_comments();
	par->push_pos();

	core::Tref<pp::Imacro> mac = nullptr;
	pp::string ident;
	f1pp_par->parse_macro_identifier( ident );
	if( !ident.is_empty() )
	{
		f1pp_par->skip_real_spaces_and_comments();
		if( par->skip_char( ':' ) )
		{
			if( parser.find_macro( mac, ident, pp::Iparser::find_macro_flags_t::fl_macro_search_current_namespace_only ).failed() &&
				parser.create_macro( mac, ident ).failed_status( status ) )
			{
				parser.message( syntax::IMSG_ERROR_CANNOT_DEFINE_MACRO_S, ident.c_str() );
			}
			else
				mac->set_value_variant( pp::variant( intmax_t(0) ) );
		}
	}

	if( nullptr == mac )
		par->pop_pos();
	else
		par->pop_discard_pos();

	// Create execution context and enter new nested block
	frame * frm = new(std::nothrow) frame( *f1pp_par );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	frm->m_control_macro = mac;
	frm->m_backward = f1pp_par->exists_directive_attribute( "backward" );
	frm->m_raw = f1pp_par->exists_directive_attribute( "raw" );
	parser.enter_directive( *frm );

	if( frm->m_backward )
		frm->m_reverse_iterator = new(frm->m_iterator_area) pp::variant::const_reverse_iterator;
	else
		frm->m_forward_iterator = new(frm->m_iterator_area) pp::variant::const_iterator;

	// Check for disabled statement (fast path)
	if( frm->get_flags( pp::Iframe::fl_initially_disabled ) )
	{
		f1pp_par->skip_to_newline();
		return STATUS_SUCCESS;
	}

	// Fetch expression string and expand macros
	f1pp_par->modify_parser_flags( pp::Iparser::parser_flags_t::fl_expression_mode );
	f1pp_par->capture_line( line_capture_callback, this );

	// Exit
	return STATUS_SUCCESS;
}
f1::pp::Iframe * f1::f1pp::foreach_directive::alloc_idle_frame( f1::pp::Iparser & parser )
{
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return nullptr;
	return new(std::nothrow) frame( *f1pp_par );
}
const status_t f1::f1pp::foreach_directive::execute( f1::pp::Iparser & parser, const core::buffer & out_buf )
{
	status_t status;

	// Check current state
	core::Tref<f1::f1pp::parser> f1pp_par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1pp_par )
		return STATUS_ERROR_INVALID_TYPE;

	foreach_directive::frame * frm = type_cast<foreach_directive::frame *>( parser.get_directive_frame() );
	__debugbreak_if( nullptr == frm )
		return STATUS_ERROR_INTERNAL;
	__debugbreak_if( frm->get_openning_directive_guid() != foreach_directive::m_guid )
		return STATUS_ERROR_INTERNAL;

	// Parse buffer
	if( 0 == out_buf.get_data_size() )
	{
		parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_EXPRESSION );
		return STATUS_SUCCESS;
	}
	frm->m_buf = out_buf;
	const char *	 start = (const char *) frm->m_buf.get_ptr();
	const char * const end = (const char *) frm->m_buf.get_end_ptr();

	while( start < end )
	{
		f1pp_par->skip_real_spaces_and_comments( start, end );

		const char * elem_start = start;
		f1pp_par->skip_to_separator( start, end, ',' );

		if( !frm->m_raw )
		{
			pp::variant value;
			status = f1pp_par->parse_value( value, elem_start, start, parser::fl_parse_value_ignore_unknown_idents );
			if( status.failed() )
			{
				//parser.message( syntax::IMSG_ERROR_INVALID_VALUE );	- an error is already printed
				break;	// print only one error of this type
			}
			else if( elem_start != start )
			{
				pp::string ident;
				if( !f1pp_par->parse_macro_identifier( ident, elem_start, start ) )
				{
					parser.message( syntax::IMSG_ERROR_INVALID_VALUE );
					break;	// print only one error of this type
				}
				else
				{
					// Try to use enumeration
					core::Tref<f1pp::enumeration> en = nullptr;
					f1pp_par->find_enumeration( en, ident );
					if( nullptr != en )
					{
						core::Tarray<pp::variant> lst;
						array_foreach( pp::macro_list_t::const_iterator, it, en->get_element_list() )
						{
							core::Tconst_ref<pp::Imacro> mac = *it;
							lst.append( mac->get_value_variant() );
						}
						if( !lst.empty() )
						{
							pp::variant * var = frm->m_values.append_new();
							var->set_list( lst.data(), lst.size() );	// TODO: move
						}
					}
					else
					{
						// Try to use namespace
						core::Tref<ns::name> nm = nullptr;
						f1pp_par->get_ns_context().find_name( nm, ident.data(), ident.length() );
						if( nullptr != nm )
						{
							core::Tref<ns::name_space> nmp = type_cast<ns::name_space *>(nm->get_object());
							if( nullptr != nmp )
							{
								core::Tarray<pp::variant> lst;
								array_foreach( ns::name_list_t::const_iterator, it, nmp->get_subname_list() )
								{
									nm = *it;
									__debugbreak_if( nullptr == nm )
										continue;
									core::Tref<pp::Iparser::macro_object> mobj = type_cast<pp::Iparser::macro_object *>(nm->get_object());
									if( nullptr == mobj )
										continue;

									// Try to find macro w/o parameters; if failed, take the first one
									core::Tconst_ref<pp::Imacro> best_mac = nullptr;
									array_foreach( pp::const_macro_list_t::const_iterator, itmobj, mobj->m_macro_list )
									{
										core::Tconst_ref<pp::Imacro> mac = *itmobj;
										if( nullptr != mac )
										{
											if( nullptr == mac->get_param_list() )
											{
												__assert( nullptr == best_mac || nullptr != best_mac->get_param_list() );
												best_mac = mac;
											}
											else
											{
                                                if( nullptr == best_mac )
													best_mac = mac;
											}
										}
									}
									if( nullptr != best_mac )
										lst.append( best_mac->get_value_variant() );
								}
								if( !lst.empty() )
								{
									pp::variant * var = frm->m_values.append_new();
									var->set_list( lst.data(), lst.size() );	// TODO: move
								}
							}
						}
						else
						{
							parser.message( syntax::IMSG_ERROR_INVALID_VALUE );
							break;	// print only one error of this type
						}
					}
				}
			}
			else
				frm->m_values.append_move( value );
		}
		else
		{
			pp::string str( elem_start, start - elem_start );
			f1pp_par->trim_spaces_and_comments( str );
			frm->m_elements.append_move( str );
		}

		if( start < end && ',' == *start )
			++ start;
		else
			break;
	}

	parser.get_parser()->get_pos( frm->m_position );
	frm->m_count = frm->m_raw ? frm->m_elements.size() : frm->m_values.size();

	if( status.failed() )
	{
		frm->allow_exec( false );
		frm->modify_flags( pp::Iframe::fl_abort_statement );
		return STATUS_SUCCESS;
	}

	// Update execution context and enter new nested block
	if( 0 != frm->m_count )
	{
		if( frm->m_backward )
		{
			frm->m_index = frm->m_count - 1;
			if( !frm->m_raw )
				frm->m_reverse_iterator->set_variant( frm->m_values[frm->m_index] );
		}
		else
		{
			frm->m_index = 0;
			if( !frm->m_raw )
				frm->m_forward_iterator->set_variant( frm->m_values[frm->m_index] );
		}

		frm->allow_exec( true );
	}
	frm->_update( *f1pp_par );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::foreach_directive::line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf )
{
	parser.modify_parser_flags( pp::Iparser::parser_flags_t::null, pp::Iparser::parser_flags_t::fl_expression_mode );
#ifdef _DEBUG
	core::Tref<f1::f1pp::foreach_directive> drc = type_cast<f1::f1pp::foreach_directive *>( param );
	__debugbreak_if( nullptr == drc )
		return STATUS_ERROR_INTERNAL;
	return drc->execute( parser, buf );
#else
	return static_cast<f1::f1pp::foreach_directive *>( param )->execute( parser, buf );
#endif
}

f1::f1pp::foreach_directive::frame::frame( f1pp::parser & par ):
	super( par ),
	m_index( 0 ), m_iteration_count( 0 ), m_backward( false )
{
}
const char * f1::f1pp::foreach_directive::frame::get_openning_directive_name() const
{
	return "foreach";
}
const char * f1::f1pp::foreach_directive::frame::get_closing_directive_name() const
{
	return "endforeach";
}
const f1::guid_t & f1::f1pp::foreach_directive::frame::get_openning_directive_guid() const
{
	return foreach_directive::m_guid;
}
const f1::guid_t & f1::f1pp::foreach_directive::frame::get_closing_directive_guid() const
{
	return endforeach_directive::m_guid;
}
const status_t f1::f1pp::foreach_directive::frame::_update( f1::f1pp::parser & f1par )
{
	// Process the list
	if( !m_backward )
	{
		// Check for exit condition
		if( m_index >= m_count )
		{
			allow_exec( false );
			modify_flags( pp::Iframe::fl_abort_statement );
			return STATUS_SUCCESS;
		}

		// Store value
		if( m_raw )
		{
			m_control_macro->set_value_move( m_elements[m_index] );
			++ m_index;
		}
		else
		{
			pp::variant val;
			if( !get_next_item( val ) )
			{
				allow_exec( false );
				modify_flags( pp::Iframe::fl_abort_statement );
				return STATUS_SUCCESS;
			}
			m_control_macro->set_value_variant_move( val );
		}
	}
	else
	{
		// Check for exit condition
		if( ssize_t(m_index) < 0 )
		{
			allow_exec( false );
			modify_flags( pp::Iframe::fl_abort_statement );
			return STATUS_SUCCESS;
		}

		// Store value
		if( m_raw )
		{
			m_control_macro->set_value_move( m_elements[m_index] );
			-- m_index;
		}
		else
		{
			pp::variant val;
			if( !get_next_item( val ) )
			{
				allow_exec( false );
				modify_flags( pp::Iframe::fl_abort_statement );
				return STATUS_SUCCESS;
			}
			m_control_macro->set_value_variant_move( val );
		}
	}

	// Exit
	return STATUS_SUCCESS;
}
bool f1::f1pp::foreach_directive::frame::get_next_item( pp::variant & value )
{
	if( m_backward )
	{
		while( !m_reverse_iterator->next() )
		{
			if( 0 == m_index )
				return false;
			-- m_index;
			m_reverse_iterator->set_variant( m_values[m_index] );
		}
		return m_reverse_iterator->value( value );
	}
	else
	{
		while( !m_forward_iterator->next() )
		{
			++ m_index;
			if( m_index >= m_count )
				return false;
			m_forward_iterator->set_variant( m_values[m_index] );
		}
		return m_forward_iterator->value( value );
	}
}

/*END OF foreach_directive.cxx*/

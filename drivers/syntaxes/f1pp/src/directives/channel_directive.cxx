/*channel_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::channel_directive::channel_directive( unsigned flags ):
	super( flags ),
	m_additional_caps( 0 )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties

const char * f1::f1pp::channel_directive::get_name() const
{
	return "channel";
}

// {956D09F3-4CB1-445b-8E16-ADF2B04C75A5}
const f1::guid_t f1::f1pp::channel_directive::m_guid =
{ 0x956d09f3, 0x4cb1, 0x445b, { 0x8e, 0x16, 0xad, 0xf2, 0xb0, 0x4c, 0x75, 0xa5 } };
const f1::guid_t & f1::f1pp::channel_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::channel_directive::get_caps( unsigned mask ) const __noexcept
{
	__unreferenced_parameter( mask );
	return (cap_c_extension | m_additional_caps) & mask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Operations

f1::f1pp::channel_directive::channel_subcommand f1::f1pp::channel_directive::m_channel_subcommands[] =
{
	{ &f1::f1pp::channel_directive::cm_channel_activate,STR_LEN_PAIR( "" ),			f1::f1pp::channel_directive::cmd_f_requires_id },
	{ &f1::f1pp::channel_directive::cm_channel_close,	STR_LEN_PAIR( "close" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_define,	STR_LEN_PAIR( "define" ),	f1::f1pp::channel_directive::cmd_f_requires_id | f1::f1pp::channel_directive::cmd_f_chan_name_only },
	{ &f1::f1pp::channel_directive::cm_channel_destroy,	STR_LEN_PAIR( "destroy" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_flush,	STR_LEN_PAIR( "flush" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_footer,	STR_LEN_PAIR( "footer" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_header,	STR_LEN_PAIR( "header" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_include,	STR_LEN_PAIR( "include" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_pop,		STR_LEN_PAIR( "pop" ),		0 },
	{ &f1::f1pp::channel_directive::cm_channel_prefix,	STR_LEN_PAIR( "prefix" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_push,	STR_LEN_PAIR( "push" ),		0 },
	{ &f1::f1pp::channel_directive::cm_channel_suffix,	STR_LEN_PAIR( "suffix" ),	f1::f1pp::channel_directive::cmd_f_accepts_id },
	{ &f1::f1pp::channel_directive::cm_channel_write,	STR_LEN_PAIR( "write" ),	f1::f1pp::channel_directive::cmd_f_requires_id },
};

const status_t f1::f1pp::channel_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1pp::parser> f1par = type_cast<f1pp::parser *>(&parser);
	__debugbreak_if( nullptr == f1par )
		return STATUS_ERROR_NOT_INITIALIZED;

	core::Tref<f1::parser::Iparser> par = parser.get_parser();
	__debugbreak_if( nullptr == par )
		return STATUS_ERROR_NOT_INITIALIZED;

	m_additional_caps = 0;

	// Fetch sub-command
	channel_subcommand * cmd = nullptr;
	f1par->skip_real_spaces_and_comments();
	if( par->skip_char( '.' ) )
	{
		f1par->skip_real_spaces_and_comments();

		pp::string cmd_ident;
		f1par->parse_macro_identifier( cmd_ident );
		if( cmd_ident.is_empty() )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_EXPECTED_IDENTIFIER );
			par->skip_to_nl();
			return STATUS_SUCCESS;
		}

		for( size_t i = 0; i < _countof(m_channel_subcommands); ++ i )
		{
			if( cmd_ident.length() != m_channel_subcommands[i].m_length )
				continue;
			if( cmd_ident.equal( m_channel_subcommands[i].m_name ) )
			{
				cmd = &m_channel_subcommands[i];
				break;
			}
		}
		if( nullptr == cmd )
		{
			parser.message( c_pp::syntax::IMSG_ERROR_UNKNOWN_DIRECTIVE, cmd_ident.c_str() );
			par->skip_to_nl();
			return STATUS_SUCCESS;
		}
	}
	else
		cmd = &m_channel_subcommands[0];	// cm_channel_activate is always first
	f1par->skip_real_spaces_and_comments();

	// Parse channel identifier
	pp::string channel_name;
	core::Tref<channel> chan;
	if( cmd->m_flags & cmd_f_accepts_id )
	{
		f1par->parse_macro_identifier( channel_name );
		if( channel_name.is_empty() )
		{
			if( cmd_f_requires_id == (cmd->m_flags & cmd_f_requires_id) )
			{
				parser.message( f1pp::syntax::IMSG_ERROR_EXPECTED_CHANNEL_NAME );
				par->skip_to_nl();
				return STATUS_SUCCESS;
			}
			chan = nullptr;
		}
		else
		{
			f1par->find_channel( chan, channel_name );
			if( nullptr == chan && !(cmd->m_flags & cmd_f_chan_name_only) )
			{
				parser.message( f1pp::syntax::IMSG_ERROR_CHANNEL_NOT_FOUND_S, channel_name.c_str() );
				par->skip_to_nl();
				return STATUS_SUCCESS;
			}
		}
	}
	f1par->skip_real_spaces_and_comments();

	// Execute the command and exit
	return (this->*cmd->parse)( *f1par, chan, channel_name );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const status_t f1::f1pp::channel_directive::cm_channel_activate( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	__debugbreak_if( nullptr == chan )
		return STATUS_ERROR_INTERNAL;

	// Activate specified channel
	f1par.set_active_channel( chan );

	// Exit
	__unreferenced_parameter( channel_name );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_close( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr != chan && chan != f1par.get_active_channel() )
		return STATUS_SUCCESS;
		//return f1par.message( f1pp::syntax::IMSG_ERROR_CANNOT_DELETE_ACTIVE_CHANNEL );

	// Delete specified channel
	f1par.set_active_channel( nullptr );

	// Exit
	__unreferenced_parameter( channel_name );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_define( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Parse file name
	pp::string filename;
	f1par.parse_string( filename );
	if( filename.empty() )
	{
		f1par.message( f1pp::syntax::IMSG_ERROR_EXPECTED_CHANNEL_FILENAME );
		return STATUS_SUCCESS;
	}

	// Check the channel hasn't yet been defined
	if( nullptr != chan )
	{
		f1par.message( f1pp::syntax::IMSG_ERROR_CHANNEL_ALREADY_DEFINED_S, channel_name.c_str() );
		chan->display_channel_location( f1par );
		return STATUS_SUCCESS;
	}

	// Create channel
	core::Tref<channel> new_chan;
	f1par.create_channel( new_chan, channel_name, filename );
	__assert( nullptr != new_chan );

	// Exit
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_destroy( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( chan == f1par.get_active_channel() )
		f1par.set_active_channel( nullptr );
		//return f1par.message( f1pp::syntax::IMSG_ERROR_CANNOT_DELETE_ACTIVE_CHANNEL );

	// Delete specified channel
	f1par.remove_channel( *chan );

	// Exit
	__unreferenced_parameter( channel_name );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_flush( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr == chan )
	{
		chan = f1par.get_active_channel();
		if( nullptr == chan )
			return f1par.message( f1pp::syntax::IMSG_WARNING_NO_CHANNEL_ACTIVE );
	}

	// Flush the channel
	chan->flush();

	// Exit
	__unreferenced_parameter( channel_name );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_footer( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr == chan )
	{
		chan = f1par.get_active_channel();
		if( nullptr == chan )
			return f1par.message( f1pp::syntax::IMSG_WARNING_NO_CHANNEL_ACTIVE );
	}

	// Parse channel footer
	pp::string buf;
	f1par.parse_to_newline( buf );

	// Exit
	__unreferenced_parameter( channel_name );
	return chan->set_footer_move( buf );
}
const status_t f1::f1pp::channel_directive::cm_channel_header( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr == chan )
	{
		chan = f1par.get_active_channel();
		if( nullptr == chan )
			return f1par.message( f1pp::syntax::IMSG_WARNING_NO_CHANNEL_ACTIVE );
	}

	// Parse channel header
	pp::string buf;
	f1par.parse_to_newline( buf );

	// Exit
	__unreferenced_parameter( channel_name );
	return chan->set_header_move( buf );
}
const status_t f1::f1pp::channel_directive::cm_channel_include( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	__debugbreak_if( nullptr == chan )
		return STATUS_ERROR_INTERNAL;

	// Include channel storage file
	f1par.include_file( null, chan->get_storage_name() );
	m_additional_caps |= cap_keep_text_eol;

	// Exit
	__unreferenced_parameter( channel_name );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_pop( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Restore active channel
	if( f1par.pop_active_channel().failed() )
		f1par.message( f1pp::syntax::IMSG_ERROR_CHANNEL_STACK_EMPTY );

	// Exit
	__unreferenced_parameter( chan );
	__unreferenced_parameter( channel_name );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_prefix( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr == chan )
	{
		chan = f1par.get_active_channel();
		if( nullptr == chan )
			return f1par.message( f1pp::syntax::IMSG_WARNING_NO_CHANNEL_ACTIVE );
	}

	// Parse channel prefix
	pp::string buf;
	f1par.parse_to_newline( buf );

	// Exit
	__unreferenced_parameter( channel_name );
	return chan->set_prefix_move( buf );
}
const status_t f1::f1pp::channel_directive::cm_channel_push( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr == f1par.get_active_channel() )
		return f1par.message( f1pp::syntax::IMSG_WARNING_NO_CHANNEL_ACTIVE );

	// Store channel in the channels stack
	f1par.push_active_channel();

	// Exit
	__unreferenced_parameter( chan );
	__unreferenced_parameter( channel_name );
	return STATUS_SUCCESS;
}
const status_t f1::f1pp::channel_directive::cm_channel_suffix( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr == chan )
	{
		chan = f1par.get_active_channel();
		if( nullptr == chan )
			return f1par.message( f1pp::syntax::IMSG_WARNING_NO_CHANNEL_ACTIVE );
	}

	// Parse channel suffix
	pp::string buf;
	f1par.parse_to_newline( buf );

	// Exit
	__unreferenced_parameter( channel_name );
	return chan->set_suffix_move( buf );
}
const status_t f1::f1pp::channel_directive::cm_channel_write( f1pp::parser & f1par, channel * chan, const pp::string & channel_name )
{
	// Check current state
	if( nullptr == chan )
	{
		chan = f1par.get_active_channel();
		if( nullptr == chan )
			return f1par.message( f1pp::syntax::IMSG_WARNING_NO_CHANNEL_ACTIVE );
	}

	// Parse channel prefix
	pp::string buf;
	f1par.parse_to_newline( buf );
	if( buf.empty() )
		return STATUS_SUCCESS;

	// Write data
	status_t status = chan->write( buf );
	chan->write( pp::string(f1par.get_context()->get_current_newline_type_seq()) );

	// Exit
	__unreferenced_parameter( channel_name );
	return status;
}

/*END OF channel_directive.cxx*/

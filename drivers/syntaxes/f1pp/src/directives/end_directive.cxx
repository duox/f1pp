/*end_directive.cxx*/
#include "../pch.h"
#include "../main.h"

f1::f1pp::end_directive::end_directive( unsigned flags ):
	super( flags )
{
}

// Properties
const char * f1::f1pp::end_directive::get_name() const
{
	return "end";
}
// {606CEE05-3412-43f9-BAD8-C8422F6415EB}
const f1::guid_t f1::f1pp::end_directive::m_guid =
{ 0x606cee05, 0x3412, 0x43f9, { 0xba, 0xd8, 0xc8, 0x42, 0x2f, 0x64, 0x15, 0xeb } };
const f1::guid_t & f1::f1pp::end_directive::get_guid() const
{
	return m_guid;
}

unsigned f1::f1pp::end_directive::get_caps( unsigned mask ) const __noexcept
{
	return (cap_invariant | cap_keep_text_eol | cap_c_extension | cap_forced) & mask;
}

// Operations
const status_t f1::f1pp::end_directive::parse( pp::Iparser & parser )
{
	status_t status;

	// Check current state
	core::Tref<f1::c_pp::parser> c_pp_par = type_cast<c_pp::parser *>(&parser);
	__debugbreak_if( nullptr == c_pp_par )
		return STATUS_ERROR_NOT_INITIALIZED;

	// Get ending tag, if it's present
	pp::string tag;
	c_pp_par->skip_real_spaces_and_comments();
	c_pp_par->parse_directive_identifier( tag );

	// Get latest compound statement
	f1::pp::Iframe * frm = parser.get_directive_frame();
	if( nullptr == frm )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#end" );

	core::Tref<Idirective> drc;
	if( c_pp_par->find_directive( drc, frm->get_closing_directive_guid() ).failed_status( status ) || nullptr == drc )
		return parser.message( syntax::IMSG_ERROR_UNEXPECTED_S, "#end" );

	if( !tag.empty() && !tag.equal( frm->get_openning_directive_name() ) )
	{
		if( 0 == frm->get_warning_flags( c_pp::Iframe::fl_warned_end_tag_mistach ) )
		{
			frm->set_warning_flags( c_pp::Iframe::fl_warned_end_tag_mistach );
			parser.message( syntax::IMSG_WARNING_DIRECTIVE_TAG_MISMATCH, tag.c_str(), frm->get_openning_directive_name() );
		}
	}

	// Check if ab idle frame received
	if( c_pp_par->get_c_flags( parser::c_flags_t::fl_execute_exec_top_only ) && frm->get_flags( pp::Iframe::fl_idle ) )
	{
		c_pp_par->leave_directive();
		c_pp_par->output( pp::string( c_pp_par->get_directive_start(), c_pp_par->get_parser()->get_ptr() - c_pp_par->get_directive_start() ) );
		return STATUS_SUCCESS;
	}

	// Exit
	return drc->parse( parser );	// redirect call to the specific directive
}

/*END OF end_directive.cxx*/

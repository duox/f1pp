/*switch.hxx*/
#ifndef __f1_f1pp_switch_directive_25E05B6D0ADA4EC0A3057BBBC597AE64__
#define __f1_f1pp_switch_directive_25E05B6D0ADA4EC0A3057BBBC597AE64__
#pragma once

class F1_SYNTAX_F1PP_API f1::f1pp::switch_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective		super;
	typedef f1::f1pp::switch_directive	self;

	explicit switch_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
protected:
	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
public:
	struct frame : public f1::f1pp::Iscoped_frame {
		typedef f1::f1pp::Iscoped_frame	super;
		typedef frame							self;

		explicit frame( f1pp::parser & par );
				~frame();

		pp::variant	m_switch_value;			// value, calculated by the #switch directive
		f1::parser::Iparser::position m_case_position;			// location of the correct #case clause
		f1::parser::Iparser::position m_default_position;		// location of the #default clause
		f1::parser::Iparser::position m_fallthrough_position;	// location of the #fallthrough after #case of #default
		f1::parser::Iparser::position m_endswitch_position;		// location of #endswitch if it was already parsed

		enum Etype {
			Etype_switch,
			Etype_switchdef,
			Etype_switchndef,
		};
		Etype		m_switch_type;

		virtual const char *	get_openning_directive_name() const override;
		virtual const char *	get_closing_directive_name() const override;
		virtual const f1::guid_t & get_openning_directive_guid() const override;
		virtual const f1::guid_t & get_closing_directive_guid() const override;
	};
	virtual pp::Iframe *	alloc_idle_frame( f1::pp::Iparser & parser ) override;
};

#endif // ndef __f1_f1pp_switch_directive_25E05B6D0ADA4EC0A3057BBBC597AE64__
/*END OF switch.hxx*/

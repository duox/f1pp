/*Iassignment.hxx*/

interface F1_SYNTAX_F1PP_API f1::f1pp::Iassignment_directive : public f1::pp::Idirective
{
public:
	typedef f1::pp::Idirective					super;
	typedef f1::f1pp::Iassignment_directive		self;

	explicit Iassignment_directive( unsigned flags = 0 );

	// Properties
public:
	const status_t	set_finish_callback( f1::parser::Iparser::Ibreakpoint_callback * finish_callback );
	const status_t	call_finish_callback();

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;
protected:
	f1::parser::Iparser::Ibreakpoint_callback *	m_finish_callback;

	// Operations
public:
	virtual const status_t	parse( pp::Iparser & parser ) override;
	virtual const status_t	parse( pp::Iparser & parser, int end_ch );
protected:
	virtual const status_t	execute_subcommand( f1::f1pp::parser & f1par, f1::parser::Iparser & par, const pp::string & ident );
	virtual const status_t	execute_unknown_subcommand( f1::f1pp::parser & f1par, f1::parser::Iparser & par, const pp::string & ident );
	const status_t			execute( f1::pp::Iparser & parser, const core::buffer & out_buf );
	static const status_t	line_capture_callback( core::object * param, f1::pp::Iparser & parser, const core::buffer & buf );
public:
	// Unary assignments
	interface Iunary_assignment {
		virtual ~Iunary_assignment() noexcept {}
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & val ) const abstract;
	};
	struct unary_assignment_operator {
		const char *	m_string;
		size_t			m_length;
		Iunary_assignment *	m_assignment;
	};
	const Iunary_assignment *	parse_unary_assignment( f1::parser::Iparser & par ) const;
	static const unary_assignment_operator m_intrinsic_unary_assignment_operators[];
	static class increment_unary_assignment : public Iunary_assignment {	// ++a or a++
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & val ) const override;
	} m_increment_unary_assignment;
	static class decrement_unary_assignment : public Iunary_assignment {	// --a or a--
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & val ) const override;
	} m_decrement_unary_assignment;

	// Binary assignments
	interface Iassignment {
		virtual ~Iassignment() noexcept {}
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const abstract;
	};
	struct assignment_operator {
		const char *	m_string;
		size_t			m_length;
		Iassignment *	m_assignment;
	};
	const Iassignment *	parse_assignment( f1::parser::Iparser & par ) const;
	static const assignment_operator m_intrinsic_assignment_operators[];

	static class simple_assignment : public Iassignment {	// a = b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_simple_assignment;
	static class addition_assignment : public Iassignment {	// a += b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_addition_assignment;
	static class subtraction_assignment : public Iassignment {	// a -= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_subtraction_assignment;
	static class multiplication_assignment : public Iassignment {	// a *= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_multiplication_assignment;
	static class division_assignment : public Iassignment {	// a /= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_division_assignment;
	static class modulo_assignment : public Iassignment {	// a %= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_modulo_assignment;
	static class land_assignment : public Iassignment {	// a &&= b (C/C++ extension)
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_land_assignment;
	static class lor_assignment : public Iassignment {	// a ||= b (C/C++ extension)
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_lor_assignment;
	static class lxor_assignment : public Iassignment {	// a ^^= b (C/C++ double extension, a = a != b)
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_lxor_assignment;
	static class bitand_assignment : public Iassignment {	// a &= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_bitand_assignment;
	static class bitor_assignment : public Iassignment {	// a |= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_bitor_assignment;
	static class bitxor_assignment : public Iassignment {	// a ^= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_bitxor_assignment;
	static class lshift_assignment : public Iassignment {	// a <<= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_lshift_assignment;
	static class rshift_assignment : public Iassignment {	// a >>= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_rshift_assignment;
	static class conditional_assignment : public Iassignment {	// a >>= b
		virtual const status_t	evaluate( pp::Iparser & par, pp::variant & dst, const pp::variant & src ) const override;
	} m_conditional_assignment;

	core::Tconst_thin_ptr<Iassignment>			m_assignment;
	core::Tconst_thin_ptr<Iunary_assignment>	m_unary_assignment;	// postfix unary statement
	f1::pp::variant::type_t m_type_id;
	bool	m_can_modify_target;

	// Target operation
protected:
	virtual const status_t	parse_target_name( f1pp::parser & parser, pp::string & buf ) abstract;
	virtual const status_t	create_target( f1pp::parser & parser, const pp::string & ident, const pp::variant & value ) abstract;
	virtual bool			exists_target( f1pp::parser & parser, const pp::string & ident ) abstract;
	virtual bool			target_created() const abstract;
	virtual const status_t	operator_complete( f1pp::parser & parser );

	virtual bool			can_modify_target() const abstract;
	virtual const status_t	get_target_value( 		pp::variant & var, pp::variant::type_t ty ) abstract;
	virtual const status_t	set_target_value( const pp::variant & var ) abstract;

	virtual const status_t	push_all_items( f1pp::parser & parser ) abstract;
	virtual const status_t	push_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list ) abstract;
	virtual const status_t	pop_items( f1pp::parser & parser ) abstract;
	virtual const status_t	delete_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list ) abstract;
};

/*END OF Iassignment.hxx*/

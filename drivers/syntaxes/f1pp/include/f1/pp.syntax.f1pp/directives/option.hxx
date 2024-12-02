/*option.hxx*/

class F1_SYNTAX_F1PP_API f1::f1pp::option_directive : public f1::f1pp::Iassignment_directive
{
public:
	typedef f1::f1pp::Iassignment_directive		super;
	typedef f1::f1pp::option_directive			self;

	explicit option_directive( unsigned flags = 0 );

	// Properties
public:
	virtual const char *	get_name() const override;
	static const f1::guid_t		m_guid;
	virtual const f1::guid_t &	get_guid() const override;

	virtual unsigned		get_caps( unsigned mask = unsigned(-1) ) const __noexcept override;
protected:

	// Target overrides
public:
	virtual const status_t	parse_target_name( f1pp::parser & parser, pp::string & buf ) override;
	virtual const status_t	create_target( f1pp::parser & parser, const pp::string & ident, const pp::variant & value ) override;
	virtual bool			exists_target( f1pp::parser & parser, const pp::string & ident ) override;
	virtual bool			target_created() const override;
	virtual const status_t	operator_complete( f1pp::parser & parser ) override;

	virtual bool			can_modify_target() const override;
	virtual const status_t	get_target_value( 		pp::variant & var, pp::variant::type_t ty ) override;
	virtual const status_t	set_target_value( const pp::variant & var ) override;

	virtual const status_t	push_all_items( f1pp::parser & parser ) override;
	virtual const status_t	push_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list ) override;
	virtual const status_t	pop_items( f1pp::parser & parser ) override;
	virtual const status_t	delete_items( f1pp::parser & parser, const core::Tarray<pp::string> & ident_list ) override;
private:
	core::Tref<pp::Ioption>	m_option;
};

/*END OF set.hxx*/

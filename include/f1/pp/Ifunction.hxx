/*Ifunction.hxx*/

interface F1_PP_API f1::pp::Ifunction : public f1::pp::object
{
	typedef f1::pp::object			super;
	typedef f1::pp::Ifunction		self;

	// Properties
	virtual const char *	get_name( size_t * len = nullptr ) const abstract;

	// Operations
	virtual const status_t	parse( pp::Iparser & parser, pp::variant & result ) const abstract;
};

/*END OF Ifunction.hxx*/

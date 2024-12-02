/*output.hxx*/

class F1_PP_OUTPUT_NULLOUT_API f1::pp::output::nullout::output : public f1::pp::Ioutput
{
public:
	typedef f1::pp::Ioutput						super;
	typedef f1::pp::output::nullout::output		self;

	explicit output( const core::Iidentifier * ident = nullptr ) noexcept;

	//virtual const status_t	set_output_filename( const char * filename, size_t length = size_t(-1) ) override;
	virtual const status_t	write( const char * string, size_t len = size_t(-1) ) override;

	virtual const status_t	begin_output() noexcept override;
	virtual const status_t	end_output() noexcept override;
protected:
};

/*END OF output.hxx*/

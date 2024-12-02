/*lib_application_sink.h*/

class message_sink : public lib_message_sink
{
public:
	void print( context & sender, pcsz_t file, long line, lib_message_id message_id, va_list ap ) override;
	void print( pcsz_t file, long line, lib_message_id message_id, va_list ap ) override;
};

/*END OF lib_application_sink.h*/

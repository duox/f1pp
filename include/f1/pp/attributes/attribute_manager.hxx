/*attribute_manager.hxx*/

class F1_PP_API f1::pp::attribute_manager : public f1::core::named_object
{
public:
	typedef f1::core::named_object		super;
	typedef f1::pp::attribute_manager	self;

	explicit attribute_manager( const core::Iidentifier * ident = nullptr );

	// Attribute classes
public:
	const status_t	append_attribute_class( const char *  );
	const status_t	append_attribute_class( const pp::Iattribute_class & attr_class );
	const status_t	remove_attribute_class( const pp::Iattribute_class & attr_class );
	const status_t	remove_all_attribute_classes();

	const status_t	find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
		const f1::guid_t &  guid, const pp::arg_list * args = nullptr ) const;
	const status_t	find_attribute_class( core::Tconst_ref<pp::Iattribute_class> & res,
		const pp::string & ident, const pp::arg_list * args = nullptr ) const;

	const const_attribute_class_list_t &	get_attribute_class_list() const;
protected:
	const_attribute_class_list_t	m_attribute_class_list;

	// Attributes
public:
	const status_t	create_attribute( f1::core::Tref<pp::Iattribute> & attr,
		const f1::guid_t & guid, unsigned flags = 0, f1::pp::arg_list * args = nullptr,
		const core::Iidentifier * ident = nullptr ) noexcept;
	template<typename T> const status_t	create_attribute( f1::core::Tref<T> & attr,
		const f1::guid_t & guid, unsigned flags = 0, f1::pp::arg_list * args = nullptr,
		const core::Iidentifier * ident = nullptr ) noexcept;
	const status_t	create_attribute( f1::core::Tref<pp::Iattribute> & attr,
		const pp::string & name, unsigned flags = 0, f1::pp::arg_list * args = nullptr,
		const core::Iidentifier * ident = nullptr ) noexcept;
	template<typename T> const status_t	create_attribute( f1::core::Tref<T> & attr,
		const pp::string & name, unsigned flags = 0, f1::pp::arg_list * args = nullptr,
		const core::Iidentifier * ident = nullptr ) noexcept;
	const status_t	create_attribute( f1::core::Tref<pp::Iattribute> & attr,
		Iattribute_class & attr_cls, unsigned flags = 0, f1::pp::arg_list * args = nullptr,
		const core::Iidentifier * ident = nullptr ) noexcept;
protected:
};


template<typename T>
inline
const status_t f1::pp::attribute_manager::create_attribute( f1::core::Tref<T> & tattr,
	const f1::guid_t & guid, unsigned flags, f1::pp::arg_list * args,
	const core::Iidentifier * ident ) noexcept
{
	core::Tref<pp::Iattribute> attr = nullptr;
	status_t status = create_attribute( attr, guid, flags, args, ident );
	if( status.failed() )
		return status;

	tattr = attr.dynamic_pointer_cast<T>();
	return nullptr == tattr ? STATUS_ERROR_INVALID_TYPE : STATUS_SUCCESS;
}
template<typename T>
inline
const status_t f1::pp::attribute_manager::create_attribute( f1::core::Tref<T> & tattr,
	const pp::string & name, unsigned flags, f1::pp::arg_list * args,
	const core::Iidentifier * ident ) noexcept
{
	core::Tref<pp::Iattribute> attr = nullptr;
	status_t status = create_attribute( attr, name, flags, args, ident );
	if( status.failed() )
		return status;

	tattr = attr.dynamic_pointer_cast<T>();
	return nullptr == tattr ? STATUS_ERROR_INVALID_TYPE : STATUS_SUCCESS;
}

/*END OF attribute_manager.hxx*/

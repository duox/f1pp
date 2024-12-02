/*directive_list.h*/
/**
 * List of directives defined in a file.
 */

class F1_PP_API f1::pp::directive_list : public f1::core::object
{
public:
	typedef f1::core::object			super;
	typedef f1::pp::directive_list		self;

	directive_list() __noexcept;
	~directive_list() __noexcept;

	// Properties
public:
	virtual const f1::guid_t&	get_guid() const __noexcept;

protected:

	// Directives
public:
	const status_t	append_directive( const Idirective & directive ) __noexcept;	/* appended in current directive namespace (NB: not supported yet) */
	const status_t	append_directives( const directive_list & list ) __noexcept;	/* appended in current directive namespace (NB: not supported yet) */
	const status_t	append_directives( const directive_list_t & list ) __noexcept;	/* appended in current directive namespace (NB: not supported yet) */
	const status_t	remove_directive( const Idirective & directive ) __noexcept;
	const status_t	remove_directives( const directive_list & list ) __noexcept;	/* appended in current directive namespace (NB: not supported yet) */
	const status_t	remove_directives( const directive_list_t & list ) __noexcept;	/* appended in current directive namespace (NB: not supported yet) */
	const status_t	remove_all_directives() __noexcept;

	static const unsigned	fl_compare_no_case	= 0x0001;
	const status_t	find_directive( core::Tconst_ref<Idirective> & res, unsigned flags, const f1::pp::string & name ) const __noexcept;
	const status_t	find_directive( core::Tconst_ref<Idirective> & res, unsigned flags, const f1::core::stringslice & name ) const __noexcept;
	const status_t	find_directive( core::Tconst_ref<Idirective> & res, unsigned flags, const char * name, size_t len = size_t(-1) ) const __noexcept;
	const status_t	find_directive( core::Tconst_ref<Idirective> & res, const f1::guid_t & guid ) const;
	template<typename T>	const status_t	find_directive( core::Tconst_ref<T> & res ) const;

	const const_directive_list_t &	get_directive_list() const;
protected:
	typedef stdext::hash_map< std::string, core::Tconst_ref<Idirective> >					name_map_t;
	typedef stdext::hash_map< f1::guid_t,  core::Tconst_ref<Idirective>, f1::guid_hash_compare>	guid_map_t;
	name_map_t	m_name_list;
	guid_map_t	m_guid_list;
	const_directive_list_t	m_directive_list;

	// Hierarchy
public:
	const status_t	append_extension( directive_list & dlist );
	const status_t	remove_extension( directive_list & dlist );
	const status_t	remove_all_extensions();

		  directive_list *	get_parent();
	const directive_list *	get_parent() const;

	const status_t	find_extension( core::Tref<pp::directive_list> & res, const f1::guid_t & guid ) const;
	const core::Tobject_array<pp::directive_list> &	get_children_list() const;
private:
	core::Tobject_array<pp::directive_list>	m_children;
	core::Tref<pp::directive_list>			m_parent;
};

template<typename T>
inline
const status_t f1::pp::directive_list::find_directive( core::Tconst_ref<T> & res ) const
{
	array_foreach( const_directive_list_t::const_iterator, it, m_directive_list )	// TODO: use hash table search
	{
		if( it->get_guid() == T::m_guid )
		{
			res = (*it).dynamic_pointer_cast<T>();
			if( nullptr != res )
				return STATUS_SUCCESS;
		}
	}
	res = nullptr;
	return STATUS_ERROR_NOT_FOUND;
}

/*END OF directive_list.h*/

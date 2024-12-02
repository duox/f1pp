/*object.cxx*/
#include "pch.h"
#include "main.h"

f1::pp::object::object( const core::Iidentifier * ident ):
	m_ident( ident )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const f1::core::Iidentifier & f1::pp::object::get_identifier() const
{
	return nullptr != m_ident ? *m_ident : *core::Iidentifier::null;
}

/*END OF object.cxx*/

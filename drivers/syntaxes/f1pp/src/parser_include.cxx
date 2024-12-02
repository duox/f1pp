/*parser_include.cxx*/
#include "pch.h"
#include "main.h"

const status_t f1::f1pp::parser::on_source_included( f1::parser::Isource & src ) noexcept
{
	super::on_source_included( src );

	source_context * ctx = get_source_context( src );
	if( nullptr == ctx )
	{
		ctx = create_source_context( src );
		__debugbreak_if( nullptr == ctx )
			return STATUS_ERROR_INTERNAL;
	}

	return STATUS_SUCCESS;
}
const status_t f1::f1pp::parser::on_source_enter( f1::parser::Isource & src ) noexcept
{
	super::on_source_enter( src );

	source_context * ctx = get_source_context( src );
	__debugbreak_if( nullptr == ctx )
		return STATUS_ERROR_INTERNAL;

	if( nullptr != ctx->m_macro )
	{
		ctx->m_output_previous = get_output();
		set_output( &ctx->m_output );
		ctx->m_storage_previous = get_storage();
		set_storage( &ctx->m_storage );
	}

	return m_source_stack.append( ctx );
}
const status_t f1::f1pp::parser::on_source_leave( f1::parser::Isource & src ) noexcept
{
	super::on_source_leave( src );

	// Check current state
	__debugbreak_if( m_sources.empty() )
		return STATUS_ERROR_INTERNAL;

	// Fetch source context and ensure it corresponds to the source to be left
	source_context & ctx = m_sources.back();
	__debugbreak_if( ctx.m_source != &src )
		return STATUS_ERROR_INTERNAL;

	// Restore previous namespace if it was changed
	if( nullptr != ctx.m_ns_previous )
		get_ns_context().set_current_namespace( ctx.m_ns_previous );

	// Restore previous storage
	if( nullptr != ctx.m_storage_previous )
	{
		__debugbreak_ifnot( nullptr != ctx.m_macro )
		{
			const pp::string & value = ctx.m_storage.get_string();
			size_t start = 0, len = 0;
			pp::service::trim_spaces( value, start, len );

			if( ctx.m_target_macro_append_mode )
				ctx.m_macro->append_value( value.data() + start, len );
			else
				ctx.m_macro->set_value( value.data() + start, len );
		}
		set_storage( ctx.m_storage_previous );
		set_output( ctx.m_output_previous );
	}

	// Check that all #include arguments are referred to
	if( nullptr != ctx.m_args )
	{
		//__assert( ctx.m_next_arg_index == ctx.m_args->get_arg_list().size() );
		if( ctx.m_next_arg_index < ctx.m_args->get_arg_list().size() )
			message( syntax::IMSG_WARNING_UNUSED_INCLUDE_ARGUMENTS_M_N, ctx.m_next_arg_index, ctx.m_args->get_arg_list().size() - 1 );
	}

	// Remove all param macros
	remove_macros( ctx.m_params );
	append_macros( ctx.m_saved_param_macros );

//	core::Tref<ns::name_space> ns;
//	get_ns_context().get_current_namespace()->get_subnamespace( ns, "__params" );
//	__assert( ns->get_subname_list().empty() );
//	get_ns_context().get_current_namespace()->remove_subnamespace( "__params" );

	// Remove last element
	m_sources.pop_back();

	// Exit
	return STATUS_SUCCESS;
}

const f1::f1pp::parser::source_context * f1::f1pp::parser::get_top_source_context() const
{
	__debugbreak_if( m_sources.empty() )
		return nullptr;
	return &m_sources.back();
}
f1::f1pp::parser::source_context * f1::f1pp::parser::get_top_source_context()
{
	__debugbreak_if( m_sources.empty() )
		return nullptr;
	return &m_sources.back();
}

f1::f1pp::parser::source_context * f1::f1pp::parser::create_source_context( f1::parser::Isource & src )
{
	source_context * ctx;

	ctx = get_source_context( src );
	if( nullptr != ctx )
		return ctx;

	ctx = m_sources.append_new();
	__debugbreak_if( nullptr == ctx )
		return nullptr;
	ctx->m_source = &src;
	ctx->m_storage.initialize( *m_context );
	ctx->m_output.initialize();
	ctx->m_next_arg_index = 0;
	ctx->m_target_macro_append_mode = false;

	return ctx;
}

const f1::f1pp::parser::source_context * f1::f1pp::parser::get_source_context( f1::parser::Isource & src ) const
{
	array_foreach( core::Tarray<source_context>::const_iterator, it, m_sources )
	{
		if( &src == (*it).m_source )
			return &*it;
	}
	return nullptr;
}
f1::f1pp::parser::source_context * f1::f1pp::parser::get_source_context( f1::parser::Isource & src )
{
	array_foreach( core::Tarray<source_context>::iterator, it, m_sources )
	{
		if( &src == (*it).m_source )
			return &*it;
	}
	return nullptr;
}

const f1::core::Tarray<f1::f1pp::parser::source_context> & f1::f1pp::parser::get_sources_context_list() const
{
	return m_sources;
}

/*END OF parser_include.cxx*/

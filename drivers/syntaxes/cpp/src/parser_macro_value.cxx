/*parser_macro_value.cxx*/
#include "pch.h"
#include "main.h"

// TODO: use hashes to compare values
bool f1::c_pp::parser::compare_values( const pp::string & value1, const pp::param_list * params1,
		const pp::string & value2, const pp::param_list * params2,
		unsigned flags )
{
	// Check parameter lists
	if( (nullptr == params1) != (nullptr == params2) )
		return false;
	if( nullptr != params1 )
	{
		if( !params1->match_param_list( *params2 ) )
			return false;
	}

	// Comapre two strings
	const char * s1 = value1.data();
	const char * end_s1 = value1.data_end();
	const char * s2 = value2.data();
	const char * end_s2 = value2.data_end();

	for( ; s1 < end_s1 && s2 < end_s2; )
	{
		// Repetitious spaces are compared as a single optional space
		if( isspace( *s1 ) )
		{
			if( !isspace( *s2 ) )
				return false;
			skip_spaces_and_comments( s1, end_s1 );
			skip_spaces_and_comments( s2, end_s2 );
			continue;
		}
		if( isspace( *s2 ) )
			return false;

		// Parse identifier
		if( is_ident_first_char( *s1 ) )
		{
			// Second value must have an identifier at the position too
			if( !is_ident_first_char( *s2 ) )
				return false;

			// Parse identifiers
			pp::string ident1;
			parse_definition_identifier( ident1, s1, end_s1 );
			pp::string ident2;
			parse_definition_identifier( ident2, s2, end_s2 );

			// Find arguments
			if( nullptr != params1 )
			{
				signed index1 = params1->find_param( ident1 );
				signed index2 = params2->find_param( ident2 );
				if( -1 != index1 )
				{
					if( index1 == index2 )	// OK, arguments at the same position
					{
						if( !(flags & fl_compare_values_ignore_misspellings) && ident1 != ident2 )
							return false;
						continue;
					}
					return false;
				}
				else if( -1 != index2 )		// not found in the param list #1 and found in the param list #2
					return false;
				// Otherwise, both identifiers are not parameters; continue copmaring it
			}
			if( !ident1.equal( ident2 ) )
				return false;
			continue;
		}

		// Another character should match
		if( *s1 != *s2 )
			return false;
		++ s1;
		++ s2;
	}

	// Exit
	return s1 >= end_s1 && s2 >= end_s2;
}

const status_t f1::c_pp::parser::calc_value_hash( core::md5hash::digest_t & digest, const pp::string & value, const pp::param_list * params, unsigned flags )
{
	core::md5hash	md5;

	// Comapre two strings
	const char * s = value.data();
	const char * end_s = value.data_end();
	const char * start = s;

	for( ; s < end_s; )
	{
		// Repetitious spaces are compared as a single optional space
		if( isspace( *s ) )
		{
			if( s != start )
				md5.update( start, s - start );

			skip_spaces_and_comments( s, end_s );
			start = s;

			md5.update( "\x20", 1 );
		}
		// Process '\nl'
		else if( '\\' == *s )
		{
			if( s != start )
				md5.update( start, s - start );
			skip_newline( s, end_s );
			start = s;
			md5.update( "\x20", 1 );
		}
		// Parse identifier
		else if( is_ident_first_char( *s ) )
		{
			if( !(flags & fl_compare_values_ignore_misspellings) )
			{
				pp::string ident;
				parse_definition_identifier( ident, s, end_s );
			}
			else
			{
				if( s != start && nullptr != params )
					md5.update( start, s - start );

				// Parse identifier
				pp::string ident;
				parse_definition_identifier( ident, s, end_s );
				start = s;

				// Find arguments
				if( nullptr != params )
				{
					signed index = params->find_param( ident );
					if( -1 != index )
					{
						// Generate name from index: index == 0 => "a", index == 27 ==> "za"
						char buf[16];
						size_t count;
						++ index;
						while( index > 0 )
						{
							for( count = 0; index > 0 && count < countof(buf); ++ count )
							{
								const signed char_index = std::min( index, 'z' - 'a' + 1 );
								buf[count] = char(char_index + 'a' - 1);
								index -= char_index;
							}
							md5.update( buf, count );
						}
					}
					else
						md5.update( ident.data(), ident.length() );
				}
			}
			continue;
		}
		// Another character
		else
			++ s;
	}
	if( s != start )
		md5.update( start, s - start );

	// Done
	const core::md5hash::digest_t & result = md5.get_digest();
	memcpy( &digest, &result, sizeof(digest) );

	// Exit
	return STATUS_SUCCESS;
}

/*END OF parser_string.cxx*/

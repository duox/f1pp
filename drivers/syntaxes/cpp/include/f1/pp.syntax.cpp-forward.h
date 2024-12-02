/*c-pp-forward.h*/
/** @file
 *
 * @brief C-PP forward declarations.
 *
 */
#pragma once

namespace f1 { namespace c_pp
{
interface Iframe;
class parser;
class syntax;

interface Ipragma;
typedef core::Tobject_array<Ipragma>		pragma_list_t;
typedef core::Tconst_object_array<Ipragma>	const_pragma_list_t;
interface	Isuper_pragma;

class macro;

}}

/*END OF c-pp-forward.h*/

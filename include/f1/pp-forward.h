/*pp-forward.h*/
/** @file
 *
 * @brief PP forward declarations.
 *
 */
#pragma once

namespace f1 { namespace pp
{
	begin_scoped_enum( limit_t )
		li_maximum_include_depth,			///< maximum nesting of includes
		li_maximum_identifier_length,		///< maximum length of a macro identifier
		li_maximum_macro_count,				///< maximum number of macros in a single preprocessing translation unit
		li_maximum_line_length,				///< maximum number of characters in a logical line
		li_maximum_macro_parameter_count,	///< maximim number of parameters in one macro definition
		li_maximum_macro_argument_count,	///< maximim number of parameters in one macro invocaion
		li_maximum_charconst_length,		///< maximum number of characters in a character literal
		li_error_maximum_count,				///< maximum number of errors after which preprocessing stops
		li_warning_maximum_count,			///< maximum number of warnings after which preprocessing stops
		li_maximum_block_depth,				///< maximum nesting of conditional inclusions
		li_maximum_iteration_count,			///< maximum number of iterations
		li_maximum_recursion_depth,			///< maximum depth of recursive macros
		li_count
	end_scoped_enum;

	// Base definitions
	class object;

	interface Iframe;
	//interface Ilistener;
	//typedef f1::core::Tobject_array<Ilistener>			listener_list_t;
	interface Iparser;
	typedef f1::core::Tobject_array<Iparser>			parser_list_t;
	typedef f1::core::Tconst_object_array<Iparser>		const_parser_list_t;

	// Standards and syntaxes
	interface Istandard;
	typedef f1::core::Tobject_array<Istandard>			standard_list_t;
	typedef f1::core::Tconst_object_array<Istandard>	const_standard_list_t;
	interface Isyntax;
	typedef f1::core::Tobject_array<Isyntax>			syntax_list_t;
	typedef f1::core::Tconst_object_array<Isyntax>		const_syntax_list_t;

	// Attributes
	class attribute;
	//class Iattribute;
	class attribute_list;
	//interface Iattribute_class;
	//class attribute_manager;
	typedef core::Tobject_array<attribute>					  attribute_list_t;
	typedef core::Tconst_object_array<attribute>		const_attribute_list_t;
	//typedef core::Tobject_array<Iattribute>					  attribute_list_t;
	//typedef core::Tconst_object_array<Iattribute>		const_attribute_list_t;
	//typedef core::Tobject_array<Iattribute_class>			  attribute_class_list_t;
	//typedef core::Tconst_object_array<Iattribute_class>	const_attribute_class_list_t;

	//class user_attribute;
	//class user_attribute_class;

	// Outputs
	interface Ioutput;
		//class buffer_output;
		class null_output;
		class raw_output;
		//class string_output;
	typedef f1::core::Tobject_array<Ioutput>			output_list_t;

	interface Ioption;
	typedef f1::core::Tobject_array<Ioption>			option_list_t;
	typedef f1::core::Tconst_object_array<Ioption>		const_option_list_t;
	class option;

	interface Ioutput_class;
		class buffer_output_class;
		class null_output_class;
		class raw_output_class;
		class string_output_class;
	typedef f1::core::Tobject_array<Ioutput_class>		output_class_list_t;

	// Storages
	interface Istorage;
		class buffer_storage;
		class file_storage;
		class null_storage;
		class stream_storage;
		class string_storage;
	typedef f1::core::Tobject_array<Istorage>			storage_list_t;


	interface Istorage_class;
		class buffer_storage_class;
		class file_storage_class;
		class null_storage_class;
		class stream_storage_class;
		class string_storage_class;
	typedef f1::core::Tobject_array<Istorage_class>		storage_class_list_t;

	// Directives
	class directive_list;
	interface Idirective;
		class	ignored_directive;
		class	known_directive;
	typedef core::Tobject_array<Idirective>				directive_list_t;
	typedef core::Tconst_object_array<Idirective>		const_directive_list_t;

	// Includes
	class include_list;

	// Macros
	interface Ifunction;
	typedef f1::core::Tconst_object_array<Ifunction>	function_list_t;
	interface Imacro;
	typedef f1::core::Tobject_array<Imacro>				macro_list_t;
	typedef f1::core::Tconst_object_array<Imacro>		const_macro_list_t;
	class Iuser_macro;

	class arg_list;
	class param_list;

	typedef class parser::string	string;
	class variant;

	// Service classes
	class context;
	interface Iprovider;
	class service;

}}

/*END OF pp-forward.h*/

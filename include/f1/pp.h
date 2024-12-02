/*pp.h*/
/** @file
 *
 * @brief Universal preprocessor.
 *
 */
#ifndef F1_PP_H_325A1C14C7854FDFA07E5422D0798EEB_INCLUDED
#define F1_PP_H_325A1C14C7854FDFA07E5422D0798EEB_INCLUDED
#define F1_PP_H_325A1C14C7854FDFA07E5422D0798EEB_SECTION
#pragma once

// Include std
//#include <functional>

// Include dependencies
#include <f1/core.h>
#include <f1/ns.h>
#include <f1/nls.h>
#include <f1/parser.h>

#include <f1/pp.frontend.h>

// Service includes
#include "pp-config.h"		// library configuration
#include "pp-forward.h"		// required forward definitions


// Base definitions
#include "pp/object.hxx"

//#include "pp/Ilistener.hxx"

//#include "pp/string.hxx"
#include "pp/variant.hxx"

#include "pp/arg_list.hxx"
#include "pp/param_list.hxx"

#include "pp/attributes/attribute.hxx"
//#include "pp/attributes/Iattribute.hxx"
//#include "pp/attributes/Iattribute_class.hxx"
#include "pp/attributes/attribute_list.hxx"
//#include "pp/attributes/attribute_manager.hxx"
//#include "pp/attributes/attributes/user_attribute.hxx"
//#include "pp/attributes/attributes/user_attribute_class.hxx"

#include "pp/directive_list.hxx"
#include "pp/Idirective.hxx"
#include	"pp/directives/ignored.hxx"
#include	"pp/directives/known.hxx"

//#include "pp/include_list.hxx"

#include "pp/Ifunction.hxx"
#include "pp/Imacro.hxx"
#include	"pp/macros/Iuser_macro.hxx"

#include "pp/Iframe.hxx"
#include "pp/Ioption.hxx"
#include	"pp/options/option.hxx"
#include "pp/Ioutput_class.hxx"
#include "pp/Ioutput.hxx"
//#include	"pp/outputs/buffer_output.hxx"
#include	"pp/outputs/null_output.hxx"
#include	"pp/outputs/raw_output.hxx"
//#include	"pp/outputs/string_output.hxx"
#include "pp/Iparser.hxx"
#include "pp/Istandard.hxx"
#include	"pp/Isyntax.hxx"
#include "pp/Istorage_class.hxx"
#include "pp/Istorage.hxx"
#include	"pp/storages/buffer_storage.hxx"
#include	"pp/storages/null_storage.hxx"
#include	"pp/storages/stream_storage.hxx"
#include		"pp/storages/file_storage.hxx"
#include	"pp/storages/string_storage.hxx"

// Service classes
#include "pp/Iprovider.hxx"
#include "pp/context.hxx"
#include "pp/service.hxx"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef F1_PP_H_325A1C14C7854FDFA07E5422D0798EEB_SECTION
#endif /* ndef F1_PP_H_325A1C14C7854FDFA07E5422D0798EEB_INCLUDED */
/*END OF pp.h*/

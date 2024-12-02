/*pch.h*/
// SPDX-License-Identifier: LGPL-2.1-or-later
/* Copyright (C) 2012-2024 Akimov Vitaly (duox). All Rights Reserved. */
/* See full license text in the LICENSE.md file. */

#pragma once

#include <f1/core.h>
#include <f1/fs.h>
#include <f1/ns.h>
#include <f1/nls.h>
#include <f1/parser.h>
#include <f1/util.h>

#define F1_MSGOUT_CONFIG_DEFAULT_PLUGINS	1
#include <f1/msgout.h>

#ifndef _DEV_PCH
# include <f1/pp.h>
#endif	/* ndef _DEV_PCH */

#define STR_LEN_PAIR(s)		s, countof(s) - 1

/*END OF pch.h*/

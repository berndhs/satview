#ifndef SATVIEW_CONFIG_H
#define SATVIEW_CONFIG_H

//
//  Copyright (C) 2009 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

/** @brief This file contains the preprocessor flags for including parts
 *  of the code. Note that some are mutually exclusive, although that is
 * not enforced here. 
 * For example, we can only use one of the (..QNET ..._WINSOCK ..._GNUSOCK)
 * and one of (.._MYSQL ..._QSQL) flags.
 * If all of the (..SQL) flags are set to 0, then no direct sql interface is
 * present. Similarly for the ...SOCK and ...QNET flags.
 */

#ifdef _MSC_VER

/* network interfaces */
#define SATVIEW_USE_QNET 1
#define SATVIEW_USE_WINSOCK 0
#define SATVIEW_USE_GNUSOCK 0

/* SQL interfaces */
#define SATVIEW_USE_MYSQL 0
#define SATVIEW_USE_QSQL 1

#else

/* network interfaces */
#define SATVIEW_USE_QNET 1
#define SATVIEW_USE_WINSOCK 0
#define SATVIEW_USE_GNUSOCK 0

/* SQL interfaces */
#define SATVIEW_USE_MYSQL 0
#define SATVIEW_USE_QSQL 1

#endif

#define SATVIEW_USE_TEMPFILE 0

#if SATVIEW_USE_TEMPFILE
#ifdef _WIN32
#define TMP_JPG_FILE "C:/WINDOWS/TEMP/satviewtmp.jpg"
#else
#define TMP_JPG_FILE "/tmp/tmp.jpg"
#endif
#endif

#endif

#ifndef COPYDB_CONFIG_H
#define COPYDB_CONFIG_H


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

#define DO_COPYDB 1

#ifdef _MSC_VER

/* network interfaces - no more than 1 */
#define SATVIEW_USE_QNET 0
#define SATVIEW_USE_WINSOCK 1
#define SATVIEW_USE_GNUSOCK 0

/* direct SQL interfaces - no more than 1 */
#define SATVIEW_USE_MYSQL 0
#define SATVIEW_USE_QSQL 0

#else

/* network interfaces - no more than 1 */
#define SATVIEW_USE_QNET 0
#define SATVIEW_USE_WINSOCK 0
#define SATVIEW_USE_GNUSOCK 1

/* direct SQL interfaces - no more than 1 */
#define SATVIEW_USE_MYSQL 0
#define SATVIEW_USE_QSQL 1

#endif

#if (SATVIEW_USE_QNET) + (SATVIEW_USE_WINSOCK) + (SATVIEW_USE_GNUSOCK) > 1
#error "Cannot use more than one Network interface"
#endif
#if (SATVIEW_USE_MYSQL) + (SATVIEW_USE_QSQL) > 1
#error "Cannot use more than one SQL Interface"
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

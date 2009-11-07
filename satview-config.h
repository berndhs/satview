#ifndef SATVIEW_CONFIG_H
#define SATVIEW_CONFIG_H


/** @brief Can have USE_MYSQL and any USE_xyz_SOCK at the same time,
 * but not USE_this_SOCK and USE_that_SOCK. Only one kind of SOCK.
 */
#define SATVIEW_USE_WINSOCK 0
/** @brief Can have USE_MYSQL and any USE_xyz_SOCK at the same time,
 * but not USE_this_SOCK and USE_that_SOCK. Only one kind of SOCK.
 */
#define SATVIEW_USE_GNUSOCK 1

#define SATVIEW_USE_MYSQL 1

#define SATVIEW_USE_TEMPFILE 0

#if SATVIEW_USE_TEMPFILE
#ifdef _WIN32
#define TMP_JPG_FILE "C:/WINDOWS/TEMP/satviewtmp.jpg"
#else
#define TMP_JPG_FILE "/tmp/tmp.jpg"
#endif
#endif

#endif

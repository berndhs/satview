#ifndef SJDATASRC_H
#define SJDATASRC_H

//
//  Copyright (C) 2009 - Bernd H Stramm
//
#include <string.h>
#include <stdio.h>   /* jpeglib.h needs it, but doesn't include it. */
#include "dcomp/jpeglib.h"
#include "dcomp/jerror.h"
#ifdef __cplusplus
#include <string>
#include <sstream>
#include "berndsutil.h"
#endif

#define HAVE_LIBJPEG
#define SIZEOF(object)  ((size_t) sizeof(object))

typedef struct sjdata {
  char    * indata;
  size_t  inlen;
  size_t  cur_pos;
#ifdef __cplusplus
  std::string String () { 
    std::string s;    
    s = berndsutil::toString(indata)
      + "  " + berndsutil::toString(inlen)
      + "  " + berndsutil::toString(cur_pos) ;
    return s;
  }
#endif
    
} sjdata;

#ifdef __cplusplus
extern "C" {
#endif

void sj_jpeg_buf_src (satview_jpeg::j_decompress_ptr cinfo, sjdata inbuf);
#ifdef __cplusplus
}
#endif

#endif

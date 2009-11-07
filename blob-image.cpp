
//
//  Copyright (C) 2009 - Bernd H Stramm
//

// Thanks to FLTK.org 

//
// Include necessary header files...
//

#include "blob-image.h"
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <iostream>

namespace satview {

using namespace satview_jpeg;

// Some releases of the Cygwin JPEG libraries don't have a correctly
// updated header file for the INT32 data type; the following define
// from Shane Hill seems to be a usable workaround...

#if defined(WIN32) && defined(__CYGWIN__)
#  define XMD_H
#endif // WIN32 && __CYGWIN__


extern "C"
{
#ifdef HAVE_LIBJPEG
#  include "dcomp/jpeglib.h"
#endif // HAVE_LIBJPEG
}


//
// Custom JPEG error handling structure...
//

#ifdef HAVE_LIBJPEG
struct fl_jpeg_error_mgr {
  jpeg_error_mgr	pub_;		// Destination manager...
  jmp_buf		errhand_;	// Error handler
};
#endif // HAVE_LIBJPEG


//
// Error handler for JPEG files...
//

#ifdef HAVE_LIBJPEG
extern "C" {
  static void
  fl_jpeg_error_handler(j_common_ptr dinfo) {	// I - Decompressor info
    longjmp(((fl_jpeg_error_mgr *)(dinfo->err))->errhand_, 1);
  }

  static void
  fl_jpeg_output_handler(j_common_ptr) {	// I - Decompressor info (not used)
  }
}
#endif // HAVE_LIBJPEG


//
/// @brief Blob_Image() - Load a JPEG image from a memory buffer.
//

Blob_Image::Blob_Image (sjdata & data_source)  
  : Fl_RGB_Image(0,0,0) {
#ifdef HAVE_LIBJPEG
  satview_jpeg::jpeg_decompress_struct	dinfo;	// Decompressor info
  fl_jpeg_error_mgr		jerr;	// Error handler info
  JSAMPROW			row;	// Sample row pointer

  // the following variables are pointers allocating some private space that
  // is not reset by 'setjmp()'
  char* max_finish_decompress_err;      // count errors and give up afer a while
  char* max_destroy_decompress_err;     // to avoid recusion and deadlock

  // Clear data...
  alloc_array = 0;
  array = (uchar *)0;

  // Setup the decompressor info and read the header...
  dinfo.err                = satview_jpeg::jpeg_std_error((satview_jpeg::jpeg_error_mgr *)&jerr);
  jerr.pub_.error_exit     = fl_jpeg_error_handler;
  jerr.pub_.output_message = fl_jpeg_output_handler;

  // Setup error loop variables
  max_finish_decompress_err = (char*)malloc(1);   // allocate space on the frame for error counters
  max_destroy_decompress_err = (char*)malloc(1);  // otherwise, the variables are reset on the longjmp
  *max_finish_decompress_err=10;
  *max_destroy_decompress_err=10;
  if (setjmp(jerr.errhand_))
  {
    // JPEG error handling...
    // if any of the cleanup routines hits another error, we would end up 
    // in a loop. So instead, we decrement max_err for some upper cleanup limit.
    if ( ((*max_finish_decompress_err)-- > 0) && array) {
      satview_jpeg::jpeg_finish_decompress(&dinfo);
    }
    if ( (*max_destroy_decompress_err)-- > 0) {
      satview_jpeg::jpeg_destroy_decompress(&dinfo);
    }

    w(0);
    h(0);
    d(0);

    if (array) {
      delete[] (uchar *)array;
      array = 0;
      alloc_array = 0;
    }

    free(max_destroy_decompress_err);
    free(max_finish_decompress_err);
    return;
  }

  satview_jpeg::jpeg_create_decompress(&dinfo);
  data_source.cur_pos = 0;
  sj_jpeg_buf_src (&dinfo, data_source);
  satview_jpeg::jpeg_read_header(&dinfo, 1);

  dinfo.quantize_colors      = (boolean)FALSE;
  dinfo.out_color_space      = JCS_RGB;
  dinfo.out_color_components = 3;
  dinfo.output_components    = 3;
  satview_jpeg::jpeg_calc_output_dimensions(&dinfo);

  w(dinfo.output_width); 
  h(dinfo.output_height);
  d(dinfo.output_components);  array = new uchar[w() * h() * d()];
  alloc_array = 1;

  satview_jpeg::jpeg_start_decompress(&dinfo);
  int numrows = 0;
  while (dinfo.output_scanline < dinfo.output_height) {
    numrows++;
    row = (JSAMPROW)(array +
                     dinfo.output_scanline * dinfo.output_width *
                     dinfo.output_components);
    satview_jpeg::jpeg_read_scanlines(&dinfo, &row, (JDIMENSION)1);
  }

  satview_jpeg::jpeg_finish_decompress(&dinfo);
  satview_jpeg::jpeg_destroy_decompress(&dinfo);

  free(max_destroy_decompress_err);
  free(max_finish_decompress_err);

#endif // HAVE_LIBJPEG
}

} // namespace

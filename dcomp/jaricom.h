#ifndef JARICOM_H
#define JARICOM_H
#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
namespace satview_jpeg {

const INT32 *local_jaritab;

INT32  get_jaritab (int);
}


#endif

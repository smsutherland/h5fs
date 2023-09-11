#ifndef H5FS_H
#define H5FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FUSE_USE_VERSION 27
#include <errno.h>
#include <fuse.h>
#include <fuse_opt.h>
#include <hdf5.h>

#define H5FS_DATA ((H5fsData*)fuse_get_context()->private_data)

typedef struct {
    hid_t file_id;
} H5fsData;

#endif

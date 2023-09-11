#include "h5fs.h"

static void* h5fs_init(struct fuse_conn_info* conn) {
    H5fsData* data = H5FS_DATA;
    printf("%ld\n", data->file_id);
    return data;
}

static void h5fs_destroy(void* _data) {
    H5fsData* data = (H5fsData*)_data;
    printf("%ld\n", data->file_id);
    free(data);
}

static int h5fs_getattr(const char* path, struct stat* stat) {
    printf("getattr: %s\n", path);
    memset(stat, 0, sizeof(struct stat));

    H5fsData* data = H5FS_DATA;

    H5O_info2_t info = {};
    herr_t err = H5Oget_info_by_name(data->file_id, path, &info, H5O_INFO_BASIC,
                                     H5P_DEFAULT);
    if (err < 0) {
        return -ENOENT;
    }

    switch (info.type) {
        case H5O_TYPE_GROUP: {
            stat->st_mode = S_IFDIR;
            stat->st_nlink = 2;
        } break;
        case H5O_TYPE_DATASET: {
            stat->st_mode = S_IFREG;
            stat->st_nlink = 1;
        } break;
        default: {
            return -ENOENT;
        } break;
    }

    return 0;
}

static int h5fs_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info* fi) {
    printf("readdir: %s\n", path);
    H5fsData* data = H5FS_DATA;
    printf("readdir\n");

    filler(buf, "..", NULL, 0);

    h5fs_fill_dir(data, path, buf, filler);

    return 0;
}

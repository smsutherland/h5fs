#include "h5fs.h"

static H5fsData* create_data(const char* fname) {
    H5fsData* data = malloc(sizeof(H5fsData));
    data->file_id = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);
    return data;
}

typedef struct {
    fuse_fill_dir_t filler;
    void* buf;
} H5fsVisitData;

// static herr_t h5fs_visit(hid_t obj, const char* name, const H5O_info2_t*
// info,
//                          void* _filler) {
//     H5fsVisitData* visit_data = (H5fsVisitData*)_filler;
//     visit_data->filler(visit_data->buf, name, NULL, 0);
//     return 0;
// }
static herr_t h5fs_iter(hid_t group, const char* name, const H5L_info2_t* info,
                        void* data) {

    H5fsVisitData* visit_data = (H5fsVisitData*)data;
    visit_data->filler(visit_data->buf, name, NULL, 0);
    return 0;
}

static void h5fs_fill_dir(H5fsData* data, const char* path, void* buf,
                          fuse_fill_dir_t filler) {
    H5fsVisitData visit_data = {
        .filler = filler,
        .buf = buf,
    };
    hsize_t index = 0;
    H5Literate_by_name(data->file_id, path, H5_INDEX_NAME, H5_ITER_NATIVE,
                       &index, h5fs_iter, &visit_data, 0);
}

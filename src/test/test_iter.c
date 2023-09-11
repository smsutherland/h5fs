#include <hdf5.h>
#define FILE "dset.hdf5"

herr_t op(hid_t obj, const char* name, const H5O_info2_t* info, void* op_data) {
    printf("%s\n", name);
    return 0;
}

int main(void) {
    hid_t file_id, group_id;
    herr_t status;

    file_id = H5Fopen(FILE, H5F_ACC_RDONLY, H5P_DEFAULT);

    H5Ovisit(file_id, H5_INDEX_NAME, H5_ITER_NATIVE, op, NULL, H5O_INFO_BASIC);

    status = H5Fclose(file_id);
}

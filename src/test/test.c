#include <hdf5.h>
#define FILE "dset.hdf5"

int main(void) {
    hid_t file_id, dataset_id, dataspace_id;
    herr_t status;
    int i, j, k, dset_data[4][6][2];
    hsize_t dims[3];

    for (i = 0; i < 4; i++)
        for (j = 0; j < 6; j++)
            for (k = 0; k < 2; k++)
                dset_data[i][j][k] = i * 6 * 2 + j * 2 + k + 1;
    dims[0] = 4;
    dims[1] = 6;
    dims[2] = 2;

    dataspace_id = H5Screate_simple(3, dims, NULL);

    file_id = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    dataset_id = H5Dcreate(file_id, "/dset", H5T_STD_I32BE, dataspace_id,
                           H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                      dset_data);

    status = H5Dclose(dataset_id);

    status = H5Sclose(dataspace_id);

    status = H5Fclose(file_id);
}

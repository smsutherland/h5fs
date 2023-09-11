#include "h5fs.h"
#include "h5fs_hdf5.c"

#include "h5fs_funcs.c"

struct H5fsParams {
    // bool: show help
    int help;
    // bool: show version information
    int version;
    // string: hdf5 file to mount
    const char* hdf5_filename;
    int str_arg_count;
};

#define KEY_HELP (0)
#define KEY_VERSION (1)

static const struct fuse_opt fusezip_opts[] = {
    FUSE_OPT_KEY("-h", KEY_HELP),
    FUSE_OPT_KEY("--help", KEY_HELP),
    FUSE_OPT_KEY("-v", KEY_VERSION),
    FUSE_OPT_KEY("--version", KEY_VERSION),
    FUSE_OPT_END,
};

static void print_usage() {
    fprintf(stderr, "usage: h5fs hdf5_file mountpoint [OPTIONS]\n");
}

static void print_version() { fprintf(stderr, "h5fs version: -1\n"); }

static int process_arg(void* data, const char* arg, int key,
                       struct fuse_args* outargs) {
    struct H5fsParams* params = (struct H5fsParams*)data;
    const static int KEEP = 1;
    const static int DISCARD = 0;
    const static int ERROR = -1;

    switch (key) {
        case KEY_HELP: {
            print_usage();
            params->help = 1;
            return DISCARD;
        }
        case KEY_VERSION: {
            print_version();
            params->version = 1;
            return KEEP;
        }
        case FUSE_OPT_KEY_NONOPT: {
            params->str_arg_count++;
            switch (params->str_arg_count) {
                case 1: {
                    params->hdf5_filename = arg;
                    return DISCARD;
                }
                case 2: {
                    // mountpoint. keep to pass to FUSE.
                    return KEEP;
                }
                default: {
                    fprintf(stderr, "Unexpected argument: %s", arg);
                    print_usage();
                    return ERROR;
                }
            }
            default: {
                return KEEP;
            }
        }
    }
}

int main(int argc, char** argv) {
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    struct H5fsParams params = {};
    H5fsData* data = NULL;

    if (fuse_opt_parse(&args, &params, fusezip_opts, process_arg)) {
        fuse_opt_free_args(&args);
        return -1;
    }

    if (params.help) {
        fuse_opt_free_args(&args);
        return 0;
    }

    if (!params.version) {
        if (!params.hdf5_filename) {
            print_usage();
            fuse_opt_free_args(&args);
            return -1;
        }

        data = create_data(params.hdf5_filename);
        if (!data) {
            fuse_opt_free_args(&args);
            return -1;
        }
    }

    static struct fuse_operations h5fs_ops = {
        .init = h5fs_init,
        .destroy = h5fs_destroy,
        .getattr = h5fs_getattr,
        .readdir = h5fs_readdir,
    };

    struct fuse* fuse;
    char* mountpoint;
    int multithreaded;
    int res;

    fuse = fuse_setup(args.argc, args.argv, &h5fs_ops, sizeof(h5fs_ops),
                      &mountpoint, &multithreaded, data);
    if (!fuse) {
        free(data);
        return -1;
    }
    res = fuse_loop(fuse);
    fuse_teardown(fuse, mountpoint);
    if (res == 0) {
        return 0;
    } else {
        return -1;
    }
}

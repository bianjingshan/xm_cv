
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <gflags/gflags.h>
#include "show_pp_data.h"
#include "xm_search_files.h"

using namespace std;


DEFINE_bool(r, false, "rand show");
DEFINE_string(c, "000001.SZ", "ts code");
DEFINE_int32(l, 100, "rand test show data len");

vector<char *> v_file_name;

void SearchFileCallback(void *p_cbk_data, const char* cbk_root_path_name, const char *cbk_sub_path_name, const char *cbk_file_name)
{
    char * file_name = (char *)malloc(1024);
    sprintf(file_name, "%s%s/%s", cbk_root_path_name, cbk_sub_path_name, cbk_file_name);
    v_file_name.push_back(file_name);
}

int main(int argc, char *argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    ShowPPData show;

    SearchFiles("./data", "", NULL, SearchFileCallback);

    if (FLAGS_r)
    {
        while (1)
        {
            struct NpyArray a = npy_load(v_file_name[rand() % v_file_name.size()]);
            double *pp_data = a.data<double>();
            if (a.shape[0] <= 200)
            {
                continue;
            }
            show.SetData(pp_data, a.shape[0]);
            show.SetShowData(rand() % (a.shape[0] - FLAGS_l * 2) + FLAGS_l * 2, FLAGS_l);
            show.Run();
        }
    }
    else
    {
        for (int iloop = 0; iloop < v_file_name.size(); iloop++)
        {
            if (strcasestr(v_file_name[iloop], FLAGS_c.c_str()))
            {
                struct NpyArray a = npy_load(v_file_name[iloop]);
                double *pp_data = a.data<double>();
                show.SetData(pp_data, a.shape[0]);
                show.SetShowData(0, a.shape[0]);
                show.Run();
            }
        }
    }
    return 0;
}

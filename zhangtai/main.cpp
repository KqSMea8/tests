#include <iostream>
#include <fstream>
#include <vector>
#include "include/paddle/capi.h"
#include "common.h"
#include "modelInput.h"

const char *CONFIG_BIN = "./model/trainer_config.bin";
const char *INPUTDATAPATH = "./data/input";

void* read_config(const char* filename, long* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Open %s error\n", filename);
        return NULL;
    }
    fseek(file, 0L, SEEK_END);
    *size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    void* buf = malloc(*size);
    fread(buf, 1, *size, file);
    fclose(file);
    return buf;
}

int main() {

    std::ifstream file;
    file.open(INPUTDATAPATH);

    int integer_data[18] = {};
    for (int i = 0; i < 18; i++) {
        file >> integer_data[i];
    }
    std::vector<double> travelpref_in(4);
    for (int i = 0; i < 4; ++i) {
        file >> travelpref_in[i];
    }
    std::vector<double> carowner_in(1);
    for (int i = 0; i < 1; ++i) {
        file >> carowner_in[i];
    }
    std::vector<double> privatedriver_in(1);
    for (int i = 0; i < 1; ++i) {
        file >> privatedriver_in[i];
    }
    std::vector<double> drive_in(3);
    for (int i = 0; i < 3; ++i) {
        file >> drive_in[i];
    }
    std::vector<double> carserve_in(3);
    for (int i = 0; i < 3; ++i) {
        file >> carserve_in[i];
    }
    std::vector<double> gas_in(3);
    for (int i = 0; i < 3; ++i) {
        file >> gas_in[i];
    }
    std::vector<double> poi4s_in(3);
    for (int i = 0; i < 3; ++i) {
        file >> poi4s_in[i];
    }
    std::vector<double> park_in(3);
    for (int i = 0; i < 3; ++i) {
        file >> park_in[i];
    }
    std::vector<double> drive_weekday_in(3);
    for (int i = 0; i < 3; ++i) {
        file >> drive_weekday_in[i];
    }
    std::vector<double> drive_weekend_in(3);
    for (int i = 0; i < 3; ++i) {
        file >> drive_weekend_in[i];
    }
    std::vector<double> consumption_will_in(36);
    for (int i = 0; i < 36; ++i) {
        file >> consumption_will_in[i];
    }
    std::vector<double> taghistyear_in(802);
    for (int i = 0; i < 802; ++i) {
        file >> taghistyear_in[i];
    }
    std::vector<double> taghisthour_in(802*24);
    for (int i = 0; i < 802*24; ++i) {
        file >> taghisthour_in[i];
    }
    std::vector<double> taghistweek_in(802*2);
    for (int i = 0; i < 802*2; ++i) {
        file >> taghistweek_in[i];
    }

    // Initalize Paddle
    char* argv[] = {(char *)"--use_gpu=False"};
    CHECK(paddle_init(1, (char**)argv));

    // Reading config binary file. It is generated by `convert_protobin.sh`
    long size = 0;
    void* buf = read_config(CONFIG_BIN, &size);

    // Create a gradient machine for inference.
    paddle_gradient_machine machine;
    CHECK(paddle_gradient_machine_create_for_inference(&machine, buf, (int)size));

    CHECK(paddle_gradient_machine_load_parameter_from_disk(machine, "./param"));

    clock_t start;
    clock_t finish;
    start = clock();
    ModelDataInput model_data;
    inputData input = {integer_data[0], integer_data[1], integer_data[2], integer_data[3],
                       integer_data[4], integer_data[5], integer_data[6], integer_data[7],
                       integer_data[8], integer_data[9], integer_data[10], integer_data[11],
                       integer_data[12], integer_data[13], integer_data[14], integer_data[15],
                       integer_data[16], integer_data[17],
                       travelpref_in, carowner_in, privatedriver_in, drive_in, carserve_in,
                       gas_in, poi4s_in, park_in, drive_weekday_in, drive_weekend_in,
                       consumption_will_in, taghistyear_in, taghisthour_in, taghistweek_in};
    paddle_arguments in_args = model_data.get_paddle_in_args(input);
    paddle_real *array_out = nullptr;

    paddle_arguments out_args = paddle_arguments_create_none();

    start = clock();
    for (int i = 0; i < 1; i++) {
        CHECK(paddle_gradient_machine_forward(machine,
                                              in_args,
                                              out_args,
                                              false));
    }
    finish = clock();
    double duration_calc = (double)(finish - start) * 1000  / (CLOCKS_PER_SEC * 100);
    printf("time:%f ms\n", duration_calc);

    paddle_matrix prob = paddle_matrix_create_none();

    CHECK(paddle_arguments_get_value(out_args, 0, prob));

    CHECK(paddle_matrix_get_row(prob, 0, &array_out)); // 检查输出格式是否正确

    printf("Prob: ");
    for (int i = 0; i < 802; ++i) {
        printf("%.5f ", array_out[i]);
    }
    printf("\n");

    CHECK(paddle_matrix_destroy(prob));
    CHECK(paddle_arguments_destroy(out_args));
    CHECK(paddle_gradient_machine_destroy(machine));
    file.close();

    return 0;
}

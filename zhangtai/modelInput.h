//
// Created by Zhang,Tai on 17/10/12.
//

#ifndef EXEMODEL_MODELINPUT_H
#define EXEMODEL_MODELINPUT_H

#include "include/paddle/capi.h"
#include <vector>

class ModelData;

typedef struct input_data {
    const int &month_in;
    const int &week_in;
    const int &hour_in;
    const int &city_in;
    const int &phoneos_in;
    const int &homedist_in;
    const int &compdist_in;
    const int &homecity_in;
    const int &newcity_in;
    const int &gender_in;
    const int &age_in;
    const int &life_stage_in;
    const int &status_in;
    const int &trade_in;
    const int &education_in;
    const int &job_in;
    const int &consumption_in;
    const int &catering_expense_level_in;
    const std::vector<double> &travelpref_in;
    const std::vector<double> &carowner_in;
    const std::vector<double> &privatedriver_in;
    const std::vector<double> &drive_in;
    const std::vector<double> &carserve_in;
    const std::vector<double> &gas_in;
    const std::vector<double> &poi4s_in;
    const std::vector<double> &park_in;
    const std::vector<double> &drive_weekday_in;
    const std::vector<double> &drive_weekend_in;
    const std::vector<double> &consumption_will_in;
    const std::vector<double> &taghistyear_in;
    const std::vector<double> &taghisthour_in;
    const std::vector<double> &taghistweek_in;
} inputData;

class ModelDataInput {

    std::vector<ModelData*> _data;

    paddle_arguments _in_args = nullptr;

    enum inputSlot {
        MONTH_SLOT = 0,
        WEEK_SLOT,
        HOUR_SLOT,
        CITY_SLOT,
        PHONEOS_SLOT,
        HOMEDIST_SLOT,
        COMPDIST_SLOT,
        HOMECITY_SLOT,
        NEWCITY_SLOT,
        GENDER_SLOT,
        AGE_SLOT,
        LIFE_STAGE_SLOT,
        STATUS_SLOT,
        TRADE_SLOT,
        EDUCATION_SLOT,
        JOB_SLOT,
        CONSUMPTION_SLOT,
        CATERING_EXPENSE_LEVEL_SLOT,
        TRAVELPREF_SLOT,
        CAROWNER_SLOT,
        PRIVATEDRIVER_SLOT,
        DRIVE_SLOT,
        CARSERVE_SLOT,
        GAS_SLOT,
        POI4S_SLOT,
        PARK_SLOT,
        DRIVE_WEEKDAY_SLOT,
        DRIVE_WEEKEND_SLOT,
        CONSUMPTION_WILL_SLOT,
        TAGHISTYEAR_SLOT,
        TAGHISTHOUR_SLOT,
        TAGHISTWEEK_SLOT
    };

    // 工厂模式
    ModelData* create_model_data(const uint64_t &slotId,
                               const int &value,
                               const uint64_t &bound);

    ModelData* create_model_data(const uint64_t &slotId,
                               const std::vector<double> &inputVector,
                               const unsigned int &length);

public:
    ModelDataInput();

    ~ModelDataInput();

    paddle_arguments get_paddle_in_args(inputData &input);
};

#endif //EXEMODEL_MODELINPUT_H

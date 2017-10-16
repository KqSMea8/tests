//
// Created by Zhang,Tai on 17/10/12.
//

#include "./modelInput.h"
#include "./common.h"

class ModelData {
protected:
    uint64_t _shot_id;
public:
    ModelData(uint64_t id) : _shot_id(id) {}
    virtual void set_in_args(paddle_arguments _in_args) = 0;
};

class IntegerData : public ModelData {
    paddle_ivector _ivector;

    paddle_ivector set_integer(int inputValue,
                              uint64_t rightBoundary) {
        int value = 0;
        if (inputValue >= 0 && inputValue < rightBoundary) {
            value = inputValue;
        } else {
            //TODO: print log;如果可能，改成捕获输入异常也可以
            value = 0;
        }
        int sentence_ids[] = {value};
        paddle_ivector sentence = paddle_ivector_create(
                sentence_ids, 1, true, false);

        return sentence;
    }

    IntegerData(const IntegerData&);
    IntegerData operator=(const IntegerData&);

public:
    // 设置 integer_value 的 layer._data。输入要小于rightBoundary
    IntegerData(uint64_t id, const int &value, const uint64_t &bound) :
            ModelData(id),
            _ivector(nullptr) {
        _ivector = set_integer(value, bound);
    }
    ~IntegerData() {
        CHECK(paddle_ivector_destroy(_ivector));
    }

    void set_in_args(paddle_arguments _in_args) {
        CHECK(paddle_arguments_set_ids(_in_args, _shot_id, _ivector));
    }
};

class DenseData : public ModelData {
    paddle_matrix _matrix;
    paddle_real *_array; // matrix 的数据存储部分

    paddle_matrix set_dense_vector(const std::vector<double> &inputVector,
                                   const unsigned int &length) {
        paddle_matrix dense_vector_mat = paddle_matrix_create(1, length, false);
        CHECK(paddle_matrix_get_row(dense_vector_mat, 0, &_array));
        for (int i = 0; i < length; ++i) {
            _array[i] = (paddle_real)inputVector[i]; // 如果给出的数组太长，只取前面的部分 TODO:记录日志
        }

        return dense_vector_mat;
    }

    DenseData(const DenseData&);
    DenseData operator=(const DenseData&);

public:
    // 设置 dense_vector 的 layer._data。输入要小于rightBoundary
    DenseData(uint64_t id, const std::vector<double> &inputVector, const unsigned int &length) :
            ModelData(id),
            _matrix(nullptr),
            _array(nullptr) {
        _matrix = set_dense_vector(inputVector, length);
    }
    ~DenseData() {
        CHECK(paddle_ivector_destroy(_matrix));
    }

    void set_in_args(paddle_arguments _in_args) {
        CHECK(paddle_arguments_set_value(_in_args, _shot_id, _matrix));
    }
};

ModelDataInput::ModelDataInput() {
    _in_args = paddle_arguments_create_none();
    CHECK(paddle_arguments_resize(_in_args, 32));
}

ModelDataInput::~ModelDataInput() {
    CHECK(paddle_arguments_destroy(_in_args));
    for (int i = 0; i < _data.size(); i++) {
        delete _data[i];
    }
}

// 工厂模式管理数据
ModelData* ModelDataInput::create_model_data(const uint64_t &slotId,
                                           const int &value,
                                           const uint64_t &bound) {
    return new IntegerData(slotId, value, bound);
}

ModelData* ModelDataInput::create_model_data(const uint64_t &slotId,
                           const std::vector<double> &inputVector,
                           const unsigned int &length) {
    return new DenseData(slotId, inputVector, length);
}

// 得到模型输入参数的接口
paddle_arguments ModelDataInput::get_paddle_in_args(inputData &input) {

    _data.push_back(create_model_data(MONTH_SLOT, input.month_in, 12));
    _data.push_back(create_model_data(WEEK_SLOT, input.week_in, 7));
    _data.push_back(create_model_data(HOUR_SLOT, input.hour_in, 24));
    _data.push_back(create_model_data(CITY_SLOT, input.city_in,  3240));
    _data.push_back(create_model_data(PHONEOS_SLOT, input.phoneos_in, 3));
    _data.push_back(create_model_data(HOMEDIST_SLOT, input.homedist_in, 10));
    _data.push_back(create_model_data(COMPDIST_SLOT, input.compdist_in, 10));
    _data.push_back(create_model_data(HOMECITY_SLOT, input.homecity_in, 3));
    _data.push_back(create_model_data(NEWCITY_SLOT, input.newcity_in, 3));
    _data.push_back(create_model_data(GENDER_SLOT, input.gender_in, 3));
    _data.push_back(create_model_data(AGE_SLOT, input.age_in, 8));
    _data.push_back(create_model_data(LIFE_STAGE_SLOT, input.life_stage_in, 22));
    _data.push_back(create_model_data(STATUS_SLOT, input.status_in, 3));
    _data.push_back(create_model_data(TRADE_SLOT, input.trade_in, 24));
    _data.push_back(create_model_data(EDUCATION_SLOT, input.education_in, 4));
    _data.push_back(create_model_data(JOB_SLOT, input.job_in, 7));
    _data.push_back(create_model_data(CONSUMPTION_SLOT, input.consumption_in, 37));
    _data.push_back(create_model_data(
            CATERING_EXPENSE_LEVEL_SLOT, input.catering_expense_level_in, 4));
    _data.push_back(create_model_data(TRAVELPREF_SLOT, input.travelpref_in, 4));
    _data.push_back(create_model_data(CAROWNER_SLOT, input.carowner_in, 1));
    _data.push_back(create_model_data(PRIVATEDRIVER_SLOT, input.privatedriver_in, 1));
    _data.push_back(create_model_data(DRIVE_SLOT, input.drive_in, 3));
    _data.push_back(create_model_data(CARSERVE_SLOT, input.carserve_in, 3));
    _data.push_back(create_model_data(GAS_SLOT, input.gas_in, 3));
    _data.push_back(create_model_data(POI4S_SLOT, input.poi4s_in, 3));
    _data.push_back(create_model_data(PARK_SLOT, input.park_in, 3));
    _data.push_back(create_model_data(DRIVE_WEEKDAY_SLOT, input.drive_weekday_in, 3));
    _data.push_back(create_model_data(DRIVE_WEEKEND_SLOT, input.drive_weekend_in, 3));
    _data.push_back(create_model_data(CONSUMPTION_WILL_SLOT, input.consumption_will_in, 36));
    _data.push_back(create_model_data(TAGHISTYEAR_SLOT, input.taghistyear_in, 802));
    _data.push_back(create_model_data(TAGHISTHOUR_SLOT, input.taghisthour_in, 802*24));
    _data.push_back(create_model_data(TAGHISTWEEK_SLOT, input.taghistweek_in, 802*2));

    for (int i = 0; i < _data.size(); i++) {
        _data[i]->set_in_args(_in_args);
    }
    return _in_args;
}



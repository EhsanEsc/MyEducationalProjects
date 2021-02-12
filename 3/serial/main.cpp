#include <bits/stdc++.h>
#include <chrono>

using namespace std;

using ClassType = int;
using Feature = double;
using Sample = vector<Feature>;
using DataSet = vector<Sample>;

vector<string> split_str(string str, string delimiter)
{
    size_t pos = 0;
    vector<string> vv;
    while ((pos = str.find(delimiter)) != std::string::npos) 
    {
        vv.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    vv.push_back(str);  
    return vv;
}

tuple<vector<string>, DataSet, vector<ClassType>> read_data(string csv_path)
{
    vector<string> params_name;
    DataSet data;
    vector<ClassType> sample_types;

    string line;
    fstream file(csv_path);
    file >> line;
    params_name = split_str(line, ",");
    params_name.pop_back(); // delete type from params
    
    while(file >> line)
    {
        Sample features;
        vector<string> values = split_str(line, ",");

        sample_types.push_back(stoi(values.back())); // add type to another vector
        values.pop_back();

        assert(params_name.size() == values.size());

        for(size_t i = 0; i < values.size(); i++)
            features.push_back(stod(values[i]));

        data.push_back(features);
    }

    return make_tuple(params_name, data, sample_types);
}

DataSet normalize_data(vector<string> params_name, DataSet data)
{
    DataSet normalized_data;

    vector<double> min_value, max_value;
    min_value.resize(params_name.size());
    max_value.resize(params_name.size());

    for(size_t i=0; i<params_name.size(); i++)
    {
        for(size_t j=0; j<data.size(); j++)
        {
            Feature f = data[j][i];
            if(!j)
            {   
                min_value[i] = f;    
                max_value[i] = f;
            }
            min_value[i] = min(min_value[i], f);
            max_value[i] = max(max_value[i], f);
        }
    }

    for(Sample d : data)
    {
        Sample normalized_sample;
        for(size_t i=0; i<d.size(); i++)
        {
            Feature f = d[i];
            double new_value = 0;

            if(max_value[i] != min_value[i])
                new_value = (f - min_value[i]) / (max_value[i] - min_value[i]);
            normalized_sample.push_back(new_value);
        }
        normalized_data.push_back(normalized_sample);
    }

    return normalized_data;
}

tuple<vector<vector<double>>, vector<double>> read_weights(string weights_csv_path)
{
    vector<vector<double>> weights;
    vector<double> samples_bias;

    string line;
    fstream file(weights_csv_path);
    file >> line;

    vector<string> params_name = split_str(line, ",");
    params_name.pop_back(); // remove bias from params_name
    
    while(file >> line)
    {
        vector<double> weight;
        vector<string> values = split_str(line, ",");

        samples_bias.push_back(stod(values.back()));  // add bias to another vector
        values.pop_back();

        assert(params_name.size() == values.size());

        for(size_t i = 0; i < values.size(); i++)
            weight.push_back(stod(values[i]));

        weights.push_back(weight);
    }

    return make_tuple(weights, samples_bias);
}

double calc_score(vector<double> weight, double bias, Sample d)
{
    double score = bias;
    for(size_t i = 0; i < d.size(); i++)
        score += weight[i] * d[i];
    return score;
}

vector<int> calculate_types(DataSet data, string dataset_path)
{
    vector<int> res_types;

    vector<double> samples_bias;
    vector<vector<double>> weights;
    
    tie(weights, samples_bias) = read_weights("./" + dataset_path + "/weights.csv");

    for(Sample d : data)
    {
        int res = -1;
        double max_score = -999999999;
        for(size_t i = 0 ; i < weights.size(); i++)
        {
            double score = calc_score(weights[i], samples_bias[i], d);
            if(score > max_score)
                res = i;
            max_score = max(max_score, score);
        }
        res_types.push_back(res);
    }

    return res_types;
}

double calc_precision(vector<int> sample_types, vector<int> res_types)
{
    assert(sample_types.size() == res_types.size());

    int correct_count = 0;
    for(size_t i=0; i<res_types.size(); i++)
        correct_count += (sample_types[i] == res_types[i]);
    return 1.0*100*correct_count/res_types.size();
}

void print_elapsed(string name, long long elapsed_time, long long total_time)
{
    cout << setw(22) << name 
            << setw(7)  << elapsed_time / 1000000 << "ms" 
            << setw(10) << 100.0*elapsed_time / total_time << "%\n";
}

int main(int argc, char** argv)
{
    auto start_time = chrono::high_resolution_clock::now();

    DataSet data;
    vector<string> params_name;
    vector<ClassType> sample_types;
    string dataset_path;

    assert(argc == 2);
    dataset_path = argv[1];

    tie(params_name, data, sample_types) = read_data("./" + dataset_path + "/train.csv");
    auto read_data_time = chrono::high_resolution_clock::now();

    DataSet normalized_data = normalize_data(params_name, data);
    auto normalize_data_time = chrono::high_resolution_clock::now();

    vector<int> res_types = calculate_types(normalized_data, dataset_path);
    auto calc_types_time = chrono::high_resolution_clock::now();
    
    double precision = calc_precision(sample_types, res_types);
    auto calc_precision_time = chrono::high_resolution_clock::now();

    auto finish_time = calc_precision_time;

    cout << "Accuracy: " << fixed << setprecision(2) << precision << "%" << endl;

    if(false)
    {
        auto elapsed = finish_time - start_time;
        
        auto read_elapsed = read_data_time - start_time;
        auto normalize_elapsed = normalize_data_time - read_data_time;
        auto calc_type_elapsed = calc_types_time - normalize_data_time;
        auto calc_precision_elapsed = calc_precision_time - calc_types_time;
    
        cout << endl;
        cout << "Total Elapsed Time: " << elapsed.count() / 1000000 << " ms\n";
        cout << endl;

        print_elapsed("Read Dataset", read_elapsed.count(), elapsed.count());
        print_elapsed("Normalize Dataset", normalize_elapsed.count(), elapsed.count());
        print_elapsed("Calculate Result", calc_type_elapsed.count(), elapsed.count());
        print_elapsed("Calculate Precision", calc_precision_elapsed.count(), elapsed.count());
    }

    return 0;
}
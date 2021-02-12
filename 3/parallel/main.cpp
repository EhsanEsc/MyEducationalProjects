#include <bits/stdc++.h>
#include <chrono>
#include <pthread.h>

using namespace std;

#define NUMBER_OF_THREADS 5

using ClassType = int;
using Feature = double;
using Sample = vector<Feature>;
using DataSet = vector<Sample>;

string dataset_path;
pthread_t threads[NUMBER_OF_THREADS];
pthread_mutex_t mtx;
int ready_count;

DataSet data[NUMBER_OF_THREADS];
vector<string> params_name;
vector<ClassType> sample_types[NUMBER_OF_THREADS];

DataSet normalized_data[NUMBER_OF_THREADS];
vector<double> min_value, max_value;

vector<int> res_types[NUMBER_OF_THREADS];

int correct_count;
int total_count;

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

void* read_data(void* argv)
{
    int tid = (long)argv;
    string csv_path = dataset_path + "train_" + to_string(tid) + ".csv";

    vector<string> l_params_name;

    string line;
    fstream file(csv_path);
    file >> line;
    l_params_name = split_str(line, ",");
    l_params_name.pop_back(); // delete type from params
    
    while(file >> line)
    {
        Sample features;
        vector<string> values = split_str(line, ",");

        sample_types[tid].push_back(stoi(values.back())); // add type to another vector
        values.pop_back();

        assert(l_params_name.size() == values.size());

        for(size_t i = 0; i < values.size(); i++)
            features.push_back(stod(values[i]));

        data[tid].push_back(features);
    }

    if (tid == 0)
        params_name = l_params_name;

    pthread_exit(NULL);
}

void* normalize_data(void* argv)
{
    long tid = (long) argv;

    vector<double> l_min_value, l_max_value;
    l_min_value.resize(params_name.size());
    l_max_value.resize(params_name.size());

    for(size_t i=0; i<params_name.size(); i++)
    {
        for(size_t j=0; j<data[tid].size(); j++)
        {
            Feature f = data[tid][j][i];
            if(!j)
            {   
                l_min_value[i] = f;    
                l_max_value[i] = f;
            }
            l_min_value[i] = min(l_min_value[i], f);
            l_max_value[i] = max(l_max_value[i], f);
        }
    }

    pthread_mutex_lock(&mtx);

    if(min_value.size() == 0)
    {
        min_value = l_min_value;
        max_value = l_max_value;  
    }
    else
    {
        for(size_t i = 0; i < params_name.size(); i++)
        {
            min_value[i] = min(min_value[i], l_min_value[i]);
            max_value[i] = max(max_value[i], l_max_value[i]);           
        }
    }

    ready_count++;

    pthread_mutex_unlock (&mtx);

    while(ready_count != NUMBER_OF_THREADS);

    for(Sample d : data[tid])
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
        normalized_data[tid].push_back(normalized_sample);
    }
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

void* calculate_types(void* argv)
{
    long tid = (long)argv;

    vector<double> samples_bias;
    vector<vector<double>> weights;
    
    tie(weights, samples_bias) = read_weights("./" + dataset_path + "/weights.csv");

    for(Sample d : normalized_data[tid])
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
        res_types[tid].push_back(res);
    }
}

void* calc_precision(void* argv)
{
    long tid = (long) argv;

    assert(sample_types[tid].size() == res_types[tid].size());

    int l_correct_count = 0;
    for(size_t i=0; i<res_types[tid].size(); i++)
        l_correct_count += (sample_types[tid][i] == res_types[tid][i]);
    
    pthread_mutex_lock(&mtx);

    correct_count += l_correct_count;
    total_count += res_types[tid].size();

    pthread_mutex_unlock(&mtx);
}

void print_elapsed(string name, long long elapsed_time, long long total_time)
{
    cout << setw(22) << name 
            << setw(7)  << elapsed_time / 1000000 << "ms" 
            << setw(10) << 100.0*elapsed_time / total_time << "%\n";
}

void run_parallel(void *(routine) (void *))
{
    for(long tid = 0; tid < NUMBER_OF_THREADS; tid++)
        pthread_create(&threads[tid], NULL, routine, (void*)tid);

    for(long tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
        void* status;
		pthread_join(threads[tid], &status);
    }
}

int main(int argc, char** argv)
{
    pthread_mutex_init(&mtx, NULL);

    auto start_time = chrono::high_resolution_clock::now();

    assert(argc == 2);
    dataset_path = argv[1];

    // tie(params_name, data, sample_types) = read_data("./" + dataset_path + "/train.csv");
    run_parallel(read_data);
    auto read_data_time = chrono::high_resolution_clock::now();

    // DataSet normalized_data = normalize_data(params_name, data);
    run_parallel(normalize_data);
    auto normalize_data_time = chrono::high_resolution_clock::now();

    // vector<int> res_types = calculate_types(normalized_data, dataset_path);
    run_parallel(calculate_types);
    auto calc_types_time = chrono::high_resolution_clock::now();
    
    // double precision = calc_precision(sample_types, res_types);
    run_parallel(calc_precision);
    double precision = 1.0*100*correct_count/total_count;
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

    pthread_mutex_destroy(&mtx);

    return 0;
}
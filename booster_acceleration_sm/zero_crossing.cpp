#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>

using namespace std;

vector<float> vector_sin(vector<float> data){
    vector<float> result;
    result.reserve(data.size());
    for (auto & element: data){
        result.push_back(sin(element));
    }
    return result;
}

vector<float> vector_cos(vector<float> data){
    vector<float> result;
    result.reserve(data.size());
    for (auto & element: data){
        result.push_back(cos(element));
    }
    return result;
}

vector<float> zero_crossing(vector<float> data, float harmonic_hf, int offset, float start_period){
    int iteration = 0;
    float period = start_period;
    float i = harmonic_hf;

    vector<float> amplitudes = {};
    vector<float> phases = {};
    vector<float> other = {};

    while (offset < (data.size() - floor(period) - 1)){
        float phase = 2 * M_PI * harmonic_hf / period;
        period = int(period);

        vector<float> harmonic_vector = {0, 0, 0};
        for (int i=0; i < offset+period+1; ++i){
            harmonic_vector[0] += data[n+i]
        }
        harmonic_vector[0] = sum(data)

        amplitudes.push_back(1.1);
        phases.push_back(1.1);
        other.push_back(1.1);

        period = iteration > 12? 1/10 * phases[iteration] - phases[iteration-10]: period;
        iteration++;
        i++;
    }

    return data;
}

int main(){
    vector<float> vec = {1 ,2 * M_PI ,3 ,5 ,6 ,7 ,1};
    cout<<zero_crossing(vec, 1.2, 3, 4.1)[0];
    cout<<vector<float>(vec.begin(), vec.end()-5).size();
    return 0;
}
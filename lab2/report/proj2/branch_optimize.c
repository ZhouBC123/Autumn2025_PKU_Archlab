/*
 * Arch 2025 fall course code.
 * - This is an example C code for weather data analysis with branch optimization challenges.
 * - The weather data is directly included to avoid the reading process for a
 * quicker simulation.
 * - This program will analyze weather statistics including temperature, humidity,
 * wind speed, and weather conditions across different cities.
 * - This program contains suboptimal branching patterns that students should optimize
 * to improve Branch Predictor performance and reduce branch misprediction penalties.
 * - Students are expected to identify and optimize conditional logic, data access patterns,
 * and control flow to achieve better performance in gem5 simulation.
 * - Optimization targets: reduce branch mispredictions, 
 * and minimize pipeline stalls through better code structure.
 */
#include "branch_optimize.in"

volatile int los_rainy_days = 0;
volatile int condition_count[5] = {0}; // SUNNY, CLOUDY, RAINY, STORMY, SNOWY
volatile int city_count[5] = {0}; // NEW_YORK, LOS_ANGELES, CHICAGO, HOUSTON, MIAMI
volatile int wind_scale_count[12] = {0}; // 0,1,2,3,4,5,6,7,8,9,10,>10
volatile int max_abs_temp = 0; // max absolute temperature for each city
volatile float max_temp = -100;
volatile float min_temp = 100;

volatile int low_humidity_count = 0;
volatile int high_temperature_count = 0;
volatile int valid_data_count = 0;
volatile int year_count = 0;
volatile int month_count = 0;
volatile int day_count = 0;

/*   START position of editable area    */

int min(int x, int y) {
    return y ^ ((x ^ y) & -(x < y));
}

int max(int x, int y) {
    return x ^ ((x ^ y) & -(x < y));
}

int abs(int x) {
    int const mask = x >> (sizeof(int) * 8 - 1);
    return (x + mask) ^ mask;
}


void statistics(WeatherData data[], int n) {
    
    for(int i = 0; i < n; i++) {
        if(data[i].temperature < -70 || data[i].temperature > 60)
            continue;

        if(data[i].windSpeed < 0 || data[i].windSpeed > 150)
            continue;

        if(data[i].humidity < 0 || data[i].humidity > 100)
            continue;

        if(data[i].day < 1 || data[i].day > 31)
            continue;

        if(data[i].month > 12 || data[i].month < 1)
            continue;

        if(data[i].year < 1900 || data[i].year > 2025)
            continue;

        valid_data_count++;

        if(data[i].year >= 2012 && data[i].year < 2024)
            year_count++;
        if(data[i].month > 6 && data[i].month < 8)
            month_count++;
        if(data[i].day == 15)
            day_count++;

        switch(data[i].city) {
            case NEW_YORK:
                city_count[0]++;
                break;
            case LOS_ANGELES:
                if(data[i].condition == RAINY)
                    los_rainy_days++;
                city_count[1]++;
                break;
            case CHICAGO:
                city_count[2]++;
                break;                
            case HOUSTON:
                city_count[3]++;
                break;
            case MIAMI:
                city_count[4]++;
                break;
            default:
                break;
        }

        switch(data[i].condition) {
            case SUNNY:
                condition_count[0]++;
                break;
            case CLOUDY:
                condition_count[1]++;
                break;
            case RAINY:
                condition_count[2]++;
                break;
            case STORMY:
                condition_count[3]++;
                break;
            case SNOWY:
                condition_count[4]++;
                break;
            default:
                break;
        }

        if(data[i].windSpeed > 28.4)
            wind_scale_count[11]++;
        else if(data[i].windSpeed > 24.4)
            wind_scale_count[10]++;
        else if(data[i].windSpeed > 20.7)
            wind_scale_count[9]++;
        else if(data[i].windSpeed > 17.1)
            wind_scale_count[8]++;
        else if(data[i].windSpeed > 13.8)
            wind_scale_count[7]++;
        else if(data[i].windSpeed > 10.7)
            wind_scale_count[6]++;
        else if(data[i].windSpeed > 7.9)
            wind_scale_count[5]++;
        else if(data[i].windSpeed > 5.4)
            wind_scale_count[4]++;
        else if(data[i].windSpeed > 3.3)
            wind_scale_count[3]++;
        else if(data[i].windSpeed > 1.5)
            wind_scale_count[2]++;
        else if(data[i].windSpeed > 0.2)
            wind_scale_count[1]++;
        else
            wind_scale_count[0]++;

        max_temp = max(max_temp, data[i].temperature);
        min_temp = min(min_temp, data[i].temperature);

        max_abs_temp = max(max_abs_temp, abs(data[i].temperature));

        if(data[i].humidity < 50)
            low_humidity_count++;

        if(data[i].temperature > 30)
            high_temperature_count++;
        data[i].is_scanned = 1;
    }
}

/*    END position of editable area     */

int main() {
    statistics(WEATHER_DATA, N);
    return 0;
}
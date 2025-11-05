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

// 原始实现（已注释，供对比）
// int min(int x, int y) {
//     return y ^ ((x ^ y) & -(x < y));
// }
// 
// int max(int x, int y) {
//     return x ^ ((x ^ y) & -(x < y));
// }
// 
// int abs(int x) {
//     int const mask = x >> (sizeof(int) * 8 - 1);
//     return (x + mask) ^ mask;
// }
// 
// void statistics(WeatherData data[], int n) {
//     
//     for(int i = 0; i < n; i++) {
//         if(data[i].temperature < -70 || data[i].temperature > 60)
//             continue;
// 
//         if(data[i].windSpeed < 0 || data[i].windSpeed > 150)
//             continue;
// 
//         if(data[i].humidity < 0 || data[i].humidity > 100)
//             continue;
// 
//         if(data[i].day < 1 || data[i].day > 31)
//             continue;
// 
//         if(data[i].month > 12 || data[i].month < 1)
//             continue;
// 
//         if(data[i].year < 1900 || data[i].year > 2025)
//             continue;
// 
//         valid_data_count++;
// 
//         if(data[i].year >= 2012 && data[i].year < 2024)
//             year_count++;
//         if(data[i].month > 6 && data[i].month < 8)
//             month_count++;
//         if(data[i].day == 15)
//             day_count++;
// 
//         switch(data[i].city) {
//             case NEW_YORK:
//                 city_count[0]++;
//                 break;
//             case LOS_ANGELES:
//                 if(data[i].condition == RAINY)
//                     los_rainy_days++;
//                 city_count[1]++;
//                 break;
//             case CHICAGO:
//                 city_count[2]++;
//                 break;                
//             case HOUSTON:
//                 city_count[3]++;
//                 break;
//             case MIAMI:
//                 city_count[4]++;
//                 break;
//             default:
//                 break;
//         }
// 
//         switch(data[i].condition) {
//             case SUNNY:
//                 condition_count[0]++;
//                 break;
//             case CLOUDY:
//                 condition_count[1]++;
//                 break;
//             case RAINY:
//                 condition_count[2]++;
//                 break;
//             case STORMY:
//                 condition_count[3]++;
//                 break;
//             case SNOWY:
//                 condition_count[4]++;
//                 break;
//             default:
//                 break;
//         }
// 
//         if(data[i].windSpeed > 28.4)
//             wind_scale_count[11]++;
//         else if(data[i].windSpeed > 24.4)
//             wind_scale_count[10]++;
//         else if(data[i].windSpeed > 20.7)
//             wind_scale_count[9]++;
//         else if(data[i].windSpeed > 17.1)
//             wind_scale_count[8]++;
//         else if(data[i].windSpeed > 13.8)
//             wind_scale_count[7]++;
//         else if(data[i].windSpeed > 10.7)
//             wind_scale_count[6]++;
//         else if(data[i].windSpeed > 7.9)
//             wind_scale_count[5]++;
//         else if(data[i].windSpeed > 5.4)
//             wind_scale_count[4]++;
//         else if(data[i].windSpeed > 3.3)
//             wind_scale_count[3]++;
//         else if(data[i].windSpeed > 1.5)
//             wind_scale_count[2]++;
//         else if(data[i].windSpeed > 0.2)
//             wind_scale_count[1]++;
//         else
//             wind_scale_count[0]++;
// 
//         max_temp = max(max_temp, data[i].temperature);
//         min_temp = min(min_temp, data[i].temperature);
// 
//         max_abs_temp = max(max_abs_temp, abs(data[i].temperature));
// 
//         if(data[i].humidity < 50)
//             low_humidity_count++;
// 
//         if(data[i].temperature > 30)
//             high_temperature_count++;
//         data[i].is_scanned = 1;
//     }
// }

/* 优化实现：使用局部累加器、合并验证检查、索引化城市处理、阈值数组判断风速档位 */
void statistics(WeatherData data[], int n) {
    // 局部计数器（避免频繁访问 volatile 全局）
    int loc_los_rainy = 0;
    int loc_condition_count[5] = {0}; // SUNNY, CLOUDY, RAINY, STORMY, SNOWY
    int loc_city_count[5] = {0}; // NEW_YORK..MIAMI
    int loc_wind_scale_count[12] = {0}; // 0..11
    int loc_max_abs_temp = 0;
    float loc_max_temp = -100.0f;
    float loc_min_temp = 100.0f;

    int loc_low_humidity_count = 0;
    int loc_high_temperature_count = 0;
    int loc_valid_data_count = 0;
    int loc_year_count = 0;
    int loc_month_count = 0;
    int loc_day_count = 0;

    // 风速阈值（升序），用于确定档位（index 0 表示 <=0.2）
    const float wind_thresholds[11] = {
        0.2f, 1.5f, 3.3f, 5.4f, 7.9f, 10.7f, 13.8f, 17.1f, 20.7f, 24.4f, 28.4f
    };

    for (int i = 0; i < n; ++i) {
        WeatherData *d = &data[i];

        // 将多次边界检查合并为一个无短路位运算表达式（保证所有比较都被计算，减少分支数量与短路带来的预测不稳定）
        int temp_out = (d->temperature < -70.0f) | (d->temperature > 60.0f);
        int wind_out = (d->windSpeed < 0.0f) | (d->windSpeed > 150.0f);
        int hum_out = (d->humidity < 0.0f) | (d->humidity > 100.0f);
        int day_out = (d->day < 1) | (d->day > 31);
        int month_out = (d->month < 1) | (d->month > 12);
        int year_out = (d->year < 1900) | (d->year > 2025);

        if (temp_out | wind_out | hum_out | day_out | month_out | year_out)
            continue; // 单一分支决定数据合法性

        // 数据合法，使用局部计数器累加
        loc_valid_data_count++;

        // 简洁的布尔算术，分支更可预测
        loc_year_count += (d->year >= 2012 && d->year < 2024);
        loc_month_count += (d->month > 6 && d->month < 8);
        loc_day_count += (d->day == 15);

        // 城市处理：用索引方式减少 switch 的分支抖动
        int cidx = ((int)d->city) - 1; // NEW_YORK=1 -> 0
        if ((unsigned)cidx < 5u) {
            loc_city_count[cidx]++;
            // Los Angeles rainy days: cidx==1 corresponds to LOS_ANGELES
            loc_los_rainy += (cidx == 1 && d->condition == RAINY);
        }

        // condition -> 索引映射（小范围判断，分支局部且可预测）
        int cond_idx = -1;
        if (d->condition == SUNNY) cond_idx = 0;
        else if (d->condition == CLOUDY) cond_idx = 1;
        else if (d->condition == RAINY) cond_idx = 2;
        else if (d->condition == STORMY) cond_idx = 3;
        else if (d->condition == SNOWY) cond_idx = 4;

        if (cond_idx >= 0)
            loc_condition_count[cond_idx]++;

        // 风速分档：通过阈值数组和小循环计算档位，避免长串 else-if
        int widx = 0;
        // 依次比较，通常会在较低索引处提前跳出（对常见风速友好）
        while (widx < 11 && d->windSpeed > wind_thresholds[widx]) ++widx;
        loc_wind_scale_count[widx]++;

        // 温度最大/最小更新（直接比较）
        float t = d->temperature;
        if (t > loc_max_temp) loc_max_temp = t;
        if (t < loc_min_temp) loc_min_temp = t;

        // 最大绝对温度（以整数温度为基准，与原实现语义一致）
        int ti = (int)t;
        int abs_ti = abs(ti);
        if (abs_ti > loc_max_abs_temp) loc_max_abs_temp = abs_ti;

        // 湿度、极端高温计数
        loc_low_humidity_count += (d->humidity < 50.0f);
        loc_high_temperature_count += (t > 30.0f);

        d->is_scanned = 1;
    }

    // 最后一次性写回 volatile 全局变量，减少内存写压力
    los_rainy_days = loc_los_rainy;
    for (int i = 0; i < 5; ++i) condition_count[i] = loc_condition_count[i];
    for (int i = 0; i < 5; ++i) city_count[i] = loc_city_count[i];
    for (int i = 0; i < 12; ++i) wind_scale_count[i] = loc_wind_scale_count[i];

    max_abs_temp = loc_max_abs_temp;
    max_temp = loc_max_temp;
    min_temp = loc_min_temp;

    low_humidity_count = loc_low_humidity_count;
    high_temperature_count = loc_high_temperature_count;
    valid_data_count = loc_valid_data_count;
    year_count = loc_year_count;
    month_count = loc_month_count;
    day_count = loc_day_count;
}

/*    END position of editable area     */

int main() {
    statistics(WEATHER_DATA, N);
    return 0;
}
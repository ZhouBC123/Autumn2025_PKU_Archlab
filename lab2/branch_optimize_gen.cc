/*
 * Arch 205 fall course code.
 * - This is a C++ code generating arrays for branch_optimize algorithm.
 * - This program will generate an array of weather data.
 */

#include "branch_optimize.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "<file name> <number of weather data>"
                  << std::endl;
        return 1;
    }

    std::ofstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    int n = std::atoi(argv[2]);
    std::random_device rd;
    unsigned seed = 42;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> abnormal_dist(1, 100000);
    std::uniform_int_distribution<int> year_dist(2000, 2024);
    std::uniform_int_distribution<int> month_dist(1, 12);
    std::uniform_int_distribution<int> day_dist(1, 28);
    std::normal_distribution<float> temp_dist(20.0, 5.0);
    std::normal_distribution<float> humidity_dist(50.0, 5.0);
    std::uniform_int_distribution<int> city_dist(1, 8);
    std::uniform_int_distribution<int> condition_dist(1, 8);
    std::normal_distribution<> normal_dist(0.0, 8);
    std::uniform_int_distribution<int> random_dist(0, 1);

    int error_pos = 0;
    int lager_50 = 0;
    int lager_20 = 0;
    int is_rain = 0;

    file << "#include \"branch_optimize.h\"" << std::endl;
    file << "#define N " << n << std::endl;
    file << "WeatherData WEATHER_DATA[" << n << "] = {" << std::endl;
    for (int i = 0; i < n; i++) {
        file << "  {";
        bool is_error = abnormal_dist(gen) > 99995;
        if(is_error && error_pos == 0) {
            if(random_dist(gen) == 0) {
                file << 2026 << ", ";
            } else {
                file << 1899 << ", ";
            }
        } else {
            file << year_dist(gen) << ", ";
        }
        if(is_error && error_pos == 1) {
            if(random_dist(gen) == 0) {
                file << 0 << ", ";
            } else {
                file << 13 << ", ";
            }
        } else {
            file << month_dist(gen) << ", ";
        }
        if(is_error && error_pos == 2) {
            if(random_dist(gen) == 0) {
                file << 0 << ", ";
            } else {
                file << 32 << ", ";
            }
        } else {
            file << day_dist(gen) << ", ";
        }
        if(is_error && error_pos == 3) {
            if(random_dist(gen) == 0) {
                file << -71 << ", ";
            } else {
                file << 61 << ", ";
            }
        } else {
            float temp = temp_dist(gen);
            int tempterature = std::round(temp);
            if(lager_20 < 1 && tempterature < 20)
                tempterature = 40 - tempterature;
            if(lager_20 > 0 && tempterature >= 20)
                tempterature = 40 - tempterature;
            lager_20++;
            lager_20 = lager_20 % 2;
            file << tempterature << ", ";
        }
        int temp_condition = condition_dist(gen);
        int temp_city = city_dist(gen);
        bool is_los = false;
        if(temp_city == 2 || temp_city == 6 || temp_city == 7 || temp_city == 8)
            is_los = true;
        if(is_los){
            if(is_rain < 3) {
                file << "RAINY, ";
            } else {
                switch(temp_condition) {
                    case 1:
                    case 8:
                        file << "SUNNY";
                        break;
                    case 2:
                    case 4:
                        file << "CLOUDY";
                        break;
                    case 3:
                    case 6:
                        file << "STORMY";
                        break;
                    case 5:
                    case 7:
                        file << "SNOWY";
                        break;
                    default:
                        exit(1);
                }
                file << ", ";
            }
            is_rain++;
            is_rain = is_rain % 4;
        }
        else {
            switch(temp_condition) {
                case 1:
                case 4:
                case 6:
                case 7:
                case 3:
                    file << "SUNNY";
                    break;
                case 8:
                    file << "STORMY";
                    break;
                case 2:
                    file << "CLOUDY";
                    break;
                case 5:
                    file << "SNOWY";
                    break;
                default:
                    exit(1);
            }
            file << ", ";
        }
        switch(temp_city) {
            case 1:
                file << "NEW_YORK";
                break;
            case 2:
            case 6:
            case 7:
            case 8:
                file << "LOS_ANGELES";
                break;
            case 3:
                file << "CHICAGO";
                break;
            case 4:
                file << "HOUSTON";
                break;
            case 5:
                file << "MIAMI";
                break;
            default:
                exit(1);
        }
        file << ", ";

        if(is_error && error_pos == 4) {
            if(random_dist(gen) == 0) {
                file << -0.2 << ", ";
            } else {
                file << 101.1 << ", ";
            }
        } else {
            float humidity = humidity_dist(gen);
            if(lager_50 < 1 && humidity < 50.0)
                humidity = 100 - humidity;
            if(lager_50 > 0 && humidity >= 50.0)
                humidity = 100 - humidity;
            lager_50 ++;
            lager_50 = lager_50 % 2;
            file << humidity << ", ";
        }
        float temp_x = normal_dist(gen);
        float temp_y = normal_dist(gen);
        float wind_speed = std::sqrt(temp_x * temp_x + temp_y * temp_y);
        if(is_error && error_pos == 5) {
            if(random_dist(gen) == 0) {
                file << -0.2 << ", ";
            } else {
                file << 150.7 << ", ";
            }
        } else {
            file << wind_speed << ", ";
        }
        file << "0";
        file << "}";
        if (i < n - 1) 
            file << ",";
        file << std::endl;

        if(is_error)
        {
            error_pos++;
            error_pos = error_pos % 6;
        }
    }
    file << "};" << std::endl;
    file.close();

    return 0;
}

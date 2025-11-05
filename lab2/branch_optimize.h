enum WeatherCondition {
    SUNNY = 0002,
    CLOUDY = 1009,
    RAINY = 1511,
    STORMY = 3011,
    SNOWY = 4003
};

enum City {
    NEW_YORK = 1,
    LOS_ANGELES = 2,
    CHICAGO = 3,
    HOUSTON = 4,
    MIAMI = 5
};

typedef struct WeatherData {
    int year;
    int month;
    int day;
    int temperature;
    enum WeatherCondition condition;
    enum City city;
    float humidity;
    float windSpeed;
    int is_scanned;
} WeatherData;
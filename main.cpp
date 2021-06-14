#include <iostream>

const int T_TUBE_HEATING_ON = 0;
const int T_TUBE_HEATING_OFF = 5;
const int T_HOUSE_HEATING_ON = 22;
const int T_HOUSE_HEATING_OFF = 25;

const int HOUR_EVENING = 16;
const int HOUR_MORNING = 5;

enum switches
{
    ALL_OFF = 0,
    MAIN_SWITCH = 1,
    SOCKETS = 2,
    LIGHT_INSIDE = 4,
    LIGHT_OUTSIDE = 8,
    HEATING_INSIDE = 16,
    HEATING_TUBES = 32,
    CONDITIONER = 64
};

enum positions
{
    OFF = 0,
    ON = 1
};

int switches_actual = ALL_OFF;



bool IsNight(int hour)
{
    return (hour > HOUR_EVENING || hour < HOUR_MORNING);
}

void SetSwitchPosition(switches switch_name, positions position)
{
    if (position == ON)
    {
        switches_actual |= switch_name;
    }
    else if (position == OFF)
    {
        switches_actual &= ~switch_name;
    }
}

bool GetSwitchPosition(switches switch_name)
{
    return switches_actual & switch_name;
}

int main() {
    double temperature_outside;
    double temperature_inside;
    bool movement = false;

    int time_hours = 0;



    //TUBES HEATING LOGIC:
    if (!(switches_actual & HEATING_TUBES) &&
        temperature_outside < T_TUBE_HEATING_ON)
    {
        switches_actual |= HEATING_TUBES;
    }

    else if (switches_actual & HEATING_TUBES &&
            temperature_outside > T_TUBE_HEATING_OFF)
    {
        switches_actual  &= ~HEATING_TUBES;
    }


    //OUTSIDE LIGHT LOGIC:
    if (IsNight(time_hours) && movement)
    {
        if (!GetSwitchPosition(LIGHT_OUTSIDE))
            SetSwitchPosition(LIGHT_OUTSIDE, ON);
    }
    else
    {
        if (GetSwitchPosition(LIGHT_OUTSIDE))
            SetSwitchPosition(LIGHT_OUTSIDE, OFF);
    }


    //INSIDE HEATING LOGIC:
    if (temperature_inside < T_HOUSE_HEATING_ON)
        switches_actual |= HEATING_INSIDE;
    else if (switches_actual & HEATING_INSIDE && temperature_inside >= T_HOUSE_HEATING_OFF)





    std::cout << state;


    return 0;
}

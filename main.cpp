#include <iostream>
#include <sstream>

//Temperature constants:
const int T_TUBES_MINIMAL = 0;
const int T_TUBES_NORMAL = 5;
const int T_HOUSE_MINIMAL = 22;
const int T_HOUSE_NORMAL = 25;
const int T_HOUSE_MAXIMAL = 30;

//Hours constants:
const int HOUR_EVENING_START = 16;
const int HOUR_MORNING_START = 5;
const int HOUR_LIGHT_COLOR_RESET = 0;
const int HOUR_EVENING_END = 20;

//Light colors constants:
const int HIGHEST_LIGHT_COLOR = 5000;
const int LOWEST_LIGHT_COLOR = 2700;

//Switch panel masks:
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

//Switch positions:
enum positions
{
    OFF = 0,
    ON = 1
};

//Globals:
int switches_actual = ALL_OFF;
int light_color = HIGHEST_LIGHT_COLOR;
//---------------------------------------------------------------------------------

bool CheckIfNight(int hour)
{
    return (hour > HOUR_EVENING_START || hour < HOUR_MORNING_START);
}

bool GetSwitchPosition(switches switch_name)
{
    return switches_actual & switch_name;
}

void PrintSwitchStatus(switches switch_name)
{
    std::string switch_name_string;
    if (switch_name == MAIN_SWITCH) switch_name_string = "MAIN SWITCH";
    else if (switch_name == SOCKETS) switch_name_string = "SOCKETS";
    else if (switch_name == LIGHT_INSIDE) switch_name_string = "LIGHT IN HOUSE";
    else if (switch_name == LIGHT_OUTSIDE) switch_name_string = "LIGHT IN GARDEN";
    else if (switch_name == HEATING_INSIDE) switch_name_string = "HOUSE HEATING";
    else if (switch_name == HEATING_TUBES) switch_name_string = "TUBES HEATING";
    else if (switch_name == CONDITIONER) switch_name_string = "CONDITIONER";

    std::string position_string;
    if (GetSwitchPosition(switch_name)  == ON) position_string = "ON";
    else if (GetSwitchPosition(switch_name) == OFF) position_string = "OFF";

    std::cout << switch_name_string << " is " << position_string << "\n";
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

    PrintSwitchStatus(switch_name);
}

void PrintLightColor(int color)
{
    printf("Light color temperature is: %i K\n", color);
}

void ImplementTubesHeatingLogic(int t_outside)
{
    if (GetSwitchPosition(HEATING_TUBES))
    {
        if (t_outside > T_TUBES_NORMAL)
            SetSwitchPosition(HEATING_TUBES, OFF);
    }
    else
    {
        if (t_outside < T_TUBES_MINIMAL)
            SetSwitchPosition(HEATING_TUBES, ON);
    }
}

void ImplementOutsideLightLogic(bool movement, int time_h)
{
    if (GetSwitchPosition(LIGHT_OUTSIDE))
    {
        if (!movement || !CheckIfNight(time_h))
            SetSwitchPosition(LIGHT_OUTSIDE, OFF);
    }
    else
    {
        if (movement && CheckIfNight(time_h))
            SetSwitchPosition(LIGHT_OUTSIDE, ON);
    }
}

void ImplementInsideLightLogic(int time_h)
{
    if (GetSwitchPosition(LIGHT_INSIDE))
    {
        if (time_h > HOUR_EVENING_START && time_h <= HOUR_EVENING_END)
        {
            light_color -= (HIGHEST_LIGHT_COLOR - LOWEST_LIGHT_COLOR) / (HOUR_EVENING_END - HOUR_EVENING_START);
        }
        else if (time_h == HOUR_LIGHT_COLOR_RESET)
        {
            light_color = HIGHEST_LIGHT_COLOR;
        }
        PrintLightColor(light_color);
    }
}

void ImplementInsideHeatingLogic(int t_inside)
{
    if (GetSwitchPosition(HEATING_INSIDE))
    {
        if (t_inside >= T_HOUSE_NORMAL)
            SetSwitchPosition(HEATING_INSIDE, OFF);
    }
    else
    {
        if (t_inside < T_HOUSE_MINIMAL)
            SetSwitchPosition(HEATING_INSIDE, ON);
    }
}

void ImplementConditionerLogic(int t_inside)
{
    if (GetSwitchPosition(CONDITIONER))
    {
        if (t_inside <= T_HOUSE_NORMAL)
            SetSwitchPosition(CONDITIONER, OFF);
    }
    else
    {
        if (t_inside >= T_HOUSE_MAXIMAL)
            SetSwitchPosition(CONDITIONER, ON);
    }
}

void PrintSensorsState(int t_inside, int t_outside, bool movements)
{
    printf("Temperature outside: %i C\n", t_outside);
    printf("Temperature in house: %i C\n", t_inside);
    printf("Movements: %i\n", movements);
}

int main() {
    int temperature_outside = 0;
    int temperature_inside = 0;
    bool movement;

    int day = 1;
    int time_hour = 0;
    const int SIMULATION_DAYS = 2;

    while (day <= SIMULATION_DAYS)
    {
        //Print day and hour:
        printf("Day %i, ", day);
        printf("%i:00: ", time_hour);

        //Input:
        std::string buffer_string;
        std::string movement_string;
        std::string light_inside_string;
        std::cin >> temperature_outside >> temperature_inside >> movement_string >> light_inside_string ;
        movement = (movement_string == "yes");

        //Print sensors state:
        PrintSensorsState(temperature_inside, temperature_outside, movement);

        //Check inside light state:
        if (light_inside_string == "on" && !GetSwitchPosition(LIGHT_INSIDE))
            SetSwitchPosition(LIGHT_INSIDE, ON);
        else if (light_inside_string != "on" && GetSwitchPosition(LIGHT_INSIDE))
            SetSwitchPosition(LIGHT_INSIDE, OFF);

        //Automated logic:
        ImplementInsideLightLogic(time_hour);
        ImplementTubesHeatingLogic(temperature_outside);
        ImplementOutsideLightLogic(movement, time_hour);
        ImplementInsideHeatingLogic(temperature_inside);
        ImplementConditionerLogic(temperature_inside);

        //Hours increment:
        time_hour++;
        if (time_hour > 23)
        {
            time_hour = 0;
            day++;
        }
    }
    std::cout << "\n----------------------\n";
    std::cout << "Simulation is over.\n";

    return 0;
}

// USED FOR TEST:
//----------------------------
/* TUBE HEATING TEST:
 * 0 22 no no
 * -1 22 no no
 * 5 22 no no
 * 6 22 no no
 */

/* HOUSE HEATING TEST:
 *  0 23 no on
 *  0 22 no on
 *  0 21 no on
 *  0 24 no on
 *  0 25 no on
 */

/* CONDITIONER TEST:
 *  0 29 no on
 *  0 30 no on
 *  0 26 no on
 *  0 25 no on
 */

/* LIGHT INSIDE TEST:
 * 16 23 no on
 */

/* LIGHT OUTSIDE TEST
 *  16 23 no off
 *  16 23 yes off
 */

/* MULTIPLE CONDITIONS TEST:
 * -1 0 yes on
 */
//---------------------------------
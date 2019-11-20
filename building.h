#ifndef BUILDING_DETAILS
#define BUILDING_DETAILS

class BuildingDetails
{
public:
    int buildingNum;
    int executed_time;
    int total_time;

    BuildingDetails(int buildingNum, int executed_time, int total_time) {
        this->buildingNum = buildingNum;
        this->executed_time = executed_time;
        this->total_time = total_time;
    }

    bool operator<(BuildingDetails other) const {
        if (executed_time == other.executed_time) {
            return buildingNum < other.buildingNum;
        } else {
            return executed_time < other.executed_time;
        }
    }
};

#endif
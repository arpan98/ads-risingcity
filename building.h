#ifndef BUILDING_DETAILS
#define BUILDING_DETAILS

// Building Details class to hold the parameters for each building
class BuildingDetails
{
public:
    int buildingNum;
    int executed_time;
    int total_time;

    // Constructor for BuildingDetails class
    BuildingDetails(int buildingNum, int executed_time, int total_time) {
        this->buildingNum = buildingNum;
        this->executed_time = executed_time;
        this->total_time = total_time;
    }

    // Custom < operator overload for BuildingDetails
    // Compares executed_time first, breaks ties with buildingNum
    bool operator<(BuildingDetails other) const {
        if (executed_time == other.executed_time) {
            return buildingNum < other.buildingNum;
        } else {
            return executed_time < other.executed_time;
        }
    }
};

#endif
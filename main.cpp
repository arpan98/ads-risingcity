#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

#include "building.h"
#include "myheap.cpp"
#include "rbtree.cpp"

// Enumeration for the 3 input command types
enum Command {
    insert,
    print1,
    print2
};

// Returns tuple (time, command, args) parsed from input line
std::tuple<int, Command, std::vector<int>> parse_input_line(std::string);

// Takes as input the parsed input line and executes print/insert operations
void execute_command(std::tuple<int, Command, std::vector<int>>, MyHeap&, RBTree&);

// Main driver function for the program
int main(int argc, char const *argv[])
{   
    // Input file name read from argument
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::ofstream output_file;
    output_file.open("output_file.txt");
    std::string input_line;
    
    int time = 0;
    BuildingDetails dummy = {-1, 0, -1};
    BuildingDetails *curr_bd = &dummy;
    int work_done = -1, rem_time = 0, initial_time = 0;

    MyHeap min_heap;
    RBTree rbtree;

    // Open input file and read first line
    if (input_file.is_open()) {
        getline(input_file, input_line);
        auto command_tuple = parse_input_line(input_line);
        int next_time = std::get<0>(command_tuple);
        // While loop to take input and execute commands, and extract and reinsert into heap as necessary
        while (!input_file.eof() || time <= next_time || rem_time >= 0 || !min_heap.is_empty()) {
            // When the current time matches time for the next command to be read
            if (time == next_time) {
                execute_command(command_tuple, min_heap, rbtree);
                getline(input_file, input_line);
                command_tuple = parse_input_line(input_line);
                next_time = std::get<0>(command_tuple); 
            }
            // If the remaining time for the current work period (of 5 days max) reaches 0
            if (rem_time == 0) {
                // If the current building is done and its exec_time == total_time
                // Then write the building to output file and delete it from RB Tree
                if (curr_bd->total_time == curr_bd->executed_time) {
                    std::ofstream output("output_file.txt", std::ios_base::app | std::ios_base::out);
                    output<<"("<<curr_bd->buildingNum<<","<<time<<")\n";
                    rbtree.deleteBuilding(curr_bd->buildingNum);
                } 
                // Otherwise insert it back into the min heap
                else if (work_done > 0) {
                    min_heap.insert(curr_bd);
                }
                // Break loop when End of file is reached, there are no more commands to execute and the min heap is empty
                if (input_file.eof() && time > next_time && min_heap.is_empty())
                    break;
                // If the min heap is empty, set as dummy building instead of extractmin
                if (min_heap.is_empty()) {
                    curr_bd = &dummy;
                }
                // Extract min from heap and set remaining work time
                else {
                    curr_bd = min_heap.extract_min();
                    initial_time = curr_bd->executed_time;
                    if (curr_bd->total_time - curr_bd->executed_time < 5) {
                        rem_time = curr_bd->total_time - curr_bd->executed_time;
                    } else {
                        rem_time = 5;
                    }
                }
                work_done = 0;
            }
            // If an actual building is being worked on, increment/decrement relevant variables
            if (curr_bd->buildingNum != dummy.buildingNum) {
                work_done++;
                curr_bd->executed_time = initial_time + work_done;
                rem_time--;
            }
            time++;
        }
    }
    input_file.close();
    return 0;
}

// Function takes input line, parses it and returns a tuple (time, command, arguments)
std::tuple<int, Command, std::vector<int>> parse_input_line(std::string input_line) {
    Command c;
    std::vector<int> args;
    // Split by colon : to get time
    std::string word = input_line.substr(0, input_line.find(":"));
    int time = std::stoi(word);
    
    // Split by parantheses (, ) and comma , to parse the command and it's arguments
    size_t start_index = input_line.find("(");
    size_t end_index = input_line.find(")");
    std::string arg_str = input_line.substr(start_index+1, end_index - start_index - 1);
    size_t arg_split = arg_str.find(",");
    if (arg_split != std::string::npos) {
        std::string arg = arg_str.substr(0, arg_split);
        args.push_back(std::stoi(arg));
        arg = arg_str.substr(arg_split + 1);
        args.push_back(std::stoi(arg));
    } else {
        args.push_back(std::stoi(arg_str));
    }

    // Find the command in the substring and return appropriate enum value
    if (input_line.find("Insert") != std::string::npos) {
        c = insert;
    } else if (arg_split == std::string::npos) {
        c = print1;
    } else {
        c = print2;
    }
    return std::make_tuple(time, c, args);
}

// Function to execute command with its arguments as parsed from the input line
void execute_command(std::tuple<int, Command, std::vector<int>> command_tuple, MyHeap& min_heap, RBTree& rbtree) {
    Command c = std::get<1>(command_tuple);
    auto args = std::get<2>(command_tuple);
    switch(c) {
        case insert:
        {
            // If the command is insert, insert a new building into the heap and red black tree
            BuildingDetails *bd = new BuildingDetails(args[0], 0, args[1]);
            min_heap.insert(bd);
            rbtree.insertBuilding(bd);
        }
            break;
        case print1:
        {
            // If the command is print a single building, then we get that building from the red black tree
            BuildingDetails b = rbtree.getBuilding(args[0]);
        }
            break;
        case print2:
        {
            // If the command is print range between two values, call getBetween function in red black tree and print the buildings
            std::ofstream output("output_file.txt", std::ios_base::app | std::ios_base::out);
            std::vector<BuildingDetails> buildings;
            buildings = rbtree.getBetween(args[0], args[1], buildings);
            if (buildings.size() == 0) {
                output<<"(0,0,0)\n";
            } else {
                for (std::vector<BuildingDetails>::iterator i = buildings.begin(); i != buildings.end(); ++i)
                {
                    output<<"("<<i->buildingNum<<","<<i->executed_time<<","<<i->total_time<<")";
                    if (std::next(i) != buildings.end())
                        output<<",";
                    else
                        output<<"\n";
                }
            }
        }
            break;
        default:
            std::cout<<"Invalid command\n";
    }
    return;
}
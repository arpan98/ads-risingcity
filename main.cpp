#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

#include "building.h"
#include "myheap.cpp"
#include "rbtree.cpp"

enum Command {
    insert,
    print1,
    print2
};

// Returns tuple (time, command, args) parsed from input
std::tuple<int, Command, std::vector<int>> parse_input_line(std::string);

void execute_command(std::tuple<int, Command, std::vector<int>>, MyHeap&, RBTree&);

int main(int argc, char const *argv[])
{
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string input_line;
    
    int time = 0;
    BuildingDetails dummy = {-1, 0, -1};
    BuildingDetails *curr_bd = &dummy;
    int work_done = -1, rem_time = 0, initial_time = 0;

    MyHeap min_heap;
    RBTree rbtree;

    if (input_file.is_open()) {
        getline(input_file, input_line);
        auto command_tuple = parse_input_line(input_line);
        int next_time = std::get<0>(command_tuple);
        while (!input_file.eof() || time <= next_time || rem_time >= 0 || !min_heap.is_empty()) {
            // std::cout<<"Time: "<<time<<" next time "<<next_time<<" curr bd "<<curr_bd->buildingNum<<" rem "<<rem_time<<" wd "<<work_done<<" heap size "<<min_heap.getHeapSize()<<"\n";
            // min_heap.print_heap();
            if (time == next_time) {
                execute_command(command_tuple, min_heap, rbtree);
                getline(input_file, input_line);
                command_tuple = parse_input_line(input_line);
                next_time = std::get<0>(command_tuple);
            }
            if (rem_time == 0) {
                if (curr_bd->total_time == curr_bd->executed_time) {
                    std::cout<<"("<<curr_bd->buildingNum<<","<<time<<")\n";
                    rbtree.deleteBuilding(curr_bd->buildingNum);
                    // rbtree.print();
                } else if (work_done > 0) {
                    // curr_bd->executed_time += work_done;
                    min_heap.insert(curr_bd);
                }
                if (input_file.eof() && time > next_time && min_heap.is_empty())
                    break;
                if (min_heap.is_empty()) {
                    curr_bd = &dummy;
                } else {
                    // min_heap.print_heap();
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
            if (curr_bd->buildingNum != dummy.buildingNum) {
                work_done++;
                curr_bd->executed_time = initial_time + work_done;
                rem_time--;
            }
            time++;
            // min_heap.print_heap();
        }
    }
    input_file.close();
    return 0;
}

std::tuple<int, Command, std::vector<int>> parse_input_line(std::string input_line) {
    Command c;
    std::vector<int> args;
    std::string word = input_line.substr(0, input_line.find(":"));
    int time = std::stoi(word);
    
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

    if (input_line.find("Insert") != std::string::npos) {
        c = insert;
    } else if (arg_split == std::string::npos) {
        c = print1;
    } else {
        c = print2;
    }
    return std::make_tuple(time, c, args);
}

void execute_command(std::tuple<int, Command, std::vector<int>> command_tuple, MyHeap& min_heap, RBTree& rbtree) {
    Command c = std::get<1>(command_tuple);
    auto args = std::get<2>(command_tuple);
    switch(c) {
        case insert:
        {
            BuildingDetails *bd = new BuildingDetails(args[0], 0, args[1]);
            // std::cout<<"Insert "<<args[0]<<" "<<args[1]<<"\n";
            min_heap.insert(bd);
            rbtree.insertBuilding(bd);
            // rbtree.print();
            // min_heap.print_heap();
        }
            break;
        case print1:
        {
            // std::cout<<"Print 1\n";
            BuildingDetails b = rbtree.getBuilding(args[0]);
            std::cout<<"("<<b.buildingNum<<","<<b.executed_time<<","<<b.total_time<<")\n";
        }
            break;
        case print2:
        {
            // std::cout<<"Print 2\n";
            std::vector<BuildingDetails> buildings;
            buildings = rbtree.getBetween(args[0], args[1], buildings);
            if (buildings.size() == 0) {
                std::cout<<"(0,0,0)\n";
            } else {
                for (std::vector<BuildingDetails>::iterator i = buildings.begin(); i != buildings.end(); ++i)
                {
                    std::cout<<"("<<i->buildingNum<<","<<i->executed_time<<","<<i->total_time<<")";
                    if (std::next(i) != buildings.end())
                        std::cout<<",";
                    else
                        std::cout<<"\n";
                }
            }
        }
            break;
        default:
            std::cout<<"Invalid command\n";
    }
    return;
}
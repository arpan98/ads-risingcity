#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

#include "building.h"
#include "myheap.cpp"

enum Command {
    insert,
    print1,
    print2
};

// Returns tuple (time, command, args) parsed from input
std::tuple<int, Command, std::vector<int>> parse_input_line(std::string);

void execute_command(std::tuple<int, Command, std::vector<int>>, MyHeap);

int main(int argc, char const *argv[])
{
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string input_line;
    int time = 0;

    MyHeap min_heap;

    if (input_file.is_open()) {
        getline(input_file, input_line);
        auto command_tuple = parse_input_line(input_line);
        int next_time = std::get<0>(command_tuple);
        while (!input_file.eof() || time <= next_time) {
            if (time == next_time) {
                execute_command(command_tuple, min_heap);
                getline(input_file, input_line);
                command_tuple = parse_input_line(input_line);
                next_time = std::get<0>(command_tuple);
            }
            time++;
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

void execute_command(std::tuple<int, Command, std::vector<int>> command_tuple, MyHeap min_heap) {
    Command c = std::get<1>(command_tuple);
    auto args = std::get<2>(command_tuple);
    BuildingDetails bd;
    switch(c) {
        case insert:
            bd = {args[0], 0, args[1]};
            std::cout<<"Insert "<<args[0]<<" "<<args[1]<<"\n";
            min_heap.insert(bd);
            min_heap.print_heap();
            break;
        case print1:
            std::cout<<"Print 1\n";
            break;
        case print2:
            std::cout<<"Print 2\n";
            break;
        default:
            std::cout<<"Invalid command\n";
    }
    return;
}
#ifndef UTIL_H
#define UTIL_H
#include <string>
#include<fstream>
#include<iostream>


inline void load_vert(std::string& out){
    std::ifstream inf{"C:\\Users\\rohan\\OneDrive\\Programming Projects\\C++\\VoxelEngine\\src\\shaders\\vertex.shader"};
    if(!inf)std::cout<<"A";
    std::string line{};
    while(std::getline(inf,line)){
        out+="\n";
        out+=line;
    }
}

inline void load_frag(std::string& out){
    std::ifstream inf{"C:\\Users\\rohan\\OneDrive\\Programming Projects\\C++\\VoxelEngine\\src\\shaders\\fragment.shader"};
    if(!inf)std::cout<<"A";
    std::string line{};
    while(std::getline(inf,line)){
        out+="\n";
        out+=line;
    }
}

inline void load_wafrag(std::string& out){
    std::ifstream inf{"C:\\Users\\rohan\\OneDrive\\Programming Projects\\C++\\VoxelEngine\\src\\shaders\\waterfrag.shader"};
    if(!inf)std::cout<<"A";
    std::string line{};
    while(std::getline(inf,line)){
        out+="\n";
        out+=line;
    }
}




#endif
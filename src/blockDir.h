#include <array>

struct GrassBlock{
    static inline std::array<float,12>sideCoords{0.46875f,0.5f,0,
                                                
                                                0.484375f,0.5f,0,

                                                0.484375f,0.53125f,0,

                                                0.46875f,0.53125f,0,
                                                };
    
    static inline std::array<float,12>topCoords{0.484375f,0.90625f,0,
                                                
                                                0.5f,0.90625f,0,

                                                 0.5f,0.9375f,0,

                                                 0.484375f,0.9375f,0,
                                                };

    static inline std::array<float,12>bottomCoords{0.390625f,0.9375f,0,
                                                
                                                0.390625f,0.90625f,0,

                                                 0.40625f,0.90625f,0,

                                                 0.40625f,0.9375f,0,
                                                };
};

struct DirtBlock{

    static inline std::array<float,12>sideCoords{0.390625f,0.9375f,0,
                                                
                                                0.390625f,0.90625f,0,

                                                 0.40625f,0.90625f,0,

                                                 0.40625f,0.9375f,0,
                                                };
};


struct StoneBlock{
     static inline std::array<float,12>sideCoords{0.46875f,0.0625f,0,
                                                
                                                0.484375f,0.0625f,0,

                                                 0.484375f,0.09375f,0,

                                                 0.46875f,0.09375f,0,
                                                };

};

struct Water{
    static inline std::array<float,12>sideCoords{
        0.f,0.96875f,1,
        0.015625f,0.96875f,1,
        0.015625f,1.f,1,
        0.f,1.f,1,
    };
};



struct Wood{
    static inline std::array<float, 12> sideCoords{
        0.46875f,0.34375f,0,
        0.484375,0.34375f,0,
        0.484375f,0.375f,0,
        0.46875f,0.375f,0,
    };


    static inline std::array<float, 12> topCoords{
        0.484375f,0.34375f,0,
        0.5f,0.34375f,0,
        0.5f,0.375f,0,
        0.484375f,0.375f,0,
    };

};

struct Leaf{
    static inline std::array<float, 12> sideCoords{
        0.140625f,0.84375f,0,
        0.15625f,0.84375f,0,
        0.15625f,0.875f,0,
        0.140625f,0.875f,0,
    };
};



    inline std::array<float,12>& getTexCoords(int id, int face){
        if(id==2){
            return DirtBlock::sideCoords;
        }
        else if(id==1){
            if(face==0){
                return GrassBlock::sideCoords;
            }
            else if(face==1){
                return GrassBlock::bottomCoords;
            }
            else{
                return GrassBlock::topCoords;
            }
        }
        else if(id==3){
            return StoneBlock::sideCoords;
        }
        else if(id==4){
            return Water::sideCoords;
        }
        else if(id==5){
            if(face==0) return Wood::sideCoords;
            return Wood::topCoords;
        }
        else if(id==6){
            return Leaf::sideCoords;
        }
    }

    inline bool isTransparent(int id){
        if(id==4||id==0||id==6)return true;
        else return false;
    }

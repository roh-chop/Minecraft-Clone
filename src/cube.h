#include <array>


struct Cube{

    static inline std::array<float,16> northFace {0,0,0,0.9f, 

                                1,0,0,0.9f,

                                1,1,0,0.9f,

                                0,1,0,0.9f, 
                            };



    static inline std::array<float,16>southFace {1,0,1,0.75f,

                                0,0,1,0.75f,

                                0,1,1,0.75f,

                                1,1,1,0.75f, 
                            };

    static inline std::array<float,16> westFace{0,0,1,0.75f,

                                0,0,0,0.75f, 

                                0,1,0,0.75f,

                                0,1,1,0.75f,
                            };

    static inline std::array<float,16>eastFace {1,0,0,0.9f,

                                1,0,1,0.9f,

                                1,1,1,0.9f, 

                                1,1,0,0.9f,
                            };

    static inline std::array<float,16> topFace  {0,1,0,1.f, 

                                1,1,0,1.f, 

                                1,1,1,1.f,

                                0,1,1,1.f,
                            };

    static inline  std::array<float,16> bottomFace {0,0,0,0.5f,

                                0,0,1,0.5f, 

                                1,0,1,0.5f,
                                
                                1,0,0,0.5f,

                                
                            };
};
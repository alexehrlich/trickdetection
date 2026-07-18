#ifndef MEASUREMENT_HPP
#define MEASUREMENT_HPP

#include <Arduino.h>

enum class Trick {
    OLLIE       = 0,
    BS_180      = 1,
    FS_180      = 2,
    BS_POP_SHUV = 3,
    KICKFLIP    = 4,
    UNDEFINED   = 5
};

enum class Result {
    _0 =    0,  //BAILED
    _1 =    1,  //LANDED
    _2 =    2,  //LANDED
    _3 =    3,  //LANDED
    _4 =    4,  //LANDED
    _5 =    5,  //LANDED
    _6 =    6,  //LANDED
    _7 =    7,  //LANDED
    _8 =    8,  //LANDED
    _9 =    9,  //LANDED
    _10 =   10  //LANDED
};

struct DataPoint {
    unsigned long ts;
    float acc_x, acc_y, acc_z;
    float gyr_x, gyr_y, gyr_z;
};

inline const char* trickToString(Trick t) {
    switch (t) {
        case Trick::OLLIE:       return "ollie";
        case Trick::BS_180:      return "bs_180";
        case Trick::FS_180:      return "fs_180";
        case Trick::BS_POP_SHUV: return "bs_pop_shuv";
        case Trick::KICKFLIP:    return "kickflip";
        default:                 return "unknown";
    }
}

class Measurement {
public:
    Measurement(Trick trick, unsigned long start);

    Trick         getTrick()  const;
    unsigned long getStart()  const;
    Result        getResult() const;
    void          setResult(Result result);

    static Trick  parseTrick(const String& s);
    static Result parseResult(const String& s);

private:
    Trick         _trick;
    unsigned long _start;
    Result        _result;
};

#endif
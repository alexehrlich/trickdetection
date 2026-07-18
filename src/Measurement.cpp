#include "Measurement.hpp"

Measurement::Measurement(Trick trick, unsigned long start)
    : _trick(trick), _start(start), _result(Result::_0)
{}

Trick         Measurement::getTrick()  const { return _trick; }
unsigned long Measurement::getStart()  const { return _start; }
Result        Measurement::getResult() const { return _result; }

void Measurement::setResult(Result result) {
    _result = result;
}

Trick Measurement::parseTrick(const String& s) {
    if (s == "ollie")       return Trick::OLLIE;
    if (s == "bs_180")      return Trick::BS_180;
    if (s == "fs_180")      return Trick::FS_180;
    if (s == "bs_pop_shuv") return Trick::BS_POP_SHUV;
    if (s == "kickflip")    return Trick::KICKFLIP;
    return Trick::UNDEFINED;
}

Result Measurement::parseResult(const String& s) {
    if (s == "0") return Result::_0;
    if (s == "1") return Result::_1;
    if (s == "2") return Result::_2;
    if (s == "3") return Result::_3;
    if (s == "4") return Result::_4;
    if (s == "5") return Result::_5;
    if (s == "6") return Result::_6;
    if (s == "7") return Result::_7;
    if (s == "8") return Result::_8;
    if (s == "9") return Result::_9;
    if (s == "10") return Result::_10;

    else return Result::_0;
}
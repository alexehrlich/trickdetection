#include "Measurement.hpp"

Measurement::Measurement(Trick trick, unsigned long start)
    : _trick(trick), _start(start), _result(Result::BAILED)
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
    if (s == "landed") return Result::LANDED;
    return Result::BAILED;
}
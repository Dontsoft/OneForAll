#include "appinformation.hpp"

void AppInformation::set(Type type, const QString& value) {
    _values.insert(type, value);
}

QString AppInformation::get(Type type) const { return _values[type]; }

bool AppInformation::has(Type type) const { return _values.contains(type); }

void AppInformation::addThirdPartyInformation(
    const ThirdPartyInformation& information)
{
    _thirdPartyInformation.append(information);
}

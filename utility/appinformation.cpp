#include "appinformation.hpp"

#include "external/spy/spy.hpp"

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

AppInformation::ThirdPartyIterator AppInformation::thirdPartyBegin() const
{
    return std::begin(_thirdPartyInformation);
}

AppInformation::ThirdPartyIterator AppInformation::thirdPartyEnd() const
{
    return std::end(_thirdPartyInformation);
}

QString AppInformation::compilerString()
{
    const auto version
        = QString::number(spy::compiler.version.major) + QStringLiteral(".")
          + QString::number(spy::compiler.version.minor) + QStringLiteral(".")
          + QString::number(spy::compiler.version.patch);
    QString compilerName;
    if constexpr (spy::compiler == spy::clang_)
    {
        compilerName = QStringLiteral("Clang");
#if defined(__apple_build_version__) // Apple clang has other version numbers
        compilerName += QStringLiteral(" (Apple)");
#elif defined(Q_CC_MSVC)
        compilerName += QStringLiteral(" (clang-cl)");
#endif
    }
    else if constexpr (spy::compiler == spy::gcc_)
    {
        compilerName = QStringLiteral("g++");
    }
    else if constexpr (spy::compiler == spy::msvc_)
    {
        compilerName = QStringLiteral("MSVC");
    }
    else if constexpr (spy::compiler == spy::intel_)
    {
        compilerName = QStringLiteral("Intel icpc");
    }
    else if constexpr (spy::compiler == spy::emscripten_)
    {
        compilerName = QStringLiteral("Emscripten");
    }
    else
    {
        compilerName = QStringLiteral("<unknown compiler>");
    }
#if 0
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU
                        // too
    QString platformSpecific;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    platformSpecific = QLatin1String(" (Apple)");
#elif defined(Q_CC_MSVC)
    platformSpecific = QLatin1String(" (clang-cl)");
#endif
    return QStringLiteral("Clang ") + QString::number(__clang_major__)
           + QStringLiteral(".") + QString::number(__clang_minor__)
           + platformSpecific;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC ") + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER > 1999)
        return QLatin1String("MSVC <unknown>");
    if (_MSC_VER >= 1930)
        return QLatin1String("MSVC 2022");
    if (_MSC_VER >= 1920)
        return QLatin1String("MSVC 2019");
    if (_MSC_VER >= 1910)
        return QLatin1String("MSVC 2017");
    if (_MSC_VER >= 1900)
        return QLatin1String("MSVC 2015");
#endif
#endif
    return QString("%1 %2").arg(compilerName, version);
}

#ifndef DEPENDANT_HPP
#define DEPENDANT_HPP

#include <tuple>

template <class... Ts>
class Dependant {
   public:
    struct Dependency {
        Dependency(const Ts&... args) {
            dependencyTuple = std::make_tuple(args...);
        }

        Dependency(Ts&&... args) { dependencyTuple = std::make_tuple(args...); }
        std::tuple<Ts...> dependencyTuple;
    };

    Dependant(const Dependency& dependency) : _dependency(dependency) {}
    Dependant(Dependency&& dependency) : _dependency(dependency) {}

    template <class Enum, typename std::enable_if<std::is_enum<Enum>::value,
                                                  Enum>::type dependencyIndex>
    inline auto getDependency() const {
        return std::get<static_cast<std::size_t>(dependencyIndex)>(
            _dependency.dependencyTuple);
    }

    template <std::size_t dependencyIndex>
    inline auto getDependency() const {
        return std::get<dependencyIndex>(_dependency.dependencyTuple);
    }

   private:
    const Dependency _dependency;
};

#endif  // DEPENDANT_HPP

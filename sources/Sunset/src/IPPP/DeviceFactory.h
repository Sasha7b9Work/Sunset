// 2026/03/23 11:40:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <memory>
#include <map>


class I_IPPP;


class DeviceFactory
{
public:

    enum class Type
    {
        Real,
        Emulator
    };

    static std::unique_ptr<I_IPPP> Create(Type type);

    static std::unique_ptr<I_IPPP> CreateFromConfig();

    template<typename T>
    static void RegisterType(Type type)
    {
        creators[type] = []()->std::unique_ptr<I_IPPP>
            {
                return std::make_unique<T>();
            };
    }

private:
    using Creator = std::function<std::unique_ptr<I_IPPP>()>;
    static std::map<Type, Creator> creators;
};

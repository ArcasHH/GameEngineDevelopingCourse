#pragma once

#include "INIReader.h"
#include <vector>
#include <cassert>

enum class ActionOnPressed 
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UNKNOWN
};

struct Button
{
    char button;
    ActionOnPressed action;

    Button(std::string symbol, ActionOnPressed act) 
    {
        assert(symbol != "UNKNOWN" && !symbol.empty() && "Wrong button configuration"); // Unknown button config
        button = symbol.front();
        action = act;
    }
};

class Keyboard 
{
    std::vector<Button> keys;

public:
    ActionOnPressed getAction(char symbol) const 
    {
        for (auto&& [button, action] : keys)
        {
            if (button == symbol)
            {
                return action;
            }
        }
        return ActionOnPressed::UNKNOWN;
    }

    void map(INIReader &reader, std::string name, ActionOnPressed action) 
    {
        std::string key = reader.Get("Keyboard", name, "UNKNOWN");
        keys.emplace_back(key, action);
    }

    Keyboard(std::string filename) 
    {
        INIReader reader(filename);
        assert(!reader.ParseError() && "Can't load 'Input.ini'");

        map(reader, "MoveForward", ActionOnPressed::FORWARD);
        map(reader, "MoveLeft",    ActionOnPressed::LEFT);
        map(reader, "MoveBack",    ActionOnPressed::BACKWARD);
        map(reader, "MoveRight",   ActionOnPressed::RIGHT);
    }
};

inline Keyboard &getKeyboard() 
{
    static std::unique_ptr<Keyboard> keys{ new Keyboard("../../../../../Assets/Configs/Input.ini") };
    return *keys;
}
#pragma once

class printer_api
{
public:
    virtual ~printer_api() = default;

    virtual void print() = 0;
};

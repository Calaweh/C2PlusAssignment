#pragma once

class ScreenController;

class Page
{
public:
    virtual ~Page() = default;
    virtual void display(ScreenController& screenController) = 0;
};
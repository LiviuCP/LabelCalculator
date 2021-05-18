#include "application.h"

int main()
{
    const int c_Result{Application::getInstance()->run()};
    return c_Result;
}

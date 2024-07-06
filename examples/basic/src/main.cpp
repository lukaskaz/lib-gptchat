#include "gptchat.hpp"

#include <iostream>

int main()
{
    try
    {
        auto question{"Who was the wisest man?"};
        auto [fullanswer, shortanswer] = gpt::GptChat().run(
            question,
            [&question]() {
                std::cout << "Checking question: " << question << '\n';
            },
            []() { std::cout << "Wait...\n"; }, 2);

        std::cout << "> Full answer:\n" << fullanswer << '\n';
        std::cout << "> Short answer:\n" << shortanswer << '\n';
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}

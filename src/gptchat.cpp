#include "gptchat.hpp"

#include <liboai.h>

#include <nlohmann/json.hpp>

#include <fstream>

namespace gpt
{

static liboai::OpenAI oai;
static liboai::Conversation conversation;
static liboai::FutureResponse future;

static constexpr const char* configFilePath = "../conf/init.json";

using json = nlohmann::json;

GptChat::GptChat()
{
    init();
}

std::pair<std::string, std::string> GptChat::run(
    const std::string& question, Callback&& greetcallback = []() {},
    Callback&& waitcallback = []() {}, int32_t waittimeoutsec = 3)
{
    conversation.AddUserData(question);
    future = oai.ChatCompletion->create_async("gpt-3.5-turbo", conversation);
    greetcallback();
    return output(std::move(waitcallback), waittimeoutsec);
}

std::string GptChat::history() const
{
    return conversation.GetJSON().dump();
}

inline void GptChat::init()
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Cannot open config file for GptChat");
    }

    std::string fileData;
    configFile >> fileData;
    json oaiConfig = json::parse(fileData)["oai"];

    if (oaiConfig["key"].is_null() ||
        oaiConfig["key"].get<std::string>().empty())
    {
        throw std::runtime_error("Cannot retrieve key for GptChat");
    }

    std::string oaiEnv = oaiConfig["env"];
    std::string oaiKey = oaiConfig["key"];
    if (setenv(oaiEnv.c_str(), oaiKey.c_str(), 1) < 0)
    {
        throw std::runtime_error("Cannot set env for GptChat");
    }

    if (!oai.auth.SetKeyEnv(oaiEnv))
    {
        std::cerr << "Cannot use apikey" << std::endl;
    }
}

inline std::pair<std::string, std::string>
    GptChat::output(Callback&& waitcallback, int32_t waittimeoutsec)
{
    while (future.wait_for(std::chrono::seconds(waittimeoutsec)) !=
           std::future_status::ready)
    {
        waitcallback();
    }

    auto response = future.get();
    conversation.Update(response);

    auto fullanswer = conversation.GetLastResponse();
    fullanswer.erase(remove(fullanswer.begin(), fullanswer.end(), '\"'),
                     fullanswer.end());

    if (auto pos = fullanswer.find_first_of(".\n\r"); pos != std::string::npos)
    {
        return std::make_pair(fullanswer, fullanswer.substr(0, pos));
    }
    return {fullanswer, {}};
}

} // namespace gpt

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

GptChat::GptChat(const std::string& question, Callback&& callback) :
    question{question}
{
    init();
    run(std::move(callback));
}

GptChat::GptChat(const std::string& question) : GptChat(question, []() {})
{}

std::string GptChat::history() const
{
    return conversation.GetJSON().dump();
}

std::pair<std::string, std::string> GptChat::output(
    int timeoutSec, Callback&& callback = []() {})
{
    while (future.wait_for(std::chrono::seconds(timeoutSec)) !=
           std::future_status::ready)
    {
        callback();
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

    gptChatEnv = oaiEnv;
}

inline void GptChat::run(Callback&& callback)
{
    if (!oai.auth.SetKeyEnv(gptChatEnv))
    {
        std::cerr << "Cannot use apikey" << std::endl;
    }

    conversation.AddUserData(question);
    future = oai.ChatCompletion->create_async("gpt-3.5-turbo", conversation);
    callback();
}

} // namespace gpt

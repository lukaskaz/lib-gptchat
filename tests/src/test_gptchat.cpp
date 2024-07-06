#include "gptchat.hpp"
#include "mocks/mock_callback.hpp"

#include <liboai.h>

#include <memory>

using namespace testing;

class TestGptchat : public Test
{
  public:
    std::shared_ptr<NiceMock<CallbackMock>> callbackMock =
        std::make_shared<NiceMock<CallbackMock>>();

  protected:
    void SetUp() override
    {}

    void TearDown() override
    {}
};

TEST_F(TestGptchat, testOaiAuthWithInvalidKeyThrows)
{
    EXPECT_THROW(({
                     auto question{"Test question"};
                     auto [fullanswer, shortanswer] = gpt::GptChat().run(
                         question, []() {}, []() {}, 1);
                 }),
                 liboai::exception::OpenAIException);
}

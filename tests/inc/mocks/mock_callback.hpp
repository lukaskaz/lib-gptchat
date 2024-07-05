#pragma once

#include <gmock/gmock.h>

class CallbackMock
{
  public:
    MOCK_METHOD(void, run, ());
};

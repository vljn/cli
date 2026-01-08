#include <memory>
#include "CommandWithArguments.h"

CommandWithArguments::CommandWithArguments(std::unique_ptr<CommandInvocation> invocation) : Command() {
    m_invocation = std::move(invocation);
}

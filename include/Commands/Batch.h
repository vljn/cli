#ifndef CLI_BATCH_H
#define CLI_BATCH_H

#include "Core/CommandBase/Command.h"

class Interpreter;

class Batch : public Command {
public:
    explicit Batch(std::string filename) : m_filename(std::move(filename)) { }

    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) override;

    bool consumesInput() override { return false; }
    bool producesOutput() override { return true; }
private:
    std::string m_filename;
};

#endif //CLI_BATCH_H
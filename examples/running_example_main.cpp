#include <iostream>

#include <comlint/command_line_interface.hpp>

using namespace comlint;

class AddCommandHandler : public CommandHandlerInterface
{
public:
    void Run(const ParsedCommand &command) final
    {
        std::cout << "Running add command for file " << command.values.front() << std::endl;

        if (command.flags.at("--verbose")) {
            std::cout << "Being verbose" << std::endl;
        }
        if (command.flags.at("--interactive")) {
            std::cout << "Adding interactively" << std::endl;
        }
    }
};

class CommitCommandHandler : public CommandHandlerInterface
{
public:
    void Run(const ParsedCommand &command) final
    {
        std::cout << "Running commit command!" << std::endl;

        if (command.IsOptionUsed("-m")) {
            std::cout << "Adding message " << command.options.at("-m") << std::endl;
        }
        if (command.IsOptionUsed("-c")) {
            std::cout << "Re-editting commit " << command.options.at("-c") << std::endl;
        }
        if (command.flags.at("--amend")) {
            std::cout << "Amending commit" << std::endl;
        }
        if (command.flags.at("--verbose")) {
            std::cout << "Being verbose" << std::endl;
        }
    }
};

class MergeCommandHandler : public CommandHandlerInterface
{
public:
    void Run(const ParsedCommand &command) final
    {
        std::cout << "Running merge command for branches " << command.values.at(0U) << " and " << command.values.at(1U) 
              << " using strategy " << command.options.at("-s") << std::endl;

        if (command.IsOptionUsed("-m")) {
            std::cout << "Adding message " << command.options.at("-m") << std::endl;
        }
    }
};

class SubmoduleCommandHandler : public CommandHandlerInterface
{
public:
    void Run(const ParsedCommand &command) final
    {
        std::cout << "Running submodule " << command.values.front() << " command!" << std::endl;

        if (command.flags.at("--verbose")) {
            std::cout << "Being verbose" << std::endl;
        }
    }
};

int main(int argc, char** argv)
{
    try {
        // create a command line interface which always requires command line arguments
        CommandLineInterface cli(argc, argv, "ExampleApplication", "Example usage of Comlint library basing on some git commands", false);

        // add "add" command which requires 1 value (it can be any value), has no allowed options and 2 allowed flags (no required options by default)
        cli.AddCommand("add", "Add files to commit", 1U, ANY, NONE, {"--verbose", "--interactive"});
        // add "commit" command which does not require any value, has 2 allowed options and 2 allowed flags (no required options by default)
        cli.AddCommand("commit", "Commit changes", {"-m", "-c"}, {"--verbose", "--amend"});
        // add "merge" command which requires 2 values (these can be any values), has 2 allowed option, no allowed flags and 1 required options
        cli.AddCommand("merge", "Merge two branches", 2U, ANY, {"-s", "-m"}, NONE, {"-s"});
        // add "submodule command which requires 1 value (it can be either "add" or "update"), has no allowed options and 1 allowed flag (no required options by default)
        cli.AddCommand("submodule", "Perform operation on submodule", 1U, {"add", "update"}, NONE, {"--verbose"});

        // add option "-b" (which can take any value by default)
        cli.AddOption("-b", "Specify branch name");
        // add option "-m" (which can take any value by default)
        cli.AddOption("-m", "Provide message");
        // add option "-c" (which can take any value by default)
        cli.AddOption("-c", "Provide commit hash");
        // add option "-s" which can take one of three allowed values: recursive, resolve or subtree
        cli.AddOption("-s", "Specify merging strategy", {"recursive", "resolve", "subtree"});

        // add flags
        cli.AddFlag("--verbose", "Show verbose output");
        cli.AddFlag("--interactive", "Add files to commit interactively");
        cli.AddFlag("--amend", "Join to previous commit");

        // add command handlers
        CommandHandlerPtr add_handler = std::make_shared<AddCommandHandler>();
        CommandHandlerPtr commit_handler = std::make_shared<CommitCommandHandler>();
        CommandHandlerPtr merge_handler = std::make_shared<MergeCommandHandler>();
        CommandHandlerPtr submodule_handler = std::make_shared<SubmoduleCommandHandler>();

        cli.AddCommandHandler("add", add_handler);
        cli.AddCommandHandler("commit", commit_handler);
        cli.AddCommandHandler("merge", merge_handler);
        cli.AddCommandHandler("submodule", submodule_handler);

        // run command provided by the user from the command line
        cli.Run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
#ifndef PROLOGEXECUTOR_H
#define PROLOGEXECUTOR_H

#include <string>
#include <set>
#include <unordered_map>

#include <SWI-cpp.h>

class PrologExecutor
{
public:
    static void createEngine(const std::string & appName);
    static void destoryEngine();

    PrologExecutor(const std::string & fileName, const std::set<std::string> varTable);
    ~PrologExecutor();

    bool executePredicate(const std::unordered_map<std::string, int> & states, std::unordered_map<std::string, int> & newSates);

private:
    static PlEngine* engine;

    std::string fileName;
    std::unordered_map<std::string, int> varPositionTable;
};

#endif // PROLOGEXECUTOR_H

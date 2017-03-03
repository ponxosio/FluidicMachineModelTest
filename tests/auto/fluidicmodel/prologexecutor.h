#ifndef PROLOGEXECUTOR_H
#define PROLOGEXECUTOR_H

#include <memory>
#include <string>
#include <set>
#include <unordered_map>

#include <QTemporaryFile>

#include <SWI-cpp.h>

#include <constraintssolverinterface/routingengine.h>

class PrologExecutor : public RoutingEngine
{
public:
    static void createEngine(const std::string & appName);
    static void destoryEngine();

    PrologExecutor(std::unique_ptr<QTemporaryFile> temporaryFile, const std::set<std::string> & varTable);
    virtual ~PrologExecutor();

    virtual bool calculateNewRoute(const std::unordered_map<std::string, long long> & inputStates,
                                   std::unordered_map<std::string, long long> & outStates) throw(std::runtime_error);

private:
    static PlEngine* engine;

    std::string fileName;
    std::unordered_map<std::string, int> varPositionTable;
    std::unique_ptr<QTemporaryFile> file;
};

#endif // PROLOGEXECUTOR_H

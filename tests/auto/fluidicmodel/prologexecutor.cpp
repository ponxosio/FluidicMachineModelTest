#include "prologexecutor.h"

PlEngine* PrologExecutor::engine = NULL;

void PrologExecutor::createEngine(const std::string & appName) {
    if (engine == NULL) {
        char* cstr = new char[appName.size() + 1];
        std::strcpy(cstr, appName.c_str());

        char* argv[] = {cstr};
        engine = new PlEngine(1, argv);
        delete[] cstr;
    }
}

void PrologExecutor::destoryEngine() {
    if (engine != NULL) {
        delete engine;
    }
}

PrologExecutor::PrologExecutor(std::unique_ptr<QTemporaryFile> temporaryFile, const std::set<std::string> & varTable) :
    RoutingEngine()
{
    int i = 0;
    for(std::string varName: varTable) {
        this->varPositionTable.insert(std::make_pair(varName, i));
        i++;
    }

    this->file = std::move(temporaryFile);

    try {
        PlCall(std::string("consult(\"" + file->fileName().toStdString() + "\").").c_str());
    } catch (PlException ex) {
        throw(std::runtime_error("PrologExecutor::PrologExecutor(). Exception at the Prolog constraints engine. Impossible to read temporaryFile, message: " + std::string((char*) ex)));
    }
}

PrologExecutor::~PrologExecutor() {
}

bool PrologExecutor::calculateNewRoute(const std::unordered_map<std::string, long long> & inputStates, std::unordered_map<std::string, long long> & outStates)
    throw(std::runtime_error)
{
    try {
        PlFrame frame;
        PlTermv av(varPositionTable.size());

        for(auto statePair: inputStates) {
            std::string varName = statePair.first;
            int value = statePair.second;

            int varPos = varPositionTable[varName];
            av[varPos] = value;
        }

        PlQuery q("stackAutoPredicate", av);

        if (q.next_solution()) {
            for(auto pair: varPositionTable) {
                const std::string & name = pair.first;
                int pos = pair.second;
                int value = (int) av[pos];

                outStates[name] = value;
            }
            return true;
        } else {
            return false;
        }
    } catch (PlException ex) {
        throw(std::runtime_error("PrologExecutor::calculateNewRoute(). Exception at the Prolog constraints engine, message: " + std::string((char*) ex)));
    }
}

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

PrologExecutor::PrologExecutor(const std::string & fileName, const std::set<std::string> varTable) :
    fileName(fileName)
{
    int i = 0;
    for(std::string varName: varTable) {
        this->varPositionTable.insert(std::make_pair(varName, i));
        i++;
    }
}

PrologExecutor::~PrologExecutor() {

}

bool PrologExecutor::executePredicate(const std::unordered_map<std::string, int> & states, std::unordered_map<std::string, int> & newSates) {
    PlFrame frame;
    PlCall(std::string("consult(\"" + fileName + "\").").c_str());

    PlTermv av(varPositionTable.size());

    for(auto statePair: states) {
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

            newSates.insert(std::make_pair(name, value));
        }
        return true;
    } else {
        return false;
    }

}

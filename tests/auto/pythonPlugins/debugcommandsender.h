#ifndef DEBUGCOMMANDSENDER_H
#define DEBUGCOMMANDSENDER_H

#include <sstream>

#include <communications/CommandSender.h>

class DebugCommandSender : public CommandSender
{
public:
    DebugCommandSender(){}
    virtual ~DebugCommandSender(){}

    virtual unsigned long sendString(const char * str){stream << str << ";"; return 0;}
    inline virtual std::string receiveString() throw (std::ios_base::failure){return "";}
    inline virtual std::string readUntil(char endCharacter) throw (std::ios_base::failure){return "";}
    inline virtual void disconnect() {}
    inline virtual void connect() throw (std::ios_base::failure){}
    inline virtual void synchronize() throw (std::ios_base::failure){}

    inline std::string getStr() {
        return stream.str();
    }

protected:
    std::stringstream stream;
};

#endif // DEBUGCOMMANDSENDER_H

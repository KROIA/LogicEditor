#include "debug.h"

namespace Debug
{
namespace Color
{
    const std::string black   = "\033[0;30m";
    const std::string red     = "\033[0;31m";
    const std::string green   = "\033[0;32m";
    const std::string yellow  = "\033[0;33m";
    const std::string blue    = "\033[0;34m";
    const std::string purple  = "\033[0;35m";
    const std::string cyan    = "\033[0;36m";
    const std::string white   = "\033[0;37m";

    const std::string bBlack  = "\033[1;30m";
    const std::string bRed    = "\033[1;31m";
    const std::string bGreen  = "\033[1;32m";
    const std::string bYellow = "\033[1;33m";
    const std::string bBlue   = "\033[1;34m";
    const std::string bPurple = "\033[1;35m";
    const std::string bCyan   = "\033[1;36m";
    const std::string bWhite  = "\033[1;37m";
}
void setup()
{

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return;
    }

}
QString colorizeFunc(QString name)
{
    QString output;
    QStringList classParts = name.split("::");
    QStringList nameAndType;

    nameAndType = classParts.first().split(" ");

    QString returnType = "";
    if(nameAndType.count() > 1)
        returnType = nameAndType.first() + " ";
    QString className = nameAndType.last();

    QStringList funcAndParamas = classParts.last().split("(");
    funcAndParamas.last().chop(1);
    QString functionName = funcAndParamas.first();
    QStringList params = funcAndParamas.last().split(",");

    output.append(Color::bGreen.c_str());
    output.append(returnType);
    //output.append("\033[0m\033[32m");
    output.append(Color::bGreen.c_str());
    output.append(className);
    //output.append("\033[0m::");
    output.append((Color::white+"::").c_str());
    //output.append("\033[34m");
    output.append(Color::bYellow.c_str());
    output.append(functionName);
    //output.append("\033[0m(");
    output.append((Color::white+"(").c_str());

    QStringList::const_iterator param;
    for (param = params.begin(); param != params.constEnd(); ++param) {
        if(param != params.begin()) {
            //output.append("\033[0m,");
            output.append((Color::white+",").c_str());
        }
        //output.append("\033[036m");
        output.append(Color::bGreen.c_str());
        output.append((*param));
    }
    //output.append("\033[0m)");
    output.append((Color::white+")").c_str());
    return output;
}
}

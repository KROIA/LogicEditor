#include "databaseID.h"
#include <chrono>
#include <ctime>

const std::string DatabaseID::key_id = "id";
DatabaseID::DatabaseID()
{

}
DatabaseID::DatabaseID(const std::string &id)
{
    setID(id);
}

void DatabaseID::setID(const std::string &id)
{
    m_id = id;
}
const std::string &DatabaseID::getID() const
{
    return m_id;
}

std::string DatabaseID::generateRandomID(size_t length)
{
    const std::string chars             = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t charsCount                   = chars.size();
    const static std::time_t randTime   = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::string str(length+1,'\0');
    for(size_t i=0; i<length; ++i)
        str[i] = chars[(rand()+randTime)%charsCount];
    return str;
}

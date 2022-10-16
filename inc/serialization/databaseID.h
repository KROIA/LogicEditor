#pragma once

#include <string>
/**
 * \author Alex Krieg
 * \date   12.10.2022
 *
 * \brief The DatabaseID class is used to hold the unique id string for a ISerializable object.
 */
class DatabaseID
{
public:

    DatabaseID();
    DatabaseID(const std::string &id);

    /**
    * \brief setID
    * \param id which must be unique for the same database. Take a random string.
    *        Do not change the id after it was added to the database
    */
    void setID(const std::string &id);

    /**
    * \brief getID
    * \return the unique id string of this object
    */
    const std::string &getID() const;

    /**
    * \brief generateRandomID
    * \return a random generated id string
    */
    static std::string generateRandomID(size_t length = 10);

    const static std::string key_id;
private:
    std::string m_id;



};

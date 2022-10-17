#pragma once

#include <unordered_map>
#include <vector>
#include "databaseObject.h"
#include "debug.h"

/**
 * \author Alex Krieg
 * \date   12.10.2022
 *
 * \brief The Database class
 * \details The database is used to serialize and deserialize objects.
 *          Objects must inherit the ISerializable interface for that.<br>
 *
 *          Each ISerializable object will be packed in an SatabaseObject.
 *          The DatabaseObject will not be accessible outside the database and is used
 *          to hold the unique ID string for each object.
 *
 */
class Database
{
    public:
        Database();
        ~Database();
        template<typename T>
        static bool defineSaveableObject();

        bool load(const std::string &jsonFile);
        bool save(const std::string &jsonFile) const;

        bool addObject(ISerializable* obj);
        bool removeObject(ISerializable* obj);
        bool removeObject(DatabaseObject *dbObj);
        bool removeObject(const std::string &id);
        void clear(bool deleteIt = true);
        bool objectExists(ISerializable* obj) const;
        bool objectExists(const std::string &id) const;
        size_t getObjectCount() const;
        template<typename T>
        size_t getObjectCount() const;
        //const DatabaseID &getID(ISerializable *obj) const;
        ISerializable *getObject(const std::string &id) const;
        std::vector<ISerializable*> getObjects() const;
        template<typename T>
        T* getObject(const std::string &id) const;
        template<typename T>
        std::vector<T*> getObjects() const;

        DatabaseObject* instantiateObject(const QJsonObject &obj);

        const static size_t npos = -1;
    private:
        void instantiateDatabase(const QJsonArray &objs);


        void addObjectInternal(DatabaseObject* obj);

        static std::unordered_map<std::string, ISerializable*> m_saveableObjectTypes;
        std::unordered_map<std::string, DatabaseObject*> m_objects;

};


/**
 * \macro Create a template variant for the class objectType
 *        Call this macro once for each ISerializable object type in the main
 *        outside of a function
 */
#define DATABASE_USE_OBJECT(objectType) \
template std::vector<objectType*> Database::getObjects() const; \
template objectType* Database::getObject(const std::string &id) const; \
ISERIALIZABLE_DATABASE_USE_OBJECT(objectType)


#define ISERIALIZABLE_DATABASE_USE_OBJECT(objectType) \
template size_t ISerializable::databaseGetObjectCount<objectType>() const; \
template objectType* ISerializable::databaseGetObject(const std::string &id) const; \
template std::vector<objectType*> ISerializable::databaseGetObjects() const;



template<typename T>
bool Database::defineSaveableObject()
{
    T *t = new T();
    std::string typeName = t->className();

    if (m_saveableObjectTypes.find(typeName) == m_saveableObjectTypes.end())
    {
        m_saveableObjectTypes[typeName] = t;
        return true;
    }
    else
    {
        ISerializable *inList = m_saveableObjectTypes[typeName];

        std::string typeName1 = typeid(t).name();
        std::string typeName2 = typeid(inList).name();

        if(typeName1 == typeName2)
        {
            FATAL("Objekt Type: \""<<typeName.c_str()<<"\" ist bereits definiert"<<"\n")
        }
        else
        {
            FATAL("Der ObjektName: \""<<typeName.c_str()<<"\" für "<<typeName1.c_str()<<
                  " ist bereits definiert für ein anderes Objekt: \""<<typeName2.c_str()<<
                  "\" definiert worden."<<"\n")
        }
        delete t;
    }
    return false;
}
template<typename T>
T* Database::getObject(const std::string &id) const
{
    return dynamic_cast<T*>(getObject(id));
}
template<typename T>
std::vector<T*> Database::getObjects() const
{
    std::vector<T*> list;
    list.reserve(m_objects.size());
    for (auto& it: m_objects) {
        T *obj = dynamic_cast<T*>(it.second->getObject());
        if(obj)
            list.push_back(obj);
    }
    return list;
}
template<typename T>
size_t Database::getObjectCount() const
{
    size_t count = 0;
    for (auto& it: m_objects) {
        T *obj = dynamic_cast<T*>(it.second->getObject());
        if(obj)
            ++count;
    }
    return count;
}


// The implementation of the template func's are here
// because the database must be known to define the functions
template<typename T>
size_t ISerializable::databaseGetObjectCount() const
{
    if(!m_database) return 0;
    return m_database->getObjectCount<T>();
}
template<typename T>
T* ISerializable::databaseGetObject(const std::string &id) const
{
    if(!m_database) return nullptr;
    return m_database->getObject<T>(id);
}
template<typename T>
std::vector<T*> ISerializable::databaseGetObjects() const
{
    if(!m_database) return std::vector<T*>();
    return m_database->getObjects<T>();
}

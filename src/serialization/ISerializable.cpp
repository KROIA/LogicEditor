#include "ISerializable.h"
#include "databaseID.h"
#include "database.h"

const std::string ISerializable::key_objectType = "objectType";


ISerializable::ISerializable()
{
    m_id = nullptr;
    m_database = nullptr;
    m_parent = nullptr;
}
ISerializable::ISerializable(const ISerializable &)
{
    m_id = nullptr;
    m_database = nullptr;
    m_parent = nullptr;
}
ISerializable::~ISerializable()
{
    if(m_parent && m_parent->getObject() == this)
        m_parent->objectGotDeleted();
}


const std::string &ISerializable::getID() const
{
    static const std::string dummy;
    if(!m_id)
    {
        WARNING("No ID found");
        return dummy;
    }
    return m_id->getID();
}

void ISerializable::addChild(ISerializable *obj)
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == obj)
            return;
    m_childs.push_back(obj);
}
void ISerializable::removeChild(ISerializable *obj)
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == obj)
        {
            m_childs.erase(m_childs.begin() + i);
            return;
        }
}

QJsonObject ISerializable::save() const
{
    QJsonObject obj;
    /*for(size_t i=0; i<m_childs.size(); ++i)
    {
        combine(obj,m_childs[i]->save());
    }*/

    // Combine the QJsonObject with the base object of this
    return combine(obj,
    QJsonObject
    {
        // Add the properties of this object here
        // Do not take the same keyvalues two times,
        // also not the keys of the base class
        {key_objectType.c_str(), className().c_str()},
    });
}
bool ISerializable::read(const QJsonObject &)
{
    return true;
}
void ISerializable::postLoad()
{

}

bool ISerializable::databaseObjectExists(ISerializable* obj) const
{
    if(!m_database) return false;
    return m_database->objectExists(obj);
}
bool ISerializable::databaseObjectExists(const std::string &id) const
{
    if(!m_database) return false;
    return m_database->objectExists(id);
}
size_t ISerializable::databaseGetObjectCount() const
{
    if(!m_database) return 0;
    return m_database->getObjectCount();
}
ISerializable *ISerializable::databaseGetObject(const std::string &id) const
{
    if(!m_database) return nullptr;
    return m_database->getObject(id);
}
std::vector<ISerializable*> ISerializable::databaseGetObjects() const
{
    if(!m_database) return std::vector<ISerializable*>();
    return m_database->getObjects();
}






bool ISerializable::extract(const QJsonObject &obj, std::string &value, const std::string &key)
{
    QJsonValue val = obj[key.c_str()];
    if(val.isUndefined())
        return false;
    value = val.toString(value.c_str()).toStdString();
    return true;
}
bool ISerializable::extract(const QJsonObject &obj, int &value, const std::string &key)
{
    QJsonValue val = obj[key.c_str()];
    if(val.isUndefined())
        return false;
    value = val.toInt(value);
    return true;
}
bool ISerializable::extract(const QJsonObject &obj, float &value, const std::string &key)
{
    QJsonValue val = obj[key.c_str()];
    if(val.isUndefined())
        return false;
    value = val.toDouble(value);
    return true;
}
bool ISerializable::extract(const QJsonObject &obj, double &value, const std::string &key)
{
    QJsonValue val = obj[key.c_str()];
    if(val.isUndefined())
        return false;
    value = val.toDouble(value);
    return true;
}
bool ISerializable::extract(const QJsonObject &obj, bool &value, const std::string &key)
{
    QJsonValue val = obj[key.c_str()];
    if(val.isUndefined())
        return false;
    value = val.toBool(value);
    return true;
}

QJsonObject  ISerializable::combine(const QJsonObject &a, const QJsonObject &b)
{
    QJsonObject obj3(a);
    for (auto it = b.constBegin(); it != b.constEnd(); it++) {
        obj3.insert(it.key(), it.value());
    }
    return obj3;
}

std::string ISerializable::extractClassName(const QJsonObject &data)
{
    std::string type = data[key_objectType.c_str()].toString().toStdString();
    if(type.size() == 0)
        type = "none";
    return type;
}
void ISerializable::setParent(DatabaseID *id, DatabaseObject* dbObj, Database *db)
{
    m_id = id;
    m_database = db;
    m_parent = dbObj;
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        m_childs[i]->setParent(id, dbObj, db);
    }
}

#include "databaseObject.h"
#include "database.h"

DatabaseObject::DatabaseObject(ISerializable *obj,
                               const DatabaseID &id,
                               Database *parent)
{
    m_obj = obj;
    m_id = id;
    m_parent = parent;
    m_obj->setParent(&m_id, this, parent);

}
DatabaseObject::DatabaseObject(const QJsonObject &json, ISerializable* cloanable,  Database *parent)
{
    m_parent = parent;
    m_id.setID(json[DatabaseID::key_id.c_str()].toString().toStdString());
    ISerializable *instance = cloanable->clone(json);
    m_obj = instance;
    m_obj->setParent(&m_id, this, parent);
}

DatabaseObject::~DatabaseObject()
{
    if(!m_obj) return;
    m_obj->m_id = nullptr;
    m_obj->m_database = nullptr;
    m_obj->m_parent = nullptr;

    delete m_obj;
}

ISerializable *DatabaseObject::getObject() const
{
    return m_obj;
}
const DatabaseID &DatabaseObject::getID() const
{
    return m_id;
}

void DatabaseObject::objectGotDeleted()
{
    m_obj = nullptr;
    m_parent->removeObject(this);
}
void DatabaseObject::setObjectNullptr()
{
    if(!m_obj) return;
    m_obj->setParent(nullptr, nullptr, nullptr);
    m_obj = nullptr;
}

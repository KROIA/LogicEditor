#include "database.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

std::unordered_map<std::string, ISerializable*> Database::m_saveableObjectTypes;
Database::Database()
{

}
Database::~Database()
{
    clear();
}

bool Database::load(const std::string &jsonFile)
{
    QFile file(jsonFile.c_str());
    if(!file.exists())
    {
        WARNING("Database konnte nicht geladen werden. Datei: \""<<jsonFile.c_str()<<"\" nicht gefunden\n");
        return false;
    }
    if( file.open(QIODevice::ReadOnly ) )
    {
        QByteArray bytes = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
        if( jsonError.error != QJsonParseError::NoError )
        {
            WARNING("Database konnte nicht geladen werden. Datei: \""<<jsonFile.c_str()<<"\"\nJsonError: "<<jsonError.errorString().toStdString().c_str()<<"\n");
            return false;
        }
        if(document.isArray())
        {
            QJsonArray objs = document.array();

            instantiateDatabase(objs);
            DEBUGLN(Debug::Color::green.c_str()<<"Database konnte geladen werden. Datei: \""<<jsonFile.c_str()<<"\"");
            return true;
        }
    }
    WARNING("Database konnte nicht geladen werden. Datei: \""<<jsonFile.c_str()<<"\"\n");
    return false;
}
bool Database::save(const std::string &jsonFile) const
{
    QJsonArray writer;

    for (auto& it: m_objects)
    {
        QJsonObject obj = it.second->getObject()->save();
        obj[DatabaseID::key_id.c_str()] = it.second->getID().getID().c_str();
        writer.push_back(obj);
    }

    QJsonDocument document;
    document.setArray(writer);
    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file(jsonFile.c_str());
    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
        iStream.setEncoding(QStringConverter::Encoding::Utf8);
        iStream << bytes;
        file.close();
        DEBUGLN(Debug::Color::green.c_str() << "Database gespeichert als: \""<<jsonFile.c_str()<<"\"");
        return true;
    }
    WARNING("Database konnte nicht gespeichert werden. Datei: \""<<jsonFile.c_str()<<"\"\n");
    return false;
}
bool Database::addObject(ISerializable* obj)
{
    auto findit = m_saveableObjectTypes.find(obj->className());
    if(findit == m_saveableObjectTypes.end())
    {
        WARNING("Kann Objekt (Name = \""<<obj->className().c_str()<<"\") nicht in aufnehmen,\n"
                "dieser Type wurde nicht in die speicherbaren Objekte liste aufgenommen"<<"\n");
        return false;
    }

    if(objectExists(obj))
    {
        WARNING("Objekt (Name = \""<<obj->className().c_str()<<"\", ID = \""<<obj->getID().c_str() <<"\") bereits vorhanden"<<"\n");
        return false;
    }
    DatabaseID id(DatabaseID::generateRandomID());
    while(objectExists(id.getID()))
        id.setID(DatabaseID::generateRandomID());

    addObjectInternal(new DatabaseObject(obj, id, this));
    return true;
}
void Database::addObjectInternal(DatabaseObject* obj)
{
    m_objects.insert(std::pair<std::string, DatabaseObject*>(obj->getID().getID(),obj));
}
bool Database::removeObject(ISerializable* obj)
{    
    DatabaseObject *dbObj = nullptr;

    for (auto& it: m_objects)
    {
        if(it.second->getObject() == obj)
        {
            dbObj = it.second;
            break;
        }
    }
    if(dbObj)
    {
        m_objects.erase(dbObj->getID().getID());
        dbObj->setObjectNullptr();
        delete dbObj;
        return true;
    }
    return false;
}
bool Database::removeObject(DatabaseObject *dbObj)
{
  bool found = false;
  for (auto& it: m_objects)
  {
    if(it.second == dbObj)
    {
      found = true;
      break;
    }
  }
  if(found)
  {
    m_objects.erase(dbObj->getID().getID());
    dbObj->setObjectNullptr();
    delete dbObj;
    return true;
  }
  return false;
}
bool Database::removeObject(const std::string &id)
{
    auto findit = m_objects.find(id);
    if(findit == m_objects.end())
        return false;
    DatabaseObject *dbObj = findit->second;
    m_objects.erase(dbObj->getID().getID());
    dbObj->setObjectNullptr();
    delete dbObj;
    return true;
}

bool Database::objectExists(ISerializable* obj) const
{
    for (auto& it: m_objects)
    {
        if(it.second->getObject() == obj)
        {
            return true;
        }
    }
    return false;
}
bool Database::objectExists(const std::string &id) const
{
    auto findit = m_objects.find(id);
    if(findit == m_objects.end())
    {
        return false;
    }
    return true;
}

size_t Database::getObjectCount() const
{
    return m_objects.size();
}
/*const DatabaseID &Database::getID(ISerializable *obj) const
{
    for (auto& it: m_objects)
    {
        if(it.second->getObject() == obj)
            return it.second->getID();
    }
    const static DatabaseID dummy;
    return dummy;
}*/
ISerializable *Database::getObject(const std::string &id) const
{
    auto findit = m_objects.find(id);
    if(findit == m_objects.end())
    {
        return nullptr;
    }
    return findit->second->getObject();
}
std::vector<ISerializable*> Database::getObjects() const
{
    std::vector<ISerializable*> list;
    for (auto& it: m_objects) {
        if(it.second)
            list.push_back(it.second->getObject());
    }
    return list;
}

DatabaseObject* Database::instantiateObject(const QJsonObject &obj)
{
    std::string objType = obj[ISerializable::key_objectType.c_str()].toString("none").toStdString();

    auto findit = m_saveableObjectTypes.find(objType);
    if(findit == m_saveableObjectTypes.end())
    {
        WARNING("Kann Objekt Type: \""<<objType.c_str()<<"\" keinem definierten speicherbaren Objekt zuordnen"<<"\n");
        return nullptr;
    }
    else
    {
        return new DatabaseObject(obj, findit->second, this);
       // DatabaseID id(obj[DatabaseID::key_id.c_str()].toString().toStdString());
       //
       // ISerializable *instance = findit->second->clone(obj);
       // if(instance)
       // {
       //     DatabaseID id(obj[DatabaseID::key_id.c_str()].toString().toStdString());

       //     //addObject(instance);
       //     //if(!objectExists(id.getID()))
       //     //{
       //         //addObjectInternal(instance,id);
       //         DatabaseObject *dbObj = new DatabaseObject(instance, id, this);
       //         return dbObj;
            /*}
            else
            {

            }*/
       // }
    }
    return nullptr;
}

void Database::instantiateDatabase(const QJsonArray &objs)
{
    clear();
    m_objects.reserve(objs.count());
    for(int i=0; i<objs.count(); ++i)
    {
        QJsonObject obj = objs[i].toObject();
        DatabaseObject *inst = instantiateObject(obj);
        if(!objectExists(inst->getID().getID()))
        {
            if(inst)
                addObjectInternal(inst);
        }
        else
        {
            std::string objType = obj[ISerializable::key_objectType.c_str()].toString("none").toStdString();
            ISerializable *other = m_objects[inst->getID().getID()]->getObject();
            WARNING("Kann Objekt Type: \""<<objType.c_str()<<"\" ID: \""<< inst->getID().getID().c_str()
                                           <<"\" nicht laden. Objekt Type: \""<<other->className().c_str()<<"\" hat die gleiche ID\n");
            delete inst;
        }
    }
    for (auto& it: m_objects)
    {
        it.second->getObject()->postLoad();
    }
}

void Database::clear(bool deleteIt)
{
    for (auto& it: m_objects) {
        if(!deleteIt)
            it.second->setObjectNullptr();
        delete it.second;
    }
    m_objects.clear();
}

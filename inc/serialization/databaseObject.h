#pragma once

#include "databaseDeclaration.h"
#include "ISerializable.h"
#include "databaseID.h"

/**
 * \author Alex Krieg
 * \date   12.10.2022
 *
 * \brief The DatabaseObject class is used to hold the ISerializable object and its unique ID object.
 * \details The instantiation of this class is part of the database and the object will only be available for the database.<br>
 *
 */
class DatabaseObject
{
public:
    /**
     * \brief DatabaseObject constructor which creates a new database entry.
     * \note The ownership of the obj pointer will be given to this instance.
     * \param obj which will be serialized.
     * \param id which will be used to identify the obj instance even after reloading of the database.
     * \param parent database this instance is contained in.
     */
    DatabaseObject(ISerializable *obj,
                   const DatabaseID &id,
                   Database *parent);

    /**
     * \note Will delete the ISerializable object pointer.
     */
    ~DatabaseObject();


    /**
     * \return the ISerializable object which is stored in this instance
     */
    ISerializable *getObject() const;

    /**
     * \return the unique id for the ISerializable object which is stored in this instance
     */
    const DatabaseID &getID() const;

    void objectGotDeleted();
    void setObjectNullptr();
private:

    ISerializable *m_obj;
    DatabaseID m_id;

    Database *m_parent;
};

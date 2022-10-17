#pragma once
#include <QJsonObject>
#include <string>
#include "databaseDeclaration.h"


/**
 * \author Alex Krieg
 * \date   12.10.2022
 *
 * \brief The ISerializable class is used for objects,
 *        which can be saved to a Jason file.
 */
class ISerializable
{
        friend DatabaseObject;
        friend Database;
    public:
        /**
         * \brief ISerializable
         * \details Creats a object with default values
         */
        ISerializable();

        /**
         * \brief ISerializable
         * \details Creats a object. It does not copy the parameters of this class.
         *          If you create a new object using the copy constructor, you have to add the new object
         *          to de database manually.
         * \param other object which will be copied from
         */
        ISerializable(const ISerializable &other);
        virtual ~ISerializable();

        /**
         * \brief className
         * \details Used to save the object type (class name) to know
         *          from which object this must be instantiated from when the
         *          saved file will be loaded
         * \return  Class unique type identifier
         */
        virtual std::string className() const = 0;

        /**
         * \brief clone
         * \details creates a copy of the object
         * \param reader with de json data to load parameters from.
         *        The data will not be copied from the cloning object, it is only used
         *        to recreate the same object structure.
         * \return Instantiated pointer to the same object type like the cloning object.
         */
        virtual ISerializable* clone(const QJsonObject &reader) const = 0;

        /**
         * \brief clone
         * \details creates a copy of the object
         * \param reader with de json data to load parameters from.
         *        The data will be copied from the cloning object.
         * \return Instantiated pointer to the same object type like the cloning object.
         */
        virtual ISerializable* clone() const = 0;

        /**
         * \brief getID
         * \return the unique id string of this object
         */
        const std::string &getID() const;




        const static std::string key_objectType;


    protected:
        void addChild(ISerializable *obj);
        void removeChild(ISerializable *obj);
        const std::vector<ISerializable*> &getChilds() const;

        /**
         * \brief Reimplement this function in each derived class.
         * \return The Json object with all stored data for a later reinstantiation of this object
         * \details Save Example 1 see read() example 1
         * \code
         *         QJsonObject save() const override
         *         {
         *             // Combine the QJsonObject with the base object of this
         *             return combine(ISerializable::save(),
         *             QJsonObject
         *             {
         *                 // Add the properties of this object here
         *                 // Do not take the same keyvalues two times,
         *                 // also not the keys of the base class
         *                 {"name" , m_name.c_str()},
         *                 {"color", m_color.c_str()},
         *             });
         *         }
         * \endcode
         *
         * \details Save Example 2 see read() example 2
         * If you have instances of different ISerializable objects, than you can save them too.
         * \code
         *         QJsonObject save() const override
         *         {
         *             // Add this data to thisData
         *             QJsonObject thisData = combine(ISerializable::save(),
         *             QJsonObject
         *             {
         *                 // Add the properties of this object here
         *                 // Do not take the same keyvalues two times,
         *                 // also not the keys of the base class
         *                 {"name" , m_name.c_str()},
         *                 {"color", m_color.c_str()},
         *             });
         *
         *             // Get the QJsonObject of the member object
         *             thisData["myObject"] = myObject.save();
         *
         *             return thisData;
         *         }
         * \endcode
         *
         */
        virtual QJsonObject save() const;

        /**
         * \brief Reads the saved data back into this object.
         *        Reimplement this function in each derived class.
         * \param reader to load the saved parameters back
         * \return true if the loading was a success, otherwise false
         * \details Read Example 1 see save() example 1
         *          The reader will contain the data for this object.
         *          You can only extract, what you saved before.
         * \code
         *          bool read(const QJsonObject &reader) override
         *          {
         *              m_autoSizeInterval
         *          }
         * \endcode
         *
         *
         * \details Read Example 2 see save() example 2
         *          If you have instances of ISerializable objects, than you have to call the read function on them
         *          with the data for that object.
         * \code
         *         bool read(const QJsonObject &reader) override
         *         {
         *             bool success = true;
         *             // Read the value for the base class
         *             success = ISerializable::read(reader);
         *
         *             // Read the value for the member object
         *             success &= myObject.read(thisData["myObject"]);
         *
         *             // Read the values for this class
         *             success &= extract(reader,m_name, "name");
         *             success &= extract(reader,m_color,"color");
         *             return success;
         *         }
         * \endcode
         */
        virtual bool read(const QJsonObject &reader);

        /**
         * \brief postLoad
         * \details This function will be called after the database has loaded all objects<br>
         *          If you connect to other objects, this will be the place for that.
         */
        virtual void postLoad();



        // Interface function to access the same database, this object is contained in

        DatabaseObject *getInstantiated(const QJsonObject &data);


        /**
         * \brief Checks if the given object is in the database or not
         * \return true if the given object is in the database, otherwise false
         * \code
         *      bool exists = databaseObjectExists(myObjectPointer);
         * \endcode
         */
        bool databaseObjectExists(ISerializable* obj) const;

        /**
         * \brief Checks if the given object's id is in the database or not
         * \return true if the given object's is in the database, otherwise false
         * \code
         *      bool exists = databaseObjectExists("qFVlPjfsl");
         * \endcode
         */
        bool databaseObjectExists(const std::string &id) const;

        /**
         * \brief Gets the amount of objects in the database.
         * \return amount of objects in the database
         * \code
         *      size_t amount = databaseGetObjectCount();
         * \endcode
         */
        size_t databaseGetObjectCount() const;

        /**
         * \brief Gets the object with the given id.
         * \param id unique string id for the searched object.
         * \return object of type ISerializable* with the given id.
         *         If the object is was not found, nullptr will be returned.
         * \code
         *      ISerializable *obj = databaseGetObject("qFVlPjfsl");
         * \endcode
         */
        ISerializable *databaseGetObject(const std::string &id) const;

        /**
         * \brief Gets a list of object which are stored in the database.
         * \return vector of objects which have type ISerializable*
         * \code
         *      std::vector<ISerializable*> objects = databaseGetObjects();
         * \endcode
         */
        std::vector<ISerializable*> databaseGetObjects() const;


        // The implementation of the template func's are in the database.h
        // because the database must be known to define the functions

        /**
         * \brief Gets the amount of objects in the database this object is contained in.
         * \return amount of objects of type T
         * \code
         *      size_t amount = databaseGetObjectCount<ObjectType>();
         * \endcode
         */
        template<typename T>
        size_t databaseGetObjectCount() const;

        /**
         * \brief Gets the object with the given id and type T.
         * \param id unique string id for the searched object.
         * \return object with id and type T. If the stored object with that id is not
         *         of type T, than nullptr will be returned.
         *         If the object is was not found, nullptr will be returned.
         * \code
         *      ObjectType *obj = databaseGetObject<ObjectType>("qFVlPjfsl");
         * \endcode
         */
        template<typename T>
        T* databaseGetObject(const std::string &id) const;

        /**
         * \brief Gets a list of object with the given type T.
         * \return vector of objects which have type T
         * \code
         *      std::vector<ObjectType*> objects = databaseGetObjects<ObjectType>();
         * \endcode
         */
        template<typename T>
        std::vector<T*> databaseGetObjects() const;



        /**
         * \brief extract a parameter out of the Json object
         * \param obj is the jeson object which holds the data
         * \param value in which the parameter value will be saved in
         * \param key name to find the right parameter
         * \return true, if the parameter was found in the json object
         */
        static bool extract(const QJsonObject &obj, std::string &value, const std::string &key);
        static bool extract(const QJsonObject &obj, int &value, const std::string &key);
        static bool extract(const QJsonObject &obj, float &value, const std::string &key);
        static bool extract(const QJsonObject &obj, double &value, const std::string &key);
        static bool extract(const QJsonObject &obj, bool &value, const std::string &key);

        /**
         * \brief combine two QJsonObjects together
         * \param a
         * \param b
         * \return the combined object
         */
        static QJsonObject  combine(const QJsonObject &a, const QJsonObject &b);

        /**
         * \brief extractClassName out of the QJsonObject
         * \param data
         * \return the class name string. If not found, it will return "none".
         */
        static std::string extractClassName(const QJsonObject &data);

    private:
        void setParent(DatabaseID *id, DatabaseObject* dbObj, Database *db);

        std::vector<ISerializable*> m_childs;

        DatabaseID *m_id;        //!< ptr to the parent DatabaseObjects->m_id. nullptr, if not assigned to a database
        DatabaseObject *m_parent;//!< ptr to the parent DatabaseObject. nullptr, if not assigned to a database
        Database *m_database;    //!< ptr to the parent Database. nullptr, if not assigned to a database

        std::vector<std::string> m_loadingChildIDList;
};

/**
 * \macro IMPLEMENT_ISERIALIZABLE_CONST_FUNC(classNameVal)
 *        Implements the virtual clone() function of the ISerializable
 *        Implements the virtual className() function of the ISerializable
 */
#define IMPLEMENT_ISERIALIZABLE_CONST_FUNC(classNameVal) \
classNameVal* clone(const QJsonObject &reader) const override \
{ \
    classNameVal *obj = new classNameVal(); \
    obj->read(reader); \
    return obj; \
} \
classNameVal* clone() const override \
{ \
    return new classNameVal(*this); \
} \
std::string className() const override \
{ \
    return #classNameVal; \
}


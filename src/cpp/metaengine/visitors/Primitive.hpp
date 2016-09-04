/*!
 * \file
 * \brief Visitor objects for retrieving primitive types from Documents.
 * \author David Saxon
 */
#ifndef METAENGINE_VISITORS_PRIMITIVE_HPP_
#define METAENGINE_VISITORS_PRIMITIVE_HPP_

#include <json/json.h>

#include "metaengine/Document.hpp"
#include "metaengine/Visitor.hpp"

namespace metaengine
{

//------------------------------------------------------------------------------
//                                BOOLEAN VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief Visitor object used to retrieve a primitive boolean from a
 *        metaengine::Document.
 */
class BoolV : public metaengine::Visitor<bool>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static BoolV& instance();

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message);
};

//------------------------------------------------------------------------------
//                             BOOLEAN VECTOR VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief Visitor object used to retrieve a vector of primitive booleans from a
 *        metaengine::Document.
 */
class BoolVectorV : public metaengine::Visitor<std::vector<bool>>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static BoolVectorV& instance();

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message);
};

//------------------------------------------------------------------------------
//                                INTEGER VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief Visitor object used to retrieve a primitive integral number from a
 *        metaengine::Document.
 *
 * \tparam IntType The integral type that values should be retrieve as, e.g.
 *                 ```int```, ```std::size_t```, ```arc::int64```.
 */
template<typename IntType>
class IntV : public metaengine::Visitor<IntType>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static IntV<IntType>& instance()
    {
        static IntV<IntType> v;
        return v;
    }

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message)
    {
        // check type
        if(!data->isInt())
        {
            Json::FastWriter j_writer;
            error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                          << "converted to integral type.";
            return false;
        }

        metaengine::Visitor<IntType>::m_value =
            static_cast<IntType>(data->asInt());
        return true;
    }
};

//------------------------------------------------------------------------------
//                             INTEGER VECTOR VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief Visitor object used to retrieve a vector of primitive integral numbers
 *        from a metaengine::Document.
 *
 * \tparam IntType The integral type that values should be retrieved as a vector
 *                 of, e.g. ```int```, ```std::size_t```, ```arc::int64```.
 */
template<typename IntType>
class IntVectorV : public metaengine::Visitor<std::vector<IntType>>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static IntVectorV<IntType>& instance()
    {
        static IntVectorV<IntType> v;
        return v;
    }

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message)
    {
        // check type
        if(!data->isArray())
        {
            Json::FastWriter j_writer;
            error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                          << "converted to array type.";
            return false;
        }

        // temp value
        std::vector<IntType> temp;

        // iterate over the values
        Json::Value::const_iterator child;
        for(child = data->begin(); child != data->end(); ++child)
        {
            // check if the data can be converted
            if(!child->isInt())
            {
                Json::FastWriter j_writer;
                error_message << "Array element \"" << j_writer.write(*child)
                              << "\" cannot be converted to integral type.";
                return false;
            }
            // perform conversion
            temp.push_back(static_cast<IntType>(child->asInt()));
        }

        // no errors, use the temp value
        metaengine::Visitor<std::vector<IntType>>::m_value = temp;
        return true;
    }
};

//------------------------------------------------------------------------------
//                                 FLOAT VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief Visitor object used to retrieve a primitive floating point number from
 *        a metaengine::Document.
 *
 * \tparam FloatType The floating point type that values should be retrieve as,
 *                   e.g. ```float```, ```double```.
 */
template<typename FloatType>
class FloatV : public metaengine::Visitor<FloatType>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static FloatV<FloatType>& instance()
    {
        static FloatV<FloatType> v;
        return v;
    }

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message)
    {
        // check type
        if(!data->isDouble())
        {
            Json::FastWriter j_writer;
            error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                          << "converted to floating point type.";
            return false;
        }

        metaengine::Visitor<FloatType>::m_value =
            static_cast<FloatType>(data->asDouble());
        return true;
    }
};

//------------------------------------------------------------------------------
//                              FLOAT VECTOR VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief Visitor object used to retrieve a vector of primitive floating point
 *        numbers from a metaengine::Document.
 *
 *
 * \tparam FloatType The floating point type that values should be retrieve as
 *                   a vector of, e.g. ```float```, ```double```.
 */
template<typename FloatType>
class FloatVectorV : public metaengine::Visitor<std::vector<FloatType>>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static FloatVectorV<FloatType>& instance()
    {
        static FloatVectorV<FloatType> v;
        return v;
    }

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message)
    {
        // check type
        if(!data->isArray())
        {
            Json::FastWriter j_writer;
            error_message << "\"" << j_writer.write(*data) << "\" cannot be "
                          << "converted to array type.";
            return false;
        }

        // temp value
        std::vector<FloatType> temp;

        // iterate over the values
        Json::Value::const_iterator child;
        for(child = data->begin(); child != data->end(); ++child)
        {
            // check if the data can be converted
            if(!child->isDouble())
            {
                Json::FastWriter j_writer;
                error_message << "Array element \"" << j_writer.write(*child)
                              << "\" cannot be converted to floating point "
                              << "type.";
                return false;
            }
            // perform conversion
            temp.push_back(static_cast<FloatType>(child->asDouble()));
        }

        // no errors, use the temp value
        metaengine::Visitor<std::vector<FloatType>>::m_value = temp;
        return true;
    }
};

} // namespace metaengine

#endif


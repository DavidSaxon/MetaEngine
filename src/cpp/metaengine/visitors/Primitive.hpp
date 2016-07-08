/*!
 * \file
 * \brief Visitor objects for retrieving primitive types from Documents.
 * \author David Saxon
 */
#ifndef METAENGINE_PRIMITIVE_HPP_
#define METAENGINE_PRIMITIVE_HPP_

#include <json/json.h>

#include "metaengine/Document.hpp"

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
    virtual bool retrieve(const Json::Value* value);
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
    virtual bool retrieve(const Json::Value* value);
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
    virtual bool retrieve(const Json::Value* value)
    {
        // check type
        if(!value->isInt())
        {
            return false;
        }

        metaengine::Visitor<IntType>::m_value =
            static_cast<IntType>(value->asInt());
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
    virtual bool retrieve(const Json::Value* value)
    {
        // check type
        if(!value->isArray())
        {
            return false;
        }

        // temp value
        std::vector<IntType> temp;

        // iterate over the values
        Json::Value::const_iterator child;
        for(child = value->begin(); child != value->end(); ++child)
        {
            // check if the value can be converted
            if(!child->isInt())
            {
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
    virtual bool retrieve(const Json::Value* value)
    {
        // check type
        if(!value->isDouble())
        {
            return false;
        }

        metaengine::Visitor<FloatType>::m_value =
            static_cast<FloatType>(value->asDouble());
        return true;
    }
};

} // namespace metaengine

#endif


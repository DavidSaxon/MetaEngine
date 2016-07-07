/*!
 * \file
 * \brief Visitor objects for retrieving primitive types from Documents.
 * \author David Saxon
 */
#ifndef METAENGINE_PRIMITIVE_HPP_
#define METAENGINE_PRIMITIVE_HPP_

#include "metaengine/Document.hpp"

namespace metaengine
{

//------------------------------------------------------------------------------
//                                BOOLEAN VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief TODO:
 */
class BoolV : public metaengine::Visitor<bool>
{
public:

    virtual bool retrieve(const Json::Value* value);
};

//------------------------------------------------------------------------------
//                             BOOLEAN VECTOR VISITOR
//------------------------------------------------------------------------------

/*!
 * \brief TODO:
 */
class BoolVectorV : public metaengine::Visitor<std::vector<bool>>
{
public:

    virtual bool retrieve(const Json::Value* value);
};

} // namespace metaengine

#endif


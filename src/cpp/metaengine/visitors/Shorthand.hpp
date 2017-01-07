/*!
 * \file
 * \brief Convience defines for including and accessing visitor instances.
 * \author David Saxon
 */
#ifndef METAENGINE_VISITORS_SHORTHAND_HPP_
#define METAENGINE_VISITORS_SHORTHAND_HPP_

#include "metaengine/visitors/Path.hpp"
#include "metaengine/visitors/Primitive.hpp"
#include "metaengine/visitors/String.hpp"

/*!
 * \brief Shorthand for metaengine::PathV::instance()
 */
#define ME_PATHV metaengine::PathV::instance()
/*!
 * \brief Shorthand for metaengine::PathVectorV::instance()
 */
#define ME_PATHVECV metaengine::PathVectorV::instance()

/*!
 * \brief Shorthand for metaengine::BoolV::instance()
 */
#define ME_BOOLV metaengine::BoolV::instance()
/*!
 * \brief Shorthand for metaengine::BoolVectorV::instance()
 */
#define ME_BOOLVECV metaengine::BoolVectorV::instance()
/*!
 * \brief Shorthand for metaengine::IntV<IntType>::instance()
 */
#define ME_INTV(IntType) metaengine::IntV<IntType>::instance()
/*!
 * \brief Shorthand for metaengine::IntVectorV<IntType>::instance()
 */
#define ME_INTVECV(IntType) metaengine::IntVectorV<IntType>::instance()
/*!
 * \brief Shorthand for metaengine::FloatV<FloatType>::instance()
 */
#define ME_FLOATV(FloatType) metaengine::FloatV<FloatType>::instance()
/*!
 * \brief Shorthand for metaengine::FloatVectorV<FloatType>::instance()
 */
#define ME_FLOATVECV(FloatType) metaengine::FloatVectorV<FloatType>::instance()

/*!
 * \brief Shorthand for metaengine::UTF8StringV::instance()
 */
#define ME_U8STRV metaengine::UTF8StringV::instance()
/*!
 * \brief Shorthand for metaengine::UTF8StringVectorV::instance()
 */
#define ME_U8STRVECV metaengine::UTF8StringVectorV::instance()

#endif

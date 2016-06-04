#ifndef METAENGINE_HPP_
#define METAENGINE_HPP_

/*!
 * \mainpage MetaEngine C++ Documentation
 *
 * MetaEngine is a light-weight wrapper for JSON that provides functionality
 * for loading and accessing meta programming and/or configuration data.
 *
 * The source code can be found here: https://github.com/DavidSaxon/MetaEngine
 *
 *
 * \par Purpose of MetaEngine
 *
 * MetaEngine is designed for defining program runtime configuration data that
 * can be stored outside of the source code but accessed efficiently at runtime.
 *
 * \par How to Use
 *
 * Configuration data is stored in JSON files. For example the following file
 * contains configuration data in relation to application resources:
 *
 * \code
 * {
 *     "resource_path": ["res"],
 *     "gui_resource_path": ["${resource_path}", "gui"],
 *     "fonts":
 *     {
 *         "resource_path": ["${gui_resource_path}", "fonts"]
 *         "default_font": "Roboto Th",
 *         "default_size": 10,
 *         "default_colour": [255, 255, 255]
 *     }
 * }
 * \endcode
 *
 * For the sake of this example we will assume that the above JSON data is
 * stored in the following file relative to the runtime:
 * ```meta/resources.json```
 *
 * Loading and storing the data is done by constructing a meta::Data object:
 *
 * \code
 * meta::Data meta_resources(
 *    chaos::io::sys::Path().join("meta").join("resources.json"));
 * \endcode
 *
 * To access the data the meta::Data::get() function is used.
 *
 * \code
 * std::vector<chaos::str::UTF8String> resource_path;
 * meta_resources.get("resource_path", resource_path);
 * // resource_path now contains: ["res"]
 * \endcode
 *
 * Data values can be retrieved as defined types which may vary the way the
 * value is retrieved. For example MetaEngine supplies a template specialisation
 * for chaos::io::sys::Path objects, where the JSON value is expected to be a
 * string array: meta::Data::get<chaos::io::sys::Path>().
 * Retrieving the value as a chaos::io::sys::Path applies value expansion, which
 * means keys to other valid path values can be used in the data and will be
 * resolved when retrieved, e.g. ```"{$resource_path}"```
 *
 * \code
 * chaos::io::sys::Path gui_path;
 * meta_resources.get("gui_resource_path", gui_path);
 * // gui_path now contains: "res/gui"
 * \endcode
 *
 * To access nested data the ```'.'``` character is used. For example to access
 * the value of the ```"resource_path"``` value inside of the the ```"fonts"```
 * section, the key: ```"fonts.resource_path"``` would be used. Values can be
 * nested arbitrarily deep in the JSON data with the keys being chained using
 * ```'.'```
 *
 * \code
 * chaos::io::sys::Path fonts_path;
 * meta_resources.get("fonts.resource_path", fonts_path);
 * // fonts_path now contains: "res/gui/fonts"
 * \endcode
 *
 * Values may only be retrieved as types they can be parsed as. For example the
 * value for the key "fonts.default_size" may be returned as 32-bit integer but
 * not as a string:
 *
 * \code
 * // this code is valid
 * chaos::int32 default_size_i;
 * meta_resources.get("fonts.default_size", default_size_i);
 *
 * // this code would throw a chaos::ex::TypeError
 * chaos::str::UTF8String default_size_s;
 * meta_resources.get("fonts.default_size", default_size_s);
 * \endcode
 *
 * Support for retrieving values as custom types can be added by defining
 * template specialisations for the meta::Data::get() function or for the
 * meta::Data::is_type() and meta::Data::as_type() functions.
 */

/**
  * \brief the global namespace for the MetaEngine API.
  */
namespace meta
{
} // namespace chaos

#endif

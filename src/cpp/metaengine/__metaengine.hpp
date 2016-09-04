#ifndef METAENGINE_HPP_
#define METAENGINE_HPP_

/*!
 * \mainpage MetaEngine C++ Documentation.
 *
 * MetaEngine is a JSON wrapper for loading and accessing meta programming and
 * configuration data using the Visitor Pattern.
 *
 * \par Purpose of MetaEngine
 *
 * MetaEngine is designed for storing program runtime configuration data that
 * can be stored outside of the source code, therefore program behavior can be
 * modified without recompilation.
 * By default MetaEngine reads JSON data from the file system, but provides
 * methods for sourcing "fallback" JSON data stored within program memory. This
 * means applications using MetaEngine can be fail safe since if there is an
 * unexpected error with the file system data, access will fallback to the data
 * in memory, and provide handlers to report the failure.
 * Some applications may not desire to expose the editable file system data in
 * production releases, for which MetaEngine can be initialised to only use the
 * in-memory data.
 *
 * TODO: using the MetaEngine compiler script
 *
 * \par Loading Data
 *
 * For the following examples we will assume there is a file
 * called ```meta/resources.json``` and an ```arc::str::UTF8String``` in the
 * runtime called ```resources_compiled``` that both contain this JSON data:
 *
 * \code
 * {
 *     "app_name": "MetaEngine Example",
 *     "resource_path": ["res"],
 *     "gui_resource_path": ["@{resource_path}", "gui"],
 *     "fonts":
 *     {
 *         "resource_path": ["@{gui_resource_path}", "fonts"],
 *         "default_font": "Roboto Th",
 *         "default_size": 10,
 *         "default_colour": [255, 120, 0]
 *     }
 * }
 * \endcode
 *
 * We will also assume the MetaEngine setup is done in a function
 * called ```init_meta```.
 *
 * Loading and storing the data is done by constructing a metaengine::Document:
 *
 * \code
 * void init_meta()
 * {
 *     arc::io::sys::Path resource_path;
 *     resource_path << "meta" << "resources.json";
 *
 *     // loads the Document from the file on disk
 *     metaengine::Document file_doc(resource_path);
 *
 *     // loads the Document from application memory
 *     metaengine::Document mem_doc(&resources_compiled);
 *
 *     // loads the Document from both the file system and application memory
 *     metaengine::Document fallback_doc(resource_path, &resources_compiled);
 * }
 * \endcode
 *
 * In the above example, if either the ```file_doc``` or the ```mem_doc``` fail
 * to load their data, an exception will be raised immediately. However if
 * the ```fallback_doc``` fails to load its data from the file or memory a
 * failure callback will be triggered, and only if loading from both sources
 * fails then an exception will be raised.
 *
 * The following example shows connecting a failure reporter to report if
 * loading from one of the sources fails:
 *
 * \code
 * #include <iostream>
 *
 * void meta_load_reporter(
 *         const arc::io::sys::Path& file_path,
 *         const arc::str::UTF8String& message)
 * {
 *     std::cerr << message << std::endl;
 * }
 *
 * void init_meta()
 * {
 *     // connect the load failure function
 *     metaengine::Document::set_load_fallback_reporter(meta_load_reporter);
 *
 *     arc::io::sys::Path resource_path;
 *     resource_path << "meta" << "resources.json";
 *
 *     // loads the Document from both the file system and application memory
 *     metaengine::Document fallback_doc(resource_path, &resources_compiled);
 * }
 * \endcode
 *
 * Now if loading the data from the file or from memory fails,
 * the ```meta_load_reporter``` function will be called and will print the
 * reason for failure to stderr.
 *
 * \par Accessing Data
 *
 * To access data from the document, the Visitor pattern is used to retrieve
 * values as useful types. MetaEngine comes with a built-in set of Visitors, but
 * the metaengine::Visitor type can be derived from to implement the retrieval
 * of other types.
 * All Visitors should provide a static ```instance()``` function (although it
 * is not required for user-implemented Visitors) which can be used to get an
 * existing static instance of the Visitor type. All visitors provide a ```*```
 * operator which can be used to extract the Visitor's value as the expected
 * type.
 *
 * In the following example we will use built-in Visitors to retrieve some
 * values:
 *
 * \code
 * void init_meta()
 * {
 *     ...
 *
 *     // app will contain "MetaEngine Example"
 *     arc::str::UTF8String app(
 *         *fallback_doc.get("app_name", metaengine::UTF8StringV::instance()));
 *
 *     // font_size will contain 10. Notice the . symbol is used to retrieve
 *     // nested values, and the IntV visitor expects a template type to define
 *     // the integral type
 *     arc::uint32 font_size = *fallback_doc.get(
 *         "fonts.default_size",
 *         metaengine::IntV<arc::uint32>::instance()
 *     );
 *
 *     // font_colour will contain [255, 120, 0]
 *     std::vector<arc::uint8> font_colour(*fallback_doc.get(
 *         "fonts.default_colour",
 *         metaengine::IntVectorV<arc::uint8>::instance()
 *     ));
 *
 *     // The PathV visitor provides it's own syntax, in which elements with the
 *     // patten: @{<key>} will be resolved to other valid paths or strings in
 *     // the same Document with the key. Therefore gui_resource_path will
 *     // contain: "res/gui/fonts"
 *     arc::io::sys::Path gui_resource_path(*fallback_doc.get(
 *          "fonts.gui_resource_path",
 *          metaengine::PathV::instance()
 *     ));
 * }
 * \endcode
 *
 * If the metaengine::Document is using data from both the file system and from
 * memory the fall-back protocol will be used when retrieving values. This
 * means if a value is requested from the Document, but there is no entry with
 * the key, or the data cannot be converted by the provided Visitor, the
 * Document will trigger a reporter callback, and then proceed to retrieve the
 * value from memory instead. Only if both of these operations fail will an
 * exception be raised.
 *
 * The following example shows connecting a failure reporter to report if
 * retrieving a value from data loaded from the file system fails:
 *
 * \code
 * #include <iostream>
 *
 * void meta_get_reporter(
 *         const arc::io::sys::Path& file_path,
 *         const arc::str::UTF8String& message)
 * {
 *     std::cerr << message << std::endl;
 * }
 *
 * void init_meta()
 * {
 *     // connect the get failure function
 *     metaengine::Document::set_get_fallback_reporter(meta_get_reporter);
 *
 *     ...
 * }
 * \endcode
 *
 * MetaEngine also supports an extended implementation of the
 * metaengine::Document object: metaengine::Variant. Variants work much the same
 * way as Documents except they take a base file path, and variants of this
 * file path are used to access data. The metaengine::Variant has a default
 * variant which is always loaded, and a current variant which may or may not be
 * the default variant. Data will first be attempted to be retrieved from the
 * current file path variant, if this fails it will try access the data from the
 * default variant, and finally if this fails and the metaengine::Variant has
 * data loaded from memory it will attempt to access it from here. Variants are
 * intended to be used to support multiple language representations of string
 * data, but can be used for any other relevant use.
 *
 * For example we have a file called ```meta/lang.uk.json``` which contains the
 * data:
 *
 * \code
 * {
 *     "string": "hello_world",
 *     "sentence": "This is a language variant."
 * }
 * \endcode
 *
 * And another file called ```meta/lang.de.json``` which contains the data:
 *
 * \code
 * {
 *     "string": "hallo_welt"
 * }
 * \endcode
 *
 *
 * We will use the uk variant as the default which will cause it to be loaded
 * initially:
 *
 * \code
 * // we leave uk and de out of the file path since these are variants
 * arc::io::sys::Path base_path;
 * base_path << "meta" << "lang.json";
 *
 * // using uk as the default variant which will load from meta/lang.uk.json
 * metaengine::Variant lang_var(base_path, "uk");
 *
 * // returns "hello_world"
 * *lang_var.get("string", metaengine::UTF8StringV::instance()));
 * // returns "This is a language variant."
 * *lang_var.get("sentence", metaengine::UTF8StringV::instance()));
 * \endcode
 *
 * The metaengine::Variant::set_variant function can be used to change the
 * current variant (which is currently the default variant: "uk"). Changing the
 * current variant will cause the new file variant to be loaded, however the
 * default variant will remain loaded and will be used to fallback to if data
 * cannot be accessed from the current variant:
 *
 * \code
 * // changing to the de variant will load data from meta/lang.de.json
 * lang_var.set_variant("de");
 *
 * // returns "hallo_welt"
 * *lang_var.get("string", metaengine::UTF8StringV::instance()));
 * // returns "This is a language variant." from the uk variant since the
 * // "sentence" key doesn't exist in the de variant file.
 * *lang_var.get("sentence", metaengine::UTF8StringV::instance()));
 * \endcode
 */

/*!
  * \brief the global namespace for the MetaEngine API.
  */
namespace metaengine
{
} // namespace metaengine

#endif

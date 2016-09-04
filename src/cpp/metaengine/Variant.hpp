/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_VARIANT_HPP_
#define METAENGINE_VARIANT_HPP_

#include "metaengine/Document.hpp"

namespace metaengine
{

/*!
 * \brief A derived implementation of Document which supports loading meta data
 *        from multiple variants of a file path.
 *
 * A Variant can contain up to three versions of the data, one loaded from the
 * current variant of the file, one loaded from the default variant of the file,
 * and possibly one loaded from memory. It will by default to retrieve values
 * from the current variant, but if this fails it will fallback retrieving from
 * default variant, and finally if this also fails it will fallback to
 * retrieving the value from the data loaded from memory.
 *
 * File path variants are applied by inserting the variant string as a secondary
 * extension in the file path. For example if the file path
 * was ```path/to/my/file.json``` the and the variant was ```"variant"``` it
 * would be applied like so: ```path/to/my/file.variant.json```.
 * However if the file has no extension e.g. ```path/to/my/file``` the variant
 * will be applied as the extension: ```path/to/my/file.variant```
 */
class Variant : public Document
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new Variant Document that loads its internal data from
     *        variant files.
     *
     * \param file_path Base path without any variants applied which will be
     *                  used to load the internal data of this variant.
     * \param default_variant The variant which will be both loaded as the
     *                        initial variant and used as the data to fallback
     *                        to if the current variant cannot be used.
     * \param load_immediately Whether constructing the Document will also load
     *                         the internal data. This is the same as
     *                         constructing the Document with load_immediately
     *                         set the to ```false``` and then calling reload
     *                         immediately after.
     */
    Variant(
            const arc::io::sys::Path& file_path,
            const arc::str::UTF8String& default_variant = "",
            bool load_immediately = true);

    /*!
     * \brief Creates a new Variant Document that loads its internal data from
     *        both variant file data and data in memory.
     *
     * \param file_path Base path without any variants applied which will be
     *                  used to load the internal data of this variant.
     *                  Document's internal data from.
     * \param memory Pointer to a arc::str::UTF8String that will contain JSON to
     *               load the tertiary fallback of this Document's internal data
     *               from.
     * \param default_variant The variant which will be both loaded as the
     *                        initial variant and used as the data to fallback
     *                        to if the current variant cannot be used.
     * \param load_immediately Whether constructing the Document will also load
     *                         the internal data. This is the same as
     *                         constructing the Document with load_immediately
     *                         set the to ```false``` and then calling reload
     *                         immediately after.
     */
    Variant(
            const arc::io::sys::Path& file_path,
            const arc::str::UTF8String* memory,
            const arc::str::UTF8String& default_variant = "",
            bool load_immediately = true);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Variant()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void reload();

    /*!
     * \brief Sets the current variant to use for this Document.
     *
     * This function will cause this object to load the relevant variant data
     * into its variant
     */
    void set_variant(const arc::str::UTF8String& variant);

    /*!
     * \brief Retrieves data from this Variant object using the given Visitor
     *        object.
     *
     * This object will first attempt to retrieve a JSON value with the key from
     * the data associated with current variant. If the value cannot be
     * retrieved the default variant will be used instead. If both of these
     * operations fail and the Variant has data loaded from memory this function
     * will finally fallback to attempting to retrieve the value from memory.
     *
     * \tparam VisitorType The type of the Visitor being passed in which will be
     *                     used to retrieve the value.
     *
     * \param key The key of the value to retrieve from the data.
     * \param visitor The visitor object to use to retrieve the value from the
     *                data.
     * \return A reference to the visitor object that was passed in to this
     *         function.
     *
     * \throws arc::ex::KeyError If there is no value in the data with the given
     *                           key.
     * \throws arc::ex::TypeError If the value in the data is not a valid type
     *                            that the Visitor is expecting.
     */
    template <typename VisitorType>
    VisitorType& get(
            const arc::str::UTF8String& key,
            VisitorType& visitor)
    {
        get(key, static_cast<VisitorBase*>(&visitor));
        return visitor;
    }

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual VisitorBase* get(
            const arc::str::UTF8String& key,
            VisitorBase* visitor);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The base file path before any variants are applied.
     */
    arc::io::sys::Path m_base_path;

    /*!
     * \brief The variant which will be used by default, it will be loaded as
     *        the initial variant and used if a key is not found in the current
     *        variant.
     */
    arc::str::UTF8String m_default_variant;

    /*!
     * \brief The current variant being used. If this is the same as the default
     *        variant no extra data will be loaded.
     */
    arc::str::UTF8String m_current_variant;

    /*!
     * \brief The JSON data for the current variant.
     */
    std::unique_ptr<Json::Value> m_variant_root;

    //--------------------------------------------------------------------------
    //                          PRIVATE STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Applies the given variant string to the file path.
     */
    static arc::io::sys::Path apply_variant(
            const arc::io::sys::Path& file_path,
            const arc::str::UTF8String& variant);

};

} // namespace metaengine

#endif

/*!
 * \file
 * \author David Saxon
 */
#ifndef METAENGINE_DOCUMENT_HPP_
#define METAENGINE_DOCUMENT_HPP_

#include <cassert>
#include <memory>

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/sys/Path.hpp>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace Json
{
class Value;
} // namespace Json

namespace metaengine
{

class Document;

/*!
 * \brief Abstract base class that defines a Visitor object that can be used to
 *        retrieve values from a Document and provide access to said values as a
 *        specific type.
 *
 * To define a Visitor, only the Visitor::retrieve() function must be
 * implemented.
 */
template <typename ReturnType>
class Visitor
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(Visitor);

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    Visitor()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Visitor()
    {
    }

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the value that this Visitor holds.
     */
    const ReturnType& operator*() const
    {
        return m_value;
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the value that this Visitor holds.
     */
    const ReturnType& get_value() const
    {
        return m_value;
    }

    /*!
     * \brief Attempts to parse the given JSON data as this Visitor's type and
     *        update its internal value.
     *
     * This function will be called when this Visitor is passed into the
     * Document::get() function. It will be passed the JSON data that is
     * associated with the key used with the Document::get() function.
     *
     * To implement this function the JSON data should be checked to see if it
     * is convertible to this Visitor's type, if not the function should
     * return ```false``` and let the Document::get() function preform the error
     * handling. If the data is convertible to the type then it should be
     * converted and stored in this object's Visitor::m_value and then should
     * return ```true```.
     *
     * \param data The JSON value to attempt to convert to this Visitor's type.
     * \param key The key that was used to retrieve the given JSON data from the
     *             Document.
     * \param requester The Document that has called this function and provided
     *                  the JSON data.
     * \param error_message Can be used to return an error message if the
     *                      conversion was not successful.
     */
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            Document* requester,
            arc::str::UTF8String& error_message) = 0;

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal value of this Visitor. It will be set when the
     *        retrieve function completes successfully.
     */
    ReturnType m_value;
};


/*!
 * \brief Object that is used to load and store MetaEngine data from JSON.
 *
 * A Document can contain up to two versions of the Data, one loaded from a
 * file, and another loaded from memory. This means if the data from the file
 * is invalid, the Document can fallback to using the data loaded from memory.
 */
class Document
{
private:

    ARC_DISALLOW_COPY_AND_ASSIGN(Document);

public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Function used to report a failure related to Document data loaded
     *        from the file system, which results in falling back to using
     *        Document data from memory.
     *
     * \param message The message describing the reason for failure.
     */
    typedef void (*fallback_reporter)(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new Document that loads its internal data from the given
     *        file.
     *
     * \note This Document will not have a fallback system, if loading or
     *       accessing data fails and exception will be thrown immediately.
     *
     * \param file_path Path to a JSON file to load this Document's internal
     *                  data from.
     *
     * \throw arc::ex::IOError If the file path cannot be accessed to
     *                                  load data from.
     * \throw arc::ex::ParseError If the file does not contain valid JSON data.
     */
    Document(const arc::io::sys::Path& file_path);

    /*!
     * \brief Creates a new Document that loads its internal data from the given
     *        string in memory.
     *
     * \note This Document will not have a fallback system, if loading or
     *       accessing data fails and exception will be thrown immediately.
     *
     * \param memory Pointer to a arc::str::UTF8String that will contain JSON
     *               to load this Document's internal data from.
     *
     * \throw arc::ex::ParseError If the string does not contain valid JSON
     *                            data.
     */
    Document(const arc::str::UTF8String* memory);

    /*!
     * \brief Creates a new Document that loads two copies of its internal data
     *        from the given file and string in memory.
     *
     * \note This Document will use the data loaded from the file unless loading
     *       or accessing the data fails, in which case the Document will
     *       fallback to using the data loaded from memory.
     *
     * \param file_path Path to a JSON file to load the first version of this
     *                  Document's internal data from.
     * \param memory Pointer to a arc::str::UTF8String that will contain JSON to
     *               load the second version of this Document's internal data
     *               from.
     *
     * \throw arc::ex::ParseError If both the file and the string do not contain
     *                            valid JSON data.
     */
    Document(
            const arc::io::sys::Path& file_path,
            const arc::str::UTF8String* memory);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Document();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Sets the function that will be called to report when a Document
     *        fails to load JSON data from the file system, and fallbacks to
     *        loading data from memory.
     *
     * This can be called either when a Document is constructed or when calling
     * reload() on a Document.
     */
    static void set_load_fallback_reporter(fallback_reporter func);

    /*!
     * \brief Sets the function that will be called to report when a Document
     *        retrieve a value from data loaded from the file system, and
     *        fallbacks to retrieving the value from data loaded from memory.
     *
     * This can be called when the get() function is called.
     */
    static void set_get_fallback_reporter(fallback_reporter func);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether this Document is using data loaded from the file
     *        system.
     *
     * \note This does indicate whether the data actually loaded correctly,
     *       see has_valid_file_data().
     */
    bool is_using_file_path() const;

    /*!
     * \brief Returns whether this Document is using data loaded from memory.
     *
     * \note This does indicate whether the data actually loaded correctly,
     *       see has_valid_memory_data().
     *
     * \note If is_using_file_path() returns ```true``` then data loaded from
     *       the file system will be used over data loaded from memory, unless
     *       there is an error parsing or retrieving values from the file data.
     */
    bool is_using_memory() const;

    /*!
     * \brief Returns whether this Document currently has valid loaded data from
     *        the file system.
     */
    bool has_valid_file_data() const;

    /*!
     * \brief Returns whether this Document currently has valid loaded data from
     *        memory.
     */
    bool has_valid_memory_data() const;

    /*!
     * \brief Reloads the data of this document.
     *
     * The data will be reloaded dependent on the sources provided when this
     * Document was constructed: from file and/or memory.
     *
     * \note Even if loading from the file failed the previous time this
     *       Document was loaded it will be reattempted by this function.
     *
     * \throw arc::ex::IOError If this Document is using a file path
     *                                  and the file cannot be accessed and
     *                                  there is no memory source provided.
     * \throw arc::ex::ParseError If the file and/or memory does not contain
     *                            valid JSON data.
     */
    void reload();

    /*!
     * \brief TODO:
     *
     * - visitor
     * - memory fallback
     * - fallback handler function
     *
     * \param exceptions, key, type?
     */
    template <typename VisitorType>
    VisitorType& get(const arc::str::UTF8String& key, VisitorType& visitor)
    {
        // attempt to retrieve from the file system first
        if(m_file_root != nullptr)
        {
            const Json::Value* data = nullptr;
            try
            {
                data = get_value(m_file_root.get(), key);
            }
            catch(const arc::ex::KeyError& exc)
            {
                // rethrow if there's no memory fallback
                if(m_mem_root == nullptr)
                {
                    throw exc;
                }
                else if(s_get_reporter != nullptr)
                {
                    // trigger a warning and prepare to fallback
                    arc::str::UTF8String error_message;
                    error_message << "Falling back to retrieving value from "
                                  << "memory. " << exc.get_type() << ": "
                                  << exc.get_message();
                    s_get_reporter(m_file_path, error_message);
                }
            }

            // if we got data hand off to the visitor
            if(data != nullptr)
            {
                bool retrieve_success = false;
                arc::str::UTF8String retrieve_error;
                try
                {
                    retrieve_success =
                        visitor.retrieve(data, key, this, retrieve_error);
                }
                catch(...)
                {
                    retrieve_success = false;
                }

                // if everything was successful we're done
                if(retrieve_success)
                {
                    return visitor;
                }

                // begin building the error message
                arc::str::UTF8String error_message;
                error_message << "Failed to retrieve value for key \"" << key
                              << "\" ";
                // was there an explicit message from the Visitor?
                if(!retrieve_error.is_empty())
                {
                    error_message << "with message: " << retrieve_error;
                }
                else
                {
                    error_message << "as the requested type.";
                }

                // throw if there's no memory fallback
                if(m_mem_root == nullptr)
                {
                    throw arc::ex::TypeError(error_message);
                }
                else if(s_get_reporter != nullptr)
                {
                    // trigger a warning and prepare to fallback
                    arc::str::UTF8String report_message;
                    report_message << "Falling back to retrieving value from "
                                   << "memory. " << error_message;
                    s_get_reporter(m_file_path, report_message);
                }
            }
        }

        // attempt to retrieve from memory if anything above failed
        if(m_mem_root != nullptr)
        {
            const Json::Value* data = nullptr;
            // attempt to get the data from the JSON root, if this fails we
            // just let it throw out of this function
            data = get_value(m_mem_root.get(), key);
            // if the above function didn't throw, the data should never be
            // null
            assert(data != nullptr);

            // hand off to the visitor
            bool retrieve_success = false;
            arc::str::UTF8String retrieve_error;
            try
            {
                retrieve_success =
                    visitor.retrieve(data, key, this, retrieve_error);
            }
            catch(...)
            {
                retrieve_success = false;
            }

            // if everything was successful we're done
            if(retrieve_success)
            {
                return visitor;
            }

            // throw
            arc::str::UTF8String error_message;
            error_message << "Failed to retrieve value for key: \""
                          << key << "\" ";
            if(!retrieve_error.is_empty())
            {
                error_message << "with error: " << retrieve_error;
            }
            else
            {
                error_message << "as the requested type.";
            }
            throw arc::ex::TypeError(error_message);

        }

        return visitor;
    }

private:

    //--------------------------------------------------------------------------
    //                         PRIVATE STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The function to use to report fallback when loading from a file.
     */
    static fallback_reporter s_load_reporter;
    /*!
     * \brief The function to use to report fallback when get a value.
     */
    static fallback_reporter s_get_reporter;

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The path to the file to load JSON data from.
     */
    arc::io::sys::Path m_file_path;
    /*!
     * \brief Whether the path is being used to load this Document's data.
     */
    bool m_using_path;

    /*!
     * \brief The pointer to memory to load JSON data from. (null if not being
     *        used).
     */
    const arc::str::UTF8String* m_memory;

    /*!
     * \brief The root JSON root value that has been loaded and parsed from the
     *        file system.
     */
    std::unique_ptr<Json::Value> m_file_root;
    /*!
     * \brief The root JSON root value that has been loaded and parsed from
     *        memory.
     */
    std::unique_ptr<Json::Value> m_mem_root;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Parses JSON data from the given string into the root JSON value.
     *
     * \throws arc::ex::ParseError If the data is not valid JSON.
     */
    void parse(
            const arc::str::UTF8String& json_data,
            std::unique_ptr<Json::Value>& value);

    /*!
     * \brief Retrieves the JSON value associated with the given key from the
     *        JSON data.
     *
     * \param root The root JSON value to retrieve the value from.
     * \param key The key to get the value for.
     * \return Pointer to the JSON value associated with the key,.
     *
     * \throws arc::ex::KeyError If there is no value for the key.
     */
    const Json::Value* get_value(
        const Json::Value* root,
        const arc::str::UTF8String& key) const;
};

} // namespace metaengine

#endif

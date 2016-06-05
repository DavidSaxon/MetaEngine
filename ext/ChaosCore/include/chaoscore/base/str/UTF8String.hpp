/*!
 * \file
 * \author David Saxon
 */
#ifndef CHAOSCORE_BASE_STR_UTF8STRING_HPP_
#define CHAOSCORE_BASE_STR_UTF8STRING_HPP_

#include <ostream>
#include <string>
#include <vector>

#include "chaoscore/base/Types.hpp"
#include "chaoscore/base/str/StringConstants.hpp"

namespace chaos
{
namespace str
{

/*!
 * \brief A string type designed for storing and manipulating UTF-8 encoded
 *        text.
 *
 * \note This object expects input text to already be UTF-8 encoded. For
 *       functions to convert encodings see StringOperations.hpp
 *
 * The UTF8String data type is used extensively throughout ChaosCore and other
 * Chaos Foundation projects.
 *
 * ChaosCore stands by the principle that all string handling should be Unicode
 * aware. The `char` primitive should only be used for storing and manipulation
 * raw byte data, not for representing entire language characters. Furthermore
 * ChaosCore considers UTF-8 to be the only default encoding type for Unicode
 * text. Other encodings should only be used for special cases or interacting
 * with applications that require a different encoding (such as the Windows
 * API). For more info see http://utf8everywhere.org/
 *
 * For practicality the internal byte of an UTF8String is NULL terminated. This
 * means the raw data can easily be used as native C style strings.
 *
 * \par A Brief Introduction to UTF-8
 *
 * In order to fully make use of the UTF8String type's functionality its useful
 * to understand the UTF-8 encoding method.
 *
 * UTF-8 encodes each Unicode symbol in 1-4 bytes. The value of each symbol maps
 * directly to a Unicode code point that represents the symbol. For example:
 *
 * - "a" is stored using one byte with the hex value 0x61 or binary value
 *   `01100001`
 * - "ל" is stored using two bytes with the hex value 0xD79C or binary value
 *   `11010111 10011100`
 * - "∑" is stored using three bytes with the hex value 0xE28891 or binary
 *   value `11100010 10001000 10010001`
 * - "𝄞" is stored  using four bytes with the hex value 0xF09D849E or binary
 *   value `11110000 10011101 10000100 10011110`
 *
 * The byte size of a UTF-8 symbol can be recognised by the bit pattern that
 * makes up the start of each byte:
 *
 * - One byte symbol: `0xxxxxxx`
 * - Two byte symbol: `110xxxxx 10xxxxxx`
 * - Three byte symbol: `1110xxxx 10xxxxxx 10xxxxxx`
 * - Four byte symbol: `11110xxx 10xxxxxx 10xxxxxx 10xxxxxx`
 *
 * \par UTF8String Usage
 *
 * Here follows an example of using the UTF8String type:
 *
 * We have a C style string that cannot be encoded correctly with ASCII
 * (In this case we are assuming the input string is encoded in UTF-8):
 *
 * \code
 * const char* cstring = "aל∑";
 * \endcode
 *
 * If we inspect the length of the string we get unexpected results:
 *
 * \code
 * strlen(cstring);
 * // output: 6
 * \endcode
 *
 * This is because while the string only contains 3 symbols it contains 6 bytes
 * which is what `strlen` is counting. Next we construct a UTF8String using the
 * c string data, and inspect it's length:
 *
 * \code
 * chaos::str::UTF8String utf8(cstring);
 * utf8.get_length();
 * // output: 3
 * utf8.get_byte_length();
 * // output: 6
 * \endcode
 *
 * [TODO: iterator]
 * We can now inspect each symbol in the string:
 *
 * \code
 * for(std::size_t i = 0; i < utf8.get_length(); ++i)
 * {
 *     chaos::str::UTF8String symbol(utf8.get_symbol(i));
 * }
 * \endcode
 *
 * Note that the symbol is returned as an UTF8String itself, this is due to the
 * fact that symbol has a variable byte-width so cannot be stored in a `char`,
 * nor is there any need to differentiate data types between a string of symbols
 * and an individual symbol.
 *
 * We can also inspect the byte-widths of each symbol in our string:
 *
 * \code
 * for (std::size_t i = 0; i < utf8.get_length(); ++i)
 * {
 *     utf8.get_symbol_width(i);
 * }
 * // output: 1
 * // output: 2
 * // output: 3
 * \endcode
 *
 * The UTF8String class provides many convenience functions for manipulating
 * string data, such as `split`, `trim`, `starts_with`, `find_first`,
 * `to_raw`, `to_stdstring`, etc.
 * \endcode
 *
 * TODO: optimisations
 */
class UTF8String
{
public:

    //--------------------------------------------------------------------------
    //                           OPTIMISATION STRUCTURE
    //--------------------------------------------------------------------------

    /*!
     * \brief Structure used to hold information about optimisations that
     *        UTF8String objects should perform.
     *
     * There is a static instance of this object (defaults to no optimisations)
     * that will be used by all UTF8String objects by default:
     * UTF8String::default_opt. However other instances of this object can be
     * passed in explicitly to UTF8String objects at construction time.
     */
    struct Opt
    {
    public:

        //------------------------------ENUMERATOR------------------------------

        /*!
         * \brief Flags that represent the UTF8String optimisations that can be
         *        enabled.
         */
        enum Flag
        {
            /// No optimisation
            NONE = 0,
            /// Input character array data given to a UTF8String objects will
            /// not be checked to see if it's valid UTF-8 encoded data.
            /// This optimisation can be used in most cases since input data is
            /// already known to be UTF-8 before construction.
            /// Character data can be checked to see if it's valid UTF-8 encoded
            /// data through the chaos::str::is_utf8 function. The character
            /// data will still need to be iterated over in order to inspect the
            /// first byte to determine the byte-width of each symbol, if the
            /// symbol width cannot be determined a width of 1 is assumed.
            SKIP_VALID_CHECK = 1L << 1,
            /// All symbols within the string will have a byte width equal to
            /// Opt::fixed_width_size. This commonly useful for handling pure
            /// ASCII (1-byte symbols) strings using a UTF8String object. Used
            /// in combination with SKIP_VALID_CHECK will mean that no
            /// additional logic will be performed at string construction time
            /// (other than copying character data), which is extremely useful
            /// for reading large sets of ASCII encoded data from files.
            /// TODO: link FileReader
            FIXED_WIDTH = 1L << 2
        };

        //------------------------------ATTRIBUTES------------------------------

        /*!
         * \brief Bitwise OR of the flags representing optimisations that this
         *        structure is enabling.
         */
        chaos::uint32 flags;
        /*!
         * \brief If FIXED_WIDTH is enabled this represents the width that all
         *        symbols are expected to be. Defaults to ```1```.
         */
        chaos::uint16 fixed_width_size;

        //-----------------------------CONSTRUCTOR------------------------------

        /*!
         * \brief Constructs a new UTF8String optimisation structure.
         *
         * \param _flags Bitwise OR of Opt::Flag parameters this structure
         *               represents.
         */
        Opt(chaos::uint32 _flags = NONE)
            :
            flags           (_flags),
            fixed_width_size(1)
        {
        }
    };

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The optimisation parameters (defaults to none) that will be used
     *        by default by UTF8String objects unless explicitly overridden at
     *        construction time.
     */
    static Opt default_opt;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Default constructor.
     *
     * Creates a new UTF8String that contains no symbols and holds only the NULL
     * terminator.
     *
     * \param optimisations Optimisations parameters to be used by this
     *                      UTF8String instance.
     */
    UTF8String(Opt optimisations = default_opt);

    /*!
     * \brief C style string constructor.
     *
     * Creates a new UTF8String that contains the provided data.
     *
     * \note The input data is expected to be UTF-8 encoded and NULL terminated.
     *       For functions to convert encodings see StringOperations.hpp
     *
     * \param data Character data to be used for this UTF8String instance.
     * \param optimisations Optimisations parameters to be used by this
     *                      UTF8String instance.
     *
     * \throws chaos::ex::EncodingError If the provided data is not valid UTF-8
     *                                  encoded data.
     */
    UTF8String(const char* data, Opt optimisations = default_opt);

    /*!
     * \brief C style string and length constructor.
     *
     * Creates a new UTF8String using the given number of bytes from the data.
     *
     * \note The input data is expected to be UTF-8 encoded but does not need to
     *       be NULL terminated. For functions to convert encodings see
     *       StringOperations.hpp
     *
     * \param data Character data to be used for this UTF8String instance.
     * \param length The number of bytes to read from the character data.
     * \param optimisations Optimisations parameters to be used by this
     *                      UTF8String instance.
     *
     * \throws chaos::ex::EncodingError If the provided data is not valid UTF-8
     *                                  encoded data.
     */
    UTF8String(
            const char* data,
            std::size_t length,
            Opt optimisations = default_opt);

    /*!
     * \brief Copy constructor.
     *
     * Creates a new UTF8String from a copy of the data from the given other
     * UTF8String.
     *
     * \note Optimisation parameters will be coped from the given UTF8String.
     *
     * \throws chaos::ex::EncodingError If the internal data of the provided
     *                                  object is not valid UTF-8 encoded data.
     */
    UTF8String(const UTF8String& other);

    /*!
     * \brief Move constructor.
     *
     * \param other The UTF8String to move resources from.
     */
    UTF8String(UTF8String&& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~UTF8String();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     *
     * Assigns the internal of data this UTF8String as a copy from the internal
     * data of the provided UTF8String.
     *
     * Is the same as calling assign(const UTF8String&).
     *
     * \note Optimisation parameters will be coped from the given UTF8String.
     *
     * \param other UTF8String to copy internal data from.
     * \return A reference to this UTF8String after the assignment has taken
     *         place.
     *
     * \throws chaos::ex::EncodingError If the internal data of the provided
     *                                  object is not valid UTF-8 encoded data.
     */
    const UTF8String& operator=(const UTF8String& other);

    /*!
     * \brief Equality operator.
     *
     * Compares whether this UTF8String and the other given UTF8String are
     * considered equal.
     *
     * \note Optimisation parameters are not considered in the equality
     *       comparison.
     *
     * \param other UTF8String to compare this against.
     * \return Whether the strings are equal.
     */
    bool operator==(const UTF8String& other) const;

    /*!
     * \brief Inequality operator.
     *
     * Compares whether this UTF8String and the other given UTF8String are
     * considered not equal.
     * \param other UTF8String to compare this against.
     * \return Whether the strings are not equal.
     */
    bool operator!=(const UTF8String& other) const;

    /*!
     * \brief Less than operator.
     *
     * Compares whether this UTF8String is less than the other given UTF8String.
     *
     * Less than is defined by performing a value comparison between each
     * Unicode code point in the string.
     *
     * \param other UTF8String to compare against.
     * \return Whether this UTF8String is less than the other.
     */
    bool operator<(const UTF8String& other) const;

    // TODO: does the result copy opts from first string?
    // TODO: encoding error, check both??
    /*!
     * \brief Addition operator.
     *
     * Performs the same function as concatenate() but does not cause any
     * modifications to this string, instead returns a new string which contains
     * the results of the concatenation.
     *
     * \param other UTF8String to append to the end of a copy of this string.
     * \return UTF8String that contains the results of the concatenation.
     *
     */
    UTF8String operator+(const UTF8String& other) const;

    // TODO: encoding error, check only second
    /*!
     * \brief Compound addition operator.
     *
     * Performs the same function as concatenate().
     *
     * \param other UTF8String to append to the end of this string.
     * \return A reference to this UTF8String after the concatenation has taken
     *         place.
     */
    UTF8String& operator+=(const UTF8String& other);

    // TODO: does the result copy opts from first string?
    // TODO: encoding error, check both??
    /*!
     * \brief Multiplication operator.
     *
     * Performs the same function as repeat() but does not cause any
     * modifications to this string, instead returns a new string which contains
     * the results of the repeat.
     *
     * \param count the number of times to repeat the string
     * \return UTF8String that contains the results of the repeat.
     */
    UTF8String operator*(chaos::uint32 count) const;

    // TODO: encoding error, check only second
    /*!
     * \brief Compound multiplication operator.
     *
     * Performs the same function as repeat().
     *
     * \param count the number of times to repeat this string.
     * \return A reference to this UTF8String after the repeat operation has
     *         taken place.
     */
    UTF8String& operator*=(chaos::uint32 count);

    // TODO: encoding error, check only second
    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given other UTF8String.
     */
    UTF8String& operator<<(const UTF8String& other);

    // TODO: encoding error, check only second
    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given C style string.
     *
     * \note The input data is expected to be UTF-8 encoded and NULL terminated.
     *       For functions to convert encodings see StringOperations.hpp
     */
    UTF8String& operator<<(const char* other);

    // TODO: encoding error, check only second
    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given std string.
     *
     * \note The input data is expected to be UTF-8 encoded. For functions to
     *       convert encodings see StringOperations.hpp
     */
    UTF8String& operator<<(const std::string& other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given boolean (where false = 0 and true
     * = 1).
     */
    UTF8String& operator<<(bool other);

    // TODO: character range???
    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given char.
     */
    UTF8String& operator<<(char other);

// TODO: how to solve the ambiguous problem
#ifdef CHAOS_OS_WINDOWS

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given unsigned long.
     */
    UTF8String& operator<<(unsigned long other);

#endif

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given int8.
     */
    UTF8String& operator<<(chaos::int8 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given uint8.
     */
    UTF8String& operator<<(chaos::uint8 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given int16.
     */
    UTF8String& operator<<(chaos::int16 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given uint16.
     */
    UTF8String& operator<<(chaos::uint16 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given int32.
     */
    UTF8String& operator<<(chaos::int32 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given unsigned int32.
     */
    UTF8String& operator<<(chaos::uint32 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given int64.
     */
    UTF8String& operator<<(chaos::int64 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given unsigned int64.
     */
    UTF8String& operator<<(chaos::uint64 other);

    /*!
     * \brief Stream operator.
     *
     * Extends this UTF8String with the given float.
     */
    UTF8String& operator<<(float other);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assigns the internal data of this UTF8String from the given C
     *        style string.
     *
     * This operation will delete any current internal data of this UTF8String.
     *
     * \note The input data is expected to be UTF-8 encoded and NULL terminated.
     *       For functions to convert encodings see StringOperations.hpp
     *
     * \throws chaos::ex::EncodingError If the provided data is not valid UTF-8
     *                                  encoded data.
     */
    void assign(const char* data);

    /*!
     * \brief Assigns the internal data of this UTF8String from the given C
     *        style string and byte length.
     *
     * Creates a new UTF8String using the given number of bytes from the data.
     * This operation will delete any current internal data of this UTF8String.
     *
     * \note The input data is expected to be UTF-8 encoded but does not need to
     *       be NULL terminated. For functions to convert encodings see
     *       StringOperations.hpp
     *
     * \param data Character data to be used for this UTF8String.
     * \param length The number of bytes to read from the character data.
     *
     * \throws chaos::ex::EncodingError If the provided data is not valid UTF-8
     *                                  encoded data.
     */
    void assign(const char* data, std::size_t length);

    /*!
     * \brief Assigns internal data from a copy of another UTF8String.
     *
     * This operation will delete any current internal data of this UTF8String.
     *
     * \throws chaos::ex::EncodingError If the internal data of the provided
     *                                  object is not valid UTF-8 encoded data.
     */
    void assign(const UTF8String& other);

    /*!
     * \brief Uses the given data array as the internal data array of this
     *        chaos::str::UTF8String.
     *
     * This function causes this object to delete any existing internal data it
     * already has and claim the given data array as it's own internal data
     * without performing any memory copies.
     * The provided data must be null terminated, and should not be used or
     * deleted one passed to this function.
     *
     * \throws chaos::ex::EncodingError If the provided data is not valid UTF-8
     *                                  encoded data.
     */
    void claim(char* data);

    // TODO: encoding error, check only second
    /*!
     * \brief Concatenates another UTF8String on to the end of this string.
     *
     * \note This operation modifies this UTF8String.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("Hello");
     * chaos::str::UTF8String s_2("World");
     * s_1.concatenate(s_2);
     * std::cout << s_1 << std::endl;
     * // output: Hello World
     * \endcode
     *
     * \other UTF8String to append to the end of this string.
     * \return A reference to this UTF8String after the concatenation has taken
     *         place.
     */
    UTF8String& concatenate(const UTF8String& other);

    // TODO: encoding error, check only second
    /*!
     * \brief Extends this string with a copy of itself the given number of
     *  times.
     *
     * \note This operation modifies this UTF8String.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("Hello");
     * s.repeat(3);
     * std::cout << s << std::endl;
     * // output: HelloHelloHello
     * \endcode
     *
     * \param count The number of times to repeat this string.
     * \return A reference to this UTF8String after repeat has taken place.
     */
    UTF8String& repeat(chaos::uint32 count);

    /*!
     * \brief Checks whether this UTF8String starts with the given substring.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("Hello World");
     * chaos::str::UTF8String s_2("Hello");
     * chaos::str::UTF8String s_3("World");
     * s_1.starts_with(s_2); // returns: true
     * s_1.starts_with(s_3); // returns: false
     * \endcode
     */
    bool starts_with(const UTF8String& substring) const;

    /*!
     * \brief Checks whether this UTF8String ends with the given substring.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("Hello World");
     * chaos::str::UTF8String s_2("Hello");
     * chaos::str::UTF8String s_3("World");
     * s_1.ends_with(s_2); // returns: false
     * s_1.ends_with(s_3); // returns: true
     * \endcode
     */
    bool ends_with(const UTF8String& substring) const;

    /*!
     * \brief Finds the first occurrence of the given substring and returns the
     *        symbol index of it within this UTF8String.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("Hello World");
     * chaos::str::UTF8String s_2("World");
     * chaos::str::UTF8String s_3("*");
     * s_1.find_first(s_2); // returns: 6
     * s_1.find_first(s_3); // returns: chaos::str::npos
     * \endcode
     *
     * \param substring UTF8String to find the first occurrence of in this
     *                  string.
     * \return The index of the beginning of the first occurrence of the
     *         substring in this string. If the substring could not be found,
     *         chaos::str::npos is returned instead.
     */
    std::size_t find_first(const UTF8String& substring) const;

    /*!
     * \brief Finds the last occurrence of the given substring and returns the
     *        index of it within this UTF8String.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("Hello World World");
     * chaos::str::UTF8String s_2("World");
     * chaos::str::UTF8String s_3("*");
     * s_1.find_first(s_2); // returns: 12
     * s_1.find_first(s_3); // returns: chaos::str::npos
     * \endcode
     *
     * \param substring UTF8String to find the last occurrence of in this
     *                  string.
     * \return The index of the beginning of the last occurrence of the
     *         substring in this string. If the substring could not be found,
     *         chaos::str::npos is returned instead.
     */
    std::size_t find_last(const UTF8String& substring) const;

    // TODO: does the result copy opts from first string?
    // TODO: encoding error, check both??
    /*!
     * \brief Splits this UTF8String by the given delimiter and returns the
     *        split elements in a std::vector.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("Hello_World");
     * chaos::str::UTF8String delim("_");
     * std::vector<chaos::str::UTF8String> elements = s.split(delim);
     * // vector contents: ["Hello", "World"]
     * \endcode
     *
     * \throws chaos::ex::ValueError If the provided delimiter is an empty
     *                               string.
     * \param delimiter String to use as a delimiter to split the string into
     *                  elements.
     * \return std::vector containing the results of the split.
     */
    std::vector<UTF8String> split(const UTF8String& delimiter) const;

    // TODO: does the result copy opts from first string?
    // TODO: encoding error, check both??
    /*!
     * \brief Removes consecutive duplicates of the given substring within this
     *        string.
     *
     * Consecutive duplicates are removed so that only one instance of the
     * substring is retained.
     *
     * Example usage:
     * \code
     * chaos::str::UTF8String s("this__string--has__duplicates!!");
     * s.remove_duplicates("_");
     * // s is: this_string--has_duplicates!!
     * \endcode
     *
     * \param substring Substring to remove consectituve instances of from this
     *                  string.
     */
    void remove_duplicates(const UTF8String& substring);

    /*!
     * \brief Whether the symbols of this string make up a valid integer type.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("-34");
     * chaos::str::UTF8String s_2("Hello");
     * s_1.is_int(); // returns: true
     * s_2.is_int(); //returns: false
     * \endcode
     */
    bool is_int() const;

    /*!
     * \brief Whether the symbols of this string make up a valid unsigned
     *        integer type.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("16");
     * chaos::str::UTF8String s_2("-34");
     * s_1.is_uint(); // returns: true
     * s_2.is_uint(); //returns: false
     * \endcode
     */
    bool is_uint() const;

    /*!
     * \brief Whether the symbols of this string make up a valid floating point
     *        type.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s_1("53.89");
     * chaos::str::UTF8String s_2("Hello");
     * s_1.is_float(); // returns: true
     * s_2.is_float(); //returns: false
     * \endcode
     */
    bool is_float() const;

    // TODO: does the result copy opts from first string?
    // TODO: encoding error, check both??
    /*!
     * \brief Returns a new UTF8String composed of a substring of this string.
     *
     * Example usage:
     *
     * \code
     * chaos::string::UTF8String s("Hello World");
     * s.substring(0, 5); // returns: "Hello"
     * \endcode
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided starting index
     *                                          is out of bounds of the string
     *                                          length.
     *
     * \param start Index of the symbol to start the substring from. If this is
     *              equal to the length of this string an empty string is
     *              returned.
     * \param length the length of the substring. If greater than the possible
     *               symbols to allocate it will be clamped to the maximum
     *               length of the string with relation to the starting index.
     * \return New UTF8String containing the substring.
     */
    UTF8String substring(std::size_t start, std::size_t length) const;

    /*!
     * \brief Returns this as a standard library string.
     *
     * \note The contents of the std::string is a copy of the data within this
     *       object.
     */
    std::string to_std_string() const;

    /*!
     * \brief Returns this UTF8String as an bool type.
     *
     * \throws chaos::ex::ConversionDataError If the data of this string is not
     *                                        a valid bool.
     */
    bool to_bool() const;

    /*!
     * \brief Returns this UTF8String as an int32 type.
     *
     * \throws chaos::ex::ConversionDataError If the data of the string is not a
     *                                        valid int32.
     */
    chaos::int32 to_int32() const;

    /*!
     * \brief Returns this UTF8String as an uint32 type.
     *
     * \throws chaos::ex::ConversionDataError If the data of the string is not a
     *                                        valid uint32.
     */
    chaos::uint32 to_uint32() const;

    /*!
     * \brief Returns this UTF8String as an int64 type.
     *
     * \throws chaos::ex::ConversionDataError If the data of the string is not a
     *                                        valid int64.
     */
    chaos::int64 to_int64() const;

    /*!
     * \brief Returns this UTF8String as an uint64 type.
     *
     * \throws chaos::ex::ConversionDataError If the data of the string is not a
     *                                        valid uint64.
     */
    chaos::int64 to_uint64() const;


    //--------------------------------ACCESSORS---------------------------------

    /*!
     * \brief Returns the number of symbols in this UTF8String.
     *
     * The length is defined as how many UTF-8 symbols there are in the string,
     * this doesn't not necessarily equal the number of bytes in the string.
     */
    std::size_t get_length() const;

    /*!
     * \brief Returns whether the this UTF8String contains any characters or
     *        not.
     *
     * This operation is the same as checking whether get_length() returns `0`
     * or not.
     */
    bool is_empty() const;

    // TODO: does the result copy opts from first string?
    // TODO: encoding error, check both??
    /*!
     * \brief Returns the UTF-8 symbol at the given index in this string.
     *
     * The symbol is returned contained within a new UTF8String.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("Hello World");
     * chaos::str::UTF8String symbol(s.get_symbol(6));
     * std::cout << symbol << std::endl;
     * // output: W
     * \endcode
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided index is out of
                                                bounds of the string length.
     *
     * \param index Position of the symbol to retrieve in this string with
     *              respect to the symbol length. See get_length()
     * \return A new UTF8String containing the single UTF-8 symbol at the given
     *         index.
     */
    UTF8String get_symbol(std::size_t index) const;

    /*!
     * \brief Returns the integer/hex value for the UTF-8 symbol at the given
     *        index in this string.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("Hello World");
     * s.get_symbol_value(6); // returns: 87 (0x57)
     * \endcode
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided index is out of
     *                                          bounds of the string length.
     *
     * \param index Position of the symbol to retrieve the value for in this
     *              string with respect to the symbol length. See get_length()
     * \return A uint32 containing the value of the symbol.
     */
    chaos::uint32 get_symbol_value(std::size_t index = 0) const;

    /*!
     * \brief Returns the Unicode code point for the UTF-8 symbol at the given
     *        index in this string.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("Hello World©");
     * s.get_symbol_value(11); // returns: 169 (0xA9)
     * \endcode
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided index is out of
     *                                          bounds of the string length.
     *
     * \param index Position of the symbol to retrieve the code point for in
     *              this string with respect to the symbol length. See
     *              get_length()
     * \return A uint32 containing the code point of the symbol.
     */
    chaos::uint32 get_code_point(std::size_t index = 0) const;

    /*!
     * \brief Gets the index of the first byte for the symbol at the given
     *        index in this string.
     *
     * The symbol index is defined as the index of a symbol in the string with
     * respect to the number of symbols in the string. See get_length(). While
     * the byte index is defined as the index of a byte in the string with
     * respect to the number of bytes in the string. See get_byte_length().
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("£5");
     * s.get_byte_index_for_symbol_index(0);
     * // returns: 0
     * s.get_byte_index_for_symbol_index(1);
     * // returns: 2 (as £ is two bytes wide)
     * \endcode
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided index is out of
     *                                          bounds of the string length.
     */
    std::size_t get_byte_index_for_symbol_index(
            std::size_t symbol_index) const;

    /*!
     * \brief Gets the number of bytes in the symbol at the given index in this
     *        string.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("£5");
     * s.get_symbol_width(0); // returns: 2
     * s.get_symbol_width(1); // returns: 1
     * \endcode
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided index is out of
     *                                          bounds of the string length.
     */
    std::size_t get_symbol_width(std::size_t index) const;

    /*!
     * \brief Gets the raw byte data of this UTF8String.
     *
     * This data is a direct pointer to the internal pointer of this UTF8String.
     *
     * \warning The returned data should not be deleted as it is a direct
     *          pointer to the internal data of this object. And it will be
     *          deleted when this UTF8String is deleted.
     */
    const char* get_raw() const;

    /*!
     * \brief Returns the length of the internal data in bytes.
     *
     * This is exactly the number of bytes in the internal raw data of this
     * UTF8String which can be accessed through to_raw(). Note that this
     * data is NULL ('\0') terminated and this length includes the NULL
     * terminator. Therefore if UTF8String == "" then get_byte_length() == 1.
     *
     * \note This is not equal to the symbol length of the string as UTF-8
     *       encoded characters can consist of multiple bytes.
     */
    std::size_t get_byte_length() const;

    /*!
     * \brief Returns the index of the symbol that the byte at the given index
     *        is part of.
     *
     * The symbol index is defined as the index of a symbol in the string with
     * respect to the number of symbols in the string. See get_length(). While
     * the byte index is defined as the index of a byte in the string with
     * respect to the number of bytes in the string. See get_byte_length().
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("£5");
     * s.get_symbol_index_for_byte_index(0); // returns 0
     * s.get_symbol_index_for_byte_index(1); // returns 0
     * s.get_symbol_index_for_byte_index(2); // returns 1
     * \endcode
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided index is out of
     *                                          bounds of the string byte
     *                                          length.
     */
    std::size_t get_symbol_index_for_byte_index(
            std::size_t byte_index) const;

    /*!
     * \brief Returns the number of bytes in the symbol at the given byte index.
     *
     * Example usage:
     *
     * \code
     * chaos::str::UTF8String s("£5");
     * s.get_byte_width(0); // returns: 2
     * s.get_byte_width(2); // returns: 1
     * \endcode
     *
     *
     * \throws chaos::ex::IndexOutOfBoundsError If the provided index is out of
     *                                          bounds of the string byte
     *                                          length.
     *
     * \returns The width in bytes of the symbol that starts at the given byte.
     *          Or chaos::str::npos if the width could not be evaluated, which
     *          could be either because the byte is not the start of a symbol or
     *          this UTF8String does not contain valid UTF-8 encoded data.
     */
    std::size_t get_byte_width(std::size_t byte_index) const;

    /*!
     * \brief Returns the optimisation parameters being used by this UTF8String.
     */
    const Opt& get_optimisations() const;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    // Optimization parameters being used by this UTF8String.
    Opt m_opt;

    // the array containing the data stored as consecutive bytes
    char* m_data;
    // the length of the data int bytes (not the length of the string)
    std::size_t m_data_length;

    // the number of utf-8 symbols in this string
    std::size_t m_length;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * Internal function used for assigning raw data to this UTF8String. The
     * input data is expect to be 1-byte aligned and been utf-8 encoded. This
     * function will delete any existing internal data, and allocate storage
     * for the new data.
     *
     * \param data The input data to assign to the internal buffer
     * \param existing_length Optimisation parameter. If equal to
     *                        chaos::str::npos the length of the
     *                        data will be evaluated, however if already known
     *                        the length can be passed in here to skip this
     *                        step.
     */
    void assign_internal(
            const char*  data,
            std::size_t existing_length = chaos::str::npos);

    /*!
     * Internal function used to check if a given index is within the symbol
     * length (get_length) of the string. If it is not an IndexOutOfBoundsError
     * is thrown.
     */
    void check_symbol_index(std::size_t index) const;

    /*!
     * Internal function used to check if a given index is within the byte
     * length (get_byte_length) of the string. If it is not an
     * IndexOutOfBoundsError is thrown.
     */
    void check_byte_index(std::size_t index) const;

    /*!
     * Internal function used to calculate and set the symbol length and check
     * the UTF-8 validity of the internal data (m_data).
     * Actions dependent on the optimisation parameters.
     */
    void process_raw();
};

//------------------------------------------------------------------------------
//                               EXTERNAL OPERATORS
//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& stream, const UTF8String& s);

} // namespace str
} // namespace chaos



#endif

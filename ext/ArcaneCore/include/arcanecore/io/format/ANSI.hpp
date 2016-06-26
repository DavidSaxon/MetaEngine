/*!
 * \file
 * \brief Operations relating to ANSI codes.
 * author David Saxon
 */
#ifndef ARCANECORE_IO_FORMAT_ANSI_HPP_
#define ARCANECORE_IO_FORMAT_ANSI_HPP_

#include "arcanecore/base/str/UTF8String.hpp"

namespace arc
{
namespace io
{
namespace format
{

//------------------------------------------------------------------------------
//                                  ENUMERATORS
//------------------------------------------------------------------------------

/*!
 * \brief Enumerator representing the possible unique ANSI escape sequence
 *        colours.
 *
 * ANSI colours are mutually exclusive, therefore custom background and
 * foreground colours can not be mixed.
 */
enum ANSIColour
{
    ANSI_FG_DEFAULT       = 39,
    ANSI_FG_BLACK         = 30,
    ANSI_FG_WHITE         = 97,
    ANSI_FG_RED           = 31,
    ANSI_FG_GREEN         = 32,
    ANSI_FG_YELLOW        = 33,
    ANSI_FG_BLUE          = 34,
    ANSI_FG_MAGENTA       = 35,
    ANSI_FG_CYAN          = 36,
    ANSI_FG_LIGHT_GREY    = 37,
    ANSI_FG_DARK_GREY     = 90,
    ANSI_FG_LIGHT_RED     = 91,
    ANSI_FG_LIGHT_GREEN   = 92,
    ANSI_FG_LIGHT_YELLOW  = 93,
    ANSI_FG_LIGHT_BLUE    = 94,
    ANSI_FG_LIGHT_MAGENTA = 95,
    ANSI_FG_LIGHT_CYAN    = 96,
    ANSI_BG_DEFAULT       = 49,
    ANSI_BG_RED           = 41,
    ANSI_BG_GREEN         = 42,
    ANSI_BG_BLUE          = 44
};

/*!
 * \brief Enumerator representing the possible unique ANSI escape sequence
 *        attributes.
 *
 * These attributes are mutually exclusive, therefore cannot be combined.
 */
enum ANSIAttribute
{
    /// No attribute is applied to the text.
    ANSI_ATTR_NONE,
    /// The text is made bolder than normal.
    ANSI_ATTR_BOLD,
    /// Each character in the text is overlaid with an underscore.
    ANSI_ATTR_UNDERSCORE,
    /// The text is blinking.
    ANSI_ATTR_BLINK,
    /// The colours of the text are reversed, with the background applied to the
    /// foreground and vice versa.
    ANSI_ATTR_REVERSE
};

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Applies an ANSI escape sequence to the provided text.
 *
 * \param text The string text to be wrapped with the escape sequence.
 * \param colour the colour to use on the text.
 * \param attribute the attribute to use on the text.
 */
void apply_escape_sequence(
        arc::str::UTF8String& text,
        ANSIColour colour,
        ANSIAttribute attribute = ANSI_ATTR_NONE);

} // namespace format
} // namespace io
} // namespace arc

#endif

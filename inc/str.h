#pragma once

/**
 *  \brief
 *      Clear a string.
 *
 *  \param str
 *      string to clear.
 *  \return pointer on the given string.
 *
 * Clear the specified string.
 */
char* strclr(char *str);

/**
 *  \brief
 *      Replace the given character in a string.
 *
 *  \param str
 *      The string to operate on.
 *  \param oldc
 *      The character being replaced.
 *  \param newc
 *      The character 'oldc' is replaced with.
 *  \return pointer to the null byte at the end of 'str'.
 *
 * Replace all occurrences of character in a null-terminated string.
 */
char *strreplacechar(char *str, char oldc, char newc);

/**
 *  \brief
 *      Convert a s32 value to string (input value should be in [-500000000..500000000] range).
 *
 *  \param value
 *      The s32 integer value to convert to string (input value should be in [-500000000..500000000] range).
 *  \param str
 *      Destination string (it must be large enough to receive result).
 *  \param minsize
 *      Minimum size of resulting string.
 *  \return string length
 *
 * Converts the specified s32 value to string.<br>
 * If resulting value is shorter than requested minsize the method prepends result with '0' character.
 */
u16 intToStr(s32 value, char *str, u16 minsize);

/**
 *  \brief
 *      Convert a u32 value to string (input value should be in [0..500000000] range).
 *
 *  \param value
 *      The u32 integer value to convert to string (input value should be in [0..500000000] range).
 *  \param str
 *      Destination string (it must be large enough to receive result).
 *  \param minsize
 *      Minimum size of resulting string.
 *  \return string length
 *
 * Converts the specified u32 value to string.<br>
 * If resulting value is shorter than requested minsize the method prepends result with '0' character.
 */
u16 uintToStr(u32 value, char *str, u16 minsize);

/**
 *  \brief
 *      Convert a u32 value to hexadecimal string.
 *
 *  \param value
 *      The u32 integer value to convert to hexadecimal string.
 *  \param str
 *      Destination string (it must be large enough to receive result).
 *  \param minsize
 *      Minimum size of resulting string.
 *  \return string length
 *
 * Converts the specified u32 value to hexadecimal string.<br>
 * If resulting value is shorter than requested minsize the method prepends result with '0' character.
 */
u16 intToHex(u32 value, char *str, u16 minsize);

/**
 *  \brief
 *      Convert a fix32 value to string.
 *
 *  \param value
 *      The fix32 value to convert to string.
 *  \param str
 *      Destination string (it must be large enough to receive result).
 *  \param numdec
 *      Number of wanted decimal.
 *
 * Converts the specified fix32 value to string.<br>
 */
void fix32ToStr(fix32 value, char *str, u16 numdec);

/**
 *  \brief
 *      Convert a fix16 value to string.
 *
 *  \param value
 *      The fix16 value to convert to string.
 *  \param str
 *      Destination string (it must be large enough to receive result).
 *  \param numdec
 *      Number of wanted decimal.
 *
 * Converts the specified fix16 value to string.<br>
 */
void fix16ToStr(fix16 value, char *str, u16 numdec);
